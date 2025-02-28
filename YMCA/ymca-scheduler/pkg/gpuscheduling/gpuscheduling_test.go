package gpuscheduling_test

import (
    "context"
    "log"
    "os"
    "testing"

    "dxlcq.cn/ymca-scheduler/pkg/gpuscheduling"

    "github.com/spf13/pflag"
    corev1 "k8s.io/api/core/v1"
    metav1 "k8s.io/apimachinery/pkg/apis/meta/v1"
    //"k8s.io/kubernetes/pkg/scheduler/framework"
    //"k8s.io/kubernetes/pkg/scheduler/framework/runtime"
    "k8s.io/kubernetes/cmd/kube-scheduler/app/options"
    "k8s.io/kubernetes/pkg/scheduler/apis/config"
    "k8s.io/kubernetes/pkg/scheduler/framework"
)

func TestSchedulerSetup(t *testing.T) {
    // 创建临时目录
    tmpDir, err := os.MkdirTemp("", ".ymcakube")
    if err != nil {
        t.Fatal(err)
    }
    defer os.RemoveAll(tmpDir)

    // .ymcakube/config 的内容
    configTxt := []byte(`
apiVersion: v1
clusters:
- cluster:
    server: http://localhost:8080
  name: default-cluster
contexts:
- context:
    cluster: default-cluster
    user: default-user
  name: default-context
current-context: default-context
users:
- name: default-user
`)

    // 写入到 .ymcakube/config
    ymcaKubeConfig := tmpDir + "/config"
    if err := os.WriteFile(ymcaKubeConfig, configTxt, os.FileMode(0600)); err != nil {
        t.Fatal(err)
    }

    // 测试用例定义
    testcases := []struct {
        name        string
        flags       []string
        wantPlugins map[string]*config.Plugins
    }{
        {
            name: "ymcaconfig",                         // 测试用例名称
            flags: []string{
                "--kubeconfig", ymcaKubeConfig,         // 指定 kubeconfig 文件
            },
            wantPlugins: map[string]*config.Plugins{    // 期望的插件列表
                "ymca-scheduler": {
                    Filter: config.PluginSet{           // 过滤插件
                        Enabled: []config.Plugin{
                            {Name: gpuscheduling.Name}, // 启用自定义插件
                        },
                    },
                },
            },
        },
    }

    // 执行测试用例
    for _, tc := range testcases {
        t.Run(tc.name, func(t *testing.T) {
            // 解析命令行参数
            opts := options.NewOptions()                        // k8s 允许的命令行参数
            nfs := opts.Flags                                   // 拿到这个列表
            
            fs := pflag.NewFlagSet("test", pflag.PanicOnError)  // flgset 用于解析命令行参数
            for _, f := range nfs.FlagSets {                    // 将 opts.Flags 中的参数添加到 fs 中
                fs.AddFlagSet(f)
            }

            if err := fs.Parse(tc.flags); err != nil {          // 在 tc.flags 中查找参数
                t.Fatal(err)
            }

            fs.Set("secure-port", "10086")                      // 手动设置参数，在部署的时候需要删除
            
            // ！！！
            ctx := context.Background()                         // 创建上下文

            plugin, err := gpuscheduling.New(ctx, nil, nil)     // 创建插件
            if err != nil {
                t.Fatal(err)
            }
            log.Println(plugin.Name())
            
            FilterPlugin, ok := plugin.(framework.FilterPlugin) // 断言
            if !ok {
                t.Fatal("plugin is not a FilterPlugin")
            }
            node := &corev1.Node{
                ObjectMeta: metav1.ObjectMeta{
                    Name:   "k8s1",
                    Labels: map[string]string{"gpu": "true"}, // 测试代码的 gpu 标签设为 true
                },
            }
            
            nodeInfo := framework.NewNodeInfo()
            nodeInfo.SetNode(node)
            
            FilterPlugin.Filter(ctx, nil, nil, nodeInfo) // 调用 Filter 方法

            ScorePlugin, ok := plugin.(framework.ScorePlugin)
            if !ok {
                t.Fatal("plugin is not a ScorePlugin")
            }

            scoreValue, status := ScorePlugin.Score(ctx, nil, nil, nodeInfo.Node().Name)
            if status.Code() != framework.Success {
                t.Errorf("Expected status Success, got: %v", status.Code())
            }

            log.Println("🫣 分数", scoreValue)


            log.Println("")
            log.Println("🥵 跑完啦！")
        })
    }
}
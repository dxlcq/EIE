package gpuscheduling

import (
	"context"
	"log"

	"k8s.io/api/core/v1"
	"k8s.io/apimachinery/pkg/runtime"
	"k8s.io/kubernetes/pkg/scheduler/framework"
	//"time"
)

const Name = "ymca-scheduler"

type ymca struct{}  // 结构体

var _ framework.FilterPlugin = &ymca{}

func New(_ context.Context, _ runtime.Object, _ framework.Handle) (framework.Plugin, error) {
    return &ymca{}, nil
}

func (pl *ymca) Name() string {
    return Name
}

func (pl *ymca) Filter(ctx context.Context, state *framework.CycleState, pod *v1.Pod, nodeInfo *framework.NodeInfo) *framework.Status {
    log.Println("🥵 开始 Filter")
    // 排除没有gpu=true标签的节点
    if nodeInfo.Node().Labels["gpu"] != "true" {
        return framework.NewStatus(framework.Unschedulable, "Node: "+nodeInfo.Node().Name)
    }

    return framework.NewStatus(framework.Success, "Node: "+nodeInfo.Node().Name)
}

func (pl *ymca) Score(ctx context.Context, state *framework.CycleState, pod *v1.Pod, nodeName string) (int64, *framework.Status) {
    log.Println("🔢 对节点进行评分:", nodeName)

    oneGpu := Getgpuinfo(0);
    
    for i:=1; i<=100; i++ {
        
        if oneGpu.hostName == nodeName {
            break
        }

        oneGpu = Getgpuinfo(i) // 再次获取 GPU 资源
        log.Println(oneGpu.hostName)
        //time.Sleep(time.Second)
    }

    if oneGpu.hostName != nodeName {
        return 0, framework.NewStatus(framework.Success)
    }

    log.Println("")
    log.Printf("🖥️ 主机名: %s\n", oneGpu.hostName)
    log.Printf("🖥️ GPU 型号: %s\n", oneGpu.gpuModel)
    log.Printf("🏋️ GPU 利用率: %s%%\n", oneGpu.gpuUtil)
    log.Printf("🏋️ GPU 显存已用: %d / %d\n", oneGpu.gpuFbUsed, oneGpu.gpuFbFree + oneGpu.gpuFbUsed)
    
    return int64(100.0 * oneGpu.gpuFbFree / (oneGpu.gpuFbFree + oneGpu.gpuFbUsed)), framework.NewStatus(framework.Success)
}

func (pl *ymca) ScoreExtensions() framework.ScoreExtensions {
    return pl  // 这里必须返回自己
}

func (pl *ymca) NormalizeScore(ctx context.Context, state *framework.CycleState, pod *v1.Pod, scores framework.NodeScoreList) *framework.Status {
    return nil  // 空实现但仍需要保留
}
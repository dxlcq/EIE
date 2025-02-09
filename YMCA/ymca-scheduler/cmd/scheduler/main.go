package main

import (
    "os"

    "dxlcq.cn/ymca-scheduler/pkg/gpuscheduling" // 自定义插件

    "k8s.io/component-base/cli"                // 用于执行命令
    "k8s.io/kubernetes/cmd/kube-scheduler/app" // 调度器的入口
)

func main() {

    command := app.NewSchedulerCommand(         // 创建调度器
        app.WithPlugin(gpuscheduling.Name, gpuscheduling.New), // 注册自定义插件
    )

    code := cli.Run(command)                    // 执行命令
    os.Exit(code)
}
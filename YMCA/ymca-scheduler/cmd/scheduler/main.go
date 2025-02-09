package main

import (
    "os"

    "dxlcq.cn/ymca-scheduler/pkg/gpuscheduling" // 自定义插件

    "k8s.io/component-base/cli"                // 用于执行命令
    "k8s.io/kubernetes/cmd/kube-scheduler/app" // 调度器的入口
)

// make && sudo ./scheduler --kubeconfig=/root/.kube/config --secure-port=10086 --authentication-kubeconfig=/root/.kube/config --authorization-kubeconfig=/root/.kube/config --leader-elect=false

// GOOS=linux GOARCH=amd64 go build -o ymca-scheduler cmd/scheduler/main.go

func main() {

    command := app.NewSchedulerCommand(         // 创建调度器
        app.WithPlugin(gpuscheduling.Name, gpuscheduling.New), // 注册自定义插件
    )

    code := cli.Run(command)                    // 执行命令
    os.Exit(code)
}
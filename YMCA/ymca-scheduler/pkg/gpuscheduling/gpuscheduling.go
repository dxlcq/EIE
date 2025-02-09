package gpuscheduling

import (
    "context"
    "log"

    "k8s.io/api/core/v1"
    "k8s.io/apimachinery/pkg/runtime"
    "k8s.io/kubernetes/pkg/scheduler/framework"
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
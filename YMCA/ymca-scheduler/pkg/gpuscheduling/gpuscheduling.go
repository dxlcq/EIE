package gpuscheduling

import (
    "context"
    "log"

    "k8s.io/apimachinery/pkg/runtime"
    "k8s.io/kubernetes/pkg/scheduler/framework"
)

const Name = "ymca-scheduler"

type ymca struct{}

func New(_ context.Context, _ runtime.Object, _ framework.Handle) (framework.Plugin, error) {
    log.Println("🥵 终于是 new 上了")
    return &ymca{}, nil
}

func (pl *ymca) Name() string {
    return Name
}
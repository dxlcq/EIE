package gpuscheduling

import (
    "fmt"
    "io"
    "net/http"
    "regexp"
    "strconv"
)

type Gpu struct {
    hostName string
    gpuModel string
    gpuUtil  string
    gpuFbUsed int
    gpuFbFree int
}

func Getgpuinfo(i int) Gpu {
    // 随机访问一个 GPU 监控指标的地址
    url := []string{"http://180.85.207.42:30049/metrics",
                    "http://180.85.207.45:30049/metrics",
                    "http://180.85.207.27:30049/metrics",
                    "http://180.85.207.104:30049/metrics"}

    // 发送 GET 请求
    resp, err := http.Get(url[i%4])
    if err != nil {
        fmt.Printf("请求失败: %v\n", err)
        return Gpu{}
    }
    defer resp.Body.Close() // 确保关闭响应体

    //fmt.Printf("响应状态码: %d\n", resp.StatusCode)

    // 读取响应体
    body, err := io.ReadAll(resp.Body)
    if err != nil {
        fmt.Printf("读取响应体失败: %v\n", err)
        return Gpu{}
    }
    //fmt.Printf("响应体: %s\n", body)

    gpu := Gpu{}

    // 通过正则表达式提取信息
    hostNameRegexp := regexp.MustCompile(`Hostname="(.+?)"`)
    gpu.hostName = hostNameRegexp.FindStringSubmatch(string(body))[1]
    gpuModelRegexp := regexp.MustCompile(`modelName="(.+?)"`)
    gpu.gpuModel = gpuModelRegexp.FindStringSubmatch(string(body))[1]
    gpuUtilRegexp := regexp.MustCompile(`DCGM_FI_DEV_GPU_UTIL\{[^}]*}\s+(\d+)`)
    gpu.gpuUtil = gpuUtilRegexp.FindStringSubmatch(string(body))[1]
    
    fbUsedRegexp := regexp.MustCompile(`DCGM_FI_DEV_FB_USED\{[^}]*}\s+(\d+)`)
    gpuFbUsed := fbUsedRegexp.FindStringSubmatch(string(body))[1]
    gpu.gpuFbUsed, err = strconv.Atoi(gpuFbUsed)
    if err != nil {
        fmt.Printf("字符串转整数失败: %v\n", err)
        return Gpu{}
    }
    
    fbFreeRegexp := regexp.MustCompile(`DCGM_FI_DEV_FB_FREE\{[^}]*}\s+(\d+)`)
    gpuFbFree := fbFreeRegexp.FindStringSubmatch(string(body))[1]
    gpu.gpuFbFree, err = strconv.Atoi(gpuFbFree)
    if err != nil {
        fmt.Printf("字符串转整数失败: %v\n", err)
        return Gpu{}
    }
    
    //fmt.Printf("主机名: %s\n", gpu.hostName)
    //fmt.Printf("GPU型号: %s\n", gpu.gpuModel)
    //fmt.Printf("GPU利用率: %s%%\n", gpu.gpuUtil)
    //fmt.Printf("GPU显存已用: %d / %d\n", gpu.gpuFbUsed, gpu.gpuFbFree)

    return gpu
}
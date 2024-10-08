#pragma once

#include <pcl/point_types.h>
#include <pcl/point_cloud.h>

namespace lio_ndt
{
    class CloudData
    {
    public:
        using POINT = pcl::PointXYZ; // 三维点
        using CLOUD = pcl::PointCloud<POINT>; // 多个三维点组成点云容器
        using CLOUD_PTR = CLOUD::Ptr; // 指向点云容器的指针

    public:
        // cloud_ptr(new CLOUD()) 等价于 cloud_ptr = new CLOUD 给cloud_ptr 分配内存
        // cloud_ptr后面就是指向点云容器的指针   cloud_ptr是后面用到的
        CloudData() : cloud_ptr(new CLOUD()){}

    public:
        double time = 0.0;
        CLOUD_PTR cloud_ptr;
    };
}



#pragma once

#include <ros/ros.h>                            // ROS 系统的头文件
#include <sensor_msgs/PointCloud2.h>            // 用于 ROS 系统中的点云数据格式
#include <pcl_conversions/pcl_conversions.h>    // PCL 与 ROS 数据格式转换的头文件
#include <pcl/point_cloud.h>                    // PCL 的点云处理库
#include <pcl/point_types.h>                    // PCL 中不同类型的点定义
#include <iostream>

class my_lidar{
private:
    // PCL 中的点云数据格式
    pcl::PointCloud<pcl::PointXYZ>::Ptr point_cloud_ptr;
    // 点云数据是否准备好
    bool point_cloud_ready;
public:
    my_lidar();
    ~my_lidar();

    // 当 ROS 中的点云数据到来时，调用此函数
    void point_cloud_callback(const sensor_msgs::PointCloud2ConstPtr &cloud_msg);
    // 判断点云数据是否准备好
    bool is_point_cloud_ready() const;
    // 获取点云数据
    pcl::PointCloud<pcl::PointXYZ>::Ptr get_point_cloud_ptr() const;
};
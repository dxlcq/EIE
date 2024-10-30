#pragma once

#include <ros/ros.h>            // ROS 系统的头文件
#include <sensor_msgs/Image.h>  // 用于 ROS 系统中的图像数据格式
#include <cv_bridge/cv_bridge.h>// OpenCV 与 ROS 数据格式转换的头文件
#include <opencv2/opencv.hpp>   // OpenCV 头文件

class my_camera{
private:
    // OpenCV 中的图像数据格式
    cv_bridge::CvImagePtr cv_image_ptr;
    // 图像数据是否准备好
    bool image_ready;
public:
    my_camera();
    ~my_camera();

    // 当 ROS 中的图像数据到来时，调用此函数
    void image_callback(const sensor_msgs::ImageConstPtr &image_msg);
    // 判断图像数据是否准备好
    bool is_image_ready() const;
    // 获取图像数据
    cv_bridge::CvImagePtr get_image_ptr() const;
};
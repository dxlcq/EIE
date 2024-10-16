#pragma once

#include <ros/ros.h>
#include <opencv2/opencv.hpp>
#include <Eigen/Dense>
#include <string>

class my_config{
private:
    std::string file_path;  // 配置文件路径
public:
    cv::Mat extrinsic_mat, camera_mat, dist_coeff;      // 相机外参、内参、畸变系数
    cv::Mat rotate_mat, translate_mat;                  // 旋转矩阵、平移矩阵
    Eigen::Matrix4d transform_mat, inv_transform_mat;   // 变换矩阵、逆变换矩阵

    my_config(const std::string &file_path="");
    ~my_config();

    // 读取配置文件
    void read_config();
};
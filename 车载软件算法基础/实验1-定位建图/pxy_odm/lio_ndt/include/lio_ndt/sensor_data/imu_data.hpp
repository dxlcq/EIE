#pragma once

#include <eigen3/Eigen/Dense> // 稠密矩阵的代数运算（逆、特征值等）

namespace lio_ndt
{
    class IMUData
    {
    public:
        struct LinearAcceleration // 三轴线加速度
        {
            double x = 0.0;
            double y = 0.0;
            double z = 0.0;
        };

        struct AngularVelocity // 三轴角速度
        {
            double x = 0.0;
            double y = 0.0;
            double z = 0.0;
        };
        
        struct Orientation // 三轴姿态角
        {
            double x = 0.0;
            double y = 0.0;
            double z = 0.0;
            double w = 0.0;
        };
        
        double time = 0.0;
        LinearAcceleration linear_acceleration;
        AngularVelocity angular_velocity;
        Orientation orientation;

    public:
        // 把四元数转换成矩阵旋转矩阵送出去
        Eigen::Matrix3f GetOrientationMatrix()
        {
            Eigen::Quaterniond q(orientation.w, orientation.x, orientation.y, orientation.z);
            Eigen::Matrix3f matrix = q.matrix().cast<float>();
            
            return matrix;
        }
    };
}
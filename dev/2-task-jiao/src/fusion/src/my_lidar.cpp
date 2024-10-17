#include "fusion/my_lidar.hpp"

my_lidar::my_lidar() : point_cloud_ptr(new pcl::PointCloud<pcl::PointXYZ>), point_cloud_ready(false) {
    ROS_INFO("YES! PCL init!");
}

my_lidar::~my_lidar(){  // 析构函数
}

// 当 ROS 中的点云数据到来时，调用此函数
void my_lidar::point_cloud_callback(const sensor_msgs::PointCloud2ConstPtr& cloud_msg){
    try {
        ROS_INFO("YES! PCL callback!");
        // 将 ROS 中的点云数据转换为 PCL 中的点云数据
        pcl::fromROSMsg(*cloud_msg, *(this->point_cloud_ptr));
        // 点云数据准备好
        this->point_cloud_ready = true;
    } catch (std::runtime_error e) {
        ROS_INFO("NO! ERROR ROS -> PCL");
    }
    return ;
}

// 判断点云数据是否准备好
bool my_lidar::is_point_cloud_ready() const {
    return this->point_cloud_ready;
}

// 获取点云数据
pcl::PointCloud<pcl::PointXYZ>::Ptr my_lidar::get_point_cloud_ptr() const {
    return this->point_cloud_ptr;
}
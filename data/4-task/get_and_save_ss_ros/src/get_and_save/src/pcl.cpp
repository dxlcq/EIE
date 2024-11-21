#include <chrono>
#include <sstream>
#include <iomanip>
#include <thread>
#include <mutex>

// ROS 和 PCL 相关头文件
#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h> // 用于从 ROS 点云消息转换为 PCL 点云
#include <pcl/io/pcd_io.h>                   // 用于保存点云数据到 PCD 文件
#include <pcl/visualization/cloud_viewer.h>  // 用于点云可视化

// 创建一个CloudViewer对象
static pcl::visualization::CloudViewer viewer("Point Cloud Viewer");

// 回调函数：处理接收到的点云消息
void pointCloudCallback(const sensor_msgs::PointCloud2ConstPtr& cloud_msg)
{
    // 创建一个新的 PCL 点云对象
    pcl::PointCloud<pcl::PointXYZ>::Ptr cloud(new pcl::PointCloud<pcl::PointXYZ>);

    // 将 ROS 点云消息转换为 PCL 点云
    pcl::fromROSMsg(*cloud_msg, *cloud);

    // 获取当前日期和时间，用于文件名
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_time);

    std::ostringstream oss;
    oss << std::put_time(&now_tm, "%Y%m%d%H%M%S");
    std::string date_time = oss.str();

    // 生成文件名并保存点云
    std::string filename = date_time + ".pcd";
    if (pcl::io::savePCDFileBinary(filename, *cloud) == 0) {
        ROS_INFO("Successfully saved point cloud to %s", filename.c_str());
    } else {
        ROS_ERROR("Failed to save point cloud to %s", filename.c_str());
    }

    // 显示点云
    viewer.showCloud(cloud);
}

int main(int argc, char **argv)
{
    // 初始化 ROS 节点
    ros::init(argc, argv, "pointcloud_recorder_and_viewer");
    ros::NodeHandle nh;

    // 订阅点云话题，替换为您实际使用的话题名称
    ros::Subscriber sub = nh.subscribe("/rslidar_points", 10, pointCloudCallback);

    // 主循环
    ros::spin();
    return 0;
}
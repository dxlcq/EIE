#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>
#include <pcl/point_cloud.h>                 //pcl点云类型头文件
#include <pcl_conversions/pcl_conversions.h> //pcl与ros之间转换头文件
// #include<boost/thread/thread.hpp>//用于点云可视化，该版本不使用
// #include <pcl/visualization/pcl_visualizer.h> //用于点云可视化，提供visualization的namespace，主要是cloud_viewer包含了这个，这里就可以不用再包含了
#include <pcl/visualization/cloud_viewer.h>

static pcl::visualization::CloudViewer viewer("point cloud"); //创建一个pcl的窗口，在外面全局的，只创建一次

//需要获取雷达消息，并且用pcl显示出点云图
void laserCallback(sensor_msgs::PointCloud2ConstPtr msg)
{
    pcl::PointCloud<pcl::PointXYZ>::Ptr lasercloud(new pcl::PointCloud<pcl::PointXYZ>); //创建pcl类型的点云指针，用于存放ros激光雷达消息转换的结果
    pcl::fromROSMsg(*msg, *lasercloud);                                                 //将ros消息转换成pcl点云
    viewer.showCloud(lasercloud);
    return;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "point_cloud_by_pcl");
    ros::NodeHandle n;
    // ros::Subscriber pcl_sub = n.subscribe<sensor_msgs::PointCloud2>("/rslidar_points", 10, laserCallback);
    ros::Subscriber pcl_sub = n.subscribe("/rslidar_points", 10, laserCallback);

    ros::spin();
    return 0;
}
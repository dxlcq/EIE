#pragmea once

#include "fusion/my_config.hpp"
#include "fusion/my_lidar.hpp"
#include "fusion/my_camera.hpp"

#include <vector>

class fusion{
private:
    my_config config;
    my_lidar lidar;
    my_camera camera;

    // ROS 节点
    ros::NodeHandle n;                                      // ROS 节点
    ros::Subscriber lidar_sub, camera_sub;                  // 订阅点云数据、图像数据
    ros::Publisher fusion_pub;                              // 发布融合图

    // 点云初始化
    pcl::PointCloud<pcl::PointXYZ>::Ptr transaction_cloud;  // 点云数据
    bool init_Points3f();                                   // 初始化 3D 点云

    // 颜色信息初始化
    int color[21][3];                                       // 
    float color_dis;                                        // 颜色距离
    void init_Colors();                                     // 初始化颜色信息

    // 点云投影
    std::vector<cv::Point2f> points2d;                      // 投影后的 2D 点
    void point_to_image();                                  // 点云投影到图像

    // 融合图生成
    cv::Mat fusion_image;                                   // 融合图

    // 发布融合图
    sensor_msgs::ImagePtr fusion_msg;                       // 融合图消息
    void publish_fusion();                                  // 发布融合图

public:
    fusion(ros::NodeHandle n);
    ~fusion();

    bool generate();    // 生成融合图
    void process();     // 融合处理
    void publish();     // 发布融合图
    void show();        // 显示融合图

    void fusion_process();
}
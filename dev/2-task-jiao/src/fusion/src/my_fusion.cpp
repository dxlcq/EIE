#include "fusion/my_function.hpp"

fusion::fusion(ros::NodeHandle n): 
    // 初始化点云指针
    this->transaction_cloud(new pcl::PointCloud<pcl::PointXYZ>),
    // 初始化 ROS 节点
    this->n(n),
    this->fusion_pub(n.advertise<sensor_msgs::Image>("/fusion/image", 100)),
    this->lidar_sub(n.subscribe("/rslidar_points", 10, &my_lidar::point_cloud_callback, &(this->lidar))),
    this->camera_sub(n.subscribe("/camera/color/image_raw", 10, &my_camera::image_callback, &(this->camera))),
    // 初始化颜色距离和配置
    this->color_dis(1.2),
    this->config(){
    // 初始化颜色表
    int temp_color[21][3] = {{255, 0, 0}, {255, 69, 0}, {255, 99, 71}, {255, 140, 0}, {255, 165, 0}, {238, 173, 14}, {255, 193, 37}, {255, 255, 0}, {255, 236, 139}, {202, 255, 112}, {0, 255, 0}, {84, 255, 159}, {127, 255, 212}, {0, 229, 238}, {152, 245, 255}, {178, 223, 238}, {126, 192, 238}, {28, 134, 238}, {0, 0, 255}, {72, 118, 255}, {122, 103, 238}};
    std::copy(&temp_color[0][0], &temp_color[0][0] + 21 * 3, &this->color[0][0]);

    ROS_INFO("YES! Fusion init!");    
}

fusion::~fusion(){}

bool fusion::init_Points3f(){
    if(!this->lidar.is_point_cloud_ready()){
        ROS_INFO("NO! Lidar point cloud not ready!");
        return false;
    }
    this->points3d.reserve(this->lidar.get_point_cloud_ptr()->points.size() + 1);
    cv::Point3f point;
    for(auto &p : this->lidar.get_point_cloud_ptr()->points){
        point.x = p.x;
        point.y = p.y;
        point.z = p.z;
        this->points3d.push_back(point);
    }
    return true;
}

void fusion::init_Colors(){
    this->dis_color.clear();
    this->dis_color.reserve(this->car_lidar.get_cloud_ptr()->size() + 1);
    for (size_t i = 0; i < this->transformed_cloud->points.size(); i++){
        if (this->transformed_cloud->points[i].z > 0){
            int color_order = int(this->transformed_cloud->points[i].z / this->color_dis);
            if (color_order > 20){
                color_order = 20;
            }
            this->dis_color.push_back(cv::Scalar(this->color[color_order][2], this->color[color_order][1], this->color[color_order][0]));
        }
    }
    return;
}
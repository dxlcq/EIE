#include "fusion/my_camera.hpp"

my_camera::my_camera() : cv_image_ptr(new cv_bridge::CvImage), image_ready(false) {
    ROS_INFO("YES! OpenCV init!");
}

my_camera::~my_camera(){  // 析构函数
}

// 当 ROS 中的图像数据到来时，调用此函数
void my_camera::image_callback(const sensor_msgs::ImageConstPtr &image_msg){
    try {
        ROS_INFO("YES! OpenCV callback!");
        // 将 ROS 中的图像数据转换为 OpenCV 中的图像数据
        this->cv_image_ptr = cv_bridge::toCvCopy(image_msg, sensor_msgs::image_encodings::BGR8);
        // 图像数据准备好
        this->image_ready = true;
    } catch (cv_bridge::Exception e) {
        ROS_INFO("NO! ERROR ROS -> OpenCV");
    }
    return ;
}

// 判断图像数据是否准备好
bool my_camera::is_image_ready() const {
    return this->image_ready;
}

// 获取图像数据
cv_bridge::CvImagePtr my_camera::get_image_ptr() const {
    return this->cv_image_ptr;
}
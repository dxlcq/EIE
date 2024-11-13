#include <cv_bridge/cv_bridge.h>         // 用于转换 ROS 图像到 OpenCV
#include <opencv2/highgui/highgui.hpp>   // 图像交互界面，显示图像
#include <ros/ros.h>                     // ROS 依赖
#include <sensor_msgs/image_encodings.h> // 获取 ROS 图像
#include <string>

static const std::string IMG_WINDOW = "img";

void img(const sensor_msgs::ImageConstPtr &msg) {
  // 创建一个指向 OpenCV 图像的指针
  cv_bridge::CvImagePtr cv_image_ptr;

  try {
    // 处理 RGB 图像
    if (msg->encoding == sensor_msgs::image_encodings::RGB8 ||
        msg->encoding == sensor_msgs::image_encodings::BGR8 ||
        msg->encoding == sensor_msgs::image_encodings::MONO8) {
      // 将 ROS 图像转换为 OpenCV 格式
      cv_image_ptr =
          cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
      cv::imshow(IMG_WINDOW, cv_image_ptr->image);
    } else {
      ROS_ERROR("Unsupported image encoding: %s", msg->encoding.c_str());
    }
    cv::waitKey(1);
  } catch (cv_bridge::Exception &e) {
    ROS_ERROR("cv_bridge exception: %s", e.what());
  }

  return;
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "img");
  ros::NodeHandle n;

  // 创建要显示的窗口
  cv::namedWindow(IMG_WINDOW, cv::WINDOW_AUTOSIZE);

  // 订阅图像话题，注册回调函数，每 10 毫秒调用一次
  ros::Subscriber camera_sub = n.subscribe("/camera/color/image_raw", 10, img);

  // 循环等待回调函数
  ros::spin();

  // 关闭所有 OpenCV 窗口
  cv::destroyAllWindows();
  return 0;
}
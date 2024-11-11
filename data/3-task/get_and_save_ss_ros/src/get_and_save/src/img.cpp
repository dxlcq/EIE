#include <chrono>
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

    // 获取当前日期及时间，精确到毫秒
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm now_tm = *std::localtime(&now_time);

    std::ostringstream oss;
    oss << std::put_time(&now_tm, "%Y%m%d%H%M%S");
    std::string date_time = oss.str();

    // 保存图像至当前目录
    cv::imwrite(date_time + ".jpg", cv_image_ptr->image);

    ROS_INFO("Saved image %s.jpg", date_time.c_str());

    // 每 1 秒刷新一次窗口
    cv::waitKey(1000);
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

  // 订阅图像话题，注册回调函数，消息队列长度为 10
  ros::Subscriber camera_sub = n.subscribe("/camera/color/image_raw", 10, img);

  // 循环等待回调函数
  ros::spin();

  // 关闭所有 OpenCV 窗口
  cv::destroyAllWindows();
  return 0;
}
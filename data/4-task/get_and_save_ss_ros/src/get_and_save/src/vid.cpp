#include <chrono>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/highgui/highgui.hpp>
#include <ros/ros.h>
#include <sensor_msgs/image_encodings.h>
#include <string>

static const std::string IMG_WINDOW = "img";
cv::VideoWriter video_writer;

void img(const sensor_msgs::ImageConstPtr &msg) {
  cv_bridge::CvImagePtr cv_image_ptr;

  try {
    if (msg->encoding == sensor_msgs::image_encodings::RGB8 ||
        msg->encoding == sensor_msgs::image_encodings::BGR8 ||
        msg->encoding == sensor_msgs::image_encodings::MONO8) {
      cv_image_ptr =
          cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
      cv::imshow(IMG_WINDOW, cv_image_ptr->image);

      // 将当前帧写入视频文件
      if (video_writer.isOpened()) {
        video_writer.write(cv_image_ptr->image);
      } else {
        ROS_ERROR("VideoWriter未打开");
      }

    } else {
      ROS_ERROR("不支持的图像编码: %s", msg->encoding.c_str());
    }

    // 等待1毫秒（根据需要调整）
    cv::waitKey(1);
  } catch (cv_bridge::Exception &e) {
    ROS_ERROR("cv_bridge异常: %s", e.what());
  }

  return;
}

int main(int argc, char **argv) {
  ros::init(argc, argv, "img");
  ros::NodeHandle n;

  // 创建显示窗口
  cv::namedWindow(IMG_WINDOW, cv::WINDOW_AUTOSIZE);

  // 初始化VideoWriter
  int frame_width = 640;  // 根据您的摄像头分辨率进行调整
  int frame_height = 480; // 根据您的摄像头分辨率进行调整
  int fps = 30;           // 每秒帧数

  // 获取当前日期和时间用于视频文件名
  auto now = std::chrono::system_clock::now();
  std::time_t now_time = std::chrono::system_clock::to_time_t(now);
  std::tm now_tm = *std::localtime(&now_time);

  std::ostringstream oss;
  oss << std::put_time(&now_tm, "%Y%m%d%H%M%S");
  std::string date_time = oss.str();

  std::string video_filename = date_time + ".avi";

  // 打开视频写入器
  video_writer.open(video_filename,
                    cv::VideoWriter::fourcc('M', 'J', 'P', 'G'),
                    fps, cv::Size(frame_width, frame_height), true);

  if (!video_writer.isOpened()) {
    ROS_ERROR("无法打开输出视频文件进行写入");
    return -1;
  }

  ROS_INFO("正在录制视频到 %s", video_filename.c_str());

  // 订阅图像话题
  ros::Subscriber camera_sub =
      n.subscribe("/camera/color/image_raw", 10, img);

  // 循环等待回调
  ros::spin();

  // 释放资源
  video_writer.release();
  cv::destroyAllWindows();
  return 0;
}
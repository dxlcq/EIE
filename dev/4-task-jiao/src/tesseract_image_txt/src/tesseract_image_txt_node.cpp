#include <ros/ros.h>
#include <sensor_msgs/Image.h>
#include <cv_bridge/cv_bridge.h>
#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>
#include <std_msgs/String.h>

ros::Publisher text_publisher;  // 声明为全局变量

// 回调函数，处理相机图像
void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
  try
  {
    // 将ROS图像消息转换为OpenCV图像格式
    cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    
    // 在OpenCV图像上进行文本识别
    tesseract::TessBaseAPI tess;
    tess.Init(NULL, "eng", tesseract::OEM_DEFAULT);
    tess.SetImage(cv_ptr->image.data, cv_ptr->image.cols, cv_ptr->image.rows, 3, cv_ptr->image.step);
    tess.Recognize(0);
    const char* recognizedText = tess.GetUTF8Text();

    // 修正识别结果中的空格和不可见字符
    std::string recognizedStr(recognizedText);
    recognizedStr.erase(std::remove_if(recognizedStr.begin(), recognizedStr.end(), ::isspace), recognizedStr.end());
    
    // 打印识别结果
    ROS_INFO("%s", recognizedText);
    
    // 发布识别结果
    std::string publishText;
    if (recognizedStr == "A" || recognizedStr == "B" ||
        recognizedStr == "C" || recognizedStr == "D")
    {
      ROS_INFO("good %s", recognizedStr.c_str());
      publishText = recognizedStr;
    }
    else
    {
      ROS_INFO("publish E");
      publishText = "E";
    }

    std_msgs::String text_msg;
    text_msg.data = publishText;
    text_publisher.publish(text_msg);

    // 释放Tesseract资源
    tess.End();
  }
  catch (cv_bridge::Exception& e)
  {
    ROS_ERROR("cv_bridge exception: %s", e.what());
  }
}

int main(int argc, char** argv)
{
  // 初始化ROS节点
  ros::init(argc, argv, "text_recognition_node");
  ros::NodeHandle nh;

  // 订阅相机图像话题
  ros::Subscriber image_subscriber = nh.subscribe("/camera/color/image_raw", 1, imageCallback);

  // 创建发布者对象
  text_publisher = nh.advertise<std_msgs::String>("/text_recognition/result", 10);

  // 循环等待回调
  ros::Rate loop_rate(10);  // 设置循环频率为10Hz

  while (ros::ok())
  {
    // 处理回调函数
    ros::spinOnce();

    // 按照循环频率进行休眠
    loop_rate.sleep();
  }

  return 0;
}

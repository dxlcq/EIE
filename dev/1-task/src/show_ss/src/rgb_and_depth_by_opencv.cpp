#include <ros/ros.h> //ros依赖
// #include <image_transport/image_transport.h>//目前不需要
#include <cv_bridge/cv_bridge.h>         //需要用这个转换ros->opencv
#include <sensor_msgs/image_encodings.h> //获取ros图像
// #include <opencv2/imgproc/imgproc.hpp>//目前不需要
#include <opencv2/highgui/highgui.hpp> //图像交互界面，显示图像
#include <string>

static const std::string RGB_WINDOW = "RGB Image";
static const std::string DEPTH_WINDOW = "Depth Image";

void ImageConvert_and_Show(const sensor_msgs::ImageConstPtr &msg, const std::string &title)
{
	cv_bridge::CvImagePtr cv_image_ptr;

	try {
    		// 区分RGB图像和深度图像
    	if (msg->encoding == sensor_msgs::image_encodings::RGB8) {
			// 处理 RGB 图像
        	cv_image_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
        	cv::imshow("RGB Image", cv_image_ptr->image);
    	}
    	else if (msg->encoding == sensor_msgs::image_encodings::TYPE_16UC1) {
			// 处理深度图像
        	cv_image_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::TYPE_16UC1);
        
        	// 将深度图像归一化以便可视化
        	cv::Mat depth_image_normalized;
        	cv::normalize(cv_image_ptr->image, depth_image_normalized, 0, 255, cv::NORM_MINMAX, CV_8UC1);
        	cv::imshow("Depth Image", depth_image_normalized);
    	}
    	else {
        	ROS_ERROR("Unsupported image encoding: %s", msg->encoding.c_str());
    	}

    	cv::waitKey(1);
	} 
	catch (cv_bridge::Exception& e) {
    	ROS_ERROR("cv_bridge exception: %s", e.what());
	}
	/**
    * c从sensor_msgs/Image转换成CvImage
    * cv_bridge::CvImagePtr cv_image_ptr; //用来存放ros图像信息转换成的OpenCV格式的图像信息
    * cv_image_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8); //选择常用的编码格式，将ros图像信息转成OpenCV所期望的图像编码格式。
    * cv_image_ptr = cv_bridge::toCvCopy(msg); //由于编码问题，这里选择不指定编码，让toCvCopy自己选择合适的编码
	* cv_image_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::TYPE_16UC1);    
    * 把image显示出来。
    * cv::imshow(title, cv_image_ptr->image);
    * if (cv::waitKey(1) == 27) //设置等待时间为1ms
    * {
    *     ros::shutdown();
    *     cv::destroyAllWindows();
    * } 
	* 按esc退出
	**/    
	return;
}
void color_show(const sensor_msgs::ImageConstPtr &msg)
{
    ImageConvert_and_Show(msg, RGB_WINDOW);
    return;
}
void depth_show(const sensor_msgs::ImageConstPtr &msg)
{
    ImageConvert_and_Show(msg, DEPTH_WINDOW);
    return;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "show_rgb_and_depth_graph");
    ros::NodeHandle n;

    //创建要显示的两个窗口
    cv::namedWindow(RGB_WINDOW);
    cv::namedWindow(DEPTH_WINDOW);

    ros::Subscriber camera_color_sub = n.subscribe("/camera/color/image_raw", 10, color_show);
    ros::Subscriber camera_depth_sub = n.subscribe("/camera/depth/image_rect_raw", 10, depth_show);
    // ros::Subscriber camera_depth_sub = n.subscribe("/camera/depth/image_raw", 10, depth_show);//这个话题下的不能显示

    ros::spin();
    return 0;
}

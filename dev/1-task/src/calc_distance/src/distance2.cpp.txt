#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
// #include <math.h>
#include <iostream>
using namespace std;

#include <queue>

double t_recent = 0;
double s = 0;
std::queue<double> velocity_queue; // 滑动平均队列
const int window_size = 5;          // 滑动窗口大小

double calculateMovingAverage(std::queue<double> &queue, double new_value) {
    queue.push(new_value);
    if (queue.size() > window_size) {
        queue.pop();
    }
    double sum = 0;
    std::queue<double> temp_queue = queue;
    while (!temp_queue.empty()) {
        sum += temp_queue.front();
        temp_queue.pop();
    }
    return sum / queue.size();
}

void distanceCallback(const nav_msgs::Odometry::ConstPtr &msg)
{
    double dt;
    double ds;
    double v;

    // 获取当前时间戳
    double current_time = msg->header.stamp.toSec();
    
    // 计算时间差
    dt = current_time - t_recent;
    t_recent = current_time;

    // 获取速度
    v = msg->twist.twist.linear.x;

    // 处理无效的时间差，使用前一个有效的时间差或最小值
    if (dt > 1 || dt < 0)
    {
        dt = 0;
    }

    // 滑动平均滤波
    double filtered_v = calculateMovingAverage(velocity_queue, v);

    // 根据滤波后的速度计算校正的距离
    ds = filtered_v * dt * 1.0504; // 根据建立的线性模型进行校正
    s += ds;

    ROS_INFO("=%0.6fm/s (filtered), s=%0.6fm", filtered_v, s);
} 

int main(int argc, char **argv)
{
    ros::init(argc, argv, "speed_subscriber");
    ros::NodeHandle n;
    /*
    使用
    ```bash
    roslaunch scout_bringup open_rslidar.launch
    ```
    启动激光雷达，就可以获取/odom消息
    */
    s = 0;
    t_recent = ros::Time::now().toSec();
    ROS_INFO("init s=%f,t_recent=%0.6fs", s, t_recent);
    ros::Subscriber speed_sub = n.subscribe("/odom", 10, distanceCallback);
    ros::spin();
    return 0;
}
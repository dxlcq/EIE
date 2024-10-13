#include <ros/ros.h>
#include <nav_msgs/Odometry.h>
// #include <math.h>
#include <iostream>
using namespace std;

double t_recent;

double s;

void distanceCallback(const nav_msgs::Odometry::ConstPtr &msg)
{
    double dt;
    double ds;
    double v;
    //读取激光雷达相关的话题/odom，可以获取小车速度和采集时间
    dt = msg->header.stamp.toSec() - t_recent;
    t_recent = msg->header.stamp.toSec();

    v = msg->twist.twist.linear.x;
    if (dt > 1 || dt < 0)
    { //消息间隔太远则说明该时间差无效，这里直接舍去
        dt = 0;
    }
    ds = v * dt * 1.0504; //根据建立的线性模型进行校正
    s += ds;
    ROS_INFO("v=%0.6fm/s,s=%0.6fm", v, s);
    return;
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
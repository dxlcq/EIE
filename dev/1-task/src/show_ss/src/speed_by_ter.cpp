#include <ros/ros.h>
#include <nav_msgs/Odometry.h>

void speedCallback(const nav_msgs::Odometry::ConstPtr &msg)
{
    //读取激光雷达相关的话题/odom，可以获取小车的线速度和角速度
    ROS_INFO("Car linear velocity: vx=%0.6f,vy=%0.6f,vz=%0.6f", msg->twist.twist.linear.x, msg->twist.twist.linear.y, msg->twist.twist.linear.z);
    ROS_INFO("Car angular velocity: wx=%0.6f,wy=%0.6f,wz=%0.6f", msg->twist.twist.angular.x, msg->twist.twist.angular.y, msg->twist.twist.angular.z);
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
    ros::Subscriber speed_sub = n.subscribe("/odom", 10, speedCallback);
    ros::spin();
    return 0;
}
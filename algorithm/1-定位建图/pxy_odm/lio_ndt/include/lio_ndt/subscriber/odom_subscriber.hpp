#pragma once 

#include <ros/ros.h>
#include <nav_msgs/Odometry.h>

#include <fstream>
#include <string>


namespace lio_ndt 
{
    class OdometrySubscriber
    {
    public:
        OdometrySubscriber(ros::NodeHandle &nh, std::string topic_name, int buff_size, std::string output_file);
        OdometrySubscriber() = default;
        ~OdometrySubscriber();


    private:
        void msg_callback( const nav_msgs::Odometry::ConstPtr & msg );

        std::ofstream output_file_;
        ros::NodeHandle nh_;
        ros::Subscriber subscriber_;
        nav_msgs::Odometry odometry_;
    
    };

}
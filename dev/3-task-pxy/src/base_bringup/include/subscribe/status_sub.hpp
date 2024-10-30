#pragma once

#include <ros/ros.h>

#include <string>

#include "base_msgs/ScoutStatus.h"

namespace EIE_robot
{
    class Status_Sub
    {
    public:
        Status_Sub( const ros::NodeHandle& nh, const std::string& topic_name, const int& buff_size );
        ~Status_Sub();

    private:
        ros::NodeHandle nh_;
        ros::Subscriber status_sub_;

        void status_callback( const base_msgs::ScoutStatus::ConstPtr& msg );

    };
}
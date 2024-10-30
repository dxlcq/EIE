#pragma once

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

#include <string>

#include "ugv_sdk/tracer/tracer_base.hpp"
#include "ugv_sdk/tracer/tracer_types.hpp"


namespace EIE_robot
{
    class Motion_Sub
    {
    public:
        Motion_Sub( const ros::NodeHandle& nh, const std::string& topic_name, const int& buff_size, westonrobot::TracerBase *EIE_control );
        ~Motion_Sub();

    private:
        ros::NodeHandle nh_;
        ros::Subscriber motion_sub_;

        westonrobot::TracerBase *EIE_contral_;

        void motion_callback( const geometry_msgs::Twist::ConstPtr& msg );

    };
}

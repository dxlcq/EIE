#pragma once

#include <ros/ros.h>

#include <string>

#include "ugv_sdk/tracer/tracer_base.hpp"
#include "ugv_sdk/tracer/tracer_types.hpp"

#include "base_msgs/ScoutLightCmd.h"

namespace EIE_robot
{
    class Light_Sub
    {
    public:
        Light_Sub( const ros::NodeHandle& nh, const std::string& topic_name, const int& buff_size, westonrobot::TracerBase *EIE_contral );
        ~Light_Sub();

    private:
        ros::NodeHandle nh_;
        ros::Subscriber light_sub_;

        westonrobot::TracerBase *EIE_control_;

        void light_callback( const base_msgs::ScoutLightCmd::ConstPtr& msg );

    };
}

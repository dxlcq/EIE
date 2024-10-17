/**
 * @file EIE_base.hpp
 * @brief 松灵小车底盘驱动类
 * 
*/
#pragma once


#include <iostream>
#include <string>
#include <memory>

#include <ros/ros.h>
#include <geometry_msgs/Twist.h>

#include "ugv_sdk/scout/scout_base.hpp"
#include "ugv_sdk/scout/scout_types.hpp"

namespace EIE_robot
{
    class EIE_base
    {
        public:

        private:
            /* 地盘控制基类和句柄 */
            std::unique_ptr<westonrobot::ScoutBase> EIE_base_ptr_;
            std::unique_ptr<ros::NodeHandle>        nh_;

            /* 话题 */
            ros::Publisher  status_pub_;
            ros::Subscriber motion_sub_;
            ros::Subscriber light_sub_;

            /* 话题回调函数 */
            void MotionCmdCallback( const geometry_msgs::Twist::ConstPtr &msg );
            // void LightCmdCallback( const  &msg );
    };

}
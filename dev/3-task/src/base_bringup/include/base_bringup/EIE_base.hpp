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
#include "ugv_sdk/tracer/tracer_base.hpp"
#include "ugv_sdk/tracer/tracer_types.hpp"


#include "base_msgs/ScoutStatus.h"
#include "base_msgs/ScoutLightCmd.h"

namespace EIE_robot
{
    class EIE_base
    {
        public:
            explicit EIE_base( ros::NodeHandle &nh );
            EIE_base(   std::string device_name,
                        int32_t baud_rate,
                        ros::NodeHandle &nh );

            void SetupSubscription();

            void PublishStatus();

        private:
            /* 地盘控制基类和句柄 */
            westonrobot::ScoutBase  EIE_status_;
            westonrobot::TracerBase EIE_contral_;
            ros::NodeHandle         nh_;

            /* 话题 */
            ros::Publisher  status_pub_;
            ros::Subscriber motion_cmd_sub_;
            ros::Subscriber light_cmd_sub_;
            ros::Subscriber status_sub_;

            /* 时间 */
            ros::Time last_time_;
            ros::Time current_time_;

            /* 话题回调函数 */
            void MotionCmdCallback( const geometry_msgs::Twist::ConstPtr     &msg );
            void LightCmdCallback(  const base_msgs::ScoutLightCmd::ConstPtr &msg );
            void StatusCallback(    const base_msgs::ScoutStatus::ConstPtr   &msg );
    };

}
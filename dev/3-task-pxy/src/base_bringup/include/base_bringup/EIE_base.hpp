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

#include "subscribe/motion_sub.hpp"
#include "subscribe/light_sub.hpp"
#include "subscribe/status_sub.hpp"
#include "publisher/status_pub.hpp"

#include "base_msgs/ScoutStatus.h"
#include "base_msgs/ScoutLightCmd.h"

namespace EIE_robot
{
    class EIE_base
    {
        public:
            explicit EIE_base( ros::NodeHandle &nh );
            EIE_base(   const std::string& device_name,
                        const int32_t& baud_rate,
                        ros::NodeHandle &nh );

            void SetupSubscription( const ros::NodeHandle& nh );

            void PublishStatus();

        private:
            /* 地盘控制基类和句柄 */
            // westonrobot::ScoutBase  EIE_status_;
            westonrobot::TracerBase EIE_contral_;
            // std::unique_ptr<westonrobot::ScoutBase> EIE_status_;
            ros::NodeHandle         nh_;

            /* 话题 */
            std::shared_ptr<EIE_robot::Motion_Sub> motion_sub_;
            std::shared_ptr<EIE_robot::Light_Sub>  light_sub_;
            std::shared_ptr<EIE_robot::Status_Sub> status_sub_;
            std::shared_ptr<EIE_robot::Status_pub> status_pub_;

            std::string device_name_;
            int32_t baud_rate_;

    };

}
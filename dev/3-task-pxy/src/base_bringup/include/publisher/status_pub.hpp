#pragma once

#include <ros/ros.h>

#include <string>
#include <memory>

#include "ugv_sdk/scout/scout_base.hpp"
#include "ugv_sdk/scout/scout_types.hpp"


namespace EIE_robot
{
    class Status_pub
    {
        public:
            Status_pub( const ros::NodeHandle& nh, const std::string& topic_name, const int& buff_size, const std::string& device_name, const int32_t& baud_rate );
            ~Status_pub();

            void PublishStatus();

        private:
            ros::NodeHandle nh_;
            ros::Publisher status_pub_;

            // westonrobot::ScoutBase  EIE_status_;
            std::unique_ptr<westonrobot::ScoutBase> EIE_status_ = std::make_unique<westonrobot::ScoutBase>();

            /* 时间 */
            ros::Time last_time_;
            ros::Time current_time_;

    };
}
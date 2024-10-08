#pragma once

#include <deque>
#include <ros/ros.h>
#include <sensor_msgs/NavSatFix.h>
#include <lio_ndt/sensor_data/gnss_data.hpp>

namespace lio_ndt
{
    class GNSSSubscriber
    {
    public:
        GNSSSubscriber(ros::NodeHandle& nh, std::string topic_name, size_t buff_size);
        GNSSSubscriber() = default;
        void ParseData(std::deque<GNSSData>& deque_gnss_data);

    private:
        void msg_callback(const sensor_msgs::NavSatFixConstPtr& nav_sat_fix_ptr); // 声明订阅GNSS信息回调函数
        
    private:
        ros::NodeHandle nh_;
        ros::Subscriber subscriber_;

        std::deque<GNSSData> new_gnss_data_;
    };
}
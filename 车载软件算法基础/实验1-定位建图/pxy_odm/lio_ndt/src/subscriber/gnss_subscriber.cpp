#include <lio_ndt/subscriber/gnss_subscriber.hpp>

#include <glog/logging.h>

namespace lio_ndt
{
    GNSSSubscriber::GNSSSubscriber(ros::NodeHandle& nh, std::string topic_name, size_t buff_size):nh_(nh)
    {
        subscriber_ = nh_.subscribe(topic_name, buff_size, &GNSSSubscriber::msg_callback, this);
    }

    void GNSSSubscriber::msg_callback(const sensor_msgs::NavSatFixConstPtr& nav_sat_fix_ptr) // 取出bag包记录gnss的各种数据信息存在一个类的对象中gnss_data中
    {
        GNSSData gnss_data;
        gnss_data.time = nav_sat_fix_ptr->header.stamp.toSec();
        gnss_data.latitude = nav_sat_fix_ptr->latitude;
        gnss_data.longitude = nav_sat_fix_ptr->longitude;
        gnss_data.altitude = nav_sat_fix_ptr->altitude;
        gnss_data.status = nav_sat_fix_ptr->status.status;
        gnss_data.service = nav_sat_fix_ptr->status.service;

        new_gnss_data_.push_back(gnss_data); // 将一次回调时间内对应的数据其存入队列new_gnss_data_中
    }

    void GNSSSubscriber::ParseData(std::deque<GNSSData>& deque_gnss_data) // 再将队列new_gnss_data_中数据插入到总的队列deque_gnss_data中
    {
        if(new_gnss_data_.size() > 0)
        {
            deque_gnss_data.insert(deque_gnss_data.end(), new_gnss_data_.begin(), new_gnss_data_.end());
            new_gnss_data_.clear();
        }
    }
}
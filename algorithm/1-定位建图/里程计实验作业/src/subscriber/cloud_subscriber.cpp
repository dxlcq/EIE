#include <lio_ndt/subscriber/cloud_subscriber.hpp>

#include <glog/logging.h>

namespace lio_ndt
{
    CloudSubscriber::CloudSubscriber(ros::NodeHandle& nh, std::string topic_name, size_t buff_size):nh_(nh)
    {
        subscriber_ = nh_.subscribe(topic_name, buff_size, &CloudSubscriber::msg_callback, this);
    }

    // 回调函数接受点云数据，并把点云从ros格式转化为pcl格式，然后存储在new_cloud_data_中
    void CloudSubscriber::msg_callback(const sensor_msgs::PointCloud2::ConstPtr& cloud_msg_ptr)
    {
        CloudData cloud_data; // cloud_data类 包含时间信息和点云容器
        cloud_data.time = cloud_msg_ptr->header.stamp.toSec();
        pcl::fromROSMsg(*cloud_msg_ptr, *(cloud_data.cloud_ptr));

        new_cloud_data_.push_back(cloud_data);
    }

    // 用于数据的传输，把新来的点云数据保存到当前点云数据中
    // deque_cloud_data由很多CloudData组成的容器
    void CloudSubscriber::ParseData(std::deque<CloudData>& deque_cloud_data)
    {
        if(new_cloud_data_.size() > 0)
        {
            deque_cloud_data.insert(deque_cloud_data.end(), new_cloud_data_.begin(), new_cloud_data_.end());
            new_cloud_data_.clear();
        }
    }
}

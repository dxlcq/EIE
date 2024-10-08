#include <lio_ndt/publisher/cloud_publisher.hpp>

namespace lio_ndt
{
    CloudPublisher::CloudPublisher(ros::NodeHandle& nh, std::string topic_name, size_t buff_size, std::string frame_id):nh_(nh), frame_id_(frame_id)
    {
        publisher_ = nh_.advertise<sensor_msgs::PointCloud2>(topic_name, buff_size);
    }

    void CloudPublisher::Publish(CloudData::CLOUD_PTR cloud_ptr_input)
    {
        sensor_msgs::PointCloud2Ptr cloud_ptr_output(new sensor_msgs::PointCloud2()); // 给 cloud_ptr_output 分配内存 
        // 转化成ros格式进行发布
        pcl::toROSMsg(*cloud_ptr_input, *cloud_ptr_output);
        // 提取时间戳，ros::Time::now()随当前时间变化
        cloud_ptr_output->header.stamp = ros::Time::now();
        cloud_ptr_output->header.frame_id = frame_id_;
        publisher_.publish(*cloud_ptr_output);
    }
}
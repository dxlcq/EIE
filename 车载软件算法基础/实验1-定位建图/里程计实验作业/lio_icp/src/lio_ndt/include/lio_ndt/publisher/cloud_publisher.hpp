#pragma once

#include <sensor_msgs/PointCloud2.h>
#include <pcl/point_types.h>
#include <pcl/point_cloud.h>
#include <pcl_conversions/pcl_conversions.h>

#include <lio_ndt/sensor_data/cloud_data.hpp>

namespace lio_ndt
{
    class CloudPublisher
    {
    public:
        CloudPublisher(ros::NodeHandle& nh, std::string topic_name, size_t buff_size, std::string frame_id);
        CloudPublisher() = default;
        void Publish(CloudData::CLOUD_PTR cloud_ptr_input);

    private:
        ros::NodeHandle nh_;
        ros::Publisher publisher_;
        std::string frame_id_;
    };
}
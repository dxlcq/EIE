#pragma once

#include <ros/ros.h>
#include <rosbag/bag.h>
#include <nav_msgs/Odometry.h>


namespace lio_ndt
{
    class OdomBagSubscriber
    {
    public:
        OdomBagSubscriber()  = default;
        ~OdomBagSubscriber();
        OdomBagSubscriber( ros::NodeHandle & nh, std::string topic_name, int buff_size, std::string bag_file_path, std::string bag_topic );

    private:
        std::string     bag_topic_;
        std::string     bag_file_;
        rosbag::Bag     bag_;
        ros::NodeHandle nh_;
        ros::Subscriber subscriber_;

        void odomCallback( const nav_msgs::Odometry::ConstPtr & msg );

    };
}
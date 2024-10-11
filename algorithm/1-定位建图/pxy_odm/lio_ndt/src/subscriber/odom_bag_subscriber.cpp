#include "lio_ndt/subscriber/odom_bag_subscriber.hpp"

namespace lio_ndt
{
    OdomBagSubscriber::OdomBagSubscriber( ros::NodeHandle & nh, std::string topic_name, int buff_size, std::string bag_file, std::string bag_topic ) : nh_(nh), bag_file_(bag_file), bag_topic_(bag_topic)
    {
        subscriber_ = nh_.subscribe<nav_msgs::Odometry>( topic_name, buff_size, &OdomBagSubscriber::odomCallback, this );
        bag_.open( bag_file_, rosbag::bagmode::Write );
    }

    OdomBagSubscriber::~OdomBagSubscriber()
    {
        if ( bag_.isOpen() ) bag_.close();
    }

    void OdomBagSubscriber::odomCallback( const nav_msgs::Odometry::ConstPtr & msg )
    {
        bag_.write( bag_topic_, ros::Time::now(), msg );
    }
}
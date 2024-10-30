#include "publisher/traj_pub.hpp"

namespace EIE_robot
{
    Traj_Pub::Traj_Pub( const ros::NodeHandle& nh, const std::string& topic_name, const int& buff_size )
    : nh_( nh )
    {
        odom_sub_ = nh_.subscribe( "odom", 10, &Traj_Pub::odomCallback, this );
        Traj_Pub_ = nh_.advertise<visualization_msgs::Marker>( topic_name, buff_size );
    }

    Traj_Pub::~Traj_Pub()
    {
    }

    void Traj_Pub::odomCallback( const nav_msgs::Odometry::ConstPtr& odom_msg )
    {
        traj_msg_.header.frame_id = "odom";
        traj_msg_.header.stamp = ros::Time::now();
        traj_msg_.ns = "traj";
        traj_msg_.id = 0;
        traj_msg_.type = visualization_msgs::Marker::LINE_STRIP;
        traj_msg_.action = visualization_msgs::Marker::ADD;
        traj_msg_.pose.orientation.w = 1.0;
        traj_msg_.scale.x = 0.1;
        traj_msg_.color.r = 1.0;
        traj_msg_.color.a = 1.0;
        traj_msg_.points.push_back( odom_msg->pose.pose.position );

        Traj_Pub_.publish( traj_msg_ );
    }

}
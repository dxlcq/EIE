#include "subscribe/motion_sub.hpp"
#include "base_bringup/EIE_base.hpp"

namespace EIE_robot
{
    Motion_Sub::Motion_Sub( const ros::NodeHandle& nh, const std::string& topic_name, const int& buff_size, westonrobot::TracerBase* EIE_control )
    : nh_( nh ), EIE_contral_( EIE_control )
    {
        motion_sub_ = nh_.subscribe( topic_name, buff_size, &Motion_Sub::motion_callback, this );
    }

    Motion_Sub::~Motion_Sub()
    {
    }

    void Motion_Sub::motion_callback( const geometry_msgs::Twist::ConstPtr& msg )
    {
        westonrobot::ScoutMotionCmd cmd;
        cmd.linear_velocity  = msg->linear.x;
        cmd.angular_velocity = msg->angular.z;

        EIE_contral_->SetMotionCommand( cmd.linear_velocity, cmd.angular_velocity );
    }


}
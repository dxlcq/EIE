#include "base_bringup/EIE_base.hpp"
#include "utils/interface.hpp"
namespace EIE_robot
{
    EIE_base::EIE_base( const std::string& device_name,
                        const int32_t& baud_rate,
                        ros::NodeHandle &nh) 
    : nh_( nh )
    {
        // EIE_status_->Connect( device_name, baud_rate );
        EIE_contral_.Connect( device_name );
        EIE_contral_.EnableCommandedMode();
        device_name_ = device_name;
        baud_rate_ = baud_rate;
    }

    void EIE_base::SetupSubscription( const ros::NodeHandle& nh )
    {
        motion_sub_ = std::make_shared<Motion_Sub>( nh, "/cmd_vel",              10, &EIE_contral_ );
        light_sub_  = std::make_shared<Light_Sub>(  nh, "/EIE_robot_light_cmd",  10, &EIE_contral_ );
        status_sub_ = std::make_shared<Status_Sub>( nh, "/EIE_robot_status",     10 );
        status_pub_ = std::make_shared<Status_pub>( nh, "/EIE_robot_status",     10, device_name_, baud_rate_);
    }



    void EIE_base::PublishStatus()
    {
        status_pub_->PublishStatus();
    }

}
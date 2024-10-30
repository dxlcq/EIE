#include "subscribe/status_sub.hpp"
#include "base_bringup/EIE_base.hpp"
#include "utils/interface.hpp"

namespace EIE_robot
{
    Status_Sub::Status_Sub( const ros::NodeHandle& nh, const std::string& topic_name, const int& buff_size )
    : nh_( nh )
    {
        status_sub_ = nh_.subscribe( topic_name, buff_size, &Status_Sub::status_callback, this );
    }

    Status_Sub::~Status_Sub()
    {
    }

    void Status_Sub::status_callback( const base_msgs::ScoutStatus::ConstPtr& msg )
    {
        display(msg);
    }


}
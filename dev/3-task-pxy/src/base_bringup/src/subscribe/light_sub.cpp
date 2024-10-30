#include "subscribe/light_sub.hpp"
#include "base_bringup/EIE_base.hpp"

namespace EIE_robot
{

    Light_Sub::Light_Sub( const ros::NodeHandle& nh, const std::string& topic_name, const int& buff_size, westonrobot::TracerBase *EIE_control )
    : nh_( nh ), EIE_control_( EIE_control )
    {
        light_sub_ = nh_.subscribe( topic_name, buff_size, &Light_Sub::light_callback, this );
    }

    Light_Sub::~Light_Sub()
    {
    }

    void Light_Sub::light_callback( const base_msgs::ScoutLightCmd::ConstPtr& msg )
    {
        if (msg->enable_cmd_light_control)
        {
            westonrobot::TracerLightCmd cmd;
            ROS_INFO( "light cmd open!!" );

            switch (msg->front_mode)
            {
                case base_msgs::ScoutLightCmd::LIGHT_CONST_OFF:
                {
                    ROS_INFO( "LightCmd: CONST_OFF" );
                    EIE_control_->SetLightCommand({westonrobot::TracerLightCmd::LightMode::CONST_OFF, 0});
                    sleep(3);
                    break;
                }
                case base_msgs::ScoutLightCmd::LIGHT_CONST_ON:
                {
                    ROS_INFO( "LightCmd: CONST_ON" );
                    EIE_control_->SetLightCommand({westonrobot::TracerLightCmd::LightMode::CONST_ON, 0});
                    sleep(3);
                    break;
                }
                case base_msgs::ScoutLightCmd::LIGHT_BREATH:
                {
                    ROS_INFO( "LightCmd: BREATH" );
                    EIE_control_->SetLightCommand({westonrobot::TracerLightCmd::LightMode::BREATH, 0});
                    sleep(8);
                    break;
                }
                case base_msgs::ScoutLightCmd::LIGHT_CUSTOM:
                {
                    ROS_INFO( "LightCmd: CUSTOM" );
                    EIE_control_->SetLightCommand({westonrobot::TracerLightCmd::LightMode::CUSTOM, 0});
                    sleep(3);
                    break;
                }
            }
            ROS_INFO( "send cmd to light!!" );
        }
    }

    
    
}

#include "publisher/status_pub.hpp"
#include "base_bringup/EIE_base.hpp"

namespace EIE_robot
{
    Status_pub::Status_pub( const ros::NodeHandle& nh, const std::string& topic_name, const int& buff_size, const std::string& device_name, const int32_t& baud_rate )
    : nh_( nh )
    {
        EIE_status_ -> Connect( device_name, baud_rate );
        status_pub_ = nh_.advertise<base_msgs::ScoutStatus>( topic_name, buff_size );

    }

    Status_pub::~Status_pub()
    {
    }

    void Status_pub::PublishStatus()
    {
        current_time_ = ros::Time::now();
        base_msgs::ScoutStatus status_msg;
        auto state = EIE_status_->GetScoutState();
        status_msg.header.stamp = current_time_;

        status_msg.linear_velocity = state.linear_velocity;
        status_msg.angular_velocity = state.angular_velocity;

        status_msg.base_state = state.base_state;
        status_msg.control_mode = state.control_mode;
        status_msg.fault_code = state.fault_code;
        status_msg.battery_voltage = state.battery_voltage;

        for (int i = 0; i < 4; ++i)
        {
        status_msg.motor_states[i].current = state.actuator_states[i].motor_current;
        status_msg.motor_states[i].rpm = state.actuator_states[i].motor_rpm;
        status_msg.motor_states[i].temperature = state.actuator_states[i].motor_temperature;
        }

        status_msg.light_control_enabled = state.light_control_enabled;
        status_msg.front_light_state.mode = state.front_light_state.mode;
        status_msg.front_light_state.custom_value =
            state.front_light_state.custom_value;
        status_msg.rear_light_state.mode = state.rear_light_state.mode;
        status_msg.rear_light_state.custom_value =
            state.front_light_state.custom_value;

        status_pub_.publish( status_msg );
    }

}
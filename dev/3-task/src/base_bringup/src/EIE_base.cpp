#include "base_bringup/EIE_base.hpp"
#include "utils/interface.hpp"
namespace EIE_robot
{
    EIE_base::EIE_base( std::unique_ptr<westonrobot::ScoutBase> EIE_status_ptr,
                        std::unique_ptr<westonrobot::TracerBase> EIE_contral_ptr,
                        std::unique_ptr<ros::NodeHandle> nh )
    : EIE_status_ptr_( std::move( EIE_status_ptr ) ), EIE_contral_ptr_( std::move( EIE_contral_ptr ) ), nh_( std::move( nh ) ) 
    {
    // 打印指针状态
        std::cout << "EIE_status_ptr_: " << (EIE_status_ptr_ ? "valid" : "nullptr") << std::endl;
        std::cout << "EIE_contral_ptr_: " << (EIE_contral_ptr_ ? "valid" : "nullptr") << std::endl;
        std::cout << "nh_: " << (nh_ ? "valid" : "nullptr") << std::endl;
    }

    void EIE_base::SetupSubscription()
    {
        motion_cmd_sub_ = nh_->subscribe( "/cmd_vel",          1, &EIE_base::MotionCmdCallback, this );
        light_cmd_sub_  = nh_->subscribe( "/light_cmd",        1, &EIE_base::LightCmdCallback, this );
        status_sub_     = nh_->subscribe( "/EIE_robot_status", 1, &EIE_base::StatusCallback, this );

        status_pub_ = nh_->advertise<base_msgs::ScoutStatus>( "/EIE_robot_status", 10 );
    }

    void EIE_base::PublishStatus()
    {
        current_time_ = ros::Time::now();
        base_msgs::ScoutStatus status_msg;
        auto state = EIE_status_ptr_->GetScoutState();
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

    void EIE_base::MotionCmdCallback( const geometry_msgs::Twist::ConstPtr &msg )
    {
        westonrobot::ScoutMotionCmd cmd;
        cmd.linear_velocity  = msg->linear.x;
        cmd.angular_velocity = msg->angular.z;
        EIE_contral_ptr_->SetMotionCommand( cmd.linear_velocity, cmd.angular_velocity );
        // EIE_status_ptr_->SendRobotCmd();
        //@TODO: 不能在这里发送控制指令, 因为发送指令的函数SendRobotCmd是private的
    }

    void EIE_base::LightCmdCallback( const base_msgs::ScoutLightCmd::ConstPtr &msg )
    {
        
        if (msg->enable_cmd_light_control)
        {
            westonrobot::TracerLightCmd cmd;
            ROS_INFO( "light cmd open!!" );

            switch (msg->front_mode)
            {
                case base_msgs::ScoutLightCmd::LIGHT_CONST_OFF:
                {
                    cmd.front_mode = westonrobot::TracerLightCmd::LightMode::CONST_OFF;
                    break;
                }
                case base_msgs::ScoutLightCmd::LIGHT_CONST_ON:
                {
                    ROS_INFO( "LightCmd: CONST_ON" );
                    cmd.front_mode = westonrobot::TracerLightCmd::LightMode::CONST_ON;
                    break;
                }
                case base_msgs::ScoutLightCmd::LIGHT_BREATH:
                {
                    cmd.front_mode = westonrobot::TracerLightCmd::LightMode::BREATH;
                    break;
                }
                case base_msgs::ScoutLightCmd::LIGHT_CUSTOM:
                {
                    cmd.front_mode = westonrobot::TracerLightCmd::LightMode::CUSTOM;
                    cmd.front_custom_value = msg->front_custom_value;
                    break;
                }
            }
            /* write only  */
            // switch (msg->rear_mode)
            // {
            //     case base_msgs::ScoutLightCmd::LIGHT_CONST_OFF:
            //     {
            //         cmd.rear_mode = westonrobot::ScoutLightCmd::LightMode::CONST_OFF;
            //         break;
            //     }
            //     case base_msgs::ScoutLightCmd::LIGHT_CONST_ON:
            //     {
            //         cmd.rear_mode = westonrobot::ScoutLightCmd::LightMode::CONST_ON;
            //         break;
            //     }
            //     case base_msgs::ScoutLightCmd::LIGHT_BREATH:
            //     {
            //         cmd.rear_mode = westonrobot::ScoutLightCmd::LightMode::BREATH;
            //         break;
            //     }
            //     case base_msgs::ScoutLightCmd::LIGHT_CUSTOM:
            //     {
            //         cmd.rear_mode = westonrobot::ScoutLightCmd::LightMode::CUSTOM;
            //         cmd.rear_custom_value = msg->rear_custom_value;
            //         break;
            //     }
            // }

            EIE_contral_ptr_->SetLightCommand(cmd);
            ROS_INFO( "send cmd to light!!" );
        }
        else
        {
            EIE_status_ptr_->DisableLightCmdControl();
        }
    }

    void EIE_base::StatusCallback( const base_msgs::ScoutStatus::ConstPtr &msg )
    {
        return;
        // display(msg);
        /* 输出状态信息 */
        // ROS_INFO( "----------------------------");
        // ROS_INFO( "Received status message" );
        // ROS_INFO( "Linear velocity: %f", msg->linear_velocity );
        // ROS_INFO( "Angular velocity: %f", msg->angular_velocity );
        // ROS_INFO( "Base state: %d", msg->base_state );
        // ROS_INFO( "Control mode: %d", msg->control_mode );
        // ROS_INFO( "Fault code: %d", msg->fault_code );
        // ROS_INFO( "Battery voltage: %f", msg->battery_voltage );
        // ROS_INFO( "Motor states:" );
        // for (int i = 0; i < 4; ++i)
        // {
        //     ROS_INFO( "Motor %d current: %f", i, msg->motor_states[i].current );
        //     ROS_INFO( "Motor %d rpm: %f", i, msg->motor_states[i].rpm );
        //     ROS_INFO( "Motor %d temperature: %f", i, msg->motor_states[i].temperature );
        // }
        // ROS_INFO( "Light control enabled: %d", msg->light_control_enabled );
        // ROS_INFO( "Front light mode: %d", msg->front_light_state.mode );
        // ROS_INFO( "Front light custom value: %d", msg->front_light_state.custom_value );
        // ROS_INFO( "Rear light mode: %d", msg->rear_light_state.mode );
        // ROS_INFO( "Rear light custom value: %d", msg->rear_light_state.custom_value );

        // @TODO: 使用QT界面显示状态信息 或者 写个好看点的输出函数
    }

}
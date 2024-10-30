#include "base_bringup/EIE_base.hpp"
#include "utils/interface.hpp"
namespace EIE_robot
{
    EIE_base::EIE_base( std::string device_name,
                        int32_t baud_rate,
                        ros::NodeHandle &nh )
    :nh_( nh ) 
    {
        EIE_status_.Connect( device_name, baud_rate );
        EIE_contral_.Connect( device_name );
        EIE_contral_.EnableCommandedMode();
    }

    void EIE_base::SetupSubscription()
    {
        if (!nh_.ok()) ROS_ERROR("NodeHandle is not initialized");
        try {
        std::cout<<"aaa"<<std::endl;
        motion_cmd_sub_ = nh_.subscribe( "/cmd_vel",          1, &EIE_base::MotionCmdCallback, this );
        std::cout<<"bbb"<<std::endl;
        light_cmd_sub_  = nh_.subscribe( "/light_cmd",        1, &EIE_base::LightCmdCallback, this );
        std::cout<<"ccc"<<std::endl;
        status_sub_     = nh_.subscribe( "/EIE_robot_status", 1, &EIE_base::StatusCallback, this );

        std::cout<<"ddd"<<std::endl;
        status_pub_ = nh_.advertise<base_msgs::ScoutStatus>( "/EIE_robot_status", 10 );
        std::cout<<"eee"<<std::endl;
        } catch ( const std::exception& e ){
            std::cerr << "setup func Exception caught: " << e.what() << std::endl;
        }
    }

    void EIE_base::PublishStatus()
    {
        current_time_ = ros::Time::now();
        base_msgs::ScoutStatus status_msg;
        auto state = EIE_status_.GetScoutState();
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

        EIE_contral_.SetMotionCommand( cmd.linear_velocity, cmd.angular_velocity );
        // std::cout << "EIE_contral_ptr_: " << (EIE_contral_ptr_ ? "valid" : "nullptr") << std::endl;
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
                    ROS_INFO( "LightCmd: CONST_OFF" );
                    EIE_contral_.SetLightCommand({westonrobot::TracerLightCmd::LightMode::CONST_OFF, 0});
                    sleep(3);
                    // cmd.front_mode = westonrobot::TracerLightCmd::LightMode::CONST_OFF;
                    break;
                }
                case base_msgs::ScoutLightCmd::LIGHT_CONST_ON:
                {
                    ROS_INFO( "LightCmd: CONST_ON" );
                    EIE_contral_.SetLightCommand({westonrobot::TracerLightCmd::LightMode::CONST_ON, 0});
                    sleep(3);
                    // cmd.front_mode = westonrobot::TracerLightCmd::LightMode::CONST_ON;
                    break;
                }
                case base_msgs::ScoutLightCmd::LIGHT_BREATH:
                {
                    ROS_INFO( "LightCmd: BREATH" );
                    EIE_contral_.SetLightCommand({westonrobot::TracerLightCmd::LightMode::BREATH, 0});
                    sleep(8);
                    // cmd.front_mode = westonrobot::TracerLightCmd::LightMode::BREATH;
                    break;
                }
                case base_msgs::ScoutLightCmd::LIGHT_CUSTOM:
                {
                    ROS_INFO( "LightCmd: CUSTOM" );
                    EIE_contral_.SetLightCommand({westonrobot::TracerLightCmd::LightMode::CUSTOM, 0});
                    sleep(3);
                    // cmd.front_mode = westonrobot::TracerLightCmd::LightMode::CUSTOM;
                    // cmd.front_custom_value = msg->front_custom_value;
                    break;
                }
            }

            // EIE_contral_.SetLightCommand(cmd);
            ROS_INFO( "send cmd to light!!" );
        }
        else
        {
            EIE_status_.DisableLightCmdControl();
        }
    }

    void EIE_base::StatusCallback( const base_msgs::ScoutStatus::ConstPtr &msg )
    {
        // std::cout << "EIE_status_ptr_: " << (EIE_status_ptr_ ? "valid" : "nullptr") << std::endl;
        // std::cout << "wocaonmaaaaaaaa" << std::endl;
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
#include "utils/interface.hpp"

void info(const base_msgs::ScoutStatus::ConstPtr &msg)
{
    system("clear");
    printf("线速度:    %lf\n",msg->linear_velocity);
    printf("角速度:    %lf\n",msg->angular_velocity);
    printf("基础状态:  %d\n",msg->base_state);
    printf("控制模式:  %d\n",msg->control_mode);
    printf("故障代码:  %d\n",msg->fault_code);
    printf("电池电压:  %lf\n",msg->battery_voltage);
    printf("电机:\n");
        printf("\t0电机电流:  %lf\n",msg->motor_states[0].current);
        printf("\t0电机转速:  %lf\n",msg->motor_states[0].rpm);
        printf("\t0电机温度:  %lf\n",msg->motor_states[0].temperature);
        printf("\t1电机电流:  %lf\n",msg->motor_states[1].current);
        printf("\t1电机转速:  %lf\n",msg->motor_states[1].rpm);
        printf("\t1电机温度:  %lf\n",msg->motor_states[1].temperature);
        printf("\t2电机电流:  %lf\n",msg->motor_states[2].current);
        printf("\t2电机转速:  %lf\n",msg->motor_states[2].rpm);
        printf("\t2电机温度:  %lf\n",msg->motor_states[2].temperature);
        printf("\t3电机电流:  %lf\n",msg->motor_states[3].current);
        printf("\t3电机转速:  %lf\n",msg->motor_states[3].rpm);
        printf("\t3电机温度:  %lf\n",msg->motor_states[3].temperature);
    printf("灯光控制:  %d\n",msg->light_control_enabled);
    printf("前灯模式:  %d\n",msg->front_light_state.mode);
    printf("前灯值:    %d\n",msg->front_light_state.custom_value);
    sleep(1);
}

std::string base_state,control_mode,light_state,pwm;
int flag=0;
void output(const base_msgs::ScoutStatus::ConstPtr &msg)
{
    switch(msg->control_mode)
    {
        case 1:
            control_mode="终端控制模式";
            break;
        case 3:
            control_mode="遥控器控制模式";
            break;
        default:
            control_mode="未知或扫描中";
            break;
    }
    switch(msg->base_state)
    {
        case 0:
            base_state="正常";
            break;
        default:
            base_state="未知";
            break;
    }
    switch(msg->front_light_state.mode)
    {
        case 0:
            light_state = "关灯";
            flag=0;
            break;
        case 2:
            light_state = "开灯";
            flag=1;
            break;
        default:
            light_state = "未知";
            break;
    }
    if(flag)
        pwm = std::to_string(msg->front_light_state.custom_value)+"\t";
    else
        pwm = "前灯已关闭";
}

/**
    线速度：    msg->linear_velocity
    角速度：    msg->angular_velocity
    基础状态：  msg->base_state
    控制模式：  msg->control_mode（3：遥控器，1：终端）
    故障代码：  msg->fault_code
    电池电压：  msg->battery_voltage
    电机:
        一共四个，索引(item)分别是0~3, 0: , 1: , 2: , 3:
        电机电流：  msg->motor_states[item].current
        电机转速：  msg->motor_states[item].rpm
        电机温度：  msg->motor_states[item].temperature
    灯光控制：  msg->light_control_enabled
    前灯模式：  msg->front_light_state.mode(2：遥控器模式的开灯， 0：遥控器模式的关灯)
    前灯值：    msg->front_light_state.custom_value（呼吸灯的pwm值）
 */
void display(const base_msgs::ScoutStatus::ConstPtr &msg)
{
    system("clear");
    output(msg);
    printf("                                                                        │\n");
    printf("              Mode: %s\t 呼吸灯PWM: %s\t\t│\n",control_mode.c_str(),pwm.c_str());
    printf("                               \033[1m\033[94mVVV V VVV\033[0m                                │  system state\t\t: \033[1m\033[32m%s\033[0m\n",base_state.c_str());
    printf("                        -------------------------                       │  Light state\t\t: %s\n", light_state.c_str());
    printf("                 -------|                     \t|-------                │  Battery voltage\t: %.1f V\n",msg->battery_voltage);
    printf("  电流: %.1lf\t |     ||                     \t||     |  电流: %.1lf\t│\n",msg->motor_states[0].current,msg->motor_states[1].current);
    printf("  转速: %.1lf\t |     ||                     \t||     |  转速: %.1lf\t│\033[1m--System faults\033[0m\n",msg->motor_states[0].rpm,msg->motor_states[1].rpm);
    printf("  温度: %.1lf\t |     ||                     \t||     |  温度: %.1lf\t│  - Drv over-heat\t: \033[1m\033[32mN\033[0m\n",msg->motor_states[0].temperature,msg->motor_states[1].temperature);
    printf("                 |     ||                     \t||     |                │  - Mt over-curren\t: \033[1m\033[32mN\033[0m\n");
    printf("                 -------|                     \t|-------                │  - Bat under volt\t: \033[1m\033[32mN\033[0m\n");
    printf("                        |                     \t|                       │  - Bat over volt\t: \033[1m\033[32mN\033[0m\n");
    printf("                        |                     \t|                       │\n");
    printf("                        |                     \t|                       │\033[1m--Comm faults\033[0m\n");
    printf("                        |                     \t|                       │  - CAN cmd checksum\t: \033[1m\033[32mN\033[0m\n");
    printf("                 -------|                     \t|-------                │  - Motor 1 comm\t: \033[1m\033[32mN\033[0m\n");
    printf("  电流: %.1lf\t |     ||                     \t||     |  电流: %.1lf\t│  - Motor 2 comm\t: \033[1m\033[32mN\033[0m\n",msg->motor_states[2].current,msg->motor_states[3].current);
    printf("  转速: %.1lf\t |     ||                     \t||     |  转速: %.1lf\t│  - Motor 3 comm\t: \033[1m\033[32mN\033[0m\n",msg->motor_states[2].rpm,msg->motor_states[3].rpm);
    printf("  温度: %.1lf\t |     ||                     \t||     |  温度: %.1lf\t│  - Motor 4 comm\t: \033[1m\033[32mN\033[0m\n",msg->motor_states[2].temperature,msg->motor_states[3].temperature);
    printf("                 |     ||     线速度 : %.3lf\t||     |                │\n",msg->linear_velocity);
    printf("                 -------|     角速度 : %.3lf\t|-------                │  \033[1m\033[32mN: normal    \033[33mW: warning\033[0m\n",msg->angular_velocity);
    printf("                        -------------------------                       │  \033[1m\033[31mF: fault     P: prrtection\033[0m\n");
    sleep(1);
}
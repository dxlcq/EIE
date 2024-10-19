/**
 * @file interface.hpp
 * @brief 终端界面显示
 * 
*/
#pragma once
#include "../base_bringup/EIE_base.hpp"

void display(const base_msgs::ScoutStatus::ConstPtr &msg);
void vehicleFrame();



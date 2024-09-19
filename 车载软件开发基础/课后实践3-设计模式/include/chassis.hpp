#pragma once
#include <string>
#include <iostream>

class Chassis{
public:
    Chassis();
    ~Chassis();

    void setChassisNo(std::string chassisNo);
    std::string getChassisNo();

    void setChassisModel(std::string chassisModel);
    std::string getChassisModel();

    void setWheelbase(uint32_t wheelbase);
    uint32_t getWheelbase();

    void setTrackWidth(uint32_t trackWidth);
    uint32_t getTrackWidth();

    void setGroundClearance(uint32_t groundClearance);
    uint32_t getGroundClearance();

    void setTurningRadius(uint32_t turningRadius);
    uint32_t getTurningRadius();

    void setDriveType(std::string driveType);
    std::string getDriveType();

    void setMaxSuspensionTravel(uint32_t maxSuspensionTravel);
    uint32_t getMaxSuspensionTravel();

    void showInfo();

    void deal(std::string s);

private:
    std::string _chassisNo          = "dp110001";   // 底盘编号
    std::string _chassisModel       = "SCOUT-MINI"; // 底盘型号
    uint32_t    _wheelbase          = 451;          // 底盘轴距
    uint32_t    _trackWidth         = 490;          // 底盘轮距
    uint32_t    _groundClearance    = 115;          // 底盘最小离地间隙
    uint32_t    _turningRadius      = 0;            // 底盘最小转弯半径
    std::string _driveType          = "四轮四驱";   // 底盘驱动形式
    uint32_t    _maxSuspensionTravel= 10;           // 底盘最大行程
};
#pragma once
#include <string>
#include <vector>
#include <thread>
#include <deque>
#include "chassis.hpp"

class Lidar{
public:
    Lidar();
    ~Lidar();

    void setModel(std::string model);
    std::string getModel();

    void setChannels(uint32_t channels);
    uint32_t getChannels();

    void setTestRange(uint32_t testRange);
    uint32_t getTestRange();

    void setTdp(uint32_t tdp);
    uint32_t getTdp();

    void showInfo();

    std::vector<std::string> getStatus();

private:
    std::deque<std::string> _status;
    // deque 线程安全

    std::string _model      = "RS-Helios-16p";  // 型号
    uint32_t    _channels   = 16;               // 通道数
    uint32_t    _testRange  = 100;              // 测试范围
    uint32_t    _tdp        = 8;                // 功耗
};
#pragma once
#include <string>
#include <vector>
#include <memory>
#include "lidar.hpp"

class EventPoll{
public:
    EventPoll(std::shared_ptr<Lidar> l);
    ~EventPoll();

    std::vector<std::string> wait();// 持续监听激光雷达对象的状态
private:
    std::shared_ptr<Lidar> _l;
};
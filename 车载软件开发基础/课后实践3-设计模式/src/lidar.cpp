#include "../include/lidar.hpp"

Lidar::Lidar(){
    std::thread([this](){   // 开启一个线程，持续添加状态
        while(true){
            std::this_thread::sleep_for(std::chrono::seconds(1));
            // 根据当前时间 + 随机数生成状态
            uint8_t r = (std::chrono::system_clock::now().time_since_epoch().count() + rand()) % 3 + 1;
            switch(r){
                case 1:
                    _status.push_back("前方");
                    break;
                case 2:
                    _status.push_back("右前方");
                    break;
                case 3:
                    _status.push_back("左前方");
                    break;
            }
        }
    }).detach();            // 分离线程
}
Lidar::~Lidar(){}

std::vector<std::string> Lidar::getStatus(){
    std::vector<std::string> v;
    while(!_status.empty() && v.size() < 100){
        v.push_back(_status.front());
        _status.pop_front();
    }
    return v;
}
#include <iostream>
#include <memory>
#include <vector>
#include "include/chassis.hpp"
#include "include/lidar.hpp"
#include "include/eventpoll.hpp"

int main(){
    auto l = std::make_shared<Lidar>();
    auto c = std::make_unique<Chassis>();
    auto e = std::make_unique<EventPoll>(l);// 事件轮询持有激光雷达
    while(true){                            // 完成注册，开始持续监听
        auto v = e->wait();
        for(auto& s : v){   // 获取到激光雷达的状态
            c->deal(s);     // 汇报给底盘对象
        }
    }
    return 0;
}
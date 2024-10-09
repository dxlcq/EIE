#include "../include/chassis.hpp"

Chassis::Chassis(){}
Chassis::~Chassis(){}

void Chassis::deal(std::string s){
    if(s == "前方"){
        std::cout << "后退。。。。" << std::endl;
    }else if(s == "左前方"){
        std::cout << "右转。。。。" << std::endl;
    }else if(s == "右前方"){
        std::cout << "左转。。。。" << std::endl;
    }
}
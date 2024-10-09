#include "../头文件/电池模块.hpp"
#include <iostream>
#include <fstream>

电池模块::电池模块(){}

电池模块::~电池模块(){}

void 电池模块::设置电池模块参数(const std::string& 参数){
    this->_参数 = 参数;
}

void 电池模块::设置电池模块对外供电(const std::string& 对外供电){
    this->_对外供电 = 对外供电;
}

void 电池模块::设置电池模块充电时长(const std::string& 充电时长){
    this->_充电时长 = 充电时长;
}

void 电池模块::打印设备信息() const {
    std::cout << "电池模块参数: "    << this->_参数    << std::endl;
    std::cout << "电池模块对外供电: "<< this->_对外供电<< std::endl;
    std::cout << "电池模块充电时长: "<< this->_充电时长<< std::endl;
}

void 电池模块::内存写到文件(std::ofstream& ofs) const {
    ofs << this->_参数    << std::endl;
    ofs << this->_对外供电<< std::endl;
    ofs << this->_充电时长<< std::endl;
}

void 电池模块::文件读到内存(std::ifstream& ifs){
    ifs >> this->_参数;
    ifs >> this->_对外供电;
    ifs >> this->_充电时长;
}
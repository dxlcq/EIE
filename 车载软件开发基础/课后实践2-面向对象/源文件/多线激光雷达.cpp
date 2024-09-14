#include "../头文件/多线激光雷达.hpp"
#include <iostream>
#include <fstream>

多线激光雷达::多线激光雷达(){}

多线激光雷达::~多线激光雷达(){}

void 多线激光雷达::设置多线激光雷达型号(const std::string& 型号){
    this->_型号 = 型号;
}

void 多线激光雷达::设置多线激光雷达通道数(const std::string& 通道数){
    this->_通道数 = 通道数;
}

void 多线激光雷达::设置多线激光雷达测试范围(const std::string& 测试范围){
    this->_测试范围 = 测试范围;
}

void 多线激光雷达::设置多线激光雷达功耗(const std::string& 功耗){
    this->_功耗 = 功耗;
}

void 多线激光雷达::打印设备信息() const {
    std::cout << "多线激光雷达型号: "    << this->_型号    << std::endl;
    std::cout << "多线激光雷达通道数: "  << this->_通道数  << std::endl;
    std::cout << "多线激光雷达测试范围: "<< this->_测试范围<< std::endl;
    std::cout << "多线激光雷达功耗: "    << this->_功耗    << std::endl;
}

void 多线激光雷达::内存写到文件(std::ofstream& ofs) const {
    ofs << this->_型号    << std::endl;
    ofs << this->_通道数  << std::endl;
    ofs << this->_测试范围<< std::endl;
    ofs << this->_功耗    << std::endl;
}

void 多线激光雷达::文件读到内存(std::ifstream& ifs){
    ifs >> this->_型号;
    ifs >> this->_通道数;
    ifs >> this->_测试范围;
    ifs >> this->_功耗;
}
#include "../头文件/九轴陀螺仪.hpp"
#include <iostream>
#include <fstream>

九轴陀螺仪::九轴陀螺仪(){}

九轴陀螺仪::~九轴陀螺仪(){}

void 九轴陀螺仪::设置九轴陀螺仪型号(const std::string& 型号){
    this->_型号 = 型号;
}

void 九轴陀螺仪::设置九轴陀螺仪厂家(const std::string& 厂家){
    this->_厂家 = 厂家;
}

void 九轴陀螺仪::打印设备信息() const {
    std::cout << "九轴陀螺仪型号: " << this->_型号 << std::endl;
    std::cout << "九轴陀螺仪厂家: " << this->_厂家 << std::endl;
}

void 九轴陀螺仪::内存写到文件(std::ofstream& ofs) const {
    ofs << this->_型号 << std::endl;
    ofs << this->_厂家 << std::endl;
}

void 九轴陀螺仪::文件读到内存(std::ifstream& ifs){
    ifs >> this->_型号;
    ifs >> this->_厂家;
}
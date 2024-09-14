#include "../头文件/液晶显示屏.hpp"
#include <iostream>
#include <fstream>

液晶显示屏::液晶显示屏(){}

液晶显示屏::~液晶显示屏(){}

void 液晶显示屏::设置液晶显示屏型号(const std::string& 型号){
    this->_型号 = 型号;
}

void 液晶显示屏::设置液晶显示屏尺寸(const std::string& 尺寸){
    this->_尺寸 = 尺寸;
}

void 液晶显示屏::打印设备信息() const {
    std::cout << "液晶显示屏型号: " << this->_型号 << std::endl;
    std::cout << "液晶显示屏尺寸: " << this->_尺寸 << std::endl;
}

void 液晶显示屏::内存写到文件(std::ofstream& ofs) const {
    ofs << this->_型号 << std::endl;
    ofs << this->_尺寸 << std::endl;
}

void 液晶显示屏::文件读到内存(std::ifstream& ifs){
    ifs >> this->_型号;
    ifs >> this->_尺寸;
}
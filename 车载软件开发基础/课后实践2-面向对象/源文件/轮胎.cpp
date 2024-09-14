#include "../头文件/轮胎.hpp"
#include <iostream>
#include <fstream>

轮胎::轮胎(){}

轮胎::~轮胎(){}

void 轮胎::设置轮胎型号(const std::string& 型号){
    this->_型号 = 型号;
}

void 轮胎::设置轮胎尺寸(const std::string& 尺寸){
    this->_尺寸 = 尺寸;
}

void 轮胎::打印设备信息() const {
    std::cout << "轮胎型号: " << this->_型号 << std::endl;
    std::cout << "轮胎尺寸: " << this->_尺寸 << std::endl;
}

void 轮胎::内存写到文件(std::ofstream& ofs) const {
    ofs << this->_型号 << std::endl;
    ofs << this->_尺寸 << std::endl;
}

void 轮胎::文件读到内存(std::ifstream& ifs){
    ifs >> this->_型号;
    ifs >> this->_尺寸;
}
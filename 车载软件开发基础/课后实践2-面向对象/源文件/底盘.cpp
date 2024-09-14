#include "../头文件/底盘.hpp"
#include <iostream>
#include <fstream>

底盘::底盘(std::string 编号): _编号(编号){}

底盘::~底盘(){}

void 底盘::设置底盘编号(const std::string& 编号){
    this->_编号 = 编号;
}

void 底盘::设置底盘型号(const std::string& 型号){
    this->_型号 = 型号;
}

void 底盘::设置底盘轴距(const std::string& 轴距){
    this->_轴距 = 轴距;
}

void 底盘::设置底盘轮距(const std::string& 轮距){
    this->_轮距 = 轮距;
}

void 底盘::设置底盘最小离地间隙(const std::string& 最小离地间隙){
    this->_最小离地间隙 = 最小离地间隙;
}

void 底盘::设置底盘最小转弯半径(const std::string& 最小转弯半径){
    this->_最小转弯半径 = 最小转弯半径;
}

void 底盘::设置底盘驱动形式(const std::string& 驱动形式){
    this->_驱动形式 = 驱动形式;
}

void 底盘::设置底盘最大行程(const std::string& 最大行程){
    this->_最大行程 = 最大行程;
}

void 底盘::打印设备信息() const {
    std::cout << "底盘编号: "         << this->_编号         << std::endl;
    std::cout << "底盘型号: "         << this->_型号         << std::endl;
    std::cout << "底盘轴距: "         << this->_轴距         << std::endl;
    std::cout << "底盘轮距: "         << this->_轮距         << std::endl;
    std::cout << "底盘最小离地间隙: "  << this->_最小离地间隙  << std::endl;
    std::cout << "底盘最小转弯半径: "  << this->_最小转弯半径  << std::endl;
    std::cout << "底盘驱动形式: "      << this->_驱动形式     << std::endl;
    std::cout << "底盘最大行程: "      << this->_最大行程     << std::endl;
}

void 底盘::内存写到文件(std::ofstream& ofs) const {
    ofs << this->_编号        << std::endl;
    ofs << this->_型号        << std::endl;
    ofs << this->_轴距        << std::endl;
    ofs << this->_轮距        << std::endl;
    ofs << this->_最小离地间隙 << std::endl;
    ofs << this->_最小转弯半径 << std::endl;
    ofs << this->_驱动形式     << std::endl;
    ofs << this->_最大行程     << std::endl;
}

void 底盘::文件读到内存(std::ifstream& ifs) {
    ifs >> this->_编号;
    ifs >> this->_型号;
    ifs >> this->_轴距;
    ifs >> this->_轮距;
    ifs >> this->_最小离地间隙;
    ifs >> this->_最小转弯半径;
    ifs >> this->_驱动形式;
    ifs >> this->_最大行程;
}
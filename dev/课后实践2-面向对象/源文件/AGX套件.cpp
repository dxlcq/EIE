#include "../头文件/AGX套件.hpp"
#include <iostream>
#include <fstream>

AGX套件::AGX套件(){}

AGX套件::~AGX套件(){}

void AGX套件::设置AGX套件型号(const std::string& 型号){
    this->_型号 = 型号;
}

void AGX套件::设置AGX套件AI(const std::string& AI){
    this->_AI = AI;
}

void AGX套件::设置AGX套件CUDA核心(const std::string& CUDA核心){
    this->_CUDA核心 = CUDA核心;
}

void AGX套件::设置AGX套件TensorCORE(const std::string& TensorCORE){
    this->_TensorCORE = TensorCORE;
}

void AGX套件::设置AGX套件显存(const std::string& 显存){
    this->_显存 = 显存;
}

void AGX套件::设置AGX套件存储(const std::string& 存储){
    this->_存储 = 存储;
}

void AGX套件::打印设备信息() const {
    std::cout << "AGX套件型号: "        << this->_型号      << std::endl;
    std::cout << "AGX套件AI: "         << this->_AI        << std::endl;
    std::cout << "AGX套件CUDA核心: "    << this->_CUDA核心  << std::endl;
    std::cout << "AGX套件TensorCORE: " << this->_TensorCORE<< std::endl;
    std::cout << "AGX套件显存: "        << this->_显存      << std::endl;
    std::cout << "AGX套件存储: "        << this->_存储      << std::endl;
}

void AGX套件::内存写到文件(std::ofstream& ofs) const {
    ofs << this->_型号        << std::endl;
    ofs << this->_AI          << std::endl;
    ofs << this->_CUDA核心    << std::endl;
    ofs << this->_TensorCORE  << std::endl;
    ofs << this->_显存        << std::endl;
    ofs << this->_存储        << std::endl;
}

void AGX套件::文件读到内存(std::ifstream& ifs){
    ifs >> this->_型号;
    ifs >> this->_AI;
    ifs >> this->_CUDA核心;
    ifs >> this->_TensorCORE;
    ifs >> this->_显存;
    ifs >> this->_存储;
}
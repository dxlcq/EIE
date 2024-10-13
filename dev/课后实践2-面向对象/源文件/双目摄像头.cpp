#include "../头文件/双目摄像头.hpp"
#include <iostream>
#include <fstream>

双目摄像头::双目摄像头(){}

双目摄像头::~双目摄像头(){}

void 双目摄像头::设置双目摄像头型号(const std::string& 型号){
    this->_型号 = 型号;
}

void 双目摄像头::设置双目摄像头摄像头(const std::string& 摄像头){
    this->_摄像头 = 摄像头;
}

void 双目摄像头::设置双目摄像头RGB帧分辨率(const std::string& RGB帧分辨率){
    this->_RGB帧分辨率 = RGB帧分辨率;
}

void 双目摄像头::设置双目摄像头RGB帧率(const std::string& RGB帧率){
    this->_RGB帧率 = RGB帧率;
}

void 双目摄像头::设置双目摄像头FOV(const std::string& FOV){
    this->_FOV = FOV;
}

void 双目摄像头::设置双目摄像头深度帧率(const std::string& 深度帧率){
    this->_深度帧率 = 深度帧率;
}

void 双目摄像头::打印设备信息() const {
    std::cout << "双目摄像头型号: "        << this->_型号      << std::endl;
    std::cout << "双目摄像头摄像头: "      << this->_摄像头    << std::endl;
    std::cout << "双目摄像头RGB帧分辨率: " << this->_RGB帧分辨率<< std::endl;
    std::cout << "双目摄像头RGB帧率: "     << this->_RGB帧率    << std::endl;
    std::cout << "双目摄像头FOV: "         << this->_FOV        << std::endl;
    std::cout << "双目摄像头深度帧率: "    << this->_深度帧率   << std::endl;
}

void 双目摄像头::内存写到文件(std::ofstream& ofs) const {
    ofs << this->_型号        << std::endl;
    ofs << this->_摄像头      << std::endl;
    ofs << this->_RGB帧分辨率 << std::endl;
    ofs << this->_RGB帧率     << std::endl;
    ofs << this->_FOV         << std::endl;
    ofs << this->_深度帧率    << std::endl;
}

void 双目摄像头::文件读到内存(std::ifstream& ifs){
    ifs >> this->_型号;
    ifs >> this->_摄像头;
    ifs >> this->_RGB帧分辨率;
    ifs >> this->_RGB帧率;
    ifs >> this->_FOV;
    ifs >> this->_深度帧率;
}
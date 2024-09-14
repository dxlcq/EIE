#include "../头文件/智能小车.hpp"
#include <iostream>
#include <fstream>

智能小车::智能小车(std::string 编号, std::string 底盘编号, std::string 学生学号, std::string 学生姓名): _编号(编号), _底盘(底盘编号), _学生信息(学生学号, 学生姓名){}

智能小车::~智能小车(){}

void 智能小车::设置智能小车编号(const std::string& 编号){
    this->_编号 = 编号;
}

void 智能小车::设置底盘编号(const std::string& 编号){
    this->_底盘.设置底盘编号(编号);
}

void 智能小车::设置底盘型号(const std::string& 型号){
    this->_底盘.设置底盘型号(型号);
}

void 智能小车::设置底盘轴距(const std::string& 轴距){
    this->_底盘.设置底盘轴距(轴距);
}

void 智能小车::设置底盘轮距(const std::string& 轮距){
    this->_底盘.设置底盘轮距(轮距);
}

void 智能小车::设置底盘最小离地间隙(const std::string& 最小离地间隙){
    this->_底盘.设置底盘最小离地间隙(最小离地间隙);
}

void 智能小车::设置底盘最小转弯半径(const std::string& 最小转弯半径){
    this->_底盘.设置底盘最小转弯半径(最小转弯半径);
}

void 智能小车::设置底盘驱动形式(const std::string& 驱动形式){
    this->_底盘.设置底盘驱动形式(驱动形式);
}

void 智能小车::设置底盘最大行程(const std::string& 最大行程){
    this->_底盘.设置底盘最大行程(最大行程);
}

void 智能小车::设置轮胎型号(const std::string& 型号){
    this->_轮胎.设置轮胎型号(型号);
}

void 智能小车::设置轮胎尺寸(const std::string& 尺寸){
    this->_轮胎.设置轮胎尺寸(尺寸);
}

void 智能小车::设置九轴陀螺仪型号(const std::string& 型号){
    this->_九轴陀螺仪.设置九轴陀螺仪型号(型号);
}

void 智能小车::设置九轴陀螺仪厂家(const std::string& 厂家){
    this->_九轴陀螺仪.设置九轴陀螺仪厂家(厂家);
}

void 智能小车::设置液晶显示屏型号(const std::string& 型号){
    this->_液晶显示屏.设置液晶显示屏型号(型号);
}

void 智能小车::设置液晶显示屏尺寸(const std::string& 尺寸){
    this->_液晶显示屏.设置液晶显示屏尺寸(尺寸);
}

void 智能小车::设置AGX套件型号(const std::string& 型号){
    this->_AGX套件.设置AGX套件型号(型号);
}

void 智能小车::设置AGX套件AI(const std::string& AI){
    this->_AGX套件.设置AGX套件AI(AI);
}

void 智能小车::设置AGX套件CUDA核心(const std::string& CUDA核心){
    this->_AGX套件.设置AGX套件CUDA核心(CUDA核心);
}

void 智能小车::设置AGX套件TensorCORE(const std::string& TensorCORE){
    this->_AGX套件.设置AGX套件TensorCORE(TensorCORE);
}

void 智能小车::设置AGX套件显存(const std::string& 显存){
    this->_AGX套件.设置AGX套件显存(显存);
}

void 智能小车::设置AGX套件存储(const std::string& 存储){
    this->_AGX套件.设置AGX套件存储(存储);
}

void 智能小车::设置电池模块参数(const std::string& 参数){
    this->_电池模块.设置电池模块参数(参数);
}

void 智能小车::设置电池模块对外供电(const std::string& 对外供电){
    this->_电池模块.设置电池模块对外供电(对外供电);
}

void 智能小车::设置电池模块充电时长(const std::string& 充电时长){
    this->_电池模块.设置电池模块充电时长(充电时长);
}

void 智能小车::设置双目摄像头型号(const std::string& 型号){
    this->_双目摄像头.设置双目摄像头型号(型号);
}

void 智能小车::设置双目摄像头摄像头(const std::string& 摄像头){
    this->_双目摄像头.设置双目摄像头摄像头(摄像头);
}

void 智能小车::设置双目摄像头RGB帧分辨率(const std::string& RGB帧分辨率){
    this->_双目摄像头.设置双目摄像头RGB帧分辨率(RGB帧分辨率);
}

void 智能小车::设置双目摄像头RGB帧率(const std::string& RGB帧率){
    this->_双目摄像头.设置双目摄像头RGB帧率(RGB帧率);
}

void 智能小车::设置双目摄像头FOV(const std::string& FOV){
    this->_双目摄像头.设置双目摄像头FOV(FOV);
}

void 智能小车::设置双目摄像头深度帧率(const std::string& 深度帧率){
    this->_双目摄像头.设置双目摄像头深度帧率(深度帧率);
}

void 智能小车::设置多线激光雷达型号(const std::string& 型号){
    this->_多线激光雷达.设置多线激光雷达型号(型号);
}

void 智能小车::设置多线激光雷达通道数(const std::string& 通道数){
    this->_多线激光雷达.设置多线激光雷达通道数(通道数);
}

void 智能小车::设置多线激光雷达测试范围(const std::string& 测试范围){
    this->_多线激光雷达.设置多线激光雷达测试范围(测试范围);
}

void 智能小车::设置多线激光雷达功耗(const std::string& 功耗){
    this->_多线激光雷达.设置多线激光雷达功耗(功耗);
}

void 智能小车::设置学生姓名(const std::string& 姓名){
    this->_学生信息.设置学生信息姓名(姓名);
}

void 智能小车::设置学生学号(const std::string& 学号){
    this->_学生信息.设置学生信息学号(学号);
}

std::string 智能小车::获取智能小车编号() const {
    return this->_编号;
}

void 智能小车::打印设备信息() const {
    std::cout << "智能小车编号: " << this->_编号 << std::endl;
    this->_底盘.打印设备信息();
    this->_轮胎.打印设备信息();
    this->_九轴陀螺仪.打印设备信息();
    this->_液晶显示屏.打印设备信息();
    this->_AGX套件.打印设备信息();
    this->_电池模块.打印设备信息();
    this->_双目摄像头.打印设备信息();
    this->_多线激光雷达.打印设备信息();
    this->_学生信息.打印设备信息();
}

void 智能小车::内存写到文件(std::ofstream& ofs) const {
    ofs << this->_编号 << std::endl;
    this->_底盘.内存写到文件(ofs);
    this->_轮胎.内存写到文件(ofs);
    this->_九轴陀螺仪.内存写到文件(ofs);
    this->_液晶显示屏.内存写到文件(ofs);
    this->_AGX套件.内存写到文件(ofs);
    this->_电池模块.内存写到文件(ofs);
    this->_双目摄像头.内存写到文件(ofs);
    this->_多线激光雷达.内存写到文件(ofs);
    this->_学生信息.内存写到文件(ofs);
}

void 智能小车::文件读到内存(std::ifstream& ifs){
    ifs >> this->_编号;
    this->_底盘.文件读到内存(ifs);
    this->_轮胎.文件读到内存(ifs);
    this->_九轴陀螺仪.文件读到内存(ifs);
    this->_液晶显示屏.文件读到内存(ifs);
    this->_AGX套件.文件读到内存(ifs);
    this->_电池模块.文件读到内存(ifs);
    this->_双目摄像头.文件读到内存(ifs);
    this->_多线激光雷达.文件读到内存(ifs);
    this->_学生信息.文件读到内存(ifs);
}
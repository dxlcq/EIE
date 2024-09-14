#pragma once
#include "设备.hpp"
#include "底盘.hpp"
#include "轮胎.hpp"
#include "九轴陀螺仪.hpp"
#include "液晶显示屏.hpp"
#include "学生信息.hpp"
#include "AGX套件.hpp"
#include "电池模块.hpp"
#include "双目摄像头.hpp"
#include "多线激光雷达.hpp"
#include <string>

class 智能小车: public 设备 {
public:
    智能小车(std::string 编号, std::string 底盘编号, std::string 学生学号, std::string 学生姓名);
    ~智能小车() override;
    
    void 设置智能小车编号(const std::string& 编号);
    void 设置底盘编号(const std::string& 编号);
    void 设置底盘型号(const std::string& 型号);
    void 设置底盘轴距(const std::string& 轴距);
    void 设置底盘轮距(const std::string& 轮距);
    void 设置底盘最小离地间隙(const std::string& 最小离地间隙);
    void 设置底盘最小转弯半径(const std::string& 最小转弯半径);
    void 设置底盘驱动形式(const std::string& 驱动形式);
    void 设置底盘最大行程(const std::string& 最大行程);
    void 设置轮胎型号(const std::string& 型号);
    void 设置轮胎尺寸(const std::string& 尺寸);
    void 设置九轴陀螺仪型号(const std::string& 型号);
    void 设置九轴陀螺仪厂家(const std::string& 厂家);
    void 设置液晶显示屏型号(const std::string& 型号);
    void 设置液晶显示屏尺寸(const std::string& 尺寸);
    void 设置AGX套件型号(const std::string& 型号);
    void 设置AGX套件AI(const std::string& AI);
    void 设置AGX套件CUDA核心(const std::string& CUDA核心);
    void 设置AGX套件TensorCORE(const std::string& TensorCORE);
    void 设置AGX套件显存(const std::string& 显存);
    void 设置AGX套件存储(const std::string& 存储);
    void 设置电池模块参数(const std::string& 参数);
    void 设置电池模块对外供电(const std::string& 对外供电);
    void 设置电池模块充电时长(const std::string& 充电时长);
    void 设置双目摄像头型号(const std::string& 型号);
    void 设置双目摄像头摄像头(const std::string& 摄像头);
    void 设置双目摄像头RGB帧分辨率(const std::string& RGB帧分辨率);
    void 设置双目摄像头RGB帧率(const std::string& RGB帧率);
    void 设置双目摄像头FOV(const std::string& FOV);
    void 设置双目摄像头深度帧率(const std::string& 深度帧率);
    void 设置多线激光雷达型号(const std::string& 型号);
    void 设置多线激光雷达通道数(const std::string& 通道数);
    void 设置多线激光雷达测试范围(const std::string& 测试范围);
    void 设置多线激光雷达功耗(const std::string& 功耗);
    void 设置学生姓名(const std::string& 姓名);
    void 设置学生学号(const std::string& 学号);

    std::string 获取智能小车编号() const;

    void 打印设备信息() const override;

    void 内存写到文件(std::ofstream& ofs) const override;
    void 文件读到内存(std::ifstream& ifs) override;
private:
    std::string _编号;
    底盘 _底盘;
    轮胎 _轮胎;
    九轴陀螺仪 _九轴陀螺仪;
    液晶显示屏 _液晶显示屏;
    AGX套件 _AGX套件;
    电池模块 _电池模块;
    双目摄像头 _双目摄像头;
    多线激光雷达 _多线激光雷达;
    学生信息 _学生信息;    
};
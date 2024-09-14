#pragma once
#include "设备.hpp"
#include <string>

class 双目摄像头: public 设备 {
public:
    双目摄像头();
    ~双目摄像头() override;

    void 设置双目摄像头型号(const std::string& 型号);
    void 设置双目摄像头摄像头(const std::string& 摄像头);
    void 设置双目摄像头RGB帧分辨率(const std::string& RGB帧分辨率);
    void 设置双目摄像头RGB帧率(const std::string& RGB帧率);
    void 设置双目摄像头FOV(const std::string& FOV);
    void 设置双目摄像头深度帧率(const std::string& 深度帧率);

    void 打印设备信息() const override;

    void 内存写到文件(std::ofstream& ofs) const override;
    void 文件读到内存(std::ifstream& ifs) override;
private:
    std::string _型号       = "RealSense-D435i";
    std::string _摄像头     = "D430";
    std::string _RGB帧分辨率= "1920x1080";
    std::string _RGB帧率    = "30";
    std::string _FOV        = "8758";
    std::string _深度帧率   = "90";
};
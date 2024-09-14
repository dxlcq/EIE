#pragma once
#include "设备.hpp"
#include <string>

class 多线激光雷达: public 设备 {
public:
    多线激光雷达();
    ~多线激光雷达() override;

    void 设置多线激光雷达型号(const std::string& 型号);
    void 设置多线激光雷达通道数(const std::string& 通道数);
    void 设置多线激光雷达测试范围(const std::string& 测试范围);
    void 设置多线激光雷达功耗(const std::string& 功耗);

    void 打印设备信息() const override;

    void 内存写到文件(std::ofstream& ofs) const override;
    void 文件读到内存(std::ifstream& ifs) override;
private:
    std::string _型号    = "RS-Helios-16p";
    std::string _通道数  = "16";
    std::string _测试范围 =" 100";
    std::string _功耗    = "8";
};
#pragma once
#include "设备.hpp"
#include <string>

class 九轴陀螺仪: public 设备 {
public:
    九轴陀螺仪();
    ~九轴陀螺仪() override;

    void 设置九轴陀螺仪型号(const std::string& 型号);
    void 设置九轴陀螺仪厂家(const std::string& 厂家);

    void 打印设备信息() const override;

    void 内存写到文件(std::ofstream& ofs) const override;
    void 文件读到内存(std::ifstream& ifs) override;
private:
    std::string _型号 = "CH110";
    std::string _厂家 = "NXP";
};
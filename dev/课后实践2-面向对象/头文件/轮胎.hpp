#pragma once
#include "设备.hpp"
#include <string>

class 轮胎: public 设备 {
public:
    轮胎();
    ~轮胎() override;

    void 设置轮胎型号(const std::string& 型号);
    void 设置轮胎尺寸(const std::string& 尺寸);

    void 打印设备信息() const override;

    void 内存写到文件(std::ofstream& ofs) const override;
    void 文件读到内存(std::ifstream& ifs) override;
private:
    std::string _型号 = "公路轮";
    std::string _尺寸 = "175mm";
};
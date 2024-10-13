#pragma once
#include "设备.hpp"
#include <string>

class 电池模块: public 设备 {
public:
    电池模块();
    ~电池模块() override;

    void 设置电池模块参数(const std::string& 参数);
    void 设置电池模块对外供电(const std::string& 对外供电);
    void 设置电池模块充电时长(const std::string& 充电时长);

    void 打印设备信息() const override;

    void 内存写到文件(std::ofstream& ofs) const override;
    void 文件读到内存(std::ifstream& ifs) override;
private:
    std::string _参数    = "24V/5Ah";
    std::string _对外供电 = "24V";
    std::string _充电时长 = "2H";
};
#pragma once
#include "设备.hpp"
#include <string>

class 学生信息: public 设备 {
public:
    学生信息(const std::string& 学号, const std::string& 姓名);
    ~学生信息() override;

    void 设置学生信息姓名(const std::string& 姓名);
    void 设置学生信息学号(const std::string& 学号);

    void 打印设备信息() const override;

    void 内存写到文件(std::ofstream& ofs) const override;
    void 文件读到内存(std::ifstream& ifs) override;
private:
    std::string _姓名 = "戴兴龙";
    std::string _学号 = "E2021244";
};
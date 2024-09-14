#pragma once
#include "设备.hpp"
#include <string>

class 底盘: public 设备 {
public:
    底盘(std::string 编号);
    ~底盘() override;

    void 设置底盘编号(const std::string& 编号);
    void 设置底盘型号(const std::string& 型号);
    void 设置底盘轴距(const std::string& 轴距);
    void 设置底盘轮距(const std::string& 轮距);
    void 设置底盘最小离地间隙(const std::string& 最小离地间隙);
    void 设置底盘最小转弯半径(const std::string& 最小转弯半径);
    void 设置底盘驱动形式(const std::string& 驱动形式);
    void 设置底盘最大行程(const std::string& 最大行程);

    void 打印设备信息() const override;

    void 内存写到文件(std::ofstream& ofs) const override;
    void 文件读到内存(std::ifstream& ifs) override;
private:
    std::string _编号        = "NULL";
    std::string _型号        = "SCOUT-MINI";
    std::string _轴距        = "451mm";
    std::string _轮距        = "490mm";
    std::string _最小离地间隙 = "115mm";
    std::string _最小转弯半径 = "0m";
    std::string _驱动形式    = "四轮四驱";
    std::string _最大行程    = "10KM";
};
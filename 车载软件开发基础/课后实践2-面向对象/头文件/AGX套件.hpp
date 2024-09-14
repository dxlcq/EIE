#pragma once
#include "设备.hpp"
#include <string>

class AGX套件: public 设备 {
public:
    AGX套件();
    ~AGX套件() override;

    void 设置AGX套件型号(const std::string& 型号);
    void 设置AGX套件AI(const std::string& AI);
    void 设置AGX套件CUDA核心(const std::string& CUDA核心);
    void 设置AGX套件TensorCORE(const std::string& TensorCORE);
    void 设置AGX套件显存(const std::string& 显存);
    void 设置AGX套件存储(const std::string& 存储);

    void 打印设备信息() const override;

    void 内存写到文件(std::ofstream& ofs) const override;
    void 文件读到内存(std::ifstream& ifs) override;
private:
    std::string _型号       = "AGX-Xavier";
    std::string _AI         = "32TOPS";
    std::string _CUDA核心   = "512";
    std::string _TensorCORE = "64";
    std::string _显存       = "32G";
    std::string _存储       = "32G";
};
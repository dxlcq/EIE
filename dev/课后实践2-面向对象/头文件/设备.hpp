#pragma once
#include <fstream>

class 设备 {
public:
    virtual ~设备(){};
    
    virtual void 打印设备信息() const = 0;
    
    virtual void 内存写到文件(std::ofstream& ofs) const = 0;
    virtual void 文件读到内存(std::ifstream& ifs) = 0;
};
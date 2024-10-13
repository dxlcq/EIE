#include "../头文件/学生信息.hpp"
#include <iostream>
#include <fstream>

学生信息::学生信息(const std::string& 学号, const std::string& 姓名): _学号(学号), _姓名(姓名){}

学生信息::~学生信息(){}

void 学生信息::设置学生信息姓名(const std::string& 姓名){
    this->_姓名 = 姓名;
}

void 学生信息::设置学生信息学号(const std::string& 学号){
    this->_学号 = 学号;
}

void 学生信息::打印设备信息() const {
    std::cout << "学生姓名: " << this->_姓名 << std::endl;
    std::cout << "学生学号: " << this->_学号 << std::endl;
}

void 学生信息::内存写到文件(std::ofstream& ofs) const {
    ofs << this->_姓名 << std::endl;
    ofs << this->_学号 << std::endl;
}

void 学生信息::文件读到内存(std::ifstream& ifs){
    ifs >> this->_姓名;
    ifs >> this->_学号;
}
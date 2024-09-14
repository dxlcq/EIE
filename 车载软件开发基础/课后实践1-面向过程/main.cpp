#include <iostream>
#include <fstream>
#include <string>
#include <map>

class 智能小车 {
public:
    智能小车() {};
    智能小车(std::string 车辆编号, std::string 底盘编号) {
        编号 = 车辆编号;
        底盘.编号 = 底盘编号;
    }

    std::string 获取车辆编号() const {
        return 编号;
    }

    bool 编辑车辆信息() {
        显示车辆信息();
        std::cout << "需要修改的信息:";
        std::string 修改项;
        std::cin >> 修改项;

        if (修改项 == "底盘编号") {
            std::cout << "请输入底盘编号: ";
            std::cin >> 底盘.编号;
        }
        else if (修改项 == "底盘型号") {
            std::cout << "请输入底盘型号: ";
            std::cin >> 底盘.型号;
        }
        else if (修改项 == "底盘轴距") {
            std::cout << "请输入底盘轴距: ";
            std::cin >> 底盘.轴距;
        }
        else if (修改项 == "底盘轮距") {
            std::cout << "请输入底盘轮距: ";
            std::cin >> 底盘.轮距;
        }
        else if (修改项 == "底盘最小离地间隙") {
            std::cout << "请输入底盘最小离地间隙: ";
            std::cin >> 底盘.最小离地间隙;
        }
        else if (修改项 == "底盘最小转弯半径") {
            std::cout << "请输入底盘最小转弯半径: ";
            std::cin >> 底盘.最小转弯半径;
        }
        else if (修改项 == "底盘驱动形式") {
            std::cout << "请输入底盘驱动形式: ";
            std::cin >> 底盘.驱动形式;
        }
        else if (修改项 == "底盘最大行程") {
            std::cout << "请输入底盘最大行程: ";
            std::cin >> 底盘.最大行程;
        }
        else if (修改项 == "底盘轮胎数量") {
            std::cout << "请输入底盘轮胎数量: ";
            std::cin >> 底盘.轮胎.数量;
        }
        else if (修改项 == "底盘轮胎型号") {
            std::cout << "请输入底盘轮胎型号: ";
            std::cin >> 底盘.轮胎.型号;
        }
        else if (修改项 == "底盘轮胎尺寸") {
            std::cout << "请输入底盘轮胎尺寸: ";
            std::cin >> 底盘.轮胎.尺寸;
        }
        else if (修改项 == "AGX套件型号") {
            std::cout << "请输入AGX套件型号: ";
            std::cin >> AGX套件.型号;
        }
        else if (修改项 == "AGX套件AI") {
            std::cout << "请输入AGX套件AI: ";
            std::cin >> AGX套件.AI;
        }
        else if (修改项 == "AGX套件CUDA核心") {
            std::cout << "请输入AGX套件CUDA核心: ";
            std::cin >> AGX套件.CUDA核心;
        }
        else if (修改项 == "AGX套件TensorCORE") {
            std::cout << "请输入AGX套件TensorCORE: ";
            std::cin >> AGX套件.TensorCORE;
        }
        else if (修改项 == "AGX套件显存") {
            std::cout << "请输入AGX套件显存: ";
            std::cin >> AGX套件.显存;
        }
        else if (修改项 == "AGX套件存储") {
            std::cout << "请输入AGX套件存储: ";
            std::cin >> AGX套件.存储;
        }
        else if (修改项 == "双目摄像头型号") {
            std::cout << "请输入双目摄像头型号: ";
            std::cin >> 双目摄像头.型号;
        }
        else if (修改项 == "双目摄像头摄像头") {
            std::cout << "请输入双目摄像头摄像头: ";
            std::cin >> 双目摄像头.摄像头;
        }
        else if (修改项 == "双目摄像头RGB帧分辨率") {
            std::cout << "请输入双目摄像头RGB帧分辨率: ";
            std::cin >> 双目摄像头.RGB帧分辨率;
        }
        else if (修改项 == "双目摄像头RGB帧率") {
            std::cout << "请输入双目摄像头RGB帧率: ";
            std::cin >> 双目摄像头.RGB帧率;
        }
        else if (修改项 == "双目摄像头FOV") {
            std::cout << "请输入双目摄像头FOV: ";
            std::cin >> 双目摄像头.FOV;
        }
        else if (修改项 == "双目摄像头深度帧率") {
            std::cout << "请输入双目摄像头深度帧率: ";
            std::cin >> 双目摄像头.深度帧率;
        }
        else if (修改项 == "多线激光雷达型号") {
            std::cout << "请输入多线激光雷达型号: ";
            std::cin >> 多线激光雷达.型号;
        }
        else if (修改项 == "多线激光雷达通道数") {
            std::cout << "请输入多线激光雷达通道数: ";
            std::cin >> 多线激光雷达.通道数;
        }
        else if (修改项 == "多线激光雷达测试范围") {
            std::cout << "请输入多线激光雷达测试范围: ";
            std::cin >> 多线激光雷达.测试范围;
        }
        else if (修改项 == "多线激光雷达功耗") {
            std::cout << "请输入多线激光雷达功耗: ";
            std::cin >> 多线激光雷达.功耗;
        }
        else if (修改项 == "九轴陀螺仪型号") {
            std::cout << "请输入九轴陀螺仪型号: ";
            std::cin >> 九轴陀螺仪.型号;
        }
        else if (修改项 == "九轴陀螺仪厂家") {
            std::cout << "请输入九轴陀螺仪厂家: ";
            std::cin >> 九轴陀螺仪.厂家;
        }
        else if (修改项 == "液晶显示屏尺寸") {
            std::cout << "请输入液晶显示屏尺寸: ";
            std::cin >> 液晶显示屏.尺寸;
        }
        else if (修改项 == "液晶显示屏型号") {
            std::cout << "请输入液晶显示屏型号: ";
            std::cin >> 液晶显示屏.型号;
        }
        else if (修改项 == "电池模块参数") {
            std::cout << "请输入电池模块参数: ";
            std::cin >> 电池模块.参数;
        }
        else if (修改项 == "电池模块对外供电") {
            std::cout << "请输入电池模块对外供电: ";
            std::cin >> 电池模块.对外供电;
        }
        else if (修改项 == "电池模块充电时长") {
            std::cout << "请输入电池模块充电时长: ";
            std::cin >> 电池模块.充电时长;
        }
        else if (修改项 == "q") {
            system("clear");
            return false;
        }
        return true;	
    }
    
    void 显示车辆信息() const {
        std::cout << "车辆编号: " << 编号 << std::endl;
        std::cout << "底盘:" << std::endl;
        std::cout << "  编号: " << 底盘.编号 << std::endl;
        std::cout << "  型号: " << 底盘.型号 << std::endl;
        std::cout << "  轴距: " << 底盘.轴距 << std::endl;
        std::cout << "  轮距: " << 底盘.轮距 << std::endl;
        std::cout << "  最小离地间隙: " << 底盘.最小离地间隙 << std::endl;
        std::cout << "  最小转弯半径: " << 底盘.最小转弯半径 << std::endl;
        std::cout << "  驱动形式: " << 底盘.驱动形式 << std::endl;
        std::cout << "  最大行程: " << 底盘.最大行程 << std::endl;
        std::cout << "  轮胎:" << std::endl;
        std::cout << "    数量: " << 底盘.轮胎.数量 << std::endl;
        std::cout << "    型号: " << 底盘.轮胎.型号 << std::endl;
        std::cout << "    尺寸: " << 底盘.轮胎.尺寸 << std::endl;
        std::cout << "AGX套件:" << std::endl;
        std::cout << "  型号: " << AGX套件.型号 << std::endl;
        std::cout << "  AI: " << AGX套件.AI << std::endl;
        std::cout << "  CUDA核心: " << AGX套件.CUDA核心 << std::endl;
        std::cout << "  Tensor CORE: " << AGX套件.TensorCORE << std::endl;
        std::cout << "  显存: " << AGX套件.显存 << std::endl;
        std::cout << "  存储: " << AGX套件.存储 << std::endl;
        std::cout << "双目摄像头:" << std::endl;
        std::cout << "  型号: " << 双目摄像头.型号 << std::endl;
        std::cout << "  摄像头: " << 双目摄像头.摄像头 << std::endl;
        std::cout << "  RGB帧分辨率: " << 双目摄像头.RGB帧分辨率 << std::endl;
        std::cout << "  RGB帧率: " << 双目摄像头.RGB帧率 << std::endl;
        std::cout << "  FOV: " << 双目摄像头.FOV << std::endl;
        std::cout << "  深度帧率: " << 双目摄像头.深度帧率 << std::endl;
        std::cout << "多线激光雷达:" << std::endl;
        std::cout << "  型号: " << 多线激光雷达.型号 << std::endl;
        std::cout << "  通道数: " << 多线激光雷达.通道数 << std::endl;
        std::cout << "  测试范围: " << 多线激光雷达.测试范围 << std::endl;
        std::cout << "  功耗: " << 多线激光雷达.功耗 << std::endl;
        std::cout << "九轴陀螺仪:" << std::endl;
        std::cout << "  型号: " << 九轴陀螺仪.型号 << std::endl;
        std::cout << "  厂家: " << 九轴陀螺仪.厂家 << std::endl;
        std::cout << "液晶显示屏:" << std::endl;
        std::cout << "  尺寸: " << 液晶显示屏.尺寸 << std::endl;
        std::cout << "  型号: " << 液晶显示屏.型号 << std::endl;
        std::cout << "电池模块:" << std::endl;
        std::cout << "  参数: " << 电池模块.参数 << std::endl;
        std::cout << "  对外供电: " << 电池模块.对外供电 << std::endl;
        std::cout << "  充电时长: " << 电池模块.充电时长 << std::endl;
    }
    void 内存写到文件(std::ofstream& ofs) const {
        ofs << 编号 << std::endl;
        ofs << 底盘.编号 << std::endl;
        ofs << 底盘.型号 << std::endl;
        ofs << 底盘.轴距 << std::endl;
        ofs << 底盘.轮距 << std::endl;
        ofs << 底盘.最小离地间隙 << std::endl;
        ofs << 底盘.最小转弯半径 << std::endl;
        ofs << 底盘.驱动形式 << std::endl;
        ofs << 底盘.最大行程 << std::endl;
        ofs << 底盘.轮胎.数量 << std::endl;
        ofs << 底盘.轮胎.型号 << std::endl;
        ofs << 底盘.轮胎.尺寸 << std::endl;
        ofs << AGX套件.型号 << std::endl;
        ofs << AGX套件.AI << std::endl;
        ofs << AGX套件.CUDA核心 << std::endl;
        ofs << AGX套件.TensorCORE << std::endl;
        ofs << AGX套件.显存 << std::endl;
        ofs << AGX套件.存储 << std::endl;
        ofs << 双目摄像头.型号 << std::endl;
        ofs << 双目摄像头.摄像头 << std::endl;
        ofs << 双目摄像头.RGB帧分辨率 << std::endl;
        ofs << 双目摄像头.RGB帧率 << std::endl;
        ofs << 双目摄像头.FOV << std::endl;
        ofs << 双目摄像头.深度帧率 << std::endl;
        ofs << 多线激光雷达.型号 << std::endl;
        ofs << 多线激光雷达.通道数 << std::endl;
        ofs << 多线激光雷达.测试范围 << std::endl;
        ofs << 多线激光雷达.功耗 << std::endl;
        ofs << 九轴陀螺仪.型号 << std::endl;
        ofs << 九轴陀螺仪.厂家 << std::endl;
        ofs << 液晶显示屏.尺寸 << std::endl;
        ofs << 液晶显示屏.型号 << std::endl;
        ofs << 电池模块.参数 << std::endl;
        ofs << 电池模块.对外供电 << std::endl;
        ofs << 电池模块.充电时长 << std::endl;
    }
    void 文件读到内存(std::ifstream& ifs) {
        ifs >> 编号;
        ifs >> 底盘.编号;
        ifs >> 底盘.型号;
        ifs >> 底盘.轴距;
        ifs >> 底盘.轮距;
        ifs >> 底盘.最小离地间隙;
        ifs >> 底盘.最小转弯半径;
        ifs >> 底盘.驱动形式;
        ifs >> 底盘.最大行程;
        ifs >> 底盘.轮胎.数量;
        ifs >> 底盘.轮胎.型号;
        ifs >> 底盘.轮胎.尺寸;
        ifs >> AGX套件.型号;
        ifs >> AGX套件.AI;
        ifs >> AGX套件.CUDA核心;
        ifs >> AGX套件.TensorCORE;
        ifs >> AGX套件.显存;
        ifs >> AGX套件.存储;
        ifs >> 双目摄像头.型号;
        ifs >> 双目摄像头.摄像头;
        ifs >> 双目摄像头.RGB帧分辨率;
        ifs >> 双目摄像头.RGB帧率;
        ifs >> 双目摄像头.FOV;
        ifs >> 双目摄像头.深度帧率;
        ifs >> 多线激光雷达.型号;
        ifs >> 多线激光雷达.通道数;
        ifs >> 多线激光雷达.测试范围;
        ifs >> 多线激光雷达.功耗;
        ifs >> 九轴陀螺仪.型号;
        ifs >> 九轴陀螺仪.厂家;
        ifs >> 液晶显示屏.尺寸;
        ifs >> 液晶显示屏.型号;
        ifs >> 电池模块.参数;
        ifs >> 电池模块.对外供电;
        ifs >> 电池模块.充电时长;
    }

private:
    std::string 编号 = "NULL";
    struct {
        std::string 编号 = "NULL";
        std::string 型号 = "SCOUT-MINI";
        std::string 轴距 = "451mm";
        std::string 轮距 = "490mm";
        std::string 最小离地间隙 = "115mm";
        std::string 最小转弯半径 = "0m";
        std::string 驱动形式 = "四轮四驱";
        std::string 最大行程 = "10KM";
        struct {
            std::string 数量 = "4个";
            std::string 型号 = "麦克纳姆轮";
            std::string 尺寸 = "175mm";
        } 轮胎;
    } 底盘;
    struct {
        std::string 型号 = "AGX-Xavier";
        std::string AI = "32-T选项S";
        std::string CUDA核心 = "512";
        std::string TensorCORE = "64";
        std::string 显存 = "32G";
        std::string 存储 = "32G";
    } AGX套件;
    struct {
        std::string 型号 = "RealSense-D435i";
        std::string 摄像头 = "D430";
        std::string RGB帧分辨率 = "1920x1080";
        std::string RGB帧率 = "30";
        std::string FOV = "87x58";
        std::string 深度帧率 = "90";
    } 双目摄像头;
    struct {
        std::string 型号 = "RS-Helios-16p";
        std::string 通道数 = "16";
        std::string 测试范围 = "100m";
        std::string 功耗 = "8W";
    } 多线激光雷达;
    struct {
        std::string 型号 = "CH110";
        std::string 厂家 = "NXP";
    } 九轴陀螺仪;
    struct {
        std::string 尺寸 = "11.6";
        std::string 型号 = "super";
    } 液晶显示屏;
    struct {
        std::string 参数 = "24V/15Ah";
        std::string 对外供电 = "24V";
        std::string 充电时长 = "2H";
    } 电池模块;
};

class 学生 {
public:
    学生() {};
    学生(std::string 学号, std::string 姓名) {
        this->学号 = 学号;
        this->姓名 = 姓名;
    }

    void 显示学生信息() const {
        std::cout << "学生信息:" << std::endl;
        std::cout << "学号: " << 学号 << std::endl;
        std::cout << "姓名: " << 姓名 << std::endl;
    }
    void 内存写到文件(std::ofstream& ofs) const {
        ofs << 学号 << std::endl;
        ofs << 姓名 << std::endl;
    }
    void 文件读到内存(std::ifstream& ifs) {
        ifs >> 学号;
        ifs >> 姓名;
    }

private:
    std::string 学号, 姓名;
};

std::map<std::string, 智能小车> 车库;
std::map<std::string, 学生> 班级;

int main() {
    // 测试数据
    /*
    车库["cqusn0001"] = 智能小车("cqusn0001", "dp0001");
    车库["cqusn0002"] = 智能小车("cqusn0002", "dp0002");
    班级["cqusn0001"] = 学生("E2021001", "龙一");
    班级["cqusn0002"] = 学生("E2021002", "龙二");
    */
    while (true) {
        std::cout << "1.读取数据\n";
        std::cout << "2.保存数据\n";
        std::cout << "3.查看数据\n";
        std::cout << "4.新建小车\n";
        std::cout << "5.编辑小车\n";
        //std::cout << "6.删除小车\n";

        int 选项;
        std::cin >> 选项; system("clear");

        if (选项 == 1) {
            std::cout << "输入读取文件路径:";
            std::string path;
            std::cin >> path;
            std::ifstream ifs(path);
            if (!ifs) {
                std::cout << "文件不存在\n";
                continue;
            }
            车库.clear();
            班级.clear();
			while (!ifs.eof()) {
                智能小车 车辆;
                车辆.文件读到内存(ifs);
                学生 学生;
                学生.文件读到内存(ifs);
				if (车辆.获取车辆编号() == "NULL") break;   // 文件末尾

                车库[车辆.获取车辆编号()] = 车辆;
                班级[车辆.获取车辆编号()] = 学生;
            }
        }
        else if (选项 == 2) {
            std::cout << "输入保存文件路径:";
            std::string path;
            std::cin >> path;
            std::ofstream ofs(path);
            if (!ofs) {
                std::cout << "保存路径错误！\n";
                continue;
            }
            for (auto& 车车 : 车库) {
                车车.second.内存写到文件(ofs);
                班级[车车.first].内存写到文件(ofs);
            }
        }
        else if (选项 == 3) {
            auto 车车 = 车库.begin();
            char 前进后退 = ' ';
            while (车库.size()) {
                车车->second.显示车辆信息();
                班级[车车->first].显示学生信息();
                std::cin >> 前进后退; system("clear");
                if (前进后退 == 'n') {
                    车车++;
                    if (车车 == 车库.end()) {
                        std::cout << "按n无效\n";
                        车车--;
                        continue;
                    }
                }
                else if (前进后退 == 'p') {
                    if (车车 == 车库.begin()) {
                        std::cout << "按p无效\n";
                        continue;
                    }
                    车车--;
                }
                else break;
            }
        }
        else if (选项 == 4) {
            std::string 车辆编号, 底盘编号, 学生姓名, 学生学号;
            std::cout << "(1/4) 请输入车辆编号: "; std::cin >> 车辆编号;
            std::cout << "(2/4) 请输入底盘编号: "; std::cin >> 底盘编号;
            std::cout << "(3/4) 请输入学生姓名: "; std::cin >> 学生姓名;
            std::cout << "(4/4) 请输入学生学号: "; std::cin >> 学生学号;
            system("clear");

            车库[车辆编号] = 智能小车(车辆编号, 底盘编号);
            班级[车辆编号] = 学生(学生姓名, 学生学号);
        }
        else if (选项 == 5) {
            std::string 车辆编号;
            std::cout << "请输入车辆编号: "; std::cin >> 车辆编号;
            system("clear");
            while(车库[车辆编号].编辑车辆信息()) system("clear");
        }
        else break;
    }

    return 0;
}

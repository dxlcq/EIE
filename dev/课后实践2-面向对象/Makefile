CXX = g++
CXXFLAGS = -std=c++11
TARGET = 🚗
SRCS = 主.cpp 源文件/AGX套件.cpp 源文件/双目摄像头.cpp 源文件/学生信息.cpp 源文件/智能小车.cpp 源文件/电池模块.cpp 源文件/九轴陀螺仪.cpp 源文件/多线激光雷达.cpp 源文件/底盘.cpp 源文件/液晶显示屏.cpp 源文件/轮胎.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

.PHONY: all clean

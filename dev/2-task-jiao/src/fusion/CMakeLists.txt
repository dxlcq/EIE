cmake_minimum_required(VERSION 3.0.2)
project(fusion)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
)

find_package(catkin REQUIRED COMPONENTS
  roscpp            # 与 ROS 通信的基本功能
  std_msgs          # ROS 标准消息类型
  # OpenCV
  cv_bridge         # OpenCV 和 ROS 图像消息类型之间的转换
  # PCL
  sensor_msgs       # 传感器相关消息类型
  pcl_conversions   # PCL 和 ROS 数据类型之间的转换
  pcl_ros           # PCL 和 ROS 之间的基本功能
)

find_package(OpenCV REQUIRED COMPONENTS
  highgui           # OpenCV 图像显示功能
)

find_package(PCL REQUIRED COMPONENTS
  common            # PCL 基本功能
  io                # PCL 输入输出
  visualization     # PCL 可视化
)

catkin_package()

include_directories(
  ${catkin_INCLUDE_DIRS}  # catkin 包含目录
  ${PCL_INCLUDE_DIRS}     # PCL 包含目录
  ${OpenCV_INCLUDE_DIRS}  # OpenCV 包含目录
  ${CMAKE_SOURCE_DIR}/fusion/include
)

add_executable(fusion
  src/my_lidar.cpp
  src/my_camera.cpp
  src/my_config.cpp
  src/main.cpp
)

target_link_libraries(fusion
  ${catkin_LIBRARIES}
  ${PCL_LIBRARIES}
  ${OpenCV_LIBS}
)
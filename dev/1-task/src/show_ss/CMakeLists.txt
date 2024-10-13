cmake_minimum_required(VERSION 3.0.2)
project(show_ss)

find_package(catkin REQUIRED COMPONENTS
  roscpp            # 与 ROS 通信的基本功能
  std_msgs          # ROS 标准消息类型
  # Navigation
  nav_msgs          # 导航相关消息类型
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
)

add_executable(point_cloud_by_pcl src/point_cloud_by_pcl.cpp)
add_executable(rgb_and_depth_by_opencv src/rgb_and_depth_by_opencv.cpp)
add_executable(speed_by_ter src/speed_by_ter.cpp)

target_link_libraries(point_cloud_by_pcl
  ${catkin_LIBRARIES}
  ${PCL_LIBRARIES}
)

target_link_libraries(rgb_and_depth_by_opencv
  ${catkin_LIBRARIES}
  ${OpenCV_LIBS}
)

target_link_libraries(speed_by_ter
  ${catkin_LIBRARIES}
)
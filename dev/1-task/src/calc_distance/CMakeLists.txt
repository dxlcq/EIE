cmake_minimum_required(VERSION 3.0.2)
project(calc_distance)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
)

catkin_package()

include_directories(
  ${catkin_INCLUDE_DIRS}  # catkin 包含目录
)

add_executable(calc_distance src/distance.cpp)

target_link_libraries(calc_distance
  ${catkin_LIBRARIES}
)
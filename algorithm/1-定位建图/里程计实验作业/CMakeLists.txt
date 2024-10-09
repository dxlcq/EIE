cmake_minimum_required(VERSION 3.0.2)
project(lio_ndt)

#Release:发布版本进行优化，Debug：调试版本不进行优化
set(CMAKE_BUILD_TYPE "Debug") 
set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
set(CMAKE_CXX_STANDARD 14)
# add_compile_options(-std=c++14)
# add_definitions(-std=c++14) 

#发现依赖包
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  pcl_ros
  geometry_msgs
  tf
  eigen_conversions
)

#将所有依赖打包，可以避免target_link_libraries后面跟很长一串库的名字，而且库增删减的时候也跟着增删减
set(ALL_TARGET_LIBRARIES "")

#将文件放入cmake文件下，方便管理，在cmake文件中在去找对应的包
include(cmake/glog.cmake)
include(cmake/pcl.cmake)
include(cmake/eigen.cmake)
include(cmake/geographic.cmake)

include_directories(include ${catkin_INCLUDE_DIRS})
include(cmake/global_defination.cmake)
catkin_package()

#整合所有的cpp文件 到 ALL_SRCS
file(GLOB_RECURSE ALL_SRCS "*.cpp")
#整合所有的src/*_node.cpp文件 到 NODE_SRCS
file(GLOB_RECURSE NODE_SRCS "src/*_node.cpp")
#整合所有的third_party/*.cpp文件 到 THIRD_PARTY_SRCS
file(GLOB_RECURSE THIRD_PARTY_SRCS "third_party/*.cpp")
#在 ALL_SRCS 剔除 NODE_SRCS THIRD_PARTY_SRCS 所包含的内容
list(REMOVE_ITEM ALL_SRCS ${NODE_SRCS})
list(REMOVE_ITEM ALL_SRCS ${THIRD_PARTY_SRCS})                                                          

add_executable(test_frame_node src/test_frame_node.cpp  ${ALL_SRCS})
target_link_libraries(test_frame_node ${catkin_LIBRARIES} ${ALL_TARGET_LIBRARIES})

add_executable(front_end_node src/front_end_node.cpp ${ALL_SRCS})
target_link_libraries(front_end_node ${catkin_LIBRARIES} ${ALL_TARGET_LIBRARIES})

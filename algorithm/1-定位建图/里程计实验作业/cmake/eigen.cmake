find_package(Eigen3 REQUIRED)

include_directories(${Eigen3_INCLUDE_DIRS})
list(APPEND ALL_TARGET_LIBRARIES ${Eigen3_LIBRARIES})
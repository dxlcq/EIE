# 一、搭建环境

松灵小车重庆大学卓工镜像：https://eie.cqu.edu.cn/Research/clwsys/sysb.htm

# 二、运行前的准备

## 查看所有的 topic

```shell
rostopic list
```

## 启动ROS

```shell
roscore
```

# 三、激光雷达

## 启动激光雷达

```shell
roslaunch scout_bringup open_rslidar.launch
```

**雷达打不开的情况：**

- 如果报错`ERRO：“base_link[某个映射]” passed to lookupTransform argument source_frame does not exist.`等映射的变换错误，是因为TF没有完全启动，完全启动之后会自动修正
- 如果报错`Waiting for laser_scans...`，是因为和激光雷达的链接有问题。
  - 物理连接：检查线路是否稳定。如果稳定，就是网络配置有问题。
  - 检查主机ip地址是不是`192.168.1.102`

## 启动建图

```
roslaunch scout_bringup gmapping.launch
```

## 打开Rviz

```shell
rosrun rviz rviz
```

# 三、深度相机

## 启动深度相机

```
roslaunch realsense2_camera rs_camera.launch
```

## UI 界面

```
realsense-viewer
```

# 四、定位

## 启动定位

```
roslaunch scout_bringup open_rslidar.launch
```

# 开发、任务 1

1. `roscore`
2. ``


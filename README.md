<h1 align="center">

[![](https://english.cqu.edu.cn/img/logo.png)](https://eie.cqu.edu.cn/)

</h1>

<h3 align="center">

国家（重庆）卓越工程师学院 2021级 智能网联汽车方向课程

</h3>

* 毕业论文: [自动驾驶并行仿真测试系统设计与实现](./YMCA/)

* 必修 1: [车载软件开发基础](./dev/)

* 必修 2: [智能驾驶算法基础](./algorithm/)

* 选修 1: [智能网联汽车数据工程](./data/)

* 选修 2: 车联网系统与应用

* 选修 3: 智能座舱与人机交互

* 选修 4: 自动驾驶模拟仿真

<br>

---

# Git push冲突一直报错的解决方案

## 解决办法一

保留本地的更改，终止合并 -> 重新合并 -> 重新拉取

```shell
git merge --abort
git reset --merge
git pull
```

## 解决办法二

舍弃本地代码,远端版本覆盖本地版本 **(慎重)**

```shell
git fetch --all
git reset --hard origin/master
git fetch
```

# 一、搭建环境

## 安装Jetson Xavier镜像

### 安装需求
1. Ubuntu主机一台, 后文简称主机
2. SCOUT MINI xavier从机一台, 后文简称从机
3. type-C 数据线

### 为主机安装NIVIDIAJetPack驱动

1. 松灵小车重庆大学卓工镜像: https://caiyun.139.com/m/i?185CDxwXeQ8OU, 提取码:gX8g
2. 进入存放镜像的目录并打开终端, 解压缩: `tar -zxvf system_back_image.tar.gz`
3. 再进入目录: `cd ./JetPack_4.4.1_Linux_JETSON_AGX_XAVIER`
3. 更新系统列表: `sudo apt update`
3. 安装指令: `sudo apt install ./sdkmanager_1.9.1-10844_amd64.deb`

### 连接xavier进行烧录
1. 将xavier关机, 使xavier处于通电不开机状态, 使用数据线将xavier与主机连接.
2. 进入刷机模式: xavier有三个按钮, 首先按住中间的Force Rest大约三秒, 同时再按住Power键, 持续约5秒, 最后同时松开两个按键。
3. 打开主机终端, 输入`lsusb`, 如果出现 NVIDIA Corp说明此时系统已经进入刷机状态
4. 进入U盘Linux_for_Tegra目录下: `cd ./Linux_for_Tegra`
5. `sudo ./flash.sh -r jetson-xavier mmcblk0p1`运行烧录程序, 当结束后进入设备即可正常使用

## 搭建OpenCV、Numpy等常见库

在Jetson Xavier中自带了适配Jetson的OpenCV、Numpy等常用库, 可以利用GPU进行加速, 可以通过python3激活环境, 无需手动安装。

如果手动安装的OpenCV、Numpy等库, 则无法使用Jetson的加速功能。

## 搭建Pytorch环境

Jetson所使用的Pytorch版本是**特殊的**, 直接在Pytorch官网拉取安装**会报错**, 应该使用Nvidia官方的**JetPack**所提供的Pytorch版本, `jtop`可以查看JetPack的版本

- **查看Nvidia版本**

```shell
nvcc -V
```

**注意: **`nvidia-smi`指令是无法在Jetson上查看cuda版本的, 因为Jetson使用的驱动并非是Nvidia, 而是L4T(Linux for Tegra)驱动。

- **安装步骤**

Jetson的操作系统是定制的, 安装Pytorch跟一般的Ubuntu系统有很大的区别。

参考[安装Pytorch Nvidia官方指导](https://forums.developer.nvidia.com/t/pytorch-for-jetson/72048)

大致步骤如下: 

1. `wget`下载上面链接中的本地包
2. `pip`本地安装`torch`
3. 安装`torch`对应版本的`torchvision`
4. 检查是否能够使用cuda

# 二、运行前的准备

## 启动ROS

```shell
roscore
```

## 查看所有的 topic

```shell
rostopic list
```

# 三、激光雷达

## 启动激光雷达

```shell
roslaunch scout_bringup open_rslidar.launch
```

**雷达打不开的情况: **

- 如果报错`ERRO: “base_link[某个映射]” passed to lookupTransform argument source_frame does not exist.`等映射的变换错误, 是因为TF没有完全启动, 完全启动之后会自动修正。
- 如果报错`Waiting for laser_scans...`, 是因为和激光雷达的链接有问题。
  - 物理连接: 检查线路是否稳定。如果稳定, 就是网络配置有问题。
  - 检查主机ip地址是不是`192.168.1.102`

## 启动建图

```shell
roslaunch scout_bringup gmapping.launch
```

## 打开Rviz

```shell
rosrun rviz rviz
```

# 三、深度相机

## 启动深度相机

```shell
roslaunch realsense2_camera rs_camera.launch
```

## UI 界面

```shell
realsense-viewer
```

# 四、定位

## 启动定位

```shell
roslaunch scout_bringup open_rslidar.launch
```

# 五、Docker连接

## 1. 下载安装Docker

```shell
curl -fsSL https://get.docker.com -o get-docker.sh
sudo sh get-docker.sh
```

为了不用每次都用`sudo`管理员权限, 可以把用户添加到docker组: 

```shell
sudo usermod -aG docker $USER
```

## 2. 下载镜像

```shell
docker pull liujiboy/ros:3.0
```

PS: 下载需要科技上网

## 3. 进入镜像

```shell
docker run -p 5900:5900 -p 6080:80 --rm -e RESOLUTION=1920x1080 liujiboy/ros:3.0
```

进行了一个端口映射

**注意: ** 在VScode进行远程连接的时候需要手快一点, 启动上述命令之后迅速用VSCode进行连接, 不然登不上去

## 4. 进入网址

````shell
127.0.0.1:6080
````

即可进入图形化界面

# 开发、任务 1

（1）显示激光雷达、相机、定位

1. 启动 ros 内核 `roscore`
2. 发布激光雷达 `roslaunch scout_bringup open_rslidar.launch`
3. 发布相机 `roslaunch realsense2_camera rs_camera.launch`
4. 发布定位 `roslaunch scout_bringup open_rslidar.launch`
5. 订阅激光雷达 `rosrun show_ss point_cloud_by_pcl`
6. 订阅相机 `rosrun show_ss rgb_and_depth_by_opencv`
7. 订阅定位 `rosrun show_ss speed_by_ter`

（2）里程计计算距离

1. 启动 ros 内核 `roscore`
2. 发布激光雷达 `roslaunch scout_bringup open_rslidar.launch`
3. 订阅激光雷达 `rosrun calc_distance calc_distance`

# 开发、任务 2

## 录制标定视频数据

- 打开激光雷达

```shell
roslaunch scout_bringup open_rslidar.launch
```

- 打开摄像头

```shell
roslaunch realsense2_camera rs_camera.launch
```

- 启动录制

```shell
rosbag record -a
```

使用`ctl + c`结束录制，视频会保存在终端路径下，`.bag`文件较大，建议录制前检查剩余空间，或者可以使用移动硬盘录制

## 播放录制的视频

- 进入docker, 同时映射`.bag`文件

```shell
docker run -p 5900:5900 -p 6080:80 -v /home/ubuntu/Documents:/mnt/d --rm -e RESOLUTION=1920x1080 liujiboy/ros:3.0
```
此命令将Ubuntu的`Documents`文件夹映射到`/mnt/d`文件夹，即可使用`.bag`文件.

- 启动roscore

```shell
roscore
```

- 播放bag文件

```shell
rosbag play bagName.bag /rslidar_points:=/points_raw --pause 
```

`bagName.bag`是录制的标定包的名称

>  话题重映射：

```shell
rosbag play --clock XXX.bag /old_topic:=/new_topic
```

- 启动标定软件

```shell
rosrun calibration_camera_lidar calibration_toolkit
```

## 融合标定

1. 启动 ros 内核 `roscore`
2. 发布激光雷达 `roslaunch scout_bringup open_rslidar.launch`
3. 发布相机 `roslaunch realsense2_camera rs_camera.launch`
4. 订阅融合数据 `rosrun fusion_of_camera_and_lidar fusion_of_camera_and_lidar_node`

> 有 bug，有时候需要多订阅几次（失败的时候 kill 相机，再启动）

# 开发、任务 3
> 启动can2usb gs_usb
```
sudo bash dev/3-task/src/base_bringup/scripts/setup_can2usb.bash
#具体的信息去脚本里面看
```
> 测试can2usb是否成功
```
candump can0
# 有数据输出就OK
```
> 启动状态查看和控制主程序
```
rosrun base_bringup base_bringup_node can0
# 记得source 
# 记得加can0参数，就是指定用哪个口来通信
# 如果启动失败，就多启动几次，甚至十多次，详细原因待查明
```
> 电机控制
```
# 方法一(使用scout封装的ros封装好的键盘控制)
roslaunch scout_bringup scout_teleop_keyboard.launch
# 方法二(使用rostopic直接pub出msg)
rostopic pub /cmd_vel geometry_msgs/Tlinear:
  x: 1.0
  y: 0.0
  z: 0.0
angular:
  x: 0.0
  y: 0.0
  z: 0.0"
# 不推荐，因为地盘控制有个什么看门狗，必须一直pub才可以让电机一直动，这种方式只能pub一次
```
> 灯光控制
```
# 直接pub，因为我还没有封装到程序里面，待后续完善
rostopic pub /light_cmd base_msgs/ScoutLightCmd "{enable_cmd_light_control: false, front_mode: 0, front_custom_value: 0, rear_mode: 0,
  rear_custom_value: 0}" 
# @pama: enable_cmd_light_control: 是否使用上位机指令来控制灯光
# @parma: enable_cmd_light_control: 是否使用上位机指令来控制灯光
#        front_mode: 关灯 0 开灯 1 呼吸灯 2 自定义模式 3
#        front_custom_value: 应该是亮度
#        rear_mode: 尾灯，但是车上没有尾灯，应该是其他车型的
#        rear_custom_value: 无需多言
# @bref: 可以只使用部分参数，比如要开灯就
# @brief: 可以只使用部分参数，比如要开灯就
        rostopic pub /light_cmd base_msgs/ScoutLightCmd "{enable_cmd_light_control: true, front_mode: 1}" 
```

# 开发、任务 4

**注意，全部得在小车内的终端上操作，因为需要打开 rviz**

## （一）自动导航场地定位建图

1. 启动激光雷达，发布base_link->laser_link的坐标变换

```shell
roslaunch scout_bringup open_rslidar.launch
```

2. 启动gmapping建图算法

```shell
roslaunch scout_bringup gmapping.launch
```

3. 用手柄遥控小车去在场景中走完。构建完地图之后，把地图保存到指定目录（一般是保存到description），下面指令地图的名称为map，可以修改，注意地图名字不要重复

```shell
rosrun map_server map_saver -f ~/catkin_ws/src/scout_base/scout_description/maps/map
```

## （二）启动自主导航

1. 使能gs_usb内核模块

```shell
sudo modprobe gs_usb
```

2. 打开can设备且设置波特率

```shell
sudo ip link set can0 up type can bitrate 500000
```

3. 启动激光雷达

```shell
roslaunch scout_bringup open_rslidar.launch
```

4. 启动move_base

```shell
roslaunch scout_bringup navigation_4wd.launch
```

**注：** 如需自定义打开的地图，请打开 navigation_4wd.launch 文件修改参数，如下图所示, 请在标记横线处修改为需要打开的地图名称。

![navigation_4wd.launch](img\navigation_4wd_launch.jpg)

# 算法：傅春耕

# 算法：南智雄

# 算法：古富强

# 数据、任务 3

## 数据采集

1. 获取及保存点云数据

    ```shell
    cd /media/agilex/ly/
    source ~/dxl/EIE/data/3-task/get_and_save_ss_ros/devel/setup.bash
    roslaunch scout_bringup open_rslidar.launch
    rosrun get_and_save pcl
    ```

2. 获取及保存图像数据

    ```shell
    cd /media/agilex/ly/
    source ~/dxl/EIE/data/3-task/get_and_save_ss_ros/devel/setup.bash
    roslaunch realsense2_camera rs_camera.launch
    rosrun get_and_save img
    ````

3. 获取及保存视频数据

    ```shell
    cd /media/agilex/ly/
    source ~/dxl/EIE/data/3-task/get_and_save_ss_ros/devel/setup.bash
    roslaunch realsense2_camera rs_camera.launch
    rosrun get_and_save vid
    ```

## 数据标注

1. 下载 label-studio

    ```shell
    sudo docker pull heartexlabs/label-studio:latest
    ```

2. 启动 label-studio

    ```shell
    mkdir mydata
    sudo chmod -R 777 mydata/
    sudo docker run -it -p 8080:8080 -v $(pwd)/mydata:/label-studio/data heartexlabs/label-studio:latest
    ```

3. 启动预标注

    > label-studio 的插件



## 数据识别

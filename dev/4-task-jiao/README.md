## step1
启动雷达，以及雷达里程计
```
roslaunch scout_bringup open_rslidar.launch
```
## step2
启动move_base, move_base 包括以下四个参数
>     base_local_planner_params.yaml   局部路径规划器参数
>
>     costmap_common_params.yaml       全局和局部代价地图共用的参数
>
>     global_costmap_params.yaml       全局cost_map参数
>
>     local_costmap_params.yaml        局部cost_map参数
>
>     move_base_params.yaml            move_base基础配置文件

启动map_server 导入地图
> 使用gmapping算法构建二维栅格地图

启动定位算法, 一般为amcl
> 一种粒子滤波算法，根据传感器信息不断迭代获取最确定的位姿 

启动机器人节点, 模型文件

```
roslaunch scout_bringup navigation_4wd.launch
```

## step3
启动识别程序，发布识别结果
```
rosrun tesseract_image_txt tesseract_image_txt_node
```
启动接受识别结果节点, 接受到识别结果, 根据不同的结果设置不同的导航点
```
rosrun nav_goal_publisher_subscriber nav_goal_publisher_subscriber_node
```

## topic
记得开usb2can

记得关闭遥控器操控
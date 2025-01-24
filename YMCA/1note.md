* [遇事不决，先看文档](https://carla.readthedocs.io/en/0.9.15/)

* 如果外部做了端口映射，carla本身也需要修改端口，详见：[Command-line options](https://carla.readthedocs.io/en/latest/start_quickstart/#command-line-options)

* 部署 nfs

    1. `kubectl apply -f persistentvolume-nfs.yaml` pv-nfs

    2. `kubectl apply -f persistentvolumeclaim-nfs.yaml` pvc-nfs

* 部署 carla
    
    1. `kubectl apply -f deployment-carla.yaml` deployment-carla

    2. `kubectl apply -f service-carla.yaml` service-carla

* 部署效果：

    ![carla-base](./img/carla-base.png)

* `tool/` 目录中有一些 `py api`

    * `get_world.py` 切换地图，获取当前地图的信息，并且保存照片到 `world.png`


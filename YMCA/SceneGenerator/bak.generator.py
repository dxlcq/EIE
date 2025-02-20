import carla
import json
import time
import sys
import math
import cv2
from datetime import datetime
import os
import numpy as np
import gc

def setWeather(world, weatherScene):
    world.set_weather( 
        carla.WeatherParameters(
            cloudiness              = weatherScene["cloudiness"],
            
            precipitation           = weatherScene["precipitation"],
            precipitation_deposits  = weatherScene["precipitation_deposits"],
            
            sun_azimuth_angle       = weatherScene["sun_azimuth_angle"],
            sun_altitude_angle      = weatherScene["sun_altitude_angle"]
        )
    )

def setVehicles(world, vehicleScene, log_root):
    vehicle = world.spawn_actor(
        world.get_blueprint_library().find(vehicleScene["type"]),
        carla.Transform(
            carla.Location(
                x = vehicleScene["transform"]["x"],
                y = vehicleScene["transform"]["y"],
                z = vehicleScene["transform"]["z"]
            ),
            carla.Rotation(
                pitch = vehicleScene["transform"]["pitch"],
                yaw = vehicleScene["transform"]["yaw"],
                roll = vehicleScene["transform"]["roll"]
            )
        )
    )
    # 开启自动驾驶
    if vehicleScene["auto"] == "true":
        vehicle.set_autopilot(True)

    # 创建日志目录
    log_dir = f"{log_root}/{datetime.now().strftime('%Y%m%d_%H%M%S')}_{vehicle.id}"
    os.makedirs(log_dir, exist_ok=True)

    # 初始化数据记录文件
    data_file = open(f"{log_dir}/vehicle_data.csv", "w")
    data_file.write("timestamp,speed(m/s),acceleration(m/s2),throttle,steer,brake\n")
    
    # 状态记录缓存
    last_speed = 0.0
    last_time = time.time()

    def record_vehicle_data():
        nonlocal last_speed, last_time
        current_time = time.time()
        delta_time = current_time - last_time
        
        # 获取车辆状态
        control = vehicle.get_control()
        velocity = vehicle.get_velocity()
        current_speed = 3.6 * math.sqrt(velocity.x**2 + velocity.y**2 + velocity.z**2)  # 转为km/h
        
        # 计算加速度
        acceleration = (current_speed - last_speed) / delta_time if delta_time > 0 else 0.0
        
        # 写入数据
        data_file.write(
            f"{current_time:.2f},"
            f"{current_speed/3.6:.2f},"  # 转回m/s
            f"{acceleration:.2f},"
            f"{control.throttle:.2f},"
            f"{control.steer:.2f},"
            f"{control.brake:.2f}\n"
        )

        # 保存数据
        data_file.flush()
        
        # 更新缓存
        last_speed = current_speed
        last_time = current_time

    # 摄像头配置函数
    def setup_camera(vehicle, config, log_dir):
        camera_bp = world.get_blueprint_library().find(config["type"])
        for attr, value in config["attributes"].items():
            camera_bp.set_attribute(attr, str(value))

        # 创建摄像头实例
        camera = world.spawn_actor(
            camera_bp,
            carla.Transform(
                carla.Location(*config["location"]),
                carla.Rotation(*config["rotation"])
            ),
            attach_to=vehicle
        )

        # 初始化视频写入器
        fourcc = cv2.VideoWriter_fourcc(*'XVID')
        out = cv2.VideoWriter(
            f"{log_dir}/{config['name']}.avi",
            fourcc,
            config["fps"],
            (config["width"], config["height"])
        )

        # 图像处理回调
        def sensor_callback(image):
            array = np.frombuffer(image.raw_data, dtype=np.uint8)
            array = np.reshape(array, (image.height, image.width, 4))
            array = array[:, :, :3]
            out.write(array)

        camera.listen(sensor_callback)
        return camera, out

    # 配置并安装摄像头
    cameras = []
    video_writers = []
    
    # 前置摄像头配置
    front_cam_config = {
        "type": "sensor.camera.rgb",
        "name": "front_view",
        "attributes": {
            "image_size_x": "1280",
            "image_size_y": "720",
            "fov": "110"
        },
        "location": (2.0, 0.0, 1.4),
        "rotation": (0.0, 0.0, 0.0),
        "fps": 20,
        "width": 1280,
        "height": 720
    }
    
    # 第三视角配置
    third_cam_config = {
        "type": "sensor.camera.rgb",
        "name": "third_person_view",
        "attributes": {
            "image_size_x": "1920",
            "image_size_y": "1080",
            "fov": "90"
        },
        "location": (-8.0, 0.0, 6.0),
        "rotation": (-25.0, 0.0, 0.0),
        "fps": 20,
        "width": 1920,
        "height": 1080
    }

    # 安装摄像头
    for config in [front_cam_config, third_cam_config]:
        cam, writer = setup_camera(vehicle, config, log_dir)
        cameras.append(cam)
        video_writers.append(writer)

    # 将资源引用附加到车辆对象
    vehicle.data_recorder = {
        "log_dir": log_dir,
        "data_file": data_file,
        "cameras": cameras,
        "writers": video_writers,
        "record_func": record_vehicle_data
    }

    return vehicle

def loadWorldScene(sceneFile):
    with open(sceneFile, 'r') as f:
        return json.load(f)

def lanChange(vehicle):
    distance = 20  # 检测距离（米）
    forward_vector = vehicle.get_transform().get_forward_vector()
    
    # 将射线起点前移（避开自车碰撞体）
    start_location = vehicle.get_location() + carla.Location(forward_vector.x * 3, forward_vector.y * 3, 1.0)
    end_location = start_location + forward_vector * distance

    # 发射射线
    raycast_result = vehicle.get_world().cast_ray(start_location, end_location)

    if raycast_result:
        hit_point = raycast_result[0]
        
        # 调试输出原始标签值
        print(f"射线命中标签值 (类型: {type(hit_point.label)}): {hit_point.label}")

        distance_to_hit = hit_point.location.distance(start_location)
        print(f"检测到前方车辆，距离射线起点: {distance_to_hit:.2f}米")

        # 安全距离判断
        if distance_to_hit < 5.0:
            print("执行左转避让")
            vehicle.apply_control(carla.VehicleControl(
                throttle=0.3, 
                steer=-0.5,
            ))
            vehicle.set_autopilot(False)

            return
                
    # 默认自动驾驶
    vehicle.set_autopilot(True)

def main():
    gc.collect()
    client = carla.Client(sys.argv[1], int(sys.argv[2]))
    client.set_timeout(10)

    client.reload_world()                   # 重新加载世界
    time.sleep(10)                          # 等待世界加载完成
    world = client.get_world()              # 获取世界
    scene = loadWorldScene(sys.argv[3])     # 加载场景

    vs = []

    try:
        if "weather" in scene:
            setWeather(world, scene["weather"])
        if "vehicles" in scene:
            for vehicleScene in scene["vehicles"]:
                vs.append(setVehicles(world, vehicleScene, "/mnt/nfs/"))
        
        for i in range(20): # 模拟20秒
            for v in vs:
                if v.type_id == "vehicle.tesla.model3":
                    lanChange(v)    # 是否需要变道
                    v.data_recorder["record_func"]()  # 记录车辆数据
            time.sleep(1)
    except KeyError as e:
        print(f"键错误: {e}")
    except RuntimeError as e:
        print(f"运行时错误: {e}")
    except KeyboardInterrupt:
        print("退出程序")

if __name__ == '__main__':
    main()
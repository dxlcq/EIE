import time
import carla
import json
import sys
import os
import cv2
from datetime import datetime
import numpy as np
import math
import openpyxl


# 精确添加路径（必须使用绝对路径）
#carla_root = "/root/EIE/YMCA/SceneGenerator/PythonAPI"
#sys.path.insert(0, f"{carla_root}/carla/dist/carla-0.9.15-py3.7-linux-x86_64.egg")
#sys.path.insert(1, f"{carla_root}/carla")  # 关键层级！

# 动态加载模块
#from importlib.machinery import SourceFileLoader

#basic_agent = SourceFileLoader(
#    'basic_agent',
#    f"{carla_root}/carla/agents/navigation/basic_agent.py"
#).load_module()

#from agents.navigation.basic_agent import BasicAgent
#from agents.navigation.behavior_agent import BehaviorAgent

def get_ttc(vehicle_a_data, vehicle_b_data):
    # 提取位置（忽略高度Z）
    a_loc = vehicle_a_data["location"]
    b_loc = vehicle_b_data["location"]
    dx = b_loc["x"] - a_loc["x"]
    dy = b_loc["y"] - a_loc["y"]
    distance = math.sqrt(dx**2 + dy**2)

    # 如果撞都撞了
    if distance < 5.0:
        return 0.0
    
    # 计算速度向量
    a_speed = {
        "x": vehicle_a_data["speed"] * vehicle_a_data["forward_vector"]["x"],
        "y": vehicle_a_data["speed"] * vehicle_a_data["forward_vector"]["y"]
    }
    b_speed = {
        "x": vehicle_b_data["speed"] * vehicle_b_data["forward_vector"]["x"],
        "y": vehicle_b_data["speed"] * vehicle_b_data["forward_vector"]["y"]
    }
    
    # 车辆A相对于B的速度
    rel_vx = b_speed["x"] - a_speed["x"]
    rel_vy = b_speed["y"] - a_speed["y"]

    # 分量修正方向
    closing_speed = -(dx * rel_vx + dy * rel_vy) / (distance + 1e-6)
    if closing_speed <= 1e-6:
        return -1
    
    # ttc
    ttc = distance / closing_speed
    return ttc if ttc >= 0 else -1

class Vehicle:
    # 车辆数据
    def get_vehicle_data(self):
        # 车辆速度
        current_speed = self.vehicle.get_velocity()
        current_speed = math.sqrt(current_speed.x**2 + current_speed.y**2 + current_speed.z**2)

        # 车辆位置
        current_location = self.vehicle.get_transform().location
        
        # 油门、转向、刹车数据
        control = self.vehicle.get_control()
        throttle = control.throttle
        steer = control.steer
        brake = control.brake

        # 车辆方向
        forward_vector = self.vehicle.get_transform().get_forward_vector()

        return {
            "speed": current_speed,
            "location": {
                "x": current_location.x,
                "y": current_location.y,
                "z": current_location.z
            },
            "throttle": throttle,
            "steer": steer,
            "brake": brake,
            "forward_vector": {
                "x": forward_vector.x,
                "y": forward_vector.y,
                "z": forward_vector.z
            }
        }

    # 简单导航
    def simple_navigate(self):
        vehicle = self.vehicle

        # 速度
        current_speed = vehicle.get_velocity()
        current_speed = math.sqrt(current_speed.x**2 + current_speed.y**2 + current_speed.z**2)
        
        # 位置
        target_location = self.targetLocation                   # 目标位置
        current_location = vehicle.get_transform().location     # 当前位置
        distance = current_location.distance(target_location)   # 相对距禽
        
        # 油门刹车大小统一控制
        vt = 0.0                # 油门大小
        vb = 0.0                # 刹车大小
        if current_speed < 4:
            vt = 0.5
            vb = 0.0
        elif current_speed > 4:
            vt = 0.0
            vb = 0.1

        # 右转
        if self.ttt == False and distance < 2:
            vehicle.apply_control(carla.VehicleControl(
                throttle=0.5,
                steer=0.6,      # 转向
                brake=vb
            ))
            self.ttt = True     # 已经转向
            time.sleep(0.9)
            return

        # 左转
        if self.ttt == True and distance > 4:
            vehicle.apply_control(carla.VehicleControl(
                throttle=0.5,
                steer=-0.6,     # 转向
                brake=vb
            ))
            self.ttt = False
            time.sleep(0.9)
            return

        vehicle.apply_control(carla.VehicleControl(
            throttle=vt,
            brake=vb
        ))


    # 将场景中的车辆信息转换为车辆对象，并添加到世界中，logPath为日志路径
    def __init__(self, world, vehicleScene, logPath):
        # 将车辆添加到世界中
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

        # 有 auto 就不管啦
        self.aaa = False

        if vehicleScene["auto"] == "true":
            vehicle.set_autopilot(True)
            self.vehicle = vehicle
            self.aaa = True
            return

        #targetVelocity = carla.Vector3D(x=2.0, y=0.0, z=0.0)
        #vehicle.enable_constant_velocity(targetVelocity)

        self.vehicle = vehicle
        self.targetLocation = carla.Location(
            x = vehicleScene["target"]["x"],
            y = vehicleScene["target"]["y"],
            z = vehicleScene["target"]["z"]
        )
        self.ttt = False

def loadScene(sceneFile):
    with open(sceneFile, 'r') as f:
        return json.load(f)

def setWeather(world, sceneWeather):
    world.set_weather( 
        carla.WeatherParameters(
            # 云量
            cloudiness              = sceneWeather["cloudiness"],
            
            precipitation           = sceneWeather["precipitation"],
            precipitation_deposits  = sceneWeather["precipitation_deposits"],
            
            sun_azimuth_angle       = sceneWeather["sun_azimuth_angle"],
            sun_altitude_angle      = sceneWeather["sun_altitude_angle"]
        )
    )
    print("设置天气成功")

def main(): # python generator.py localhost 32000 scene1.json /mnt/nfs/job1
    scene = loadScene(sys.argv[3])  # 获取场景信息
    client = carla.Client(sys.argv[1], int(sys.argv[2]))
    client.set_timeout(10)

    try:
        #client.reload_world()       # 重新加载世界
        #time.sleep(10)              # 等待世界加载完成
        world = client.get_world()  # 获取世界
        print("连接到 CARLA 成功")
    except RuntimeError as e:
        print(f"连接到 CARLA 失败: {e}")
        return

    try:
        vs = []

        if "weather" in scene:
            setWeather(world, scene["weather"])
        if "vehicles" in scene:
            for sceneVehicle in scene["vehicles"]:
                vs.append(Vehicle(world, sceneVehicle, sys.argv[4]))
        
        # 保存数据
        logDir = f"{sys.argv[4]}/{datetime.now().strftime('%m%d_%H:%M:%S')}"
        os.makedirs(logDir, exist_ok=True)
        # 全局摄像头
        cameraGodBp = world.get_blueprint_library().find("sensor.camera.rgb")
        cameraGodBp.set_attribute('image_size_x', '1080')
        cameraGodBp.set_attribute('image_size_y', '1920')
        cameraGod = world.spawn_actor(
            cameraGodBp,
            carla.Transform(
                carla.Location(x=20.0, y=-60.0, z=30.0),
                carla.Rotation(pitch=-90.0, yaw=0.0, roll=0.0)
            )
        )
        
        # 写入视频，只能在函数外
        fource = cv2.VideoWriter_fourcc(*'XVID')
        out = cv2.VideoWriter(f"{logDir}/Godlike.avi", fource, 30.0, (1080, 1920))
        # 回调函数
        def godlikeCameraCallback(image):
            array = np.frombuffer(image.raw_data, dtype=np.uint8)
            array = np.reshape(array, (image.height, image.width, 4))
            array = array[:, :, :3]
            
            out.write(array)
        # 绑定回调函数
        cameraGod.listen(godlikeCameraCallback)

        wb = openpyxl.Workbook()
        ws = wb.active
        ws.append(["时间", "A车辆ID", "A速度", "A位置", "A油门", "A转向", "A刹车", "A方向", "B车辆ID", "B速度", "B位置", "B油门", "B转向", "B刹车", "B方向", "TTC"])

        # 运行
        start_time = time.time()
        while True:
            runtime = time.time() - start_time
            if runtime > 20.0:   # 跑个 20 秒
                break
            
            a_data = vs[0].get_vehicle_data()
            b_data = vs[1].get_vehicle_data()

            ws.append([
                runtime,
                vs[0].vehicle.id, a_data["speed"], f'{a_data["location"]}', a_data["throttle"], a_data["steer"], a_data["brake"], f'{a_data["forward_vector"]}',
                vs[1].vehicle.id, b_data["speed"], f'{b_data["location"]}', b_data["throttle"], b_data["steer"], b_data["brake"], f'{b_data["forward_vector"]}',
                get_ttc(a_data, b_data)
            ])
            
            # 运行计算
            for v in vs:
                if v.aaa == False:
                    v.simple_navigate()
        
            time.sleep(0.02)

        wb.save(f"{logDir}/data.xlsx")
        out.release()
        
    except Exception as e:
        print(f"scene.json 错误: {e}")
    except RuntimeError as e:
        print(f"运行时错误: {e}")
    except KeyboardInterrupt:
        print("退出程序")
    
    for v in vs:
        if v.vehicle.is_alive:
            v.vehicle.destroy()
    print("释放资源，拜拜！")


if __name__ == "__main__":
    main()
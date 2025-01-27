import carla
import json
import time
import sys
import math

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

def setVehicles(world, vehicleScene):
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

        # 安全距离判断（需大于5米）
        if distance_to_hit < 5.0:
            print("执行左转避让")
            vehicle.apply_control(carla.VehicleControl(
                throttle=0.5, 
                steer=-0.5,
                brake=0.0
            ))
            vehicle.set_autopilot(False)
            return
                
    # 默认自动驾驶
    vehicle.set_autopilot(True)

def main():
    client = carla.Client(sys.argv[1], int(sys.argv[2]))
    client.set_timeout(10)  

    client.reload_world()                   # 重新加载世界
    time.sleep(6)                           # 等待世界加载完成
    world = client.get_world()              # 获取世界
    scene = loadWorldScene(sys.argv[3])     # 加载场景

    vs = []

    try:
        if "weather" in scene:
            setWeather(world, scene["weather"])
        if "vehicles" in scene:
            for vehicleScene in scene["vehicles"]:
                vs.append(setVehicles(world, vehicleScene))
        
        while True:
            for v in vs:
                if v.type_id == "vehicle.tesla.model3":
                    lanChange(v)    # 是否需要变道    
            time.sleep(1)
    except KeyError as e:
        print(f"键错误: {e}")
    except RuntimeError as e:
        print(f"运行时错误: {e}")
    except KeyboardInterrupt:
        print("退出程序")

if __name__ == '__main__':
    main()
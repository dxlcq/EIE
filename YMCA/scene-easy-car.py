import carla
import time
import random

def connect_to_carla(ip='10.0.0.30', port=32000, timeout=10.0):
    client = carla.Client(ip, port)
    client.set_timeout(timeout)
    return client.get_world()

def get_vehicle_blueprint(world):
    blueprints = world.get_blueprint_library().filter('vehicle.*')  # 获取所有车辆蓝图
    # 过滤掉不适用的蓝图（如不可用的模型）
    # 在 CARLA 0.9.15 中，没有 is_traffic_generator 属性，改为其他过滤条件
    # 例如，选择可用的车辆蓝图
    available_bps = [bp for bp in blueprints if bp.has_attribute('is_invincible') or bp.has_attribute('color')]
    if not available_bps:
        print("未找到可用的车辆蓝图。")
        return None
    # 随机选择一个蓝图
    vehicle_bp = random.choice(available_bps)
    return vehicle_bp

def get_spawn_point_near_zebra_crossing(world):
    # 示例坐标，请根据你的地图实际情况调整
    # 确保 z 轴接近地面
    spawn_location = carla.Location(x=-52, y=-20, z=1.0)  
    spawn_rotation = carla.Rotation(yaw=90)  # 车辆朝向
    spawn_point = carla.Transform(spawn_location, spawn_rotation)
    return spawn_point

def spawn_vehicle(world, blueprint, spawn_point):
    try:
        vehicle = world.spawn_actor(blueprint, spawn_point)
        print("车辆已成功生成。")
        return vehicle
    except Exception as e:
        print(f"车辆生成失败: {e}")
        return None

def main():
    world = connect_to_carla()
    vehicle_bp = get_vehicle_blueprint(world)
    
    if vehicle_bp is None:
        print("未找到合适的车辆蓝图，程序终止。")
        return
    
    spawn_point = get_spawn_point_near_zebra_crossing(world)
    vehicle = spawn_vehicle(world, vehicle_bp, spawn_point)

    if vehicle:
        # 启用自动驾驶
        vehicle.set_autopilot(True)
        print("自动驾驶已启用。")
    else:
        print("无法生成车辆。")
        return

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print("程序中断，销毁车辆。")
    finally:
        if vehicle:
            vehicle.destroy()
            print("车辆已销毁。")

if __name__ == '__main__':
    main()
import carla
import time
import math

def main():
    # 连接到 CARLA 服务器
    client = carla.Client('10.0.0.30', 32000)
    client.set_timeout(10.0)

    world = client.get_world()
    blueprints = world.get_blueprint_library()

    # 选择行人蓝图
    walker_bp = blueprints.filter('walker.pedestrian.*')[0]  # 选择第一个行人蓝图

    # 定义行人生成的位置，确保 z 轴在地面附近
    spawn_point = carla.Transform(carla.Location(x=-48, y=3.0, z=1.0))
    # (x=-48, y=3.0, z=1.0)
    # (x=-58, y=3.0, z=1.0)
    # (x=-38, y=3.0, z=1.0)

    # 生成行人
    walker = world.spawn_actor(walker_bp, spawn_point)

    # 确保行人成功生成
    if walker is not None:
        print("行人已成功生成。")
    else:
        print("行人生成失败。")
        return

    # 定义行走的两个目标点
    point_a = carla.Location(x=-59, y=3.0, z=1.0)
    point_b = carla.Location(x=-37, y=3.0, z=1.0)
    target = point_a  # 初始目标点

    try:
        while True:
            current_location = walker.get_location()
            distance = current_location.distance(target)
            
            # 当接近目标点时，切换目标点
            if distance < 1.0:
                if target == point_b:
                    target = point_a
                else:
                    target = point_b

            # 计算移动方向
            direction = carla.Location(
                target.x - current_location.x,
                target.y - current_location.y,
                target.z - current_location.z
            )
            length = math.sqrt(direction.x**2 + direction.y**2 + direction.z**2)
            if length > 0:
                direction.x /= length
                direction.y /= length
                direction.z /= length

            # 计算垂直方向的差异
            vertical_diff = target.z - current_location.z

            # 设置行人的速度，并考虑垂直运动
            speed = 1.4  # 米/秒，可以根据需要调整
            control = carla.WalkerControl()
            control.speed = speed

            # 如果有垂直差异，允许行人调整垂直速度
            if abs(vertical_diff) > 0.2 and abs(vertical_diff) < 1.0 and distance < 2.0:
                control.jump = True
                control.speed = 0.2
                control.direction = carla.Vector3D(direction.x, direction.y, vertical_diff)  # 设定垂直方向的运动
            else:
                control.jump = False
                control.direction = carla.Vector3D(direction.x, direction.y, 0)  # 保持平地行走

            # 应用控制
            walker.apply_control(control)

            # 设置行人朝向
            yaw = math.degrees(math.atan2(direction.y, direction.x))
            new_transform = carla.Transform(current_location, carla.Rotation(yaw=yaw))
            walker.set_transform(new_transform)

            # 等待下一次更新
            time.sleep(0.1)
    except KeyboardInterrupt:
        print("程序中断，销毁行人。")
    finally:
        walker.destroy()
        print("行人已销毁。")

if __name__ == '__main__':
    main()
import carla
import time

def main():
    client = carla.Client('10.0.0.31', 32000)
    client.set_timeout(1)  # 增加超时设置
    
    world = None

    for i in range(1, 5):  # 增加重试次数
        try:
            world = client.get_world()
            print(f"第 {i} 次获取 world 成功")
            break
        except Exception as e:
            print(f"第 {i} 次获取 world 失败: {e}")
    else:
        print("无法连接到 CARLA 服务器")
        return
    
    print(f"当前版本: {client.get_client_version()}")
    print(f"同步模式: {world.get_settings().synchronous_mode}")
    print(f"当前地图: {world.get_map().name}")
    print(f"可用地图: {client.get_available_maps()}")
    print(f"当前时间: {world.get_snapshot().timestamp}")
    print(f"天气情况: {world.get_weather()}")

    return 

    # 相机蓝图
    camera_bp = world.get_blueprint_library().find('sensor.camera.rgb')
    camera_spawn_point = carla.Transform(
        carla.Location(x=1.5, y=0.0, z=2.4),
        carla.Rotation(pitch=0.0, yaw=0.0, roll=0.0)
    )

    # 相机演员
    camera_actor = world.spawn_actor(camera_bp, camera_spawn_point)
    print(f"相机 ID: {camera_actor.id}")

    def process_image(image):
        image.save_to_disk('img.jpg')
        print("保存图片")

    camera_actor.listen(process_image)

    try:
        while True:
            #world.tick()    # 更新世界
            print(f"当前时间: {world.get_snapshot().timestamp}")
            time.sleep(1)
    except KeyboardInterrupt:
        print("退出程序")
    except RuntimeError as e:
        print(f"运行时错误: {e}")
    finally:
        camera_actor.destroy()

if __name__ == '__main__':
    main()
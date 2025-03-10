import carla
import time

def main():
    client = carla.Client('180.85.207.45', 32000)
    client.set_timeout(2)                       # 超时设置
    world = client.get_world()

    def change_world(string):
        try:
            print(f"切换地图: {string}")
            world = client.load_world(string)
        except RuntimeError as e:
            print(f"运行时错误: {e}")
        finally:
            return world
    
    #world = change_world("Town10HD")
    
    print(f"当前版本: {client.get_client_version()}")
    print(f"同步模式: {world.get_settings().synchronous_mode}")
    print(f"当前地图: {world.get_map().name}")
    print(f"可用地图: {client.get_available_maps()}")
    print(f"当前时间: {world.get_snapshot().timestamp}")
    print(f"天气情况: {world.get_weather()}")
    print(f"演员情况: {world.get_actors()}")

    # 相机蓝图
    camera_bp = world.get_blueprint_library().find("sensor.camera.rgb")
    camera_spawn_point = carla.Transform(
        carla.Location(x=20.0, y=-60.0, z=30.0),       # 前后，左右，上下
        carla.Rotation(pitch=-90.0, yaw=0.0, roll=0.0)  # 俯仰，偏航，翻滚
    )
    camera_bp.set_attribute('image_size_x', '1080')  # 设置图像宽度
    camera_bp.set_attribute('image_size_y', '1920')  # 设置图像高度

    # 相机演员
    camera_actor = world.spawn_actor(camera_bp, camera_spawn_point)
    print(f"相机 ID: {camera_actor.id}")

    def process_image(image):
        image.save_to_disk("world.png")
        print("保存世界图像")

    camera_actor.listen(process_image)

    try:
        while True:
            print(f"当前时间: {world.get_snapshot().timestamp}")
            time.sleep(0.5)
            break
    except KeyboardInterrupt:
        print("退出程序")
    except RuntimeError as e:
        print(f"运行时错误: {e}")
    finally:
        camera_actor.destroy()
        print(f"相机 ID: {camera_actor.id} 已销毁")

if __name__ == '__main__':
    main()
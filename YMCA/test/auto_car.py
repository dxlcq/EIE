import carla
import random
import os
import time
import uuid
import cv2
import numpy as np

def main():
    # 连接到CARLA服务器
    client = carla.Client('10.0.0.30', 32000)
    client.set_timeout(10.0)  # 设置超时时间为10秒

    world = None
    vehicles = []
    cameras = []
    video_writers = {}

    try:
        # 获取世界（地图）
        world = client.get_world()
        blueprint_library = world.get_blueprint_library()

        # 选择车辆蓝图并启用自动驾驶
        vehicle_bp = random.choice(blueprint_library.filter('vehicle.*'))
        if vehicle_bp.has_attribute('role_name'):
            vehicle_bp.set_attribute('role_name', 'autopilot')

        # 获取随机生成点
        spawn_points = world.get_map().get_spawn_points()
        if not spawn_points:
            print("没有可用的生成点")
            return

        # 生成一到两辆车辆
        num_vehicles = 2  # 设置生成车辆的数量
        for i in range(num_vehicles):
            spawn_point = random.choice(spawn_points)
            vehicle = world.spawn_actor(vehicle_bp, spawn_point)
            vehicle.set_autopilot(True)  # 启用自动驾驶
            vehicles.append(vehicle)
            print(f"生成车辆: {vehicle.type_id}, ID: {vehicle.id}")

        # 添加相机传感器到每辆车
        for vehicle in vehicles:
            camera_bp = blueprint_library.find('sensor.camera.rgb')
            camera_bp.set_attribute('image_size_x', '800')
            camera_bp.set_attribute('image_size_y', '600')
            camera_bp.set_attribute('fov', '90')  # 视场角

            # 设置相机位置（相对于车辆）
            camera_transform = carla.Transform(carla.Location(x=1.5, z=2.4))

            # 生成相机传感器
            camera = world.spawn_actor(camera_bp, camera_transform, attach_to=vehicle)
            cameras.append(camera)
            print(f"相机已附加到车辆ID: {vehicle.id}")

        # 创建用于存储图像和视频的目录
        image_dir = "captured_images"
        video_dir = "captured_videos"
        os.makedirs(image_dir, exist_ok=True)
        os.makedirs(video_dir, exist_ok=True)

        # 为每个相机创建视频写入器
        for camera in cameras:
            video_path = os.path.join(video_dir, f"vehicle_{camera.id}.avi")
            fourcc = cv2.VideoWriter_fourcc(*'XVID')
            video_writer = cv2.VideoWriter(video_path, fourcc, 20.0, (800, 600))
            video_writers[camera.id] = video_writer
            print(f"视频写入器已创建: {video_path}")

        # 定义图像回调函数
        def save_image(image, camera_id):
            try:
                # 将图像数据转换为numpy数组
                array = np.frombuffer(image.raw_data, dtype=np.uint8)
                array = array.reshape((image.height, image.width, 4))  # BGRA格式

                # 转换为BGR格式（去除Alpha通道）
                bgr_array = array[:, :, :3]

                # 保存图像
                image_filename = os.path.join(image_dir, f"vehicle_{camera_id}_{image.frame}.png")
                cv2.imwrite(image_filename, bgr_array)
                print(f"图像已保存: {image_filename}")

                # 写入视频
                if camera_id in video_writers:
                    video_writers[camera_id].write(bgr_array)
            except Exception as e:
                print(f"处理图像时发生错误: {e}")

        # 绑定回调函数
        for camera in cameras:
            camera.listen(lambda image, camera_id=camera.id: save_image(image, camera_id))

        # 让模拟运行一段时间以捕捉图像和视频
        print("开始捕捉驾驶过程中的视频和图像，等待60秒...")
        time.sleep(60)

    except Exception as e:
        print(f"发生错误: {e}")

    finally:
        # 停止相机传感器
        for camera in cameras:
            try:
                camera.stop()
                print(f"相机传感器已停止: {camera.id}")
            except Exception as e:
                print(f"停止相机传感器时发生错误: {e}")

        # 确保所有回调函数完成
        time.sleep(1)  # 添加短暂延迟

        # 释放视频写入器
        for writer in video_writers.values():
            try:
                writer.release()
            except Exception as e:
                print(f"释放视频写入器时发生错误: {e}")
        print("所有视频写入器已释放")

        # 销毁生成的车辆和传感器
        for camera in cameras:
            try:
                camera.destroy()
            except Exception as e:
                print(f"销毁相机传感器时发生错误: {e}")

        for vehicle in vehicles:
            try:
                vehicle.destroy()
            except Exception as e:
                print(f"销毁车辆时发生错误: {e}")
        print("已销毁车辆和传感器")

if __name__ == '__main__':
    main()

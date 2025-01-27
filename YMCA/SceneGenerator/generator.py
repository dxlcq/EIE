import carla
import json
import time
import sys
import math

import numpy as np
import cv2
import threading
import time
from flask import Flask, Response

# 创建一个简单的Flask应用来提供视频流
app = Flask(__name__)

# 使用全局变量保存相机图像帧
frame = None
frame_lock = threading.Lock()
frame_event = threading.Event()

# 通过CARLA创建相机
def create_camera(client, world):
    blueprint_library = world.get_blueprint_library()
    camera_bp = blueprint_library.find('sensor.camera.rgb')

    # 设置相机属性（可根据需要调整分辨率、视场等）
    camera_bp.set_attribute('image_size_x', '1920')
    camera_bp.set_attribute('image_size_y', '1080')
    camera_bp.set_attribute('fov', '90')

    # 设置相机位置（直接在世界中生成）
    spawn_point = carla.Transform(carla.Location(x=-100.0, y=90.0, z=50.0), carla.Rotation(pitch=-90.0, yaw=0.0, roll=0.0))

    # 在世界中生成相机
    camera = world.spawn_actor(camera_bp, spawn_point)

    # 设置回调函数获取图像
    camera.listen(lambda image: process_image(image))
    return camera

# 处理相机图像
def process_image(image):
    global frame
    try:
        # 使用 Raw 转换器获取原始 BGRA 图像
        image.convert(carla.ColorConverter.Raw)
        
        # 将图像数据转换为 NumPy 数组
        array = np.frombuffer(image.raw_data, dtype=np.uint8)
        
        # 重塑数组形状为 (高度, 宽度, 4) 对应 BGRA
        array = array.reshape((image.height, image.width, 4))
        
        # 转换为 BGR 格式，去除 alpha 通道
        frame_bgr = array[:, :, :3]  # 直接使用 BGR，无需颜色转换
        
        # 更新全局帧
        with frame_lock:
            frame = frame_bgr
        
        # 通知有新帧可用
        frame_event.set()
        
    except Exception as e:
        print(f"Error processing image: {e}")

# 视频流生成器
def generate():
    while True:
        frame_event.wait()
        with frame_lock:
            if frame is None:
                frame_event.clear()
                continue
            ret, jpeg = cv2.imencode('.jpg', frame)
        if ret:
            yield (b'--frame\r\n'
                   b'Content-Type: image/jpeg\r\n\r\n' + jpeg.tobytes() + b'\r\n\r\n')
        frame_event.clear()

# Flask 路由，显示视频流
@app.route('/video_feed')
def video_feed():
    return Response(generate(),
                    mimetype='multipart/x-mixed-replace; boundary=frame')

# 启动 Flask 服务器
def run_flask():
    app.run(host='0.0.0.0', port=5000, threaded=True, debug=False)

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

        # 安全距离判断
        if distance_to_hit < 6.0:
            print("执行左转避让")
            vehicle.apply_control(carla.VehicleControl(
                throttle=0.5, 
                steer=-0.8,
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

    # 摄像机
    camera = create_camera(client, world)
    print("Camera spawned.")

    flask_thread = threading.Thread(target=run_flask)
    flask_thread.start()
    print("Flask server started at http://0.0.0.0:5000/video_feed")

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
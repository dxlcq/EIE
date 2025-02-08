import carla
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
    camera_bp.set_attribute('image_size_x', '800')
    camera_bp.set_attribute('image_size_y', '600')
    camera_bp.set_attribute('fov', '90')

    # 设置相机位置（直接在世界中生成）
    spawn_point = carla.Transform(carla.Location(x=-60.0, y=5.0, z=5.0), carla.Rotation(pitch=-30.0, yaw=-30.0, roll=0.0))

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

# 主函数
def main():
    client = carla.Client('10.0.0.30', 32000)
    client.set_timeout(10.0)

    try:
        world = client.get_world()
    except RuntimeError as e:
        print(f"Failed to connect to CARLA server: {e}")
        return

    camera = create_camera(client, world)
    print("Camera spawned.")

    flask_thread = threading.Thread(target=run_flask)
    flask_thread.start()
    print("Flask server started at http://0.0.0.0:5000/video_feed")

    try:
        while True:
            time.sleep(1)
    except KeyboardInterrupt:
        print("Interrupted by user")
    finally:
        camera.stop()
        camera.destroy()
        print("Camera destroyed. Exiting...")

if __name__ == '__main__':
    main()
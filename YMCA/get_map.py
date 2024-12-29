import carla

def main():
    # 连接到 CARLA 服务器
    client = carla.Client('180.85.206.248', 2000)
    client.set_timeout(10.0)  # 设置连接超时为10秒

    try:
        # 获取世界（地图）
        world = client.get_world()

        # 输出当前地图名称
        print(f"当前地图: {world.get_map().name}")

    except Exception as e:
        print(f"连接到 CARLA 服务器失败: {e}")

if __name__ == '__main__':
    main()
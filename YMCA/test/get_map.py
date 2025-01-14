import time
import carla

def main():
    client = carla.Client('10.0.0.31', 32000)
    client.set_timeout(0.1)

    max_attempts = 10
    attempts = 0
    world = None

    while attempts < max_attempts and world is None:
        try:
            world = client.get_world()
            print(f"获取 world 成功: {world}")
        except Exception as e:
            print(f"获取 world 失败，等待重试: {e}")
            attempts += 1

    if world is None:
        print("多次尝试后，仍然无法获取 world。")
    else:
        print(f"当前地图: {world.get_map().name}")

if __name__ == '__main__':
    main()
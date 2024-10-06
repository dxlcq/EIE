import concurrent.futures
import subprocess
import random

def run_command(*params):
    subprocess.run(['./optimized_icp'] + list(map(str, params)))

# 最大并行任务数
max_jobs = 18

run_command(0.765, 0.643, -0.027, -1.472, -0.6, 0.765, -0.023, 1.3, 0.006, 0.035, 0.999, -0.1)

# 生成随机参数
parameters = [tuple(round(random.uniform(-1, 1), 3) for _ in range(12)) for _ in range(10000)]

with concurrent.futures.ProcessPoolExecutor(max_workers=max_jobs) as executor:
    futures = [executor.submit(run_command, *param) for param in parameters]
    for future in concurrent.futures.as_completed(futures):
        future.result()  # 获取结果以处理异常

print("所有任务完成")


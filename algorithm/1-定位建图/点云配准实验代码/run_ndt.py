import concurrent.futures
import subprocess
import random

def run_command(i, j, k, l):
    subprocess.run(['./ndt_test', str(i), str(j), str(k), str(l)])

# 最大并行任务数
max_jobs = 18

# 先运行一次指定参数
run_command(0.69, 1, 0, 0)

# 生成随机参数
parameters = [(round(random.uniform(0.5, 1.5), 2),  # i 范围
               round(random.uniform(0.5, 1.5), 2),  # j 范围
               round(random.uniform(0.5, 1.5), 2),  # k 范围
               round(random.uniform(0.5, 1.5), 2))  # l 范围
              for _ in range(100)]  # 生成 100 个随机参数组合

with concurrent.futures.ThreadPoolExecutor(max_workers=max_jobs) as executor:
    futures = [executor.submit(run_command, *param) for param in parameters]
    for future in concurrent.futures.as_completed(futures):
        future.result()  # 获取结果以处理异常

print("所有任务完成")


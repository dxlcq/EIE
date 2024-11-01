import cv2
import numpy as np

# 读取图像
img = cv2.imread('./code/imgs/coin.jpg')

# 显示原图像
cv2.namedWindow('原图像', cv2.WINDOW_NORMAL)
cv2.imshow('原图像', img)

# 转换为灰度图像，从 BGR 到 GRAY
# 灰度图只有一个通道，每个像素点只有一个值，表示亮度，没有颜色信息，可以减少计算量
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# 显示灰度图像
cv2.namedWindow('灰度图像', cv2.WINDOW_NORMAL)
cv2.imshow('灰度图像', gray)

# 应用高斯模糊，降低噪声
# 高斯模糊是一种线性平滑滤波器，对图像进行平滑处理，使得同一物体的像素值更接近
blur = cv2.GaussianBlur(gray, (5, 5), 0)

# 显示高斯模糊图像
cv2.namedWindow('高斯模糊图像', cv2.WINDOW_NORMAL)
cv2.imshow('高斯模糊图像', blur)

# 使用 Otsu's 二值化进行阈值分割
# Otsu's 二值化是一种自适应阈值分割方法，可以根据图像的直方图自动确定二值化的阈值
# 该方法假设图像包含两个类（前景和背景），并且这两个类之间的阈值可以使类内方差最小
ret, thresh = cv2.threshold(blur, 0, 255, cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)

# 显示二值化图像
cv2.namedWindow('二值化图像', cv2.WINDOW_NORMAL)
cv2.imshow('二值化图像', thresh)

# 定义结构元素
# 结构元素是一个二值图像，用于定义操作的形状和大小
# 用于腐蚀、膨胀、开操作、闭操作等形态学操作
kernel = np.ones((3,3), np.uint8)

# 进行开操作（先腐蚀后膨胀），去除小的噪点
# 开操作可以去除小的噪点，平滑物体的边界
opening = cv2.morphologyEx(thresh, cv2.MORPH_OPEN, kernel, iterations=2)

# 显示开操作结果
cv2.namedWindow('开操作结果', cv2.WINDOW_NORMAL)
cv2.imshow('开操作结果', opening)

# 膨胀，确保硬币区域为白色
# 膨胀可以增加物体的面积，连接物体间的缝隙
sure_bg = cv2.dilate(opening, kernel, iterations=3)

# 显示膨胀结果
cv2.namedWindow('膨胀结果', cv2.WINDOW_NORMAL)
cv2.imshow('膨胀结果', sure_bg)

# 距离变换，获取前景区域
# 距离变换是一种图像的特征提取方法，可以获取图像中的前景区域
dist_transform = cv2.distanceTransform(opening, cv2.DIST_L2, 5)
ret, sure_fg = cv2.threshold(dist_transform, 0.7*dist_transform.max(), 255, 0)

# 显示前景区域
cv2.namedWindow('前景区域', cv2.WINDOW_NORMAL)
cv2.imshow('前景区域', sure_fg)

# 将前景区域转换为uint8类型
sure_fg = np.uint8(sure_fg)
unknown = cv2.subtract(sure_bg, sure_fg)

# 标记连通区域
ret, markers = cv2.connectedComponents(sure_fg)

# 增加1，确保背景为1而不是0
markers = markers + 1

# 标记未知区域为0
markers[unknown == 255] = 0

# 应用分水岭算法
# 分水岭算法是一种图像分割算法，可以将图像分割为多个连通区域
# 该算法基于图像的梯度信息，将图像视为地形，通过水流模拟地形的分割
markers = cv2.watershed(img, markers)
img[markers == -1] = [255, 0, 0]  # 标记边界

# 计算硬币的数量（连通区域数量减1，因为背景也被计数了）
num_coins = ret - 1
print("coin num:", num_coins)

cv2.waitKey(0)
cv2.destroyAllWindows()
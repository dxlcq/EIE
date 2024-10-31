import cv2
import numpy as np

# 读取图像
image = cv2.imread('imgs/cross_line.jpg')

# 检查图像是否成功读取
if image is None:
    print("Error: Image not found.")
    exit()

# 转换为灰度图像
gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)

# 高斯模糊去噪
blurred = cv2.GaussianBlur(gray, (5, 5), 0)

# 二值化处理
_, binary = cv2.threshold(blurred, 0, 255, cv2.THRESH_BINARY_INV + cv2.THRESH_OTSU)

# 定义结构元素
kernel = cv2.getStructuringElement(cv2.MORPH_RECT, (3, 3))

# 开运算（先腐蚀后膨胀）
#opening = binary
opening = cv2.morphologyEx(binary, cv2.MORPH_OPEN, kernel, iterations=2)

# 闭运算（先膨胀后腐蚀）
#closing = opening
closing = cv2.morphologyEx(opening, cv2.MORPH_CLOSE, kernel, iterations=2)

# 使用Canny算子检测边缘
edges = cv2.Canny(closing, 50, 150)

# 查找轮廓
contours, _ = cv2.findContours(edges, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

# 绘制轮廓
result = image.copy()
cv2.drawContours(result, contours, -1, (0, 255, 0), 2)

# 将二值化图像、开运算后的图像和闭运算后的图像转换为三通道图像
binary_color = cv2.cvtColor(binary, cv2.COLOR_GRAY2BGR)
opening_color = cv2.cvtColor(opening, cv2.COLOR_GRAY2BGR)
closing_color = cv2.cvtColor(closing, cv2.COLOR_GRAY2BGR)
edges_color = cv2.cvtColor(edges, cv2.COLOR_GRAY2BGR)

# 缩放比例
scale_percent = 40  # 可以根据需要调整


def show_resized_image(title, img):
    width = int(img.shape[1] * scale_percent / 100)
    height = int(img.shape[0] * scale_percent / 100)
    dim = (width, height)
    resized = cv2.resize(img, dim, interpolation=cv2.INTER_AREA)
    cv2.imshow(title, resized)


# 分别显示每个处理步骤的结果
show_resized_image('Original Image', image)
show_resized_image('Binary Image', binary_color)
show_resized_image('Opening Image', opening_color)
show_resized_image('Closing Image', closing_color)
show_resized_image('Edges Image', edges_color)
show_resized_image('Result Image', result)

cv2.waitKey(0)
cv2.destroyAllWindows()
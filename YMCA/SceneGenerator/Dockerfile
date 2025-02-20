FROM python:3.8-slim

# 设置环境变量，防止Python生成.pyc文件，确保stdout/stderr实时输出
ENV PYTHONDONTWRITEBYTECODE=1
ENV PYTHONUNBUFFERED=1

WORKDIR /app

RUN apt-get update && apt-get install -y \
    libgl1-mesa-glx \
    libglib2.0-0

RUN pip install carla==0.9.15 opencv-python numpy shapely networkx

COPY generator.py /app
COPY scene1.json /app
COPY scene2.json /app
COPY scene3.json /app

ENTRYPOINT ["python", "generator.py"]
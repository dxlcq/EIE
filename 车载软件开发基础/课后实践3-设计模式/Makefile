CXX = g++
CXXFLAGS = -std=c++20
TARGET = main
SRCS = main.cpp src/chassis.cpp src/eventpoll.cpp src/lidar.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

.PHONY: all clean
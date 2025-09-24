# 编译器
CXX = g++
CXXFLAGS = -std=c++17 -Wall -O2

# 库路径
LIBS = -ldl -lm ./libch347.so

# 源文件
SRCS = main.cpp ch341i2cmaster.cpp qmc5883l.cpp
OBJS = $(SRCS:.cpp=.o)

# 目标可执行文件
TARGET = test

# 默认目标
all: $(TARGET)

# 链接生成可执行文件
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

# 编译源文件为对象文件
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理
clean:
	rm -f $(TARGET) $(OBJS)

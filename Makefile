# 编译器设置
CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = 
LDLIBS = -lpthread

# 目标可执行文件
TARGETS = server client

# 默认目标
all: $(TARGETS)

# 服务器编译规则
server: server.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $< $(LDLIBS)

# 客户端编译规则
client: client.cpp
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

# 清理生成文件
clean:
	rm -f $(TARGETS) *.o

# 安装到系统目录（可选）
install: all
	cp $(TARGETS) /usr/local/bin

# 声明伪目标
.PHONY: all clean install
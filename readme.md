# Linux Socket 多客户端通信系统

基于TCP协议实现的Linux Socket多客户端通信系统，支持持续对话和并发连接。

## 功能特性

### 服务器端
- ✅ 多线程处理客户端连接（支持10并发）
- ✅ 消息回显功能（自动添加"Echo: "前缀）
- 🛑 智能连接检测（自动处理客户端断连）
- 📡 支持`exit`指令关闭连接
- 📊 实时显示客户端连接信息（IP/端口/socket ID）

### 客户端
- 🔄 持续交互式命令行界面
- 📨 消息发送/接收双工通信
- 🔒 安全退出机制（`exit`指令）
- 🌐 支持IPv4地址连接
- ⚡ 即时服务器响应显示

## 快速开始

```bash

# 编译项目
make clean && make

# 生成可执行文件
➜  ls -lh
-rwxr-xr-x 1 user user 25K Jul 20 10:00 client
-rwxr-xr-x 1 user user 32K Jul 20 10:00 server

# 启动服务器（终端1）
./server
Server listening on port 8080...

# 启动客户端（终端2）
./client
Connected to server at 127.0.0.1:8080
Enter message: 

# 启动更多客户端（终端3,4...）
# 每个客户端独立运行
```

## 使用示例
```bash
# 客户端1
Enter message: Hello
Server response: Echo: Hello
Enter message: How are you?
Server response: Echo: How are you?
Enter message: exit
Closing connection...

# 服务器端日志
New connection from 127.0.0.1:53972 (socket 4)
Received from client 4: Hello
Received from client 4: How are you?
Client 4 disconnected
```
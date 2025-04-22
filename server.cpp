#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

// 客户端处理线程函数
void* handle_client(void* arg) {
    int client_socket = *(int*)arg;
    free(arg);  // 释放动态分配的内存
    char buffer[BUFFER_SIZE] = {0};

    // 持续通信循环
    while(1) {
        memset(buffer, 0, BUFFER_SIZE);
        int valread = read(client_socket, buffer, BUFFER_SIZE);
        
        // 连接断开检测
        if(valread <= 0) {
            printf("Client %d disconnected\n", client_socket);
            break;
        }
        
        printf("Received from client %d: %s\n", client_socket, buffer);

        // 退出指令检测
        if(strcmp(buffer, "exit") == 0) {
            printf("Closing connection with client %d...\n", client_socket);
            break;
        }

        // 构造响应
        char response[BUFFER_SIZE];
        snprintf(response, BUFFER_SIZE, "Echo: %s", buffer);
        send(client_socket, response, strlen(response), 0);
    }

    close(client_socket);
    pthread_exit(NULL);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // 创建Socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 设置Socket选项
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 绑定端口
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // 开始监听
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", PORT);

    // 主接受循环
    while(1) {
        if ((new_socket = accept(server_fd, (struct sockaddr*)&address, 
                               (socklen_t*)&addrlen)) < 0) {
            perror("accept");
            continue;  // 继续接受其他连接
        }

        printf("New connection from %s:%d (socket %d)\n", 
               inet_ntoa(address.sin_addr), 
               ntohs(address.sin_port), 
               new_socket);

        // 为每个客户端创建线程
        pthread_t thread_id;
        int* client_socket = (int*)malloc(sizeof(int));  // 添加显式类型转换
        *client_socket = new_socket;

        if(pthread_create(&thread_id, NULL, handle_client, (void*)client_socket) != 0) {
            perror("pthread_create failed");
            close(new_socket);
            free(client_socket);
        } else {
            pthread_detach(thread_id);  // 分离线程，自动回收资源
        }
    }

    close(server_fd);
    return 0;
}
/*
 * socket
 * */
#include <cstdio>
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "socketclient.h"
using namespace std;

SocketClient::SocketClient(const char *ip_addr, int const ip_port)
{
	strcpy(IP, ip_addr);
	PORT = ip_port;
	init();
}

int SocketClient::client()
{
	int ret = 0;
	ret = connect(sockfd, (SA*)&addr, sizeof(addr));
	if (ret == -1)
   	{
		perror("Can not connect to server.");
		printf("Client start failed!\n");
		exit(-1);
	}
	printf("Client start succeed!\n");
	printf("Type <q> to exit.\n");

	do {
		ret = msg_proc(sockfd);
		if (ret == 0) {
			break;
		}
	} while(1);
}

void SocketClient::init()
{
	printf("IP: %s, port: %d\n", IP, PORT);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("socket");
		exit(-1);
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(IP);
}

int SocketClient::raw_input(char *str, int size)
{
	printf("[input]: ");
	fgets(str, size, stdin);
	return strlen(str);
}

int SocketClient::rcv(int sfd, char *buf, int size)
{
	int rsize = 0;
	rsize = read(sfd, buf, size);
	return rsize;
}

int SocketClient::send(int sfd, char const * const buf, int size)
{
	int wsize = 0;
	wsize = write(sfd, buf, size);
	return wsize;
}

void SocketClient::stop()
{
	close(sockfd);
	printf("service is going down...\n");
}

SocketClient::~SocketClient()
{
	stop();
}

void SocketClient::start()
{
	client();
}

int SocketClient::msg_proc(int sfd)
{
	int ret = 0;
	int size = 0;
	char msg[512] = {};
	char str[512] = {};
	memset(msg, 0, sizeof(msg));
	memset(str, 0, sizeof(str));

	size = raw_input(msg, sizeof(msg));
	size = send(sfd, msg, size + 1);
	if (0 == size)
	{
		// 发送失败则退出
		perror("write");
		return 0;
	}
	printf("[Client %d]: %s", sfd, msg);
	// 接收服务端返回的消息
	rcv(sfd, str, sizeof(str));
	printf("[Server]: %s", str);
	if (0 == strcmp(str, "q\n")) {
		ret = 0; //不再发送消息
	} else {
		ret = 1;
	}
	return ret;
}

void sig_close(int signo)
{
	printf("\ntrap signal: %d\n", signo);
	printf("Service is going down...\n");
	exit(0);
}
 
int main()
{
	signal(SIGINT, sig_close);	//定义信号处理函数
	SocketClient client("127.0.0.1");
	client.start();
	return 0;
}


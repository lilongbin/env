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
	sockfd = -1;
	init();
}

int SocketClient::client()
{
	int ret = 0;
	printf("Type <q> to exit.\n");

	do {
		//send
		ret = msg_proc(sockfd);
		if (ret == 0) {
			break;
		}
		usleep(1000);
	} while(1);
}

void SocketClient::init()
{
	int ret = 0;

	printf("IP: %s, port: %d\n", IP, PORT);
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("socket");
		exit(-1);
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(IP);

	ret = connect(sockfd, (SA*)&addr, sizeof(addr));
	if (ret == -1)
   	{
		perror("Can not connect to server.");
		printf("Client start failed!\n");
		exit(-1);
	}
	printf("Client start succeed!\n");
}

int SocketClient::raw_input(char *str, int size)
{
	printf("[input]: ");
	fgets(str, size, stdin);
	return strlen(str);
}

int SocketClient::recv(int sfd, char *buf, int size)
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

int SocketClient::get_sockfd()
{
	return sockfd;
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
	char msg[4096] = {};
	char str[4096] = {};
	memset(msg, 0, sizeof(msg));
	memset(str, 0, sizeof(str));

	size = raw_input(str, sizeof(str));
	strncpy(msg, str, size);
	size = send(sfd, msg, size);

	if (0 == size)
	{
		// 发送失败则退出
		perror("write");
		return 0;
	}
	printf("[Client %d]: \n%s\n", sfd, msg);

	if (0 == strcmp(str, "q\n")) {
		ret = 0; //不再发送消息
	} else {
		ret = 1;
	}
	size = recv(sockfd, str, sizeof(str));
	printf("[Server]: \n%s\n", str);

	return ret;
}


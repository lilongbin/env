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
#include <thread>
#include <sys/epoll.h>

#include "socketserver.h"

using namespace std;

SocketServer::SocketServer(const char *ip_addr, int const ip_port):websocket_handler_map()
{
	strcpy(IP, ip_addr);
	PORT = ip_port;
	timeout = 1000;
	if (true == isalive)
	{
		return;
	}
	init();
}

int SocketServer::set_nonblock(int fd)
{
	int flags;
	flags = fcntl(fd, F_GETFL, 0);
	if (-1 == flags)
	{
		flags = 0;
	}
	return fcntl(fd, F_SETFL, flags | O_NONBLOCK);
}

void SocketServer::event_ctl(int fd, bool flag)
{
	struct epoll_event event;
	event.data.fd = fd;
	event.events = flag ? EPOLLIN : 0;

	epoll_ctl(epollfd, flag ? EPOLL_CTL_ADD : EPOLL_CTL_DEL, fd, &event);
	if (flag)
	{
		printf("Client %d has connected.\n", fd);
		set_nonblock(fd);
		websocket_handler_map[fd] = new Websocket_Handler(fd);
	}
	else
	{
		close(fd);
		delete websocket_handler_map[fd];
		websocket_handler_map.erase(fd);
		printf("%d disconnected\n", fd);
	}
}

int SocketServer::server()
{
	int ret = 0;
	int size = 0;
	int nfds = 0;
	int i = 0;
	int sfd = 0;
	struct sockaddr_in sin;

	int reuseaddr = 1; //solve "address already in use" issue
	socklen_t len = sizeof(struct sockaddr_in);

	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr,
		   sizeof(reuseaddr));
 
	if (-1 == bind(sockfd, (SA *)&addr, sizeof(addr))) {
		perror("bind");
		exit(-1);
	}
 
	// 最多监听100路
	if (-1 == listen(sockfd, 100)) {
		perror("listen");
		exit(-1);
	}
	isalive = true;
	epollfd = epoll_create(MAX_EVENTS);
	event.events = EPOLLIN | EPOLLET;
	event.data.fd = sockfd;

	// add event
	ret = epoll_ctl(epollfd, EPOLL_CTL_ADD, sockfd, &event);
	if (0 > ret)
	{
		printf("epoll add failed: fd=%d\n", sockfd);
		exit(-1);
	}

	printf("Server init OK!\n");
	printf("Press <Ctrl+C> to stop service.\n");
	printf("Waiting for clients to connect ...\n");

	while (1) {
		nfds = epoll_wait(epollfd, eventList, MAX_EVENTS, timeout);
		if (0 > nfds)
		{
			perror("epoll error");
			break;
		}
		else if (0 == nfds)
		{
			// printf("timeout ...\n");
			continue;
		}

		for (i = 0; i < nfds; i++)
		{
			if (eventList[i].data.fd == sockfd)
			{
				//accept
				sfd = accept(sockfd, (struct sockaddr *)&sin, &len);
				if (sfd < 0)
				{
					perror("accept");
					continue;
				}
				event_ctl(sfd, true);
			}
			else if (eventList[i].events & EPOLLIN)
			{
				//recv data
				sfd = eventList[i].data.fd;
				if (0 > sfd)
				{
					continue;
				}
				Websocket_Handler *handler = websocket_handler_map[sfd];
				if (NULL == handler)
				{
					continue;
				}
				// msg_proc(sfd);
				size = read(sfd, handler->getbuff(), 2048);
				if (0 >= size)
				{
					// 接收消息失败
					perror("read");
					event_ctl(sfd, false);
				}
				else
				{
					handler->process();
				}
			}
		}
	}
}

void SocketServer::init()
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

int SocketServer::rcv(int sfd, char *buf, int size)
{
	int rsize = 0;
	rsize = read(sfd, buf, size);
	return rsize;
}

int SocketServer::send(int sfd, char const * const buf, int size)
{
	int wsize = 0;
	wsize = write(sfd, buf, size);
	return wsize;
}

void SocketServer::stop()
{
	close(sockfd);
	close(epollfd);
	isalive = false;
	printf("service is going down...\n");
}

SocketServer::~SocketServer()
{
	stop();
}

void SocketServer::start()
{
	server();
}

int SocketServer::msg_proc(int sfd)
{
	int ret = 0;
	int size = 0;
	char msg[512] = {};
	char str[512] = {};
	memset(msg, 0, sizeof(msg));
	memset(str, 0, sizeof(str));

	size = rcv(sfd, str, sizeof(str));
	if (0 == size)
	{
		// 接收消息失败
		perror("read");
		event_ctl(sfd, false);
		return 0;
	}
	printf("[Client %d]: %s", sfd, str);
	//给客户端返回消息
	printf("server processing ...\n");
	strcpy(msg, str);
	size = send(sfd, msg, strlen(msg)+1);
	if (0 == size) {
		return 1;
	}
	printf("server reply: %s", msg);
}



/*
 * socket
 * */
#ifndef SOCKETSERVER_H__
#define SOCKETSERVER_H__
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
#include <sys/epoll.h>

#define MAX_EVENTS 500
using namespace std;

class SocketServer {
private:
	int isalive;
	int timeout;
	int sockfd;
	char IP[16]; //server写自己的ip,client写对方的IP
	short PORT;
	typedef struct sockaddr SA;
	struct sockaddr_in addr;
	int epollfd;
	struct epoll_event eventList[MAX_EVENTS];
	struct epoll_event event;

public:
	SocketServer(const char *ip_addr, int const ip_port=10111);
	void start();
	void stop();
	~SocketServer();
private:
	int set_nonblock(int fd);
	void event_ctl(int fd, bool flag);
	int server();
	void init();
	static int rcv(int sfd, char *buf, int size);
	static int send(int sfd, char const * const buf, int size);
	int msg_proc(int sfd);
};

#endif

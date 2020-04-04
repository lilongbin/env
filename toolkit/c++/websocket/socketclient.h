/*
 * socket
 * */
#ifndef SOCKETCLIENT_H__
#define SOCKETCLIENT_H__
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

using namespace std;
class SocketClient {
public:
	int sockfd;
	char IP[16]; //server写自己的ip,client写对方的IP
	short PORT;
	typedef struct sockaddr SA;
	struct sockaddr_in addr;

	SocketClient(const char *ip_addr, int const ip_port=10111);
	int client();
	void init();

	int raw_input(char *str, int size);
	int recv(int sfd, char *buf, int size);
	int send(int sfd, char const * const buf, int size);
	int get_sockfd();	
	int msg_proc(int sfd);
	void start();
	void stop();
	~SocketClient();
};

#endif

/*
 * websocket
 * */
#ifndef WEBSOCKETCLIENT_H__
#define WEBSOCKETCLIENT_H__
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
class WebSocketClient
{
public:
	int *socket;
	int sockfd;
	int isConnected;
	char IP[16]; //server写自己的ip,client写对方的IP
	short PORT;

	WebSocketClient(const char *ip_addr, int const ip_port=10111);
	void start();
	void stop();
	~WebSocketClient();

	int websocket_handshake();
	int fill_http_msg(char *reply);
	int parse(char *buf, uint64_t size, char *&payload, uint64_t &payload_len);
	int request(char *buf, uint64_t size);
	int raw_input(char *str, int size);
	int recv(char *buf, int &size);
	int send(char *buf, int size);
};

#endif

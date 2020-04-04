/*****************************************
* Copyright (C) 2018 * Ltd. All rights reserved.
* 
* File name   : server.cpp
* Author      : longbin
* Created date: 2018-07-18 15:53:05
* Description : 
*
*******************************************/
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

void sig_close(int signo)
{
	printf("\ntrap signal: %d\n", signo);
	printf("Service is going down...\n");
	exit(0);
}
 
int main()
{
	signal(SIGINT, sig_close);	//定义信号处理函数
	SocketServer server("127.0.0.1");
	server.start();
	return 0;
}


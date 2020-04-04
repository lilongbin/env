/*****************************************
* Copyright (C) 2018 * Ltd. All rights reserved.
* 
* File name   : client.cpp
* Author      : longbin
* Created date: 2018-07-18 15:57:38
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
#include <pthread.h>

#include "websocket_client.h"

WebSocketClient wsc("127.0.0.1");
pthread_t tid1, tid2;

void * send_task(void *)
{
	int size = 0;
	char buf[512] = {0};
	char str[512] = {0};

	while (1)
	{
		printf("[input]: ");
		fgets(str, sizeof(str), stdin);
		snprintf(buf, sizeof(buf), "%s", str);
		wsc.send(buf, strlen(buf));
		usleep(200000);
	}
}

void * recv_task(void *)
{
	int size = 0;
	char buf[512] = {0};

	while (1)
	{
		memset(buf, 0, sizeof(buf));
		wsc.recv(buf, size);
	}
}

void sig_close(int signo)
{
	printf("\ntrap signal: %d\n", signo);
	pthread_cancel(tid1);
	pthread_cancel(tid2);
	printf("Service is going down...\n");
	exit(0);
}
 
int main()
{
	signal(SIGINT, sig_close);	//定义信号处理函数

	int res = 0;
    res = pthread_create(&tid1, 0, send_task, NULL);
	res += pthread_create(&tid2, 0, recv_task, NULL);
	if (res)
	{
		printf("ERROR: %s\n", strerror(res));
		return 1;
	}

	while (1)
	{
		sleep(1);
	}
	return 0;
}


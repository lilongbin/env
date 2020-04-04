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
#include "websocket_client.h"

using namespace std;

WebSocketClient::WebSocketClient(const char *ip_addr, int const ip_port)
{
	strcpy(IP, ip_addr);
	PORT = ip_port;
	socket = (int *)new SocketClient(IP, PORT);
	sockfd = ((SocketClient *)socket)->get_sockfd();
	start();
}

void WebSocketClient::start()
{
	int ret = 0;
	int times = 0;

	isConnected = 0;
	printf("IP: %s, port: %d\n", IP, PORT);
	do {
		ret = websocket_handshake();
		if (true == ret)
		{
			break;
		}

		sleep(2);
		if (times++ >= 3)
		{
			break;
		}
	} while (isConnected == 0);
}

int WebSocketClient::raw_input(char *str, int size)
{
	printf("[input]: ");
	fgets(str, size, stdin);
	return strlen(str);
}

int WebSocketClient::recv(char *payload, int &size)
{
	int rsize = 0;
	char buf[4096] = {0};

	memset(buf, 0, sizeof(buf));
	rsize = read(sockfd, buf, sizeof(buf));
	size = parse((char*)&buf, (uint64_t)rsize, payload, (uint64_t &)size);
	printf("[recv]: %s\n", payload);
	return size;
}

int WebSocketClient::send(char *buf, int size)
{
	int wsize = 0;
	printf("[send]: %s\n", buf);
	wsize = request(buf, size);
	return wsize;
}

void WebSocketClient::stop()
{
	printf("web socket client is going down...\n");
}

WebSocketClient::~WebSocketClient()
{
	stop();
	delete (SocketClient *)socket;
}

int WebSocketClient::websocket_handshake()
{
	uint64_t size = 0;
	char request_data[4096] = {0};

	size = fill_http_msg(request_data);
	write(sockfd, request_data, size);
	printf("[Send]:\n%s\n", request_data);

	memset(request_data, 0, strlen(request_data));
	size = read(sockfd, request_data, sizeof(request_data));
	if (0 == size)
	{
		printf("Web socket handshake failed!\n");
		return false;
	}

	printf("Web socket handshake OK!\n");
	// 认为websocket建立成功了
	printf("[Server]:\n%s\n", request_data);
	isConnected = 1;
	return true;
}

int WebSocketClient::parse(char *buf, uint64_t size, char *&payload, uint64_t &payload_len)
{
	bool mask = false;
	char mask_key[4] = {0};
	int mask_len = 0;
	int i = 0;
	uint64_t len = size;
	uint64_t pos = 0;

	// printf("buf: %s", buf);
	mask = buf[1] >> 7;
	if (mask)
	{
		mask_len = 4;
	}
	// printf("mask: %d\n", mask);

	if (len - 2 - mask_len < 126)
	{
		pos += 2 + mask_len;
	}
	else if (len - 2 - mask_len < 0xFFFF)
	{
		pos += 4 + mask_len;
	}
	else
	{
		pos += 10 + mask_len;
	}
	payload_len = len - pos;

	strncpy(payload, buf+pos, payload_len);
	if (mask)
	{
		strncpy(mask_key, buf+pos-mask_len, mask_len);
		for(i = 0; i < payload_len; i++)
		{
			payload[i] = (char)(payload[i] ^ mask_key[i % 4]);
		}
	}

	return payload_len;
}

int WebSocketClient::request(char *payload, uint64_t size)
{
	uint64_t len = size;
	uint64_t pos = 0;
	char buf[4096] = {0};

	buf[0] = 0x81;
	if (len < 126)
	{
		buf[1] = (char)len;
		pos += 2;
	}
	else if (len < 0xFFFF)
	{
		buf[1] = 126;
		buf[2] = (char)(len & 0xFF);
		buf[3] = (char)(len >> 8 & 0xFF);
		pos += 4;
	}
	else
	{
		buf[1] = 127;
		buf[2] = (char)(len >> 0 & 0xFF);
		buf[3] = (char)(len >> 8 & 0xFF);
		buf[4] = (char)(len >> 16 & 0xFF);
		buf[5] = (char)(len >> 24 & 0xFF);
		buf[6] = (char)(len >> 32 & 0xFF);
		buf[7] = (char)(len >> 40 & 0xFF);
		buf[8] = (char)(len >> 48 & 0xFF);
		buf[9] = (char)(len >> 56 & 0xFF);
		pos += 10;
	}
	strncpy(&buf[pos], payload, len);

	len = write(sockfd, buf, strlen(buf));
	if (len == 0)
	{
		perror("write");
	}
	
	return true;
}

int WebSocketClient::fill_http_msg(char *buf)
{
	int ret = 0;
	int size = 0;

	strcat(buf, "GET / HTTP/1.1\r\n");
	strcat(buf, "Host: 127.0.0.1:10111\r\n");
	strcat(buf, "User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:44.0) Gecko/20100101 Firefox/44.0\r\n");
	strcat(buf, "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n");
	strcat(buf, "Accept-Language: en-US,en;q=0.5\r\n");
	strcat(buf, "Accept-Encoding: gzip, deflate\r\n");
	strcat(buf, "Sec-WebSocket-Version: 13\r\n");
	strcat(buf, "Origin: null\r\n");
	strcat(buf, "Sec-WebSocket-Extensions: permessage-deflate\r\n");
	strcat(buf, "Sec-WebSocket-Key: lf9ADAHZw6VlEr/aYHxOEQ==\r\n");
	strcat(buf, "Connection: keep-alive, Upgrade\r\n");
	strcat(buf, "Pragma: no-cache\r\n");
	strcat(buf, "Cache-Control: no-cache\r\n");
	strcat(buf, "Upgrade: websocket\r\n");

	return strlen(buf);
}


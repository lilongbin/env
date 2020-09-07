/*
 * tcp server
 * */
#include <stdio.h>
#include <iostream>
#include <string>
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
 
#define TCP_DATA_MAX_LEN 0x7FFF
#define TCP_DATA_HEADER_LEN 16
#define TCP_DATA_LEN_POS 12
#define NH_SWAP16(x) ((((uint16_t)(x) & 0xFF00) >> 8) | (((uint16_t)(x) & 0x00FF) << 8))
#define NH_SWAP32(x) ((((uint32_t)(x) & 0xFF000000) >> 24) | (((uint32_t)(x) & 0x00FF0000) >> 8) | (((uint32_t)(x) & 0x0000FF00) << 8) | (((uint32_t)(x) & 0x000000FF) << 24))


#define ASSERT_CONCAT_(a, b) a##b
#define STATIC_ASSERT(e, msg) ;enum { ASSERT_CONCAT_(assert_line_, __LINE__) = 1/(!!(e)) }
#pragma pack(push,1)
typedef struct {
    uint32_t seqId;
    uint32_t funcId;
    uint32_t cmdId;
    uint32_t dataLen;
} TcpMsgHeader_T;
STATIC_ASSERT(sizeof(TcpMsgHeader_T)==16, "RTPHeader_T size doesn't seem to be cool.");
#pragma pack(pop)

static int sg_sequeceId = 0;
static TcpMsgHeader_T sg_header {};

int sockfd;
const char* IP = "127.0.0.1";//服务端的IP
short PORT = 50013;
typedef struct sockaddr SA;
 
/* 1服务端初始化网络,开放端口*/
void init()
{
	printf("Server starting...\n");
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1) {
		perror("socket");
		exit(-1);
	}
	struct sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(PORT);
	addr.sin_addr.s_addr = inet_addr(IP);
 
	//solve "address already in use" issue
	int reuseaddr = 1;
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr,
		   sizeof(reuseaddr));
 
	if (bind(sockfd, (SA *) & addr, sizeof(addr)) == -1) {
		perror("bind");
		exit(-1);
	}
 
	if (listen(sockfd, 100) == -1) {
		perror("listen");
		exit(-1);
	}
	printf("Server init OK!\n");
}
 
/* 接收client message的函数 */
void rcv_proc(int sfd)
{
	int size = 0;
	char buf[512] = { };
	while (size = read(sfd, buf, sizeof(buf))) {
		printf("Client %d say>: %s", sfd, buf);
		if (!write(sfd, buf, strlen(buf) + 1)) {
			perror("write");
		}
		memset(buf, 0, 100);
	}
	printf("%d has disconnect!\n", sfd);
}
 
void snd_proc(int sfd)
{
	int size = 0;
    std::string str = "hello client";
	char buf[500]{};
    int bufflen = 0;
    do {
        sg_sequeceId += 1;
        sg_header.seqId = sg_sequeceId;
        sg_header.funcId = 1;
        sg_header.cmdId += 1;
        sg_header.dataLen = (int)str.length();
        //bufflen = 0;
        (void)memcpy(buf+0, &sg_header, sizeof(TcpMsgHeader_T));
        bufflen += sizeof(TcpMsgHeader_T);
        (void)memcpy(buf+sizeof(TcpMsgHeader_T), str.c_str(), str.length());
        bufflen += str.length();
		printf("send to client %d: %s, seq:%d, cmd=%d, len=%d\n",
                sfd, buf+sizeof(TcpMsgHeader_T), sg_header.seqId, sg_header.cmdId, sg_header.dataLen);
        sleep(1);
	} while (size = write(sfd, buf, bufflen));
    perror("write");
	printf("%d has disconnect!\n", sfd);
}

//2处理客户端的连接
void service()
{
	printf("Server has started!\n");
	while (1) {
		struct sockaddr_in fromaddr;
		socklen_t len = sizeof(fromaddr);
		printf("\nWaiting for clients to connect...\n");
		int fd = accept(sockfd, (SA *) & fromaddr, &len);
		if (fd == -1) {
			printf("Client connect failed!\n");
			continue;	//继续处理下一个连接
		} else {
			printf("Has connect to client: %d\n", fd);
		}
        snd_proc(fd);
		//rcv_proc(fd);
	}
}
 
void sig_close(int sig)
{
	close(sockfd);
	printf("Server is going down... since signal:%d\n", sig);
	exit(0);
}
 
int main()
{
	signal(SIGINT, sig_close);	//定义信号处理函数
	printf("Press <Ctrl+C> to stop server.\n");
	//初始化网络
	init();
	//启动服务
	service();
	return 0;
}

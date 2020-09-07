/*
 * UDP网络编程服务端
 */
#include <stdio.h>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
 
#define UDP_DATA_MAX_LEN 0x7FFF
#define UDP_DATA_HEADER_LEN 16
#define UDP_DATA_LEN_POS 12
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
} UdpMsgHeader_T;
STATIC_ASSERT(sizeof(UdpMsgHeader_T)==16, "RTPHeader_T size doesn't seem to be cool.");
#pragma pack(pop)

static int sg_sequeceId = 0;
static UdpMsgHeader_T sg_header {};

#define LOCALIP "127.0.0.1"
#define LOCALPORT 50015
#define PEERIP "127.0.0.1"
#define PEERPORT 50014

int main() {
    /* 1获取socket描述符 */
    int sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    if (sockfd == -1) {
        perror("socket"), exit(-1);
    }

    /* 2准备通信地址 */
    struct sockaddr_in servaddr;
    servaddr.sin_family = PF_INET;
    servaddr.sin_port = htons(LOCALPORT);	//host to network hostshort
    servaddr.sin_addr.s_addr = inet_addr(LOCALIP);
    //servaddr.sin_addr = "172,17.5.3"; //类型不兼容,不可以
    //servaddr.sin_addr.s_addr = inet_addr("172.17.5.3");/* 取结构中的成员转换函数 */

    /* 3绑定 */
    //bind IP地址为本服务器IP
    int res = 0;
    res = bind(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    if (res == -1) { perror("bind"), exit(-1);}
    printf("bind ok\n");

    char buf[100] = { };
    char arr[100] = { };
    int len = 0;
    int iloop = 0;

#if 0
    /* 4通信 read/write(client connected) recvfrom/sendto */
    len = read(sockfd, buf, sizeof(buf));
    if (len == -1) {
        perror("read"), exit(-1);
    }
    printf("want to read %lu bytes, actrually read %d bytes:%s\n", sizeof(buf), len, buf);

    for (iloop=0; iloop<10; iloop++) {
        snprintf(arr, sizeof(arr), "fromserver#%d", iloop);
        len = write(sockfd, arr, strlen(arr)); //will return error for udp server
        if (len == -1) {
            printf("errno %d:%s\n", errno, strerror(errno));
            perror("write");
        }
        printf("%d write: %s\n", len, arr);
        sleep(1);
    }

#else

    /* 4通信 read/write(client connected) recvfrom/sendto */
    struct sockaddr cliaddr;
    socklen_t clilen = sizeof(cliaddr); /* cannot set to 0 */
    memset(&cliaddr, 0, sizeof(cliaddr)); /* should memset */
    //len = recvfrom(sockfd, buf, sizeof(buf), 0, &cliaddr, &clilen);
    char *from = inet_ntoa(((struct sockaddr_in *)&cliaddr)->sin_addr);
    int fromport = ntohs(((struct sockaddr_in *)&cliaddr)->sin_port);
    printf("%d recvfrom %s:%d %s\n", len, from, fromport, buf);

    UdpMsgHeader_T header {};
    char udpbuff[512] {};
    for (iloop=0; iloop<500; iloop++) {
        header.seqId += 1;
        header.funcId = 1;
        header.cmdId += 1;
        snprintf(arr, sizeof(arr), "fromserver#%d", iloop);
        header.dataLen = strlen(arr);
        (void)memcpy(udpbuff, &header, sizeof(header));
        (void)memcpy(udpbuff+sizeof(header), arr,  header.dataLen);
    # if 1
        struct sockaddr_in repaddr;
        socklen_t replen = sizeof(struct sockaddr); /* cannot set to 0 */
        memset(&repaddr, 0, sizeof(struct sockaddr)); /* should memset */

        repaddr.sin_family = PF_INET;
        repaddr.sin_port = htons(PEERPORT);//client port
        repaddr.sin_addr.s_addr = inet_addr(PEERIP); //client ip
        len = sendto(sockfd, udpbuff, sizeof(header)+header.dataLen, 0, (const struct sockaddr *)&repaddr, replen);
    # else
        len = sendto(sockfd, udpbuff, sizeof(header)+header.dataLen, 0, (const struct sockaddr *)&cliaddr, clilen);
    # endif
        if (len == -1) {
            printf("errno: %d\n", errno);
            perror("sendto");
        }
        printf("%d sendto: %s\n", len, arr);
        /* sleep(1);  */
        usleep(20000); /* 20ms */
    }
#endif

    //5关闭socket描述符
    close(sockfd);
    return 0;
}


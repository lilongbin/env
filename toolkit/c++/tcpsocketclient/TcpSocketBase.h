#ifndef __TCP_SOCKET_BASE_H__
#define __TCP_SOCKET_BASE_H__

#include <cstdio>
#include <iostream>
#include <thread>
#include <mutex>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>
#include <sys/msg.h>

#include "TcpSocketSingleton.h"
#include "TcpComm.h"

typedef enum {
    SOCKET_CLIENT_UNKNOWN,
    SOCKET_CLIENT_INIT,
    SOCKET_CLIENT_CONNECT,
} ETcpSocketLinkStatus_T;

#define TCP_SOCKET_READ_TIMEOUT_MILLIS 5000
#define TCP_SOCKET_READ_TIMEOUT_MAX_COUNT (12*5)

#define TCP_RECEIVER_IP "127.0.0.1"
#define TCP_RECEIVER_PORT 50014

#define TCP_SENDER_IP "127.0.0.1"
#define TCP_SENDER_PORT 50013

/* class TcpSocketBase */
class TcpSocketBase:public Singleton<TcpSocketBase>
{
    friend class Singleton<TcpSocketBase>;
public:
    TcpSocketBase();
    virtual ~TcpSocketBase();
    bool socketCreate();
    bool socketBind();
    bool socketConnectNonBlock();
    int32_t socketReadData(char * buffer, const int32_t &size);
    int32_t socketWriteData(char * buffer, const int32_t &size);
    bool isSocketConnected();
    bool getSocketFd(int32_t & socketFd);
    void socketClose();

private:
    bool socketConfigure();
    bool setNonBlocking(bool nonblocking);

    //std::mutex m_mutex;
    int32_t m_socketFd;
    struct sockaddr_in m_servAddr;
    struct sockaddr_in m_localAddr;
};

#endif  //__TCP_SOCKET_BASE_H__

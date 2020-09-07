#ifndef __TCP_SOCKET_CLIENT_H__
#define __TCP_SOCKET_CLIENT_H__

#include <cstdio>
#include <iostream>
#include <thread>
#include <mutex>
#include <algorithm>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
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

#include "TcpSocketBase.h"

/* class TcpSocketClient */
class TcpSocketClient
{
public:
    TcpSocketClient();
    virtual ~TcpSocketClient();
    bool doConnectToServer();
    void disConnectToServer();
    bool isTcpConnected();
    bool doWriteData(uint8_t* buf, const uint32_t &maxSize, uint32_t &len);
    bool doReadData(uint8_t* buf, const uint32_t &maxSize, uint32_t &len);

private:
    //std::mutex m_mutex;
    bool initSocketClient();
    bool doReadDataBlock(uint8_t* buf, const uint32_t &maxSize, uint32_t &len);
    bool doReadDataTimeout(uint8_t* buf, const uint32_t &maxSize, uint32_t &len);
    TcpSocketBase *m_socketClient;
    uint32_t m_timeoutMillis;
    //int32_t m_lastErrorNo;
    ETcpSocketLinkStatus_T m_socketLinkStatus;
};

#endif  //__TCP_SOCKET_CLIENT_H__

#ifndef __TCP_SOCKET_RECEIVER_H__
#define __TCP_SOCKET_RECEIVER_H__

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

#include "TcpSocketClient.h"

#define TCP_PACKET_MAX_LEN 0x7FFF

/* class TcpSocketReceiver */
class TcpSocketReceiver:public TcpSocketClient, public Singleton<TcpSocketReceiver>
{
    friend class Singleton<TcpSocketReceiver>;
public:
    TcpSocketReceiver();
    virtual ~TcpSocketReceiver();
    static void * dataReceiveTask(void *args);
    static void * dataProcessTask(void *args);
    void enableConnectSocket(bool connet);
    void startReceiveData(bool start);
    void stopTasks();

private:
    bool doReceiveData();
    void transferData();
    bool processTcpPacket(TcpMsgQueue_T &msg);

    //std::mutex m_mutex;
    using QueueMsgType = TcpMsgQueue_T;
    std::unique_ptr<ConcurrentQueue<QueueMsgType>> m_Queue;
    bool m_isThreadAlive;
    uint8_t * m_recvbuffer;
    uint32_t m_bufferLen;
    bool m_enableDataRecv;
    bool m_enableConnectSocket;
};

typedef struct {
    TcpSocketReceiver       * tcpSocketReceiver;
    void * arg;
} TcpSocketThreadArgs_T;

#endif  //__TCP_SOCKET_RECEIVER_H__

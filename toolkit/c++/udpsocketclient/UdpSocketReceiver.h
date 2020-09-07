#ifndef __UDP_SOCKET_RECEIVER_H__
#define __UDP_SOCKET_RECEIVER_H__

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

#include "UdpSocketClient.h"

#define UDP_PACKET_MAX_LEN 0x7FFF

/* class UdpSocketReceiver */
class UdpSocketReceiver:public UdpSocketClient, public Singleton<UdpSocketReceiver>
{
    friend class Singleton<UdpSocketReceiver>;
public:
    UdpSocketReceiver();
    virtual ~UdpSocketReceiver();
    static void * dataReceiveTask(void *args);
    static void * dataProcessTask(void *args);
    void enableConnectSocket(bool connet);
    void startReceiveData(bool start);
    void stopTasks();

private:
    bool doReceiveData();
    void transferData();
    bool processUdpPacket(UdpMsgQueue_T &msg);

    //std::mutex m_mutex;
    using QueueMsgType = UdpMsgQueue_T;
    std::unique_ptr<ConcurrentQueue<QueueMsgType>> m_Queue;
    bool m_isThreadAlive;
    uint8_t * m_recvbuffer;
    uint32_t m_bufferLen;
    bool m_enableDataRecv;
    bool m_enableConnectSocket;
};

typedef struct {
    UdpSocketReceiver       * tcpSocketReceiver;
    void * arg;
} UdpSocketThreadArgs_T;

#endif  //__UDP_SOCKET_RECEIVER_H__

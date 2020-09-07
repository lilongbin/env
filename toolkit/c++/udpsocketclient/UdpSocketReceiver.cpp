#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <memory.h>
#include <errno.h>

#include "UdpSocketClient.h"
#include "UdpSocketReceiver.h"

/*-----------UdpSocketReceiver--begin-----------*/
UdpSocketReceiver::UdpSocketReceiver()
{
    m_recvbuffer = new uint8_t[UDP_PACKET_MAX_LEN];
    // m_Queue = std::make_unique<ConcurrentQueue<QueueMsgType>>(UDP_SOCKET_READ_TIMEOUT_MILLIS);
    m_Queue = std::unique_ptr<ConcurrentQueue<QueueMsgType>>(new ConcurrentQueue<QueueMsgType>(UDP_SOCKET_READ_TIMEOUT_MILLIS));
    m_enableConnectSocket = false;
    m_enableDataRecv = false;
    m_isThreadAlive = true;
}

UdpSocketReceiver::~UdpSocketReceiver()
{
    ALOGW("%s", __func__);
    m_enableDataRecv = false;
    if (NULL != m_recvbuffer)
    {
        delete[] m_recvbuffer;
        m_recvbuffer = NULL;
    }
}

void UdpSocketReceiver::enableConnectSocket(bool connet)
{
    m_enableConnectSocket = connet;
    ALOGI("%s m_enableConnectSocket=%d", __func__, m_enableConnectSocket);
}

void UdpSocketReceiver::startReceiveData(bool start)
{
    m_enableDataRecv = start;
    if ((m_enableDataRecv == true) && (m_enableConnectSocket != true))
    {
        /* should connect first */
        enableConnectSocket(true);
    }
    ALOGI("%s m_enableDataRecv=%d", __func__, m_enableDataRecv);
}

void UdpSocketReceiver::stopTasks()
{
    ALOGW("%s m_isThreadAlive=%d", __func__, m_isThreadAlive);
    m_isThreadAlive = false;
}

void * UdpSocketReceiver::dataReceiveTask(void *args)
{
    bool linkStatus_ok = false;
    bool enableDataRecv = false;
    int time_interval = 0;
    UdpSocketThreadArgs_T *ThreadArgs = (UdpSocketThreadArgs_T *)args;
    if (ThreadArgs == NULL)
    {
        ALOGE("%s ThreadArgs is null", __func__);
        return 0;
    }
    UdpSocketReceiver *self = ThreadArgs->tcpSocketReceiver;
    if (self == NULL) {ALOGW("%s: invalid thread param. tcpSocketReceiver", __func__);}

    ALOGW("dataReceiveTask start");
    while (self->m_isThreadAlive) {
        if (enableDataRecv != self->m_enableDataRecv) {
            /* enableDataRecv flag changed */
            enableDataRecv = self->m_enableDataRecv;
            ALOGI("dataReceiveTask enableConnectSocket=%d, enableRecv=%d", self->m_enableConnectSocket, self->m_enableDataRecv);
        }

        if (!self->m_enableConnectSocket) {
            if (self->isUdpConnected()) {
                ALOGI("dataReceiveTask disConnectToServer");
                self->disConnectToServer();
            }
            /* wait about 300ms */
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            continue;
        }

        /* !(SOCKET_CLIENT_CONNECT && fd>0) && thread running */
        if (!self->isUdpConnected() && self->m_isThreadAlive) {
            /* connect first */
            linkStatus_ok = self->doConnectToServer();
            ALOGI("dataReceiveTask connectToServer linkStatus_ok=%s", linkStatus_ok ? "true" : "false");
            if (linkStatus_ok) {
                /* connect ok */
                time_interval = 0; /* reset time_interval */
            } else {
                /* connect failed */
                /* socket will be closed when create/bind/connect/read failed */
                /* Exponential degradation */
                /* 1 2 4 8 16 32 32 ... */
                if (time_interval > 5)
                {
                    time_interval = 5;
                }
                std::this_thread::sleep_for(std::chrono::milliseconds(1000 * (2<<time_interval) ));
                time_interval += 1;
                /* sleep for a while end */
                continue;
            }
        }

        if (self->doReceiveData()) {
            if (!self->m_enableDataRecv) {
                /* 1.stopped receiver while block reading, 2.drop this data */
                //ALOGV("dataReceiveTask will drop this data: receiver not enable");
                continue;
            }
            self->transferData();
        } else {
            /* socket will be closed when create/bind/connect/read failed */
            /* recv failed */
            if (!self->isUdpConnected()) {
                linkStatus_ok = false;
                ALOGI("dataReceiveTask linkStatus_ok=%s", linkStatus_ok ? "true" : "false");
            }
        }
    }

    ALOGW("dataReceiveTask exit since m_isThreadAlive false");
    return 0;
}

void * UdpSocketReceiver::dataProcessTask(void *args)
{
    /* pull msg and convert to packet */
    bool msgret = false;
    UdpMsgQueue_T msg;
    UdpSocketThreadArgs_T *ThreadArgs = (UdpSocketThreadArgs_T *)args;
    if (ThreadArgs == NULL)
    {
        ALOGE("%s ThreadArgs is null", __func__);
        return 0;
    }
    UdpSocketReceiver *self = ThreadArgs->tcpSocketReceiver;
    if (self == NULL) {ALOGW("%s: invalid thread param. tcpSocketReceiver", __func__);}

    ALOGW("dataProcessTask start");
    while (self->m_isThreadAlive)
    {
        if (self->m_Queue == NULL)
        {
            /* wait about 300ms */
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
            continue;
        }

        self->m_Queue->waitForItems();
        if (!self->m_isThreadAlive) { break; }

        //std::this_thread::sleep_for(std::chrono::milliseconds(5));
        //if (true != m_isThreadAlive) { break; }

        msgret = self->m_Queue->pull(msg);
        if (!msgret)
        {
            /* block read with timeout or msgqueue deactive */
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            continue;
        }
        /* process tcp packet */
        if (!self->processUdpPacket(msg))
        {
            /* drop invalid packet */
            continue;
        }
    }
    ALOGW("dataProcessTask exit since m_isThreadAlive false");
    return 0;
}

bool UdpSocketReceiver::doReceiveData()
{
    bool ret = false;

    do {
        if (!isUdpConnected()) {
            ALOGI("%s isUdpConnected false", __func__);
            ret = false;
            break;
        }
        if (NULL == m_recvbuffer) {
            ALOGI("%s m_recvbuffer is NULL", __func__);
            ret = false;
            break;
        }

        if (!doReadData((uint8_t *)m_recvbuffer, UDP_MSG_MAX_LEN, m_bufferLen)) {
            ALOGW("receive data error");
            ret = false;
            break;
        }

        ret = true;
    }
    while (0);

    return ret;
}

void UdpSocketReceiver::transferData()
{
    UdpMsgQueue_T msg {};
    std::vector<uint8_t> payload;
    if (m_Queue == NULL)
    {
        ALOGE("SocketData is null.");
        return;
    }
    if (m_recvbuffer == NULL) {
        ALOGE("m_recvbuffer is null");
        return;
    }
    if ((m_bufferLen < UDP_MSG_HEADER_LEN) || (m_bufferLen > UDP_PACKET_MAX_LEN))
    {
        ALOGE("m_bufferLen invalid:%d", m_bufferLen);
        return;
    }

    /* make sure that m_recvbuffer is not NULL and size is not too large */
    (void)memcpy(&msg.header, m_recvbuffer, sizeof(UdpMsgHeader_T));
    if (m_bufferLen != (UDP_MSG_HEADER_LEN + msg.header.dataLen)) {
        ALOGD("%s seq:0x%x, func:0x%x, cmd:0x%x, len=0x%x",
                __func__, msg.header.seqId, msg.header.funcId,
                msg.header.cmdId, msg.header.dataLen);
        ALOGW("%s data length(0x%X)+%d is not match with buffer length(%d)",
                __func__, msg.header.dataLen, UDP_MSG_HEADER_LEN, m_bufferLen);
        return;
    }
    payload.assign(m_recvbuffer+UDP_MSG_HEADER_LEN,
            m_recvbuffer+UDP_MSG_HEADER_LEN+msg.header.dataLen);
    msg.payload = std::move(payload);
    /* send to recv buff */
    if (m_Queue != NULL) {
        m_Queue->push(std::move(msg));
    }
}

bool UdpSocketReceiver::processUdpPacket(UdpMsgQueue_T &msg)
{
    std::string info {};
    UdpMsgHeader_T header = msg.header;
    ALOGD("%s seq:%u, func:%u, cmd:%u, len:%u", __func__, header.seqId, header.funcId, header.cmdId, header.dataLen);
    if (msg.payload.size() > 0) {
        info.assign(msg.payload.begin(), msg.payload.end());
        ALOGI("%s size:%zu, info:%s", __func__, info.size(), info.c_str());
    }
    return true;
}

/*-----------UdpSocket--end-----------*/


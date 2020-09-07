#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <memory.h>
#include <errno.h>

#include "UdpSocketBase.h"

/*-----------UdpSocketBase--begin-----------*/
UdpSocketBase::UdpSocketBase()
{
    m_socketFd = -1;
    /* set local addr for bind */
    bzero(&m_localAddr, sizeof(m_localAddr));
    m_localAddr.sin_family = AF_INET;
    m_localAddr.sin_addr.s_addr = inet_addr(UDP_RECEIVER_IP);
    m_localAddr.sin_port = htons(UDP_RECEIVER_PORT);
    /* set server addr for connect */
    bzero(&m_servAddr, sizeof(m_servAddr));
    m_servAddr.sin_family = AF_INET;
    m_servAddr.sin_addr.s_addr = inet_addr(UDP_SENDER_IP);
    m_servAddr.sin_port = htons(UDP_SENDER_PORT);
}

UdpSocketBase::~UdpSocketBase()
{
    ALOGW("%s", __func__);
    socketClose();
}

bool UdpSocketBase::socketCreate()
{
    bool ret = false;
    int socketFd = -1;

    socketFd = socket(AF_INET, SOCK_DGRAM, 0);
    do {
        if (socketFd < 0) {
            ALOGW("%s error: %s", __func__, strerror(errno));
            break;
        }

        m_socketFd = socketFd;
        if (true != socketConfigure())
        {
            ALOGW("%s configure the socket error", __func__);
            break;
        }
        ret = true;
    } while (0);

    if ((true != ret) && (m_socketFd > 0)) {
        socketClose();
    }
    return ret;
}

bool UdpSocketBase::socketBind()
{
    bool ret = false;
    int32_t lastError;

    std::string localIp = inet_ntoa(m_localAddr.sin_addr);
    const int localPort = (int)ntohs(m_localAddr.sin_port);
    ALOGI("%s %s:%d", __func__, localIp.c_str(), localPort);

    do {
        if (m_socketFd < 0) {
            ALOGW("%s socket not created", __func__);
            break;
        }

        if (bind(m_socketFd, (struct sockaddr *)&m_localAddr, sizeof(m_localAddr)) < 0) {
            lastError = errno;
            ALOGW("%s bind error: %s", __func__, strerror(lastError));
            break;
        }
        ret = true;
    } while (0);

    if ((true != ret) && (m_socketFd > 0)) {
        socketClose();
    }
    return ret;
}

bool UdpSocketBase::socketConnectNonBlock()
{
    bool connect_ok = false;
    int ret = 0;
    fd_set wset;
    struct timeval timeValue;
    long timeoutMillis = 300;   /* 300ms */
    int32_t select_result = 0;
    int32_t lastError = 0;
    int retry_times = 0;

    std::string serverIp = inet_ntoa(m_servAddr.sin_addr);;
    const int serverPort = (int)ntohs(m_servAddr.sin_port);
    return true; //udp neednot connect;

    ALOGD("%s %s:%d", __func__, serverIp.c_str(), serverPort);
    do {
        if (m_socketFd < 0) {
            ALOGW("%s socket not created", __func__);
            return false;
        }
        if (!setNonBlocking(true)) {
            ALOGW("%s set non blocking error", __func__);
            break;
        }

        //ALOGD("%s udp neednot connect", __func__);
        ret = connect(m_socketFd, (struct sockaddr *)&m_servAddr, sizeof(m_servAddr)); 
        if (0 == ret) {
            /* connect established completed immediately */
            connect_ok = true;
            ALOGI("%s connect OK.", __func__);

            timeValue.tv_sec = timeoutMillis / 1000;
            timeValue.tv_usec = (timeoutMillis % 1000) * 1000;
            /* wait for some time to ensure connect work finish */
            select(1, NULL, NULL, NULL, &timeValue);
            break;
        }

        /* then process case(connect in process) or case(connect failed) */
        lastError = errno;
        if ((EWOULDBLOCK != lastError) && (EINPROGRESS != lastError)) {
            ALOGW("%s connect failed, error: %s", __func__, strerror(lastError));
            break;
        }

        /* socket connect Three-way handshake processing ... */
        while (retry_times++ < 5) {
            ALOGW("%s connect retry: %d", __func__, retry_times);
            do {
                timeValue.tv_sec = timeoutMillis / 1000;
                timeValue.tv_usec = (timeoutMillis % 1000) * 1000;
                FD_ZERO(&wset);
                FD_SET(m_socketFd, &wset);
                // fd+1 is necessary here, 1 more than the max FD is needed
                select_result = select(m_socketFd + 1, NULL, &wset, NULL, &timeValue);
                lastError = errno;
            } while (select_result < 0 && lastError == EINTR);

            if (select_result < 0) {
                /* select error occured */
                ALOGW("%s select error: %s", __func__, strerror(lastError));
                break;
            } else if (select_result > 0) {
                /* when readable or writeable, check if connect established */
                (void)connect(m_socketFd, (struct sockaddr *)&m_servAddr, sizeof(m_servAddr));
                lastError = errno;
                if (lastError == EISCONN) {
                    connect_ok = true;
                    ALOGW("%s connect established OK", __func__);
                    break;
                } else {
                    ALOGW("%s connect processing, error: %s", __func__, strerror(lastError));
                }
            } else {
                /* select return 0: timeout */
                ALOGW("%s connect failed, select timeout", __func__);
            }
        }
    } while (0);

    /* restore file status flags */
    if (!setNonBlocking(false)) {
        ALOGW("%s set blocking error", __func__);
        connect_ok = false;
    }

    if (!connect_ok) {
        /* socket connect failed */
        socketClose();
    }

    return connect_ok;
}

int32_t UdpSocketBase::socketReadData(char * buffer, const int32_t &size)
{
    int16_t readRet = 0;
    int16_t readLen = 0;
    int ready = 0;
    bool isBlockIndefinitely = false;
    fd_set rset;
    struct timeval tval {};
    struct sockaddr fromAddr = *(struct sockaddr *)&m_servAddr;
    socklen_t fromLen = sizeof(struct sockaddr);
    std::string fromIp;
    int fromPort;
    int select_retry_count = 0;
    (void)fromLen;

    if (NULL == buffer) {
        ALOGW("%s buffer is NULL", __func__);
        return -1;
    }

    if (!isSocketConnected()) {
        return -1;
    }

    do {
        FD_ZERO(&rset);
        FD_SET(m_socketFd, &rset);
        tval.tv_sec = (UDP_SOCKET_READ_TIMEOUT_MILLIS / 1000); /* seconds */
        tval.tv_usec = (UDP_SOCKET_READ_TIMEOUT_MILLIS % 1000);

        /* monitor socket fd and wait until socket is readable or timeout */
        ready = select(m_socketFd+1, &rset, NULL, NULL, isBlockIndefinitely ? NULL : &tval);
        if (ready < 0) {
            ALOGW("%s select error: %s", __func__, strerror(errno));
            break;
        } else if (ready == 0) {
            /* timeout expires before anything interesting happens */
            ALOGD("%s select rset fd:%d timeout", __func__, m_socketFd);
            select_retry_count += 1;
            if (select_retry_count >= UDP_SOCKET_READ_TIMEOUT_MAX_COUNT) {
                ALOGI("%s select rset fd:%d timeout count %d", __func__, m_socketFd, select_retry_count);
                break; /* break, to close socket */
            } else {
                continue; /* try to select again */
            }
        } else {
            if (!FD_ISSET(m_socketFd, &rset)) {
                /* received event but it's not the interesting, ignore it */
                continue; /* can try again */
            }
            select_retry_count = 0;
        }

        /* data is available, can read */
        //readRet = recv(m_socketFd, buffer+readLen, size-readLen, 0); 
        readRet = recvfrom(m_socketFd, buffer+readLen, size-readLen, 0, &fromAddr, &fromLen);
        if (readRet > 0) {
            fromIp = inet_ntoa(((struct sockaddr_in *)&fromAddr)->sin_addr);
            fromPort = (int)ntohs(((struct sockaddr_in *)&fromAddr)->sin_port);
            ALOGD("%s fromAddr=%s:%d readRet=%d", __func__, fromIp.c_str(), fromPort, readRet);
            readLen += readRet;
            break;/* return for udp socket read */
        } else {
            if (readRet < 0 && (errno == EAGAIN || errno == EINTR)) {
                ALOGI("%s readRet %d with EAGAIN or EINTR, try again", __func__, readRet);
                continue; /* can try again */
            }
            if (readRet == 0) {
                readLen = 0;
                ALOGW("%s readRet %d, the peer was closed.", __func__, readRet);
                break; /* peer shutdown, to close socket */
            } else {
                readLen = -1;
                ALOGW("%s readRet %d, read data error: %s", __func__, readRet, strerror(errno));
                break; /* read error, to close socket */
            }
        }
    } while (readLen < size);

    if (readLen <= 0) {
        socketClose();
    }
    return readLen;
}

int32_t UdpSocketBase::socketWriteData(char * buffer, const int32_t &size)
{
    int32_t offset = 0;
    int32_t left = size;
    int32_t ret = 0;

    if (NULL == buffer) {
        ALOGW("%s buffer is NULL", __func__);
        return -1;
    }

    if (!isSocketConnected()) {
        return -1;
    }

    while (left > 0)
    {
        /* ret = write(m_socketFd, &buffer[offset], left); */
        ret = sendto(m_socketFd, &buffer[offset], left, 0, (const struct sockaddr *)&m_servAddr, sizeof(m_servAddr));
        if (ret > 0)
        {
            offset += ret;
            left -= ret;
        } else {
            if ((EWOULDBLOCK == errno) || (EAGAIN == errno)) {
                // can't write w/o blocking - so stop with what has been written so far
                ALOGW("%s Exiting because no more data could be written without blocking", __func__);
                //return 0;
            }
            if (EINTR == errno)
            {
                /* just try again if interrupted by a signal */
                ALOGW("%s write returned due to EINTR - retrying", __func__);
            } else {
                /* For any other errors, close connection */
                ALOGW("%s write error %d: %s", __func__, errno, strerror(errno));
                // socketClose();
                return -1;
            }
        }
    }

    return size;
}

bool UdpSocketBase::isSocketConnected()
{
    return m_socketFd >= 0;
}

bool UdpSocketBase::getSocketFd(int32_t & socketFd)
{
    if (m_socketFd < 0) {
        return false;
    }
    socketFd = m_socketFd;
    return true;
}

bool UdpSocketBase::socketConfigure()
{
    bool ret = false;
    struct timeval timeout;
    timeout.tv_sec = 2;
    timeout.tv_usec = 0;
    int32_t reuseAddrOn = 1;
    int32_t rcvBufSize = 240 * 1024; /* 240k */

#if 0
    int32_t noDelay = 1;
    int keepalive = 1;
    int keepidle = 3;
    int keepinterval = 1;
    int keepcount = 5;
#endif
    (void)timeout;

    do {
        if (m_socketFd < 0) {
            ALOGW("%s socket not created", __func__);
            break;
        }
        if (!setNonBlocking(false)) {
            ALOGW("%s set blocking error", __func__);
            break;
        }
        if (setsockopt(m_socketFd, SOL_SOCKET, SO_RCVBUF, &rcvBufSize, sizeof(rcvBufSize)) < 0) {
            ALOGW("%s setsockopt SO_RCVBUF error: %s", __func__, strerror(errno));
            break;
        }
        if (setsockopt(m_socketFd, SOL_SOCKET, SO_REUSEADDR, &reuseAddrOn, sizeof(reuseAddrOn)) < 0) {
            ALOGW("%s setsockopt SO_REUSEADDR error: %s", __func__, strerror(errno));
            break;
        }
#if 0
        if (setsockopt(m_socketFd, IPPROTO_TCP, TCP_NODELAY, &noDelay, sizeof(noDelay)) < 0) {
            ALOGW("%s setsockopt TCP_NODELAY error: %s", __func__, strerror(errno));
            break;
        }

            if (setsockopt(m_socketFd, SOL_SOCKET, SO_KEEPALIVE, (void *) &keepalive, sizeof(keepalive)) < 0) {
                ALOGW("%s setsockopt SO_KEEPALIVE error: %s", __func__, strerror(errno));
                break;
            }
            if (setsockopt(m_socketFd, SOL_TCP, TCP_KEEPIDLE, (void *) &keepidle, sizeof(keepidle)) < 0) {
                ALOGW("%s setsockopt TCP_KEEPIDLE error: %s", __func__, strerror(errno));
                break;
            }
            if (setsockopt(m_socketFd, SOL_TCP, TCP_KEEPINTVL, (void *) &keepinterval, sizeof(keepinterval)) < 0) {
                ALOGW("%s setsockopt TCP_KEEPINTVL error: %s", __func__, strerror(errno));
                break;
            }
            if (setsockopt(m_socketFd, SOL_TCP, TCP_KEEPCNT, (void *) &keepcount, sizeof(keepcount)) < 0) {
                ALOGW("%s setsockopt TCP_KEEPCNT error: %s", __func__, strerror(errno));
                break;
            }

        if (setsockopt(m_socketFd, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(struct timeval)) < 0) {
            ALOGW("%s setsockopt SO_SNDTIMEO error: %s", __func__, strerror(errno));
            break;
        }
#endif
        ret = true;
    } while (0);

    return ret;

}

bool UdpSocketBase::setNonBlocking(bool nonblocking)
{
    int opts;

    opts = fcntl(m_socketFd, F_GETFL);
    if (opts < 0) {
        ALOGW("%s fcntl F_GETFL error: %s", __func__, strerror(errno));
        return false;
    }
    if (nonblocking) {
        opts |= O_NONBLOCK;
    } else {
        opts &= ~O_NONBLOCK;
    }

    if (fcntl(m_socketFd, F_SETFL, opts) < 0) {
        ALOGW("%s fcntl F_SETFL error: %s", __func__, strerror(errno));
        return false;
    }
    return true;
}

void UdpSocketBase::socketClose()
{
    if (m_socketFd > 0) {
        #if 0
        /* no need to shutdown for udp socket,
         * otherwise will shutdown error: Transport endpoint is not connected */
        if (shutdown(m_socketFd, SHUT_RDWR) != 0) {
            ALOGW("%s shutdown error: %s", __func__, strerror(errno));
        }
        #endif
        if (close(m_socketFd) != 0) {
            ALOGW("%s close fd:%d error: %s", __func__, m_socketFd, strerror(errno));
        } else {
            ALOGW("%s close fd:%d ok", __func__, m_socketFd);
        }
    }
    m_socketFd = -1;
}

/*-----------UdpSocketBase--end-----------*/


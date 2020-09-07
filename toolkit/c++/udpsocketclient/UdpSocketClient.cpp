#include <cstdio>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <memory.h>
#include <errno.h>

#include "UdpSocketClient.h"

/*-----------UdpSocketClient--begin-----------*/
UdpSocketClient::UdpSocketClient()
{
    m_socketClient = new UdpSocketBase();
    if (NULL == m_socketClient) {
        ALOGW("%s m_socketClient is NULL", __func__);
    }
    m_timeoutMillis = 3000; /* 3s */
    m_socketLinkStatus = SOCKET_CLIENT_UNKNOWN;
}

UdpSocketClient::~UdpSocketClient()
{
    ALOGW("%s", __func__);
    if (m_socketClient != NULL) {
        delete m_socketClient;
        m_socketClient = NULL;
    }
}

bool UdpSocketClient::initSocketClient()
{
    bool retValue = false;
    int times = 0;

    while (times++ < 2) {
        do {
            if (!m_socketClient->socketCreate()) {
                ALOGW("%s create socket error", __func__);
                break;
            }

            if (!m_socketClient->socketBind()) {
                ALOGW("%s bind socket error", __func__);
                break;
            }

            retValue = true;
            m_socketLinkStatus = SOCKET_CLIENT_INIT;
        } while (0);

        if (retValue) {
            break;
        }
    }

    return retValue;
}

bool UdpSocketClient::isUdpConnected()
{
    bool ret = false;

    if (SOCKET_CLIENT_CONNECT == m_socketLinkStatus) {
        if (m_socketClient->isSocketConnected()) {
            ret = true;
        } else {
            m_socketLinkStatus = SOCKET_CLIENT_UNKNOWN;
        }
    }

    return ret;
}

bool UdpSocketClient::doConnectToServer()
{
    if (NULL == m_socketClient) {
        ALOGW("%s m_socketClient is NULL", __func__);
        return false;
    }

    if (SOCKET_CLIENT_CONNECT == m_socketLinkStatus) {
        ALOGI("%s connect to server ok", __func__);
        return true;
    }

    if (SOCKET_CLIENT_UNKNOWN == m_socketLinkStatus) {
        if (!initSocketClient()) {
            ALOGW("%s initSocketClient failed", __func__);
            return false;
        }
    }

    if (!m_socketClient->socketConnectNonBlock()) {
        ALOGW("%s socketConnectNonBlock failed", __func__);
        m_socketLinkStatus = SOCKET_CLIENT_UNKNOWN;
        return false;
    }

    m_socketLinkStatus = SOCKET_CLIENT_CONNECT;
    ALOGI("%s OK", __func__);
    return true;
}

void UdpSocketClient::disConnectToServer()
{
    if ((m_socketClient != NULL)
            && (SOCKET_CLIENT_CONNECT == m_socketLinkStatus)) {
        if (m_socketClient->isSocketConnected()) {
            m_socketClient->socketClose();
            m_socketLinkStatus = SOCKET_CLIENT_UNKNOWN;
        }
    }
}

bool UdpSocketClient::doWriteData(uint8_t * buf, const uint32_t &maxSize, uint32_t &len)
{
    int32_t retValue;
    len = 0;

    if (NULL == buf) {
        ALOGW("%s buf is NULL", __func__);
        return false;
    }

    if (NULL == m_socketClient) {
        ALOGW("%s m_socketClient is NULL", __func__);
        return false;
    }

    if (SOCKET_CLIENT_CONNECT == m_socketLinkStatus) {
        retValue = m_socketClient->socketWriteData((char *)buf, maxSize);
        if (retValue < 0) {
            if (!m_socketClient->isSocketConnected()) {
                m_socketLinkStatus = SOCKET_CLIENT_UNKNOWN;
            }
        } else {
            len = retValue;
        }
    } else {
        retValue = -1;
    }

    if (retValue <= 0) {
        return false;
    }

    return true;
}

bool UdpSocketClient::doReadData(uint8_t * buf, const uint32_t &maxSize, uint32_t &len)
{
    return doReadDataBlock(buf, maxSize, len);
}

bool UdpSocketClient::doReadDataBlock(uint8_t * buf, const uint32_t &maxSize, uint32_t &len)
{
    int32_t retValue = false;
    len = 0;

    if (NULL == buf) {
        ALOGW("%s buf is NULL", __func__);
        return false;
    }

    if (NULL == m_socketClient) {
        ALOGW("%s m_socketClient is NULL", __func__);
        return false;
    }

    if (SOCKET_CLIENT_CONNECT == m_socketLinkStatus) {
        retValue = m_socketClient->socketReadData((char *)buf, maxSize);
        if (retValue <= 0) {
            if (!m_socketClient->isSocketConnected()) {
                m_socketLinkStatus = SOCKET_CLIENT_UNKNOWN;
            }
            //ALOGI("%s read timeout:%s", __func__, retValue == 0 ? "true":"false");
        } else {
            len = retValue;
            ALOGD("%s readlen=%d", __func__, retValue);
        }
    } else {
        ALOGW("%s m_socketLinkStatus is not connected", __func__);
        retValue = -1;
    }

    if (retValue <= 0) {
        return false;
    }

    return true;
}

bool UdpSocketClient::doReadDataTimeout(uint8_t * buf, const uint32_t &maxSize, uint32_t &len)
{
    int32_t ret = 0;
    int socketFd = 0;
    int32_t retValue = -1;
    struct timeval timeout { };
    struct timeval *pTimeout = &timeout;
    fd_set readfs;
    len = 0;

    if (NULL == buf) {
        ALOGW("%s buf is NULL", __func__);
        return false;
    }

    if (m_socketClient == NULL) {
        ALOGW("%s m_socketClient is NULL", __func__);
        return false;
    }

    if (SOCKET_CLIENT_CONNECT == m_socketLinkStatus) {
        m_socketClient->getSocketFd(socketFd);
        if (m_timeoutMillis > 0) {
            timeout.tv_sec = m_timeoutMillis / 1000;
            timeout.tv_usec = (m_timeoutMillis % 1000) * 1000;
        } else {
            pTimeout = NULL;
        }

        FD_ZERO(&readfs);
        FD_SET(socketFd, &readfs);

        ret = select(socketFd + 1, &readfs, NULL, NULL, pTimeout);
        if (ret > 0 && FD_ISSET(socketFd, &readfs)) {
            retValue = m_socketClient->socketReadData((char *)buf, maxSize);
            if (retValue <= 0) {
                if (!m_socketClient->isSocketConnected()) {
                    m_socketLinkStatus = SOCKET_CLIENT_UNKNOWN;
                }
            } else {
                len = retValue;
            }
            ret = retValue;
        } else if (ret == 0) {
            /* timeout */
            ALOGI("%s select rset fd:%d timeout", __func__, socketFd);
            ret = 0;
        } else {
            /* failed */
            ALOGW("%s select error: %s", __func__, strerror(errno));
            ret = -1;
        }
    } else {
        ret = -1;
    }

    if (ret <= 0) {
        return false;
    }
    return true;
}

/*-----------UdpSocketClient--end-----------*/

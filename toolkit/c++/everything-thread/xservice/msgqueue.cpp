/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* 
* File name   : msgqueue.cpp
* Author      : longbin
* Created date: 2020-01-03 13:40:08
* Description : 
*
*******************************************/

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "comm.h"
#include "msgqueue.h"

/*-----------MsgQueueBase--begin-----------*/
MsgQueueBase::MsgQueueBase()
{
	// m_Queue = std::make_unique<ConcurrentQueue<QueueMsgType>>(1000);
	m_Queue = std::make_shared<ConcurrentQueue<QueueMsgType>>(5000);
    init();
}

MsgQueueBase::~MsgQueueBase()
{
    release();
}

int MsgQueueBase::init()
{
    ALOGI("%s: entry\n", __func__);
    return true;
}

void MsgQueueBase::push(MsgQueueType &msgs)
{
    // ALOGD("push: cmdId=%u\n", msgs.header.cmdId);
    m_Queue->push(std::move(msgs)); /* msgs cannot be used any more after std::move */
}

MsgQueueType MsgQueueBase::pull()
{
    MsgQueueType msg {};
    // ALOGD("pull: waitForItems\n");
    m_Queue->waitForItems();
    msg = m_Queue->flushOne();
    // ALOGD("pull: cmdId=%u, payloadLength=%hu\n", msg.header.cmdId, msg.header.payloadLength);
    return msg;
}

void MsgQueueBase::release()
{
    ALOGI("%s\n", __func__);
    return;
}

/*-----------MsgQueueBase--end-----------*/

/*-----------MsgQueueSender--begin-----------*/
MsgQueueSender::MsgQueueSender() {}
MsgQueueSender::~MsgQueueSender() {}

void MsgQueueSender::write(MsgQueueType &msgs)
{
    ALOGI("MsgQueueSender::write cmdId=%u\n", msgs.header.cmdId);
    push(msgs);
}

MsgQueueType MsgQueueSender::read()
{
    MsgQueueType msg = pull();
    if ((msg.header.cmdId != 0)
        || (msg.header.payloadLength != 0))
    {
        ALOGI("MsgQueueSender::read cmdId=%u, payloadLength=%hu\n", msg.header.cmdId, msg.header.payloadLength);
    }
    return msg;
}

/*-----------MsgQueueSender--end-----------*/

/*-----------MsgQueueReceiver--begin-----------*/
MsgQueueReceiver::MsgQueueReceiver() {}
MsgQueueReceiver::~MsgQueueReceiver() {}

void MsgQueueReceiver::write(MsgQueueType &msgs)
{
    ALOGI("MsgQueueReceiver::write cmdId=%u\n", msgs.header.cmdId);
    push(msgs);
}

MsgQueueType MsgQueueReceiver::read()
{
    MsgQueueType msg = pull();
    if ((msg.header.cmdId != 0)
        || (msg.header.payloadLength != 0))
    {
        ALOGI("MsgQueueReceiver::read cmdId=%u, payloadLength=%hu\n", msg.header.cmdId, msg.header.payloadLength);
    }
    return msg;
}

/*-----------MsgQueueReceiver--end-----------*/

/*-----------MsgQueueDispatcher--begin-----------*/
MsgQueueDispatcher::MsgQueueDispatcher() {}
MsgQueueDispatcher::~MsgQueueDispatcher() {}

void MsgQueueDispatcher::write(MsgQueueType &msgs)
{
    ALOGI("MsgQueueDispatcher::write cmdId=%u\n", msgs.header.cmdId);
    push(msgs);
}

MsgQueueType MsgQueueDispatcher::read()
{
    MsgQueueType msg = pull();
    if ((msg.header.cmdId != 0)
        || (msg.header.payloadLength != 0))
    {
        ALOGI("MsgQueueDispatcher::read cmdId=%u, payloadLength=%hu\n", msg.header.cmdId, msg.header.payloadLength);
    }
    return msg;
}

/*-----------MsgQueueDispatcher--end-----------*/


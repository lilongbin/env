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
#include "log.h"

/*-----------MsgQueueBase--begin-----------*/
MsgQueueBase::MsgQueueBase()
{
    //m_Queue = std::make_unique<ConcurrentQueue<QueueMsgType>>(MESSAGE_QUEUE_TIMEOUT_VALUE);
    m_Queue = std::unique_ptr<ConcurrentQueue<QueueMsgType>>(new ConcurrentQueue<QueueMsgType>(MESSAGE_QUEUE_TIMEOUT_VALUE));
}

MsgQueueBase::~MsgQueueBase()
{
}

void MsgQueueBase::setTagName(std::string &tagName)
{
    m_tagName = tagName;
}

void MsgQueueBase::write(MsgQueueType &msg)
{
    ALOGV("%s::write seqId=%u, cmdId=%u, plLength=%hu", m_tagName.c_str(),
            msg.header.seqId, msg.header.cmdId, msg.header.plLength);
    /* msgs cannot be used any more after push */
    m_Queue->push(std::move(msg));
}

void MsgQueueBase::waitForItems()
{
    ALOGV("%s::waitForItems", m_tagName.c_str());
    m_Queue->waitForItems();
}

bool MsgQueueBase::read(MsgQueueType &msg)
{
    bool ret = false;
    ret = m_Queue->pull(msg);
    if (ret == true)
    {
        ALOGD("%s::read seqId=%u, cmdId=%u, plLength=%hu", m_tagName.c_str(),
                msg.header.seqId, msg.header.cmdId, msg.header.plLength);
    }
    return ret;
}

void MsgQueueBase::deactivate()
{
    ALOGI("%s::deactivate", m_tagName.c_str());
    m_Queue->deactivate();
}

void MsgQueueBase::clear()
{
    ALOGI("%s::clear", m_tagName.c_str());
    m_Queue->clear();
}

/*-----------MsgQueueBase--end-----------*/

/*-----------MsgQueueSender--begin-----------*/
MsgQueueSender::MsgQueueSender()
{
    std::string tagName = "MsgQueueSender";
    setTagName(tagName);
}

MsgQueueSender::~MsgQueueSender() {}

/*-----------MsgQueueSender--end-----------*/

/*-----------MsgQueueReceiver--begin-----------*/
MsgQueueReceiver::MsgQueueReceiver() {
	std::string tagName = "MsgQueueReceiver";
	setTagName(tagName);
}
MsgQueueReceiver::~MsgQueueReceiver() {}

/*-----------MsgQueueReceiver--end-----------*/

/*-----------MsgQueueDispatcher--begin-----------*/
MsgQueueDispatcher::MsgQueueDispatcher() {
	std::string tagName = "MsgQueueDispatcher";
	setTagName(tagName);
}
MsgQueueDispatcher::~MsgQueueDispatcher() {}

/*-----------MsgQueueDispatcher--end-----------*/


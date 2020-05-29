/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* 
* File name   : msgqueue.h
* Author      : longbin
* Created date: 2020-01-03 13:25:34
* Description : 
*
*******************************************/

#ifndef __MSG_QUEUE_H__
#define __MSG_QUEUE_H__

#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <atomic>
#include <condition_variable>
#include <algorithm>

#include "comm.h"
#include "singleton.h"

#define MESSAGE_QUEUE_TIMEOUT_VALUE 3000

class MsgQueueBase
{
private:
    std::string m_tagName;
    //using QueueMsgType = std::vector<uint8_t>;
    using QueueMsgType = MsgQueueType;
    //std::unique_ptr<ConcurrentQueue<QueueMsgType>> m_Queue;
    std::shared_ptr<ConcurrentQueue<QueueMsgType>> m_Queue;

protected:
    void setTagName(std::string &tagName);

public:
    MsgQueueBase();
    ~MsgQueueBase();

    void write(MsgQueueType &msg);
    void waitForItems();
    bool read(MsgQueueType &msg);
    void clear();
    void deactivate();
};

/* class MsgQueueSender */
class MsgQueueSender:public MsgQueueBase,public      Singleton<MsgQueueSender>
{
   friend class Singleton<MsgQueueSender>;
public:
   ~MsgQueueSender();

private:
   MsgQueueSender();
   //std::mutex m_mutex;
};

/* class MsgQueueReceiver */
class MsgQueueReceiver:public MsgQueueBase,public      Singleton<MsgQueueReceiver>
{
   friend class Singleton<MsgQueueReceiver>;
public:
   ~MsgQueueReceiver();

private:
   MsgQueueReceiver();
   //std::mutex m_mutex;
};

/* class MsgQueueDispatcher */
class MsgQueueDispatcher:public MsgQueueBase,public      Singleton<MsgQueueDispatcher>
{
   friend class Singleton<MsgQueueDispatcher>;
public:
   ~MsgQueueDispatcher();

private:
   MsgQueueDispatcher();
   //std::mutex m_mutex;
};

#endif

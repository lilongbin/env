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

template<typename T>
class ConcurrentQueue {
public:
    ConcurrentQueue() {}
    ConcurrentQueue(int wait) : mWait{wait} {}

    void waitForItems() {
        std::unique_lock<std::mutex> g(mLock);
        if (mWait > 0) {
            if (mQueue.empty() && mIsActive) {
                mCond.wait_for(g, std::chrono::milliseconds(mWait));
            }
        } else {
            while (mQueue.empty() && mIsActive) {
                mCond.wait(g);
            }
        }
    }

    std::vector<T> flushAll() {
        std::vector<T> items {};
        MuxGuard g(mLock);
        if (mQueue.empty() || !mIsActive) {
            return items;
        }
        while (!mQueue.empty()) {
            items.push_back(std::move(mQueue.front()));
            mQueue.pop();
        }
        return items;
    }

    T flushOne() {
        T item {}; /* initialize object */
        MuxGuard g(mLock);
        if (mQueue.empty() || !mIsActive) {
            return item;
        }
        if (!mQueue.empty()) {
            item = std::move(mQueue.front());
            mQueue.pop();
        }
        return item;
    }

    void push(T&& item) {
        {
            MuxGuard g(mLock);
            if (!mIsActive) {
                return;
            }
            if (mQueue.size() >= mMaxQueueSize)
            {
                /* pop queue head when queue is full */
                mQueue.pop();
            }
            mQueue.push(std::move(item));
        }
        mCond.notify_one();
    }

    /* Deactivates the queue, thus no one can push items to it, also
     * notifies all waiting thread.
     */
    void deactivate() {
        {
            MuxGuard g(mLock);
            mIsActive = false;
        }
        mCond.notify_all();  // To unblock all waiting consumers.
    }

    //ConcurrentQueue() = default;

    ConcurrentQueue(const ConcurrentQueue &) = delete;
    ConcurrentQueue &operator=(const ConcurrentQueue &) = delete;
private:
    using MuxGuard = std::lock_guard<std::mutex>;
    int mWait = 0; // wait_for time, 0 for wait forever.
    bool mIsActive = true;
    size_t mMaxQueueSize = 5000;
    mutable std::mutex mLock;
    std::condition_variable mCond;
    std::queue<T> mQueue;
};

class MsgQueueBase
{
private:
    //using QueueMsgType = std::vector<uint8_t>;
    using QueueMsgType = MsgQueueType;
    // std::unique_ptr<ConcurrentQueue<QueueMsgType>> m_Queue;
    std::shared_ptr<ConcurrentQueue<QueueMsgType>> m_Queue;

	int init();
	void release();

protected:
    void push(MsgQueueType &msgs);
	MsgQueueType pull();

public:
    MsgQueueBase();
	virtual ~MsgQueueBase();
	virtual void write(MsgQueueType &msgs)=0;
	virtual MsgQueueType read()=0;
};

/* class MsgQueueSender */
class MsgQueueSender:public MsgQueueBase,public      Singleton<MsgQueueSender>
{
   friend class Singleton<MsgQueueSender>;
public:
   ~MsgQueueSender();
   void write(MsgQueueType &msgs);
   MsgQueueType read();

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
   void write(MsgQueueType &msgs);
   MsgQueueType read();

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
   void write(MsgQueueType &msgs);
   MsgQueueType read();

private:
   MsgQueueDispatcher();
   //std::mutex m_mutex;
};

#endif

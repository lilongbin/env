/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* 
* File name   : comm.h
* Author      : longbin
* Created date: 2020-01-03 13:25:34
* Description : 
*
*******************************************/

#ifndef __COMM_H__
#define __COMM_H__

#include <iostream>
#include <sstream>
#include <thread>
#include <mutex>
#include <queue>
#include <atomic>
#include <condition_variable>
#include <algorithm>

#if 1
#define ALOGI while(0) printf
#define ALOGW while(0) printf
#define ALOGE while(0) printf
#define ALOGD while(0) printf
#define ALOGV while(0) printf
#else
#define ALOGI printf("\r\n");printf
#define ALOGW printf("\r\n");printf
#define ALOGE printf("\r\n");printf
#define ALOGD printf("\r\n");printf
#define ALOGV printf("\r\n");printf
#endif

typedef struct MsgHeaderType {
	uint32_t seqId;
	uint16_t funId;
	uint32_t cmdId;
	uint16_t payloadLength;
} MsgHeaderType;

typedef struct MsgQueueType
{
	MsgHeaderType        header;
	std::vector<uint8_t> vpayload;
} MsgQueueType;

template<typename T>
class ConcurrentQueue {
public:
    ConcurrentQueue() {}
    ConcurrentQueue(int wait) : mWait{wait} {}

    void waitForItems() {
        std::unique_lock<std::mutex> gul(mLock);
        if (mWait > 0) {
            // block here until be notified or timeout
            if (mQueue.empty() && mIsActive) {
                mCond.wait_for(gul, std::chrono::milliseconds(mWait));
            }
        } else if (mWait == 0) {
            // unblock
            return;
        } else {
            // block here until be notified and valid item exist
            while (mQueue.empty() && mIsActive) {
                mCond.wait(gul);
            }
        }
    }

   bool pullAll(std::vector<T> &items) {
        //std::vector<T> items {};
        MuxGuard g(mLock);
        if (mQueue.empty() || !mIsActive) {
            return false;
        }
        while (!mQueue.empty()) {
            items.push_back(std::move(mQueue.front()));
            mQueue.pop();
        }
        return true;
    }

    bool pull(T &item) {
        MuxGuard g(mLock);
        if (mQueue.empty() || !mIsActive) {
            return false;
        }
        if (!mQueue.empty()) {
            item = std::move(mQueue.front());
            mQueue.pop();
        }
        return true;
    }

    void push(T&& item) {
        MuxGuard g(mLock);
        if (!mIsActive) {
            return;
        }
        if (mQueue.size() >= mMaxQueueSize)
        {
            // pop queue head when queue is full
            mQueue.pop();
        }
        mQueue.push(std::move(item));
        mCond.notify_one();
    }

    // Deactivates the queue, thus no one can push items to it,
    // also notifies all waiting thread.
    void deactivate() {
        MuxGuard g(mLock);
        mIsActive = false;
        mCond.notify_all();  // To unblock all waiting consumers.
    }

    void clear()   {
        MuxGuard g(mLock);
        while (!mQueue.empty()) {
            mQueue.pop();
        }
    }

    //ConcurrentQueue() = default;
    ConcurrentQueue(const ConcurrentQueue &) = delete;
    ConcurrentQueue &operator=(const ConcurrentQueue &) = delete;
private:
    using MuxGuard = std::lock_guard<std::mutex>;
    int mWait = -1; // wait_for time, negtive for always block, 0 for unblock.
    bool mIsActive = true;
    size_t mMaxQueueSize = 5000;
    mutable std::mutex mLock;
    std::condition_variable mCond;
    std::queue<T> mQueue;
};


#endif


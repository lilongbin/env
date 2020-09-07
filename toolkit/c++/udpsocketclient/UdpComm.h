/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* 
* File name   : UdpComm.h
* Author      : longbin
* Created date: 2020-09-07 12:56:39
* Description : 
*
*******************************************/

#ifndef __UDP_COMM_H__
#define __UDP_COMM_H__


#define ALOGV(...) printf("%s:%d ", __FILE__, __LINE__);printf(__VA_ARGS__);printf("\n");
#define ALOGD(...) printf("%s:%d ", __FILE__, __LINE__);printf(__VA_ARGS__);printf("\n");
#define ALOGI(...) printf("%s:%d ", __FILE__, __LINE__);printf(__VA_ARGS__);printf("\n");
#define ALOGW(...) printf("%s:%d ", __FILE__, __LINE__);printf(__VA_ARGS__);printf("\n");
#define ALOGE(...) printf("%s:%d ", __FILE__, __LINE__);printf(__VA_ARGS__);printf("\n");

#define UDP_MSG_MAX_LEN 0x7FFF
#define UDP_MSG_HEADER_LEN 16
#define UDP_DATA_LEN_POS 12
#define NH_SWAP16(x) ((((uint16_t)(x) & 0xFF00) >> 8) | (((uint16_t)(x) & 0x00FF) << 8))
#define NH_SWAP32(x) ((((uint32_t)(x) & 0xFF000000) >> 24) | (((uint32_t)(x) & 0x00FF0000) >> 8) | (((uint32_t)(x) & 0x0000FF00) << 8) | (((uint32_t)(x) & 0x000000FF) << 24))

//#define GetUdpDataLength(buff, offset) (NH_SWAP32(buff[offset]))
#define GetUdpDataLength(buff, offset) ((*(uint32_t *)(buff+offset)))

#define ASSERT_CONCAT_(a, b) a##b
#define STATIC_ASSERT(e, msg) ;enum { ASSERT_CONCAT_(assert_line_, __LINE__) = 1/(!!(e)) }
#pragma pack(push,1)
typedef struct {
    uint32_t seqId;
    uint32_t funcId;
    uint32_t cmdId;
    uint32_t dataLen;
} UdpMsgHeader_T;
STATIC_ASSERT(sizeof(UdpMsgHeader_T)==16, "RTPHeader_T size doesn't seem to be cool.");
#pragma pack(pop)

typedef struct {
    UdpMsgHeader_T       header;
    std::vector<uint8_t> payload;
} UdpMsgQueue_T;

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
        {
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
        }
        mCond.notify_one();
    }

    // Deactivates the queue, thus no one can push items to it,
    // also notifies all waiting thread.
    void deactivate() {
        {
            MuxGuard g(mLock);
            mIsActive = false;
        }
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

#endif //__UDP_COMM_H__


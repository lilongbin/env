//mytimer.hpp
#ifndef __MYTIMER_H__
#define __MYTIMER_H__

#include <iostream>
#include <queue>
#include <functional>
#include <chrono>
#include <thread>
#include <atomic>
#include <memory>
#include <mutex>
#include <condition_variable>

// extern long long getSteadyMillis();

class MYTimer {
public:
    MYTimer():m_timerMainThreadAlive(false) {
        m_sleepMicros = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::milliseconds(1000));
        create();
    }

    ~MYTimer() {
        destroy();
        // std::cout << "~cpptimer" << std::endl;
    }

    MYTimer(const MYTimer&) = delete;
    MYTimer &operator=(const MYTimer&) = delete;
private:
    void create() {
        if (m_timerMainThreadAlive) {
            return;
        }
        m_timerMainThreadAlive = true;
        m_timerThread = std::thread([this]() { //lambda
            // std::cout << "timer create" << std::endl;
            std::unique_lock<std::mutex> tul(m_timerLock);
            m_callbackSn = 1; //ensure 1st trigger when this thread later than start() and lost notify/wait_for
            do {
                // wait for timeout or until notified
                // std::cout<<"wait_for:"<<getSteadyMillis()<<std::endl;
                m_timerCond.wait_for(tul, std::chrono::microseconds(m_sleepMicros));
                // notify work thread
                doNotify();
            } while (m_timerMainThreadAlive);
        });

        m_workerThread = std::thread([this]() {
            // wait for timer notify
            std::unique_lock<std::mutex> wul(m_workerLock);
            do {
                m_workerCond.wait(wul);
                doCallback();
            } while (m_timerMainThreadAlive);
            // std::cout << "worker thread stopped." << std::endl;
        });
        //to make sure thread wait_for then start and send notify at 1st time.
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }
    void destroy() {
        m_timerMainThreadAlive = false;
        stop();
        m_workerCond.notify_all();
        m_timerCond.notify_all();
        if (m_timerThread.joinable()) {
            m_timerThread.join();
        }
        if (m_workerThread.joinable()) {
            m_workerThread.join();
        }
    }

public:
    void start(int intervalmillis, std::function<void()> callback) {
        //callback: std::bind(funcname, parameters...);
        MuxGuard g(mLock);
        std::chrono::milliseconds interval_millis(intervalmillis);
        std::chrono::microseconds interval_micros = std::chrono::duration_cast<std::chrono::microseconds>(interval_millis);
        std::chrono::microseconds adjust_micros(500);
        std::chrono::microseconds sleep_micros(interval_micros - adjust_micros);

        m_callbackSn = 0;
        m_callbackFunc = std::function<void()>(nullptr);
        while (!m_msgQueue.empty()) {
            m_msgQueue.pop();
        }
        m_sleepMicros = sleep_micros;
        m_callbackFunc = callback;
        m_timerCond.notify_all();
        // std::cout<<"notify:"<<getSteadyMillis()<<std::endl;
    }

    void doNotify() {
        MuxGuard g(mLock);
        m_callbackSn += 1;
        m_msgQueue.push(m_callbackSn);
        m_workerCond.notify_one();
    }

    void doCallback() {
        MuxGuard g(mLock);
        if (m_msgQueue.empty()) {
            return;
        }
        uint32_t sn = 0;
        do {
            // get the latest sn
            sn = m_msgQueue.front();
            m_msgQueue.pop();
        } while (!m_msgQueue.empty());
        // std::cout<<"sn:"<<sn<<",m_callbackSn="<<m_callbackSn<<std::endl;
        if ((sn == m_callbackSn) && (sn > 1)) {
            // std::cout<<"m_callbackFunc:"<<(m_callbackFunc?"do":"dont")<<std::endl;
            if (m_callbackFunc) {
                m_callbackFunc();
            }
        }
    }

    void stop() {
        MuxGuard g(mLock);
        m_callbackSn = 0;
        m_callbackFunc = std::function<void()>(nullptr);
        while (!m_msgQueue.empty()) {
            m_msgQueue.pop();
        }
        m_timerCond.notify_all();
        // std::cout << "stop." << std::endl;
    }

private:
    mutable std::mutex m_timerLock;
    mutable std::mutex m_workerLock;
    std::condition_variable m_timerCond;
    std::condition_variable m_workerCond;
    std::thread m_timerThread;
    std::thread m_workerThread;
    std::atomic<bool> m_timerMainThreadAlive;

    std::atomic<uint32_t> m_callbackSn;
    std::function<void()> m_callbackFunc = std::function<void()>(nullptr);
    std::chrono::microseconds m_sleepMicros;
    using MuxGuard = std::lock_guard<std::mutex>;
    std::queue<uint32_t> m_msgQueue;
    mutable std::mutex mLock;
};

#endif //__MYTIMER_H__


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

#if 0
    extern long long getSteadyMillis();
    #define DEBUG_TIMER_BEGIN  std::cout<<__func__<<" begin:"<<getSteadyMillis()<<std::endl;
    #define DEBUG_TIMER_END    std::cout<<__func__<<"   end:"<<getSteadyMillis()<<std::endl;
#else
    #define DEBUG_TIMER_BEGIN
    #define DEBUG_TIMER_END
#endif

class MYTimer {
public:
    MYTimer():m_timerMainThreadAlive(false) {
        m_timerStarted = false;
        m_callbackSn = (uint32_t)-1;
        m_sleepMicros = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::milliseconds(1000));
        m_callbackFunc = std::function<void()>(nullptr);
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
        DEBUG_TIMER_BEGIN
        if (m_timerMainThreadAlive) {
            return;
        }
        m_timerMainThreadAlive = true;
        m_timerThread = std::thread([this]() { //lambda
            do {
                timerWait(); // wait for timeout or until notified
                doNotify(); // notify work thread
            } while (m_timerMainThreadAlive);
        });

        m_workerThread = std::thread([this]() {
            do {
                workWait(); // wait for timer notify
                doCallback();
            } while (m_timerMainThreadAlive);
        });
        DEBUG_TIMER_END
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
        std::chrono::milliseconds interval_millis(intervalmillis);
        std::chrono::microseconds interval_micros = std::chrono::duration_cast<std::chrono::microseconds>(interval_millis);
        std::chrono::microseconds adjust_micros(500);
        std::chrono::microseconds sleep_micros(interval_micros - adjust_micros);
        DEBUG_TIMER_BEGIN
        {
            MuxGuard g(m_MuxLock);
            while (!m_workQueue.empty()) {
                m_workQueue.pop();
            }
            m_timerStarted = true;
            m_callbackSn = 0;
            m_callbackFunc = callback;
            m_sleepMicros = sleep_micros;
            m_timerQueue.push(m_callbackSn);
            if (!m_timerMainThreadAlive) {
                create();
            }
        }
        m_timerCond.notify_one();
        DEBUG_TIMER_END
    }

    void timerWait() {
        MuxUniqLck tul(m_MuxLock);
        uint32_t sn = 0;
        if (m_timerQueue.empty() && m_timerMainThreadAlive) {
            sn = m_callbackSn;
            m_timerCond.wait_for(tul, std::chrono::microseconds(m_sleepMicros));
            m_timerQueue.push(sn); //timeout or notified
        }
    }
    void doNotify() {
        {
            MuxGuard g(m_MuxLock);
            uint32_t sn = 0;
            if (!m_timerQueue.empty()) {
                sn = m_timerQueue.front();
                m_timerQueue.pop();
                if (sn != m_callbackSn) {
                    // m_callbackSn changed between timerWait and doNotify, maybe start() called;
                    return;
                }
            }
            if (!m_timerStarted) {
                return;
            }
            m_callbackSn += 1;
            m_workQueue.push(m_callbackSn);
        }
        m_workerCond.notify_one();
    }

    void workWait() {
        MuxUniqLck wul(m_MuxLock);
        while (m_workQueue.empty() && m_timerMainThreadAlive) {
            m_workerCond.wait(wul);
        }
    }
    void doCallback() {
        DEBUG_TIMER_BEGIN
        std::function<void()> callback = std::function<void()>(nullptr);
        {
            MuxGuard g(m_MuxLock);
            uint32_t sn = 0;
            while (!m_workQueue.empty()) {
                // get the latest sn
                sn = m_workQueue.front();
                m_workQueue.pop();
            };
            // std::cout<<"sn:"<<sn<<",m_callbackSn="<<m_callbackSn<<std::endl;
            if (sn <= 1) {
                return;
            }
            if (!m_timerStarted) {
                return;
            }
            if (sn != m_callbackSn) {
                return;
            }
            // std::cout<<"m_callbackFunc:"<<(m_callbackFunc?"callable":"not callable")<<std::endl;
            if (!m_callbackFunc) {
                return;
            }
            callback = m_callbackFunc;
        }
        // release lock when do callback call, prevent callback spend too much time
        try {
            callback();
        } catch (const std::bad_function_call &e) {
            (void)e;
            // std::cout<<__func__<<" error: "<<e.what()<<std::endl;
        }
        DEBUG_TIMER_END
    }

    void stop() {
        DEBUG_TIMER_BEGIN
        {
            MuxGuard g(m_MuxLock);
            while (!m_workQueue.empty()) {
                m_workQueue.pop();
            }
            m_timerStarted = false;
            m_callbackSn = (uint32_t)-1;
            m_callbackFunc = std::function<void()>(nullptr);
        }
        DEBUG_TIMER_END
    }

private:
    mutable std::mutex m_MuxLock;
    using MuxGuard = std::lock_guard<std::mutex>;
    using MuxUniqLck = std::unique_lock<std::mutex>;
    std::condition_variable m_timerCond;
    std::condition_variable m_workerCond;
    std::thread m_timerThread;
    std::thread m_workerThread;
    std::atomic<bool> m_timerMainThreadAlive;
    std::atomic<bool> m_timerStarted;

    std::atomic<uint32_t> m_callbackSn;
    std::function<void()> m_callbackFunc = std::function<void()>(nullptr);
    std::chrono::microseconds m_sleepMicros;
    std::queue<uint32_t> m_workQueue;
    std::queue<uint32_t> m_timerQueue;
};

#endif //__MYTIMER_H__


//mytimer.hpp
#ifndef __SWTIMER_H__
#define __SWTIMER_H__

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

class SWTimer {
public:
    SWTimer():m_timerMainThreadAlive(false) {
        m_timerStarted = false;
        m_callbackSn = (uint32_t)-1;
        m_sleepMicros = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::milliseconds(1000));
        m_callbackFunc = std::function<void()>(nullptr);
        create();
    }

    ~SWTimer() {
        destroy();
        // std::cout << "~cpptimer" << std::endl;
    }

    SWTimer(const SWTimer&) = delete;
    SWTimer &operator=(const SWTimer&) = delete;
private:
    void create() {
        DEBUG_TIMER_BEGIN
        if (m_timerMainThreadAlive) {
            return;
        }
        m_timerMainThreadAlive = true;
        m_timerThread = std::thread([this]() { //lambda
            do {
                if (timerWait()) {
                    doNotify(); // notify work thread
                }
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

    bool timerWait() {
        DEBUG_TIMER_BEGIN
        MuxUniqLck tul(m_MuxLock);
        uint32_t sn = 0;
        if (!m_timerQueue.empty()) {
            while (!m_timerQueue.empty()) {
                m_timerQueue.pop();
            }
            // std::cout<<"return false queue, "<<getSteadyMillis()<<std::endl;
            return false;
        }

        sn = m_callbackSn;
        m_startTimePoint = std::chrono::steady_clock::now();
        m_timerCond.wait_for(tul, std::chrono::microseconds(m_sleepMicros));
        if (!m_timerStarted) {
            // std::cout<<"return false not started, "<<getSteadyMillis()<<std::endl;
            return false;
        }
        if (sn != m_callbackSn) {
            // std::cout<<"return false sn!=, "<<getSteadyMillis()<<std::endl;
            return false;
        }
        m_nowTimePoint = std::chrono::steady_clock::now();
        m_deltaMicros = std::chrono::duration_cast<DurationMicrosT>(m_nowTimePoint-m_startTimePoint);
        // std::cout<<"delta:"<<m_deltaMicros.count()<<",sleep:"<<m_sleepMicros.count()<<std::endl;
        if (m_deltaMicros.count() < m_sleepMicros.count()/2) {
            // std::cout<<"return false interval, "<<getSteadyMillis()<<std::endl;
            return false;
        }
        DEBUG_TIMER_END
        // std::cout<<"return true: "<<getSteadyMillis()<<std::endl;
        m_timerQueue.push(sn);
        return true;
    }
    void doNotify() {
        {
            MuxGuard g(m_MuxLock);
            uint32_t sn = 0;
            if (m_timerQueue.empty()) {
                return;
            }
            sn = m_timerQueue.front();
            m_timerQueue.pop();
            if (sn != m_callbackSn) {
                return;
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
                sn = m_workQueue.front();
                m_workQueue.pop();
            };
            // std::cout<<"sn:"<<sn<<",m_callbackSn="<<m_callbackSn<<std::endl;
            if (sn == 0) {
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
    typedef std::chrono::duration<int, std::ratio<1, 1000000>> DurationMicrosT;
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
    std::chrono::steady_clock::time_point m_startTimePoint;
    std::chrono::steady_clock::time_point m_nowTimePoint;
    DurationMicrosT m_deltaMicros;
};

#endif //__SWTIMER_H__


//cpptimer.hpp
#ifndef __CPPTIMER_H__
#define __CPPTIMER_H__

#include <iostream>
#include <functional>
#include <chrono>
#include <thread>
#include <atomic>
#include <memory>
#include <mutex>
#include <condition_variable>

class CPPTimer {
public:
    CPPTimer():m_timerRunning(false) {
        m_intervalMicros = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::milliseconds(1000));
        create();
    }

    ~CPPTimer() {
        stop();
        destroy();
        // std::cout << "~cpptimer" << std::endl;
    }

    CPPTimer(const CPPTimer&) = delete;
    CPPTimer &operator=(const CPPTimer&) = delete;
private:
    void create() {
        m_timerStarted = false;
        MuxGuard g(mLock);
        if (m_timerRunning == true) {
            return;
        }
        m_timerRunning = true;
        m_timerThread = std::thread([this]() { //lambda
            // std::cout << "timer start" << std::endl;
            std::unique_lock<std::mutex> tul(m_timerLock);
            do {
                // wait for timeout or until notified
                m_timerCond.wait_for(tul, std::chrono::microseconds(m_intervalMicros));
                // notify work thread
                if (m_timerStarted) {
                    m_workerCond.notify_one();
                }
            } while (m_timerRunning);
        });

        m_workerThread = std::thread([this]() {
            // wait for timer notify
            std::unique_lock<std::mutex> wul(m_workerLock);
            do {
                m_workerCond.wait(wul);
                if (!m_timerStarted) {
                    continue;
                }
                if (m_callback) {
                    // std::cout<<"m_callback:"<<(m_callback?"do":"dont")<<std::endl;
                    m_callback();
                }
            } while (m_timerRunning);
            // std::cout << "worker thread stopped." << std::endl;
        });
    }
    void destroy() {
        m_timerRunning = false;
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
        {
            m_timerStarted = false;
            m_callback = std::function<void()>(nullptr);
            m_timerCond.notify_all();
            m_intervalMicros = interval_micros;
            m_callback = callback;
        }
        create();
        {
            //need fall back the cpu 1~3ms for other thread then start timer
            std::this_thread::sleep_for(std::chrono::milliseconds(3));
            m_timerStarted = true;
        }
    }

    void stop() {
        m_timerStarted = false;
        m_callback = std::function<void()>(nullptr);
        m_timerCond.notify_all();
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
        // std::cout << "stop." << std::endl;
    }

private:
    std::atomic<bool> m_timerRunning;
    std::atomic<bool> m_timerStarted;
    mutable std::mutex m_timerLock;
    mutable std::mutex m_workerLock;
    std::condition_variable m_timerCond;
    std::condition_variable m_workerCond;
    std::thread m_timerThread;
    std::thread m_workerThread;
    std::function<void()> m_callback = std::function<void()>(nullptr);
    std::chrono::microseconds m_intervalMicros;
    using MuxGuard = std::lock_guard<std::mutex>;
    mutable std::mutex mLock;
};

#endif //__CPPTIMER_H__


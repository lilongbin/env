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
    MYTimer():m_timerThreadAlive(false) {
        m_sleepMicros = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::milliseconds(1000));
        create();
    }

    ~MYTimer() {
        stop();
        destroy();
        // std::cout << "~cpptimer" << std::endl;
    }

    MYTimer(const MYTimer&) = delete;
    MYTimer &operator=(const MYTimer&) = delete;
private:
    void create() {
        m_timerRunning = false;
        if (m_timerThreadAlive) {
            return;
        }
        m_timerThreadAlive = true;
        m_timerThread = std::thread([this]() { //lambda
            std::cout << "timer create" << std::endl;
            std::unique_lock<std::mutex> tul(m_timerLock);
            m_timerResetting = false;
            do {
                // wait for timeout or until notified
                // std::cout<<"wait_for:"<<getSteadyMillis()<<std::endl;
                m_timerCond.wait_for(tul, std::chrono::microseconds(m_sleepMicros));
                if (m_timerResetting) {
                    m_timerResetting = false;
                    continue;
                }
                // notify work thread
                // std::cout<<"m_timerRunning:"<<m_timerRunning<<std::endl;
                if (m_timerRunning) {
                    m_workerCond.notify_one();
                }
            } while (m_timerThreadAlive);
        });

        m_workerThread = std::thread([this]() {
            // wait for timer notify
            std::unique_lock<std::mutex> wul(m_workerLock);
            do {
                m_workerCond.wait(wul);
                if (!m_timerRunning) {
                    continue;
                }
                if (m_callback) {
                    // std::cout<<"m_callback:"<<(m_callback?"do":"dont")<<std::endl;
                    m_callback();
                }
            } while (m_timerThreadAlive);
            // std::cout << "worker thread stopped." << std::endl;
        });
        //to make sure thread wait_for then start and send notify at 1st time.
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
    }
    void destroy() {
        m_timerThreadAlive = false;
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

        m_timerRunning = false;
        m_callback = std::function<void()>(nullptr);
        m_timerResetting = true;
        m_sleepMicros = sleep_micros;
        m_callback = callback;
        m_timerCond.notify_all();
        // std::cout<<"notify:"<<getSteadyMillis()<<std::endl;
        if (!m_timerThreadAlive) {
            //first run
            create();
        }
        m_timerRunning = true;
    }

    void stop() {
        MuxGuard g(mLock);
        m_timerRunning = false;
        m_callback = std::function<void()>(nullptr);
        m_timerResetting = true;
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

    std::atomic<bool> m_timerThreadAlive;
    std::atomic<bool> m_timerRunning;
    std::atomic<bool> m_timerResetting;
    std::function<void()> m_callback = std::function<void()>(nullptr);
    std::chrono::microseconds m_sleepMicros;
    using MuxGuard = std::lock_guard<std::mutex>;
    mutable std::mutex mLock;
};

#endif //__MYTIMER_H__


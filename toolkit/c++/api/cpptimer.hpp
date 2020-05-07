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
    typedef enum {
        TIMER_ONCE = 0,
        TIMER_PERIODIC = 1,
    } CPPTimerType;
public:
    CPPTimer():m_timerRunning(false) {
    }

    CPPTimer(const CPPTimer& t) {
        m_timerRunning = t.m_timerRunning.load();
    }
    ~CPPTimer() {
        stop();
        // std::cout << "~cpptimer" << std::endl;
    }

    void start(int interval, std::function<void()> callback) {
        //callback: std::bind(funcname, parameters);
        if (m_timerRunning == true) {
            stop();
        }
        m_timerRunning = true;
        m_timerThread = std::thread([this, interval]() { //lambda
            // std::cout << "timer start" << std::endl;
            std::unique_lock<std::mutex> tul(m_timerLock);
            std::chrono::milliseconds clock_millis(interval);
            std::chrono::microseconds clock_micros = std::chrono::duration_cast<std::chrono::microseconds>(clock_millis);
            std::chrono::microseconds spent_micros(500);
            std::chrono::microseconds sleep_micros(clock_micros - spent_micros);
            do {
                // wait for timeout or until notified
                // m_timerCond.wait_for(tul, std::chrono::milliseconds(clock_millis));
                // m_timerCond.wait_for(tul, std::chrono::microseconds(clock_micros.count() - spent_micros.count()));
                m_timerCond.wait_for(tul, std::chrono::microseconds(sleep_micros));
                // notify work thread
                m_workerCond.notify_one();
            } while (true == m_timerRunning);
        });
        m_workerThread = std::thread([this, callback]() {
            // wait for timer notify
            std::unique_lock<std::mutex> wul(m_workerLock);
            do {
                m_workerCond.wait(wul);
                if (true != m_timerRunning) break;
                callback();
            } while (true == m_timerRunning);
            // std::cout << "worker thread stopped." << std::endl;
        });
    }

    void stop() {
        m_timerRunning = false;
        m_timerCond.notify_all();
        m_workerCond.notify_all();
        if (m_timerThread.joinable()) {
            m_timerThread.join();
        }
        if (m_workerThread.joinable()) {
            m_workerThread.join();
        }
        std::cout << "stop." << std::endl;
    }

    template<typename callable, class... arguments>
        void syncDelay(int delay, callable&& fcb, arguments&&... args){
            std::function<typename std::result_of<callable(arguments...)>::type()> callback
                (std::bind(std::forward<callable>(fcb), std::forward<arguments>(args)...));
            m_timerRunning = true;

            m_timerThread = std::thread([this, delay, callback]() {
                std::unique_lock<std::mutex> tul(m_timerLock);
                m_timerCond.wait_for(tul, std::chrono::milliseconds(delay));
                // notify work thread
                m_workerCond.notify_one();
            });
            m_workerThread = std::thread([this, delay, callback]() {
                // wait for timer notify
                std::unique_lock<std::mutex> wul(m_workerLock);
                do {
                    m_workerCond.wait(wul);
                    callback();
                    m_timerRunning = false;
                } while (0);
            });
            if (m_timerThread.joinable()) {
                m_timerThread.join();
            }
            if (m_workerThread.joinable()) {
                m_workerThread.join();
            }
        }

    template<typename callable, class... arguments>
        void asyncDelay(int delay, callable&& fcb, arguments&&... args){
            std::function<typename std::result_of<callable(arguments...)>::type()> callback
                (std::bind(std::forward<callable>(fcb), std::forward<arguments>(args)...));
            m_timerRunning = true;

            m_timerThread = std::thread([this, delay, callback]() {
                std::unique_lock<std::mutex> tul(m_timerLock);
                m_timerCond.wait_for(tul, std::chrono::milliseconds(delay));
                // notify work thread
                m_workerCond.notify_one();
            });
            m_workerThread = std::thread([this, delay, callback]() {
                // wait for timer notify
                std::unique_lock<std::mutex> wul(m_workerLock);
                do {
                    m_workerCond.wait(wul);
                    if (true != m_timerRunning) break;
                    callback();
                    m_timerRunning = false;
                } while (0);
            });
        }

private:
    std::atomic<bool> m_timerRunning;
    mutable std::mutex m_timerLock;
    mutable std::mutex m_workerLock;
    std::condition_variable m_timerCond;
    std::condition_variable m_workerCond;
    std::thread m_timerThread;
    std::thread m_workerThread;
};

#endif //__CPPTIMER_H__


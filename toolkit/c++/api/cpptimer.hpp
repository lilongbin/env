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

#define __USE_SIMPLE_TIMER__ 0

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
#if __USE_SIMPLE_TIMER__
        m_timerThread = std::thread([this, interval]() { //lambda
            // std::cout << "timer start" << std::endl;
            std::chrono::milliseconds interval_millis(interval);
            std::unique_lock<std::mutex> tul(m_timerLock);
            std::chrono::microseconds interval_micros = std::chrono::duration_cast<std::chrono::microseconds>(interval_millis);
            std::chrono::microseconds adjust_micros(600);
            std::chrono::microseconds sleep_micros(interval_micros - adjust_micros);
            do {
                // wait for timeout or until notified
                m_timerCond.wait_for(tul, std::chrono::microseconds(sleep_micros));
                // notify work thread
                m_workerCond.notify_one();
            } while (true == m_timerRunning);
        });
#else
        m_timerThread = std::thread([this, interval]() { //lambda
            // std::cout << "timer start" << std::endl;
            int TIMER_SLICE_COUNT = 10;
            std::chrono::milliseconds interval_millis(interval);
            long loopcnt = 0;
            std::unique_lock<std::mutex> tul(m_timerLock);
            std::chrono::microseconds interval_micros = std::chrono::duration_cast<std::chrono::microseconds>(interval_millis);
            std::chrono::microseconds slice_micros = interval_micros / TIMER_SLICE_COUNT;
            std::chrono::microseconds adjust_micros(0);
            std::chrono::microseconds sleep_micros(slice_micros - adjust_micros);
            std::chrono::microseconds diff_start_micros;
            std::chrono::microseconds delay_expired_micros;
            std::chrono::steady_clock::time_point tp_elapsed;
            std::chrono::steady_clock::time_point tp_start = std::chrono::steady_clock::now();
            do {
                loopcnt += 1;
                sleep_micros = (slice_micros - adjust_micros);
                // wait for timeout or until notified
                m_timerCond.wait_for(tul, std::chrono::microseconds(sleep_micros));
                // notify work thread
                if (0 == (loopcnt % TIMER_SLICE_COUNT)) {
                    m_workerCond.notify_one();
                }
                tp_elapsed = std::chrono::steady_clock::now();
                diff_start_micros = std::chrono::duration_cast<std::chrono::microseconds>(tp_elapsed - tp_start);
                // std::cout << "micros: " << diff_start_micros.count() << std::endl;
                delay_expired_micros = diff_start_micros - (slice_micros * loopcnt);
                if ((delay_expired_micros.count() > 200) || (delay_expired_micros.count() < -200)) {
                    adjust_micros = delay_expired_micros / 10 * 9;
                    if (adjust_micros.count() > slice_micros.count()) {
                        //prevent overshoot
                        adjust_micros = slice_micros / 10 * 9;
                    }
                } else {
                    adjust_micros = std::chrono::microseconds(0);
                }
                // std::cout << "adjust_micros: " << adjust_micros.count() << std::endl;
                #if 0
                std::cout<<"diff_start_micros="<<diff_start_micros.count()
                    <<",sleep_micros="<<sleep_micros.count()
                    <<",delay_expired_micros="<<delay_expired_micros.count()
                    <<",adjust_micros="<<adjust_micros.count()
                    <<std::endl;
                #endif
            } while (true == m_timerRunning);
        });
#endif
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
        // std::cout << "stop." << std::endl;
    }

    template<typename callable, class... arguments>
        void syncDelay(int delay, callable&& fcb, arguments&&... args) {
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
        void asyncDelay(int delay, callable&& fcb, arguments&&... args) {
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


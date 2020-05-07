/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* File name   : cpptimer.cpp
* Created date: 2020-05-07 00:35:00
*******************************************/

#include <iostream>
#include <string>
#include <memory>
#include <time.h>
#include "cpptimer.hpp"

using namespace std;

long long getClkMiliseconds()
{
    struct timespec ts {};
    //CLOCK_REALTIME:系统相对时间,从UTC 1970-1-1 0:0:0开始计时,更改系统时间会更改获取的值;
    //CLOCK_MONOTONIC:系统绝对时间,为系统重启到现在的时间,更改系统时间对它没有影响;
    //CLOCK_PROCESS_CPUTIME_ID:本进程到当前代码系统CPU花费的时间;
    //CLOCK_THREAD_CPUTIME_ID:本线程到当前代码系统CPU花费的时间;
    (void)clock_gettime(CLOCK_MONOTONIC, &ts);
    long long milliseconds = (ts.tv_sec*1000) + (ts.tv_nsec/1000000);
    return milliseconds;
}

long long getSysMiliseconds()
{
    struct timespec ts {};
    (void)clock_gettime(CLOCK_REALTIME, &ts);
    long long milliseconds = (ts.tv_sec*1000) + (ts.tv_nsec/1000000);
    return milliseconds;
}

void timerCallbackFunc(std::string&& s, std::string &&info=""){
    long long millis = getClkMiliseconds();
    std::cout << "test: " << millis << ", " << s << ", " << info << endl;
}
 
int main() {
    CPPTimer t;
    //周期性执行定时任务
    t.start(1000, std::bind(timerCallbackFunc,"hello periodic timer!", "hahaha"));
    std::this_thread::sleep_for(std::chrono::seconds(6));
    std::cout << "time: " << getClkMiliseconds() << ", restart" << std::endl;

    t.start(1000, std::bind(timerCallbackFunc,"hello periodic timer!", "haha2"));
    std::cout << "time: " << getClkMiliseconds() << ", start" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(601));
    std::cout << "time: " << getClkMiliseconds() << ", try to stop timer!" << std::endl;
    t.stop();
 
    t.start(1000, std::bind(timerCallbackFunc,  "hello periodic timer!", "hi"));
    std::cout << "time: " << getClkMiliseconds() << ", start" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(4));
    std::cout << "time: " << getClkMiliseconds() << ", try to stop timer!" << std::endl;
    t.stop();
 
    std::this_thread::sleep_for(std::chrono::seconds(2));
 
    //只执行一次定时任务
    //同步
    std::cout << "time: " << getClkMiliseconds()<< ", syncdelay" << std::endl;
    t.syncDelay(1000, timerCallbackFunc, "hello once timer!", "good");
    //异步
    std::cout << "time: " << getClkMiliseconds()<< ", asyncdelay" << std::endl;
    t.asyncDelay(1000, timerCallbackFunc, "hello once timer!", "ok");
 
    std::this_thread::sleep_for(std::chrono::seconds(2));
 
    return 0;
}


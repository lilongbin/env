//cpptimer.cpp
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
long long g_totalCnt = 0;
long long g_starttime = 0;
long long g_lasttime = 0;

long long getSteadyMillis()
{
    //CLOCK_REALTIME:系统相对时间,从UTC 1970-1-1 0:0:0开始计时,更改系统时间会更改获取的值;
    //CLOCK_MONOTONIC:系统绝对时间/单调时间,为系统重启到现在的时间,更改系统时间对它没有影响;
    //CLOCK_PROCESS_CPUTIME_ID:本进程到当前代码系统CPU花费的时间;
    //CLOCK_THREAD_CPUTIME_ID:本线程到当前代码系统CPU花费的时间;
    struct timespec ts {};
    (void)clock_gettime(CLOCK_MONOTONIC, &ts);
    long long milliseconds = (ts.tv_sec*1000) + (ts.tv_nsec/1000000);
    return milliseconds;
}

long long getSystemMillis()
{
    struct timespec ts {};
    (void)clock_gettime(CLOCK_REALTIME, &ts);
    long long milliseconds = (ts.tv_sec*1000) + (ts.tv_nsec/1000000);
    return milliseconds;
}

void timerCallbackFunc(std::string&& s, std::string &&info=""){
    g_totalCnt += 1;
    long long millis = getSteadyMillis();
    if (g_starttime == 0) {
        g_starttime = millis;
    }
    if (g_lasttime == 0) {
        g_lasttime = millis;
    }
    long long interval = millis - g_lasttime;
    long long spendtime = millis - g_starttime;
    std::cout<<__func__<<": sn="<<g_totalCnt<<",now="<<millis
        <<",spendtime="<<spendtime<<",interval="<<interval
        <<",s="<<s<<",info="<<info<<std::endl;
    g_lasttime = millis;
}

int main() {
    CPPTimer t;
    //周期性执行定时任务
    g_lasttime = getSteadyMillis();
    t.start(100, std::bind(timerCallbackFunc,"periodic timer!", "hahaha"));
    std::cout << "time: " << getSteadyMillis() << ", start" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(3));
    std::cout << "time: " << getSteadyMillis() << ", try to stop timer!" << std::endl;
    t.stop();

    g_lasttime = getSteadyMillis();
    t.start(300, std::bind(timerCallbackFunc,"periodic timer!", "haha2"));
    std::cout << "time: " << getSteadyMillis() << ", start" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(6));
    std::cout << "time: " << getSteadyMillis() << ", try to stop timer!" << std::endl;
    t.stop();

    g_lasttime = getSteadyMillis();
    t.start(100, std::bind(timerCallbackFunc,  "periodic timer!", "hi"));
    std::cout << "time: " << getSteadyMillis() << ", start" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(4));
    std::cout << "time: " << getSteadyMillis() << ", try to stop timer!" << std::endl;
    t.stop();

    std::this_thread::sleep_for(std::chrono::seconds(3));
    return 0;
}



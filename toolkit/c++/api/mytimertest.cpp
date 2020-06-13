//mytimer.cpp
/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* File name   : mytimer.cpp
* Created date: 2020-05-07 00:35:00
*******************************************/

#include <iostream>
#include <string>
#include <memory>
#include <time.h>
#include "mytimer.hpp"

using namespace std;
long long g_totalCnt = 0;
long long g_starttime = 0;
long long g_lasttime = 0;
long long g_warncount = 0;

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

void timerCallbackFunc(std::string &&info, int number) {
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
    if (interval < 50) {
        std::cout<<"###### "<<__func__<<" warning: unexpected interval="<<interval<<std::endl;
        g_warncount += 1;
    }
    std::cout<<__func__<<":sn="<<g_totalCnt<<",warn="<<g_warncount
        <<",now="<<millis<<",spendtime="<<spendtime<<",interval="<<interval
        <<",info="<<info<<",number="<<number<<std::endl;
    g_lasttime = millis;
}

int main() {
    MYTimer *ptimer = new MYTimer();
    if (ptimer == NULL) {
        std::cout << "ERROR: create timer failed." << std::endl;
        return -1;
    }
    // std::this_thread::sleep_for(std::chrono::seconds(3));
    //周期性执行定时任务
    g_starttime = getSteadyMillis();
    g_lasttime = getSteadyMillis();
    ptimer->start(100, std::bind(timerCallbackFunc,"periodic timer!", 1));
    std::this_thread::sleep_for(std::chrono::seconds(3));
    ptimer->stop();

    int i = 0;
    std::string info;
    for (i = 0; i< 2000; i++) {
        info = "hi";
        g_lasttime = getSteadyMillis();
        ptimer->start(100, std::bind(timerCallbackFunc, info.c_str(), i));
        std::this_thread::sleep_for(std::chrono::seconds(1));
        ptimer->stop();
    }

    g_lasttime = getSteadyMillis();
    ptimer->start(1000, std::bind(timerCallbackFunc,"periodic timer!", 1));
    std::this_thread::sleep_for(std::chrono::seconds(60));
    ptimer->stop();

    std::this_thread::sleep_for(std::chrono::seconds(3));
    if (ptimer != NULL) {
        delete ptimer;
        ptimer = NULL;
    }
    std::cout <<"warning time="<<g_warncount<<std::endl;
    return 0;
}



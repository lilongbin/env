/*****************************************
 * Copyright (C) 2020 * Ltd. All rights reserved.
 * 
 * File name   : main.cpp
 * Author      : longbin
 * Created date: 2020-09-04 15:31:06
 * Description : 
 *
 *******************************************/

#include <iostream>

#include <string>
#include <map>
#include <vector>
#include <thread>

#include "UdpSocketReceiver.h"

using namespace std;

UdpSocketReceiver *receiver = NULL;
UdpSocketThreadArgs_T * args = NULL;
std::thread t_recv;
std::thread t_proc;

void start() {
    receiver = new UdpSocketReceiver();
    args = new UdpSocketThreadArgs_T();
    args->tcpSocketReceiver = receiver;
    t_recv = std::thread(receiver->dataReceiveTask, (void*)args);
    t_proc = std::thread(receiver->dataProcessTask, (void*)args);
}

void stop() {
    receiver->stopTasks();
    if (t_recv.joinable()) {
        t_recv.join();
    }
    if (t_proc.joinable()) {
        t_proc.join();
    }
}
int main() {
    start();
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    receiver->enableConnectSocket(true);
    receiver->startReceiveData(true);
    while (1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
    }
    return 0;
}


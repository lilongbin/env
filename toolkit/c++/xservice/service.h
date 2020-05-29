/*****************************************
 * Copyright (C) 2020 * Ltd. All rights reserved.
 * 
 * File name   : service.h
 * Author      : longbin
 * Created date: 2020-01-03 14:49:26
 * Description : 
 *
 *******************************************/

#ifndef __SERVICE_H__
#define __SERVICE_H__

#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>

#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <atomic>
#include <condition_variable>
#include <algorithm>

#include "comm.h"
#include "singleton.h"
#include "msgqueue.h"
#include "msgmanager.h"
#include "eventcallback.h"

struct MsgManager;

/* class Service */
class Service:public      Singleton<Service>
{
	friend class Singleton<Service>;
public:
	~Service();
	void start();
	void stop();
	void registerEventCallback(EventCallback *callback);
	void onRequest(MsgQueueType msg);
	void handleMsg(MsgQueueType msg);
	void onResponse(MsgQueueType msg);

private:
	Service();
	std::mutex m_mutex;
	EventCallback      * m_eventCallback;
	MsgSequenceID      * m_msgSequence;
	MsgQueueSender     * m_msgQueueSender;
	MsgQueueReceiver   * m_msgQueueReceiver;
	MsgQueueDispatcher * m_msgQueueDispatcher;
	MsgManager         * m_msgManager;

	std::thread m_senderThread;
	std::thread m_receiverThread;
	std::thread m_dispatcherThread;
};

#endif


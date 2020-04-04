/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* 
* File name   : msgmanager.h
* Author      : longbin
* Created date: 2020-01-03 13:25:34
* Description : 
*
*******************************************/

#ifndef __MSG_MANAGER_H__
#define __MSG_MANAGER_H__

#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <atomic>
#include <condition_variable>
#include <algorithm>

#include "comm.h"
#include "singleton.h"
#include "msgsequence.h"
#include "msgqueue.h"
#include "service.h"

struct MsgManager;
struct Service;

typedef struct ThreadArgsType
{
	Service            * mService;
	MsgQueueSender     * msgQueueSender;
	MsgQueueReceiver   * msgQueueReceiver;
	MsgQueueDispatcher * msgQueueDispatcher;
	void * arg;
} ThreadArgsType;

extern volatile bool g_MainThreadIsAlive;

/* class MsgManager */
class MsgManager:public      Singleton<MsgManager>
{
   friend class Singleton<MsgManager>;
public:
   ~MsgManager();
   static void * sender(void * mstruct); /* read msgque T3 and send to tbox */
   static void * receiver(void * mstruct); /* read msg from tbox and send to dispatcher msgque T1 */
   static void * dispatcher(void * mstruct); /* read msgque T1 and handle msgs */

private:
   MsgManager();
   std::mutex m_mutex;
};

#endif


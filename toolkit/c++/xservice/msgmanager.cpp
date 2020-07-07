/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* 
* File name   : msgmanager.cpp
* Author      : longbin
* Created date: 2020-01-03 14:24:38
* Description : 
*
*******************************************/

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "comm.h"
#include "msgmanager.h"
#include "log.h"

volatile bool g_MainThreadIsAlive;

/*-----------MsgManager--begin-----------*/
MsgManager::MsgManager()
{
    ALOGW("%s ok", __func__);
}

MsgManager::~MsgManager()
{
    ALOGW("%s ok", __func__);
}

void * MsgManager::sender(void * mstruct)
{
    pid_t tid = 0;//gettid();
	/*
     * std::thread::id tid = std::this_thread::get_id();
	 * std::ostringstream oss;
	 * oss << tid;
	 * std::string stid = oss.str();
	 * ALOGI("%s: threadId=%s", __func__, stid.c_str());
	 */
	ALOGI("%s: start tid=%d", __func__, tid);

    ThreadArgsType * ThreadArgs = (ThreadArgsType *)mstruct;
    MsgQueueSender     *msgQueueSender     = ThreadArgs->msgQueueSender;
    MsgQueueReceiver   *msgQueueReceiver   = ThreadArgs->msgQueueReceiver;
    MsgQueueDispatcher *msgQueueDispatcher = ThreadArgs->msgQueueDispatcher;
    if (msgQueueSender == NULL) {ALOGW("%s: invalid thread param. msgQueueSender", __func__);}
    if (msgQueueReceiver == NULL) {ALOGW("%s: invalid thread param. msgQueueReceiver", __func__);}
    if (msgQueueDispatcher == NULL) {ALOGW("%s: invalid thread param. msgQueueDispatcher", __func__);}

    MsgQueueType msg {};
    bool msgret = false;

    while (g_MainThreadIsAlive == true)
    {
    	if (msgQueueSender == NULL)
		{
    		std::this_thread::sleep_for(std::chrono::milliseconds(300));
    		continue;
		}

    	/* block read timeout or msgqueue deactived */
		msgQueueSender->waitForItems();
		if (g_MainThreadIsAlive != true) { break; }

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		if (g_MainThreadIsAlive != true) { break; }

		msgret = msgQueueSender->read(msg);
		if (msgret != true)
		{
			continue;
		}

		if (msgQueueDispatcher != NULL)
		{
			msgQueueDispatcher->write(msg);
		}
    }

    ALOGW("MsgManager::sender exit since g_MainThreadIsAlive false");
    return 0;
}

void * MsgManager::receiver(void * mstruct)
{
    pid_t tid = 0;//gettid();
	/*
     * std::thread::id tid = std::this_thread::get_id();
     * std::ostringstream oss;
     * oss << tid;
     * std::string stid = oss.str();
     * ALOGI("%s: threadId=%s", __func__, stid.c_str());
	 */
	ALOGI("%s: start tid=%d", __func__, tid);

    ThreadArgsType * ThreadArgs = (ThreadArgsType *)mstruct;
    MsgQueueSender     *msgQueueSender     = ThreadArgs->msgQueueSender;
    MsgQueueReceiver   *msgQueueReceiver   = ThreadArgs->msgQueueReceiver;
    MsgQueueDispatcher *msgQueueDispatcher = ThreadArgs->msgQueueDispatcher;
    if (msgQueueSender == NULL) {ALOGW("%s: invalid thread param. msgQueueSender", __func__);}
    if (msgQueueReceiver == NULL) {ALOGW("%s: invalid thread param. msgQueueReceiver", __func__);}
    if (msgQueueDispatcher == NULL) {ALOGW("%s: invalid thread param. msgQueueDispatcher", __func__);}

    MsgQueueType msg {};
    bool msgret = false;
    uint16_t bufflength = 0;

    while (g_MainThreadIsAlive == true)
    {
    	if (msgQueueReceiver == NULL) {
    		std::this_thread::sleep_for(std::chrono::milliseconds(300));
    		continue;
    	}

    	/* block read timeout or msgqueue deactived */
    	msgQueueReceiver->waitForItems();
		if (g_MainThreadIsAlive != true) { break; }

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		if (g_MainThreadIsAlive != true) { break; }

		msgret = msgQueueReceiver->read(msg);
		if (msgret != true)
		{
			continue;
		}

        if (msgQueueDispatcher != NULL)
        {
            msgQueueDispatcher->write(msg);
        }
    }

    ALOGW("MsgManager::receiver exit since g_MainThreadIsAlive false");
    return 0;
}

void * MsgManager::dispatcher(void * mstruct)
{
    pid_t tid = 0;//gettid();
	/*
     * std::thread::id tid = std::this_thread::get_id();
     * std::ostringstream oss;
     * oss << tid;
     * std::string stid = oss.str();
     * ALOGI("%s: threadId=%s", __func__, stid.c_str());
	 */
	ALOGI("%s: start tid=%d", __func__, tid);

    ThreadArgsType * ThreadArgs = (ThreadArgsType *)mstruct;
	Service *mService = ThreadArgs->mService;
    MsgQueueSender     *msgQueueSender     = ThreadArgs->msgQueueSender;
    MsgQueueReceiver   *msgQueueReceiver   = ThreadArgs->msgQueueReceiver;
    MsgQueueDispatcher *msgQueueDispatcher = ThreadArgs->msgQueueDispatcher;
    if (msgQueueSender == NULL) {ALOGW("%s: invalid thread param. msgQueueSender", __func__);}
    if (msgQueueReceiver == NULL) {ALOGW("%s: invalid thread param. msgQueueReceiver", __func__);}
    if (msgQueueDispatcher == NULL) {ALOGW("%s: invalid thread param. msgQueueDispatcher", __func__);}

    MsgQueueType msg {};
    bool msgret = false;

    /* msgque dispatcher --> callback */
    while (g_MainThreadIsAlive == true)
    {
    	if (msgQueueDispatcher == NULL) {
			std::this_thread::sleep_for(std::chrono::milliseconds(300));
			continue;
		}

    	/* block read timeout or msgqueue deactived */
    	msgQueueDispatcher->waitForItems();
		if (g_MainThreadIsAlive != true) { break; }

		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		if (g_MainThreadIsAlive != true) { break; }

        msgret = msgQueueDispatcher->read(msg);
        if (msgret != true)
		{
			continue;
		}

        /* handle msg */
		ALOGI("%s cmdId=%d", __func__, msg.header.cmdId);
        switch (msg.header.cmdId)
        {
            default:
				mService->handleMsg(msg);
                break;
        }
    }

    ALOGW("MsgManager::dispatcher exit since g_MainThreadIsAlive false");
    return 0;
}

/*-----------MsgManager--end-----------*/


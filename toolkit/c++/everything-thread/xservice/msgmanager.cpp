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

volatile bool g_MainThreadIsAlive;

/*-----------MsgManager--begin-----------*/
MsgManager::MsgManager()
{
    ALOGW("%s\n", __func__);
}

MsgManager::~MsgManager()
{
    ALOGW("%s\n", __func__);
}

/* read msgque T3 and send to tbox */
void * MsgManager::sender(void * mstruct)
{
	/*
     * std::thread::id tid = std::this_thread::get_id();
	 * std::ostringstream oss;
	 * oss << tid;
	 * std::string stid = oss.str();
	 * ALOGI("%s: threadId=%s", __func__, stid.c_str());
	 */
	ALOGI("%s: start\n", __func__);

    ThreadArgsType * ThreadArgs = (ThreadArgsType *)mstruct;
    MsgQueueSender     *msgQueueSender     = ThreadArgs->msgQueueSender;
    MsgQueueReceiver   *msgQueueReceiver   = ThreadArgs->msgQueueReceiver;
    MsgQueueDispatcher *msgQueueDispatcher = ThreadArgs->msgQueueDispatcher;
    if (msgQueueSender == NULL) {ALOGW("%s: invalid thread param. msgQueueSender\n", __func__);}
    if (msgQueueReceiver == NULL) {ALOGW("%s: invalid thread param. msgQueueReceiver\n", __func__);}
    if (msgQueueDispatcher == NULL) {ALOGW("%s: invalid thread param. msgQueueDispatcher\n", __func__);}

    MsgQueueType msg;
    uint32_t commandId = 0;
    uint16_t payloadLength = 0;

    /* msgque sender --> tbox */
    while (g_MainThreadIsAlive == true)
    {
        /* read msgque T3 TASK_TBOX_MSG_SENDER */
		msg.header.cmdId = 0;
        if (msgQueueSender != NULL)
        {
            msg = msgQueueSender->read();
        }

        commandId = msg.header.cmdId;
        payloadLength = msg.header.payloadLength;
        if ((commandId == 0) && (payloadLength == 0))
        {
            /* invalid msg */
            /* wait about 30ms */
            // std::this_thread::sleep_for(std::chrono::milliseconds(30));
			usleep(30);
            continue;
        }

		if (msgQueueDispatcher != NULL)
		{
			msgQueueDispatcher->write(msg);
		}

        /* wait about 10ms */
        // std::this_thread::sleep_for(std::chrono::milliseconds(10));
		usleep(10);
    }

    ALOGW("MsgManager::sender exit since g_MainThreadIsAlive false\n");
    return 0;
}

/* read msg from tbox and send to dispatcher msgque T1 */
/* msgque T2 reserved for receiver debug */
void * MsgManager::receiver(void * mstruct)
{
	/*
     * std::thread::id tid = std::this_thread::get_id();
     * std::ostringstream oss;
     * oss << tid;
     * std::string stid = oss.str();
     * ALOGI("%s: threadId=%s", __func__, stid.c_str());
	 */
	ALOGI("%s: start\n", __func__);

    ThreadArgsType * ThreadArgs = (ThreadArgsType *)mstruct;
    MsgQueueSender     *msgQueueSender     = ThreadArgs->msgQueueSender;
    MsgQueueReceiver   *msgQueueReceiver   = ThreadArgs->msgQueueReceiver;
    MsgQueueDispatcher *msgQueueDispatcher = ThreadArgs->msgQueueDispatcher;
    if (msgQueueSender == NULL) {ALOGW("%s: invalid thread param. msgQueueSender\n", __func__);}
    if (msgQueueReceiver == NULL) {ALOGW("%s: invalid thread param. msgQueueReceiver\n", __func__);}
    if (msgQueueDispatcher == NULL) {ALOGW("%s: invalid thread param. msgQueueDispatcher\n", __func__);}

    MsgQueueType msg;
    uint32_t commandId = 0;
    uint16_t payloadLength = 0;
    uint16_t bufflength = 0;

    /* tbox --> msgque dispatcher */
    while (g_MainThreadIsAlive == true)
    {
        /* read from tbox */
		msg.header.cmdId = 0;
        bufflength = 0;
		if (msgQueueReceiver != NULL)
		{
			msg = msgQueueReceiver->read();
		}

        commandId = msg.header.cmdId;
        payloadLength = msg.header.payloadLength;
        if ((commandId == 0) && (payloadLength == 0))
        {
            /* wait about 30ms */
            // std::this_thread::sleep_for(std::chrono::milliseconds(30));
			usleep(30);
            continue;
        }

        /* send to msgque T1 TASK_TBOX_MSG_DISPATCHER */
        if (msgQueueDispatcher != NULL)
        {
            msgQueueDispatcher->write(msg);
        }
        /* wait about 10ms */
        // std::this_thread::sleep_for(std::chrono::milliseconds(10));
		usleep(10);
    }

    ALOGW("MsgManager::receiver exit since g_MainThreadIsAlive false\n");
    return 0;
}

/* read msgque T1 and handle msgs */
void * MsgManager::dispatcher(void * mstruct)
{
	/*
     * std::thread::id tid = std::this_thread::get_id();
     * std::ostringstream oss;
     * oss << tid;
     * std::string stid = oss.str();
     * ALOGI("%s: threadId=%s", __func__, stid.c_str());
	 */
	ALOGI("%s: start\n", __func__);

    ThreadArgsType * ThreadArgs = (ThreadArgsType *)mstruct;
	Service *mService = ThreadArgs->mService;
    MsgQueueSender     *msgQueueSender     = ThreadArgs->msgQueueSender;
    MsgQueueReceiver   *msgQueueReceiver   = ThreadArgs->msgQueueReceiver;
    MsgQueueDispatcher *msgQueueDispatcher = ThreadArgs->msgQueueDispatcher;
    if (msgQueueSender == NULL) {ALOGW("%s: invalid thread param. msgQueueSender\n", __func__);}
    if (msgQueueReceiver == NULL) {ALOGW("%s: invalid thread param. msgQueueReceiver\n", __func__);}
    if (msgQueueDispatcher == NULL) {ALOGW("%s: invalid thread param. msgQueueDispatcher\n", __func__);}

    MsgQueueType msg;
    uint16_t payloadLength = 0;
    uint32_t commandId = 0;

    /* msgque dispatcher --> callback */
    while (g_MainThreadIsAlive == true)
    {
        /* read msgque T1 dispatcher */
		msg.header.cmdId = 0;
        if (msgQueueDispatcher != NULL)
        {
            msg = msgQueueDispatcher->read();
        }
        payloadLength = msg.header.payloadLength;
        commandId = msg.header.cmdId;
        if ((commandId == 0) && (payloadLength == 0))
        {
            /* wait about 30ms */
            // std::this_thread::sleep_for(std::chrono::milliseconds(30));
			usleep(30);
            continue;
        }

        /* handle tbox msg */
		ALOGI("%s cmdId=%d\n", __func__, msg.header.cmdId);
        switch (commandId)
        {
            default:
				mService->handleMsg(msg);
                break;
        }
        /* wait about 10ms */
        // std::this_thread::sleep_for(std::chrono::milliseconds(10));
		usleep(10);
    }

    ALOGW("MsgManager::dispatcher exit since g_MainThreadIsAlive false\n");
    return 0;
}

/*-----------MsgManager--end-----------*/


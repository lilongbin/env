/*****************************************
 * Copyright (C) 2020 * Ltd. All rights reserved.
 * 
 * File name   : service.cpp
 * Author      : longbin
 * Created date: 2020-01-03 14:51:59
 * Description : 
 *
 *******************************************/

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <thread>

#include "comm.h"
#include "service.h"
#include "log.h"

#ifdef LOG_TAG
#undef LOG_TAG
#endif
// #define LOG_TAG "Service"
#define LOG_TAG __FILE__ ":" LOG_XSTR(__LINE__)

/*-----------Service--begin-----------*/
Service::Service()
{
	g_MainThreadIsAlive = true;
	m_eventCallback = NULL;
	ALOGW("%s ok", __func__);
}

Service::~Service()
{
	g_MainThreadIsAlive = false;
	ALOGW("%s ok", __func__);
}

void Service::start()
{
	ALOGW("%s: begin", __func__);
	m_msgSequence = MsgSequenceID::getInstance();
	if (m_msgSequence == NULL) {
		ALOGE("MsgSequenceID getInstance failed.");
	}
	m_msgQueueSender = MsgQueueSender::getInstance();
	if (m_msgQueueSender == NULL) {
		ALOGE("MsgQueueSender getInstance failed.");
	}
	m_msgQueueReceiver = MsgQueueReceiver::getInstance();
	if (m_msgQueueReceiver == NULL) {
		ALOGE("MsgQueueReceiver getInstance failed.");
	}
	m_msgQueueDispatcher = MsgQueueDispatcher::getInstance();
	if (m_msgQueueDispatcher == NULL) {
		ALOGE("MsgQueueDispatcher getInstance failed.");
	}
	m_msgManager = MsgManager::getInstance();
	if (m_msgManager == NULL) {
		ALOGE("MsgManager getInstance failed.");
	}

	ALOGI("%s: create new threads", __func__);
	ThreadArgsType *tArgs = new ThreadArgsType();
	tArgs->mService           = this;
	tArgs->msgQueueSender     = m_msgQueueSender;
	tArgs->msgQueueReceiver   = m_msgQueueReceiver;
	tArgs->msgQueueDispatcher = m_msgQueueDispatcher;
	tArgs->arg = 0;

	ALOGI("%s: create thread: sender", __func__);
	m_senderThread = std::thread(&m_msgManager->sender, (void *)tArgs);
	ALOGI("%s: create thread: receiver", __func__);
	m_receiverThread = std::thread(&m_msgManager->receiver, (void *)tArgs);
	ALOGI("%s: create thread: dispatcher", __func__);
	m_dispatcherThread = std::thread(&m_msgManager->dispatcher, (void *)tArgs);

	ALOGI("%s: end", __func__);
}

void Service::stop()
{
	ALOGW("%s begin", __func__);

	// set flag
	g_MainThreadIsAlive = false;

	// deactive all msg queue
	if (m_msgQueueSender != NULL) {
		m_msgQueueSender->deactivate();
	}
	if (m_msgQueueReceiver != NULL) {
		m_msgQueueReceiver->deactivate();
	}
	if (m_msgQueueDispatcher != NULL) {
		m_msgQueueDispatcher->deactivate();
	}

	// wait thread stopped
	if (m_senderThread.joinable())
	{
		m_senderThread.join();
	}
	if (m_receiverThread.joinable())
	{
		m_receiverThread.join();
	}
	if (m_dispatcherThread.joinable())
	{
		m_dispatcherThread.join();
	}

	m_msgQueueSender = NULL;
	m_msgQueueReceiver = NULL;
	m_msgQueueDispatcher = NULL;
    ALOGI("%s end", __func__);
}

void Service::registerEventCallback(EventCallback *callback)
{
	ALOGI("%s callback", __func__);
	if (callback == NULL)
	{
		ALOGI("%s callback is null", __func__);
	}
	m_eventCallback = callback;
}

void Service::onRequest(MsgQueueType msg)
{
	ALOGI("%s: cmdId=%d", __func__, msg.header.cmdId);
	m_msgQueueSender->write(msg);
}

void Service::handleMsg(MsgQueueType msg)
{
	ALOGI("%s: cmdId=%d", __func__, msg.header.cmdId);
	ALOGI("%s: processing ****************************************", __func__);
	onResponse(msg);
}

void Service::onResponse(MsgQueueType msg)
{
	ALOGI("%s: cmdId=%d callback begin", __func__, msg.header.cmdId);
	if (m_eventCallback != NULL)
	{
		m_eventCallback->onResponse(msg);
	}
	ALOGI("%s: cmdId=%d callback end", __func__, msg.header.cmdId);
}

/*-----------Service--end-----------*/


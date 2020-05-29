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

/*-----------Service--begin-----------*/
Service::Service()
{
	g_MainThreadIsAlive = true;
	m_eventCallback = NULL;
	ALOGW("%s\n", __func__);
}

Service::~Service()
{
	ALOGW("%s\n", __func__);
	g_MainThreadIsAlive = false;
}

void Service::start()
{
	ALOGW("%s\n", __func__);
	m_msgSequence = MsgSequenceID::getInstance();
	if (m_msgSequence == NULL) {
		ALOGE("MsgSequenceID getInstance failed.\n");
	}
	m_msgQueueSender = MsgQueueSender::getInstance();
	if (m_msgQueueSender == NULL) {
		ALOGE("MsgQueueSender getInstance failed.\n");
	}
	m_msgQueueReceiver = MsgQueueReceiver::getInstance();
	if (m_msgQueueReceiver == NULL) {
		ALOGE("MsgQueueReceiver getInstance failed.\n");
	}
	m_msgQueueDispatcher = MsgQueueDispatcher::getInstance();
	if (m_msgQueueDispatcher == NULL) {
		ALOGE("MsgQueueDispatcher getInstance failed.\n");
	}
	m_msgManager = MsgManager::getInstance();
	if (m_msgManager == NULL) {
		ALOGE("MsgManager getInstance failed.\n");
	}

	ALOGI("%s: create new threads\n", __func__);
	ThreadArgsType *tArgs = new ThreadArgsType();
	tArgs->mService           = this;
	tArgs->msgQueueSender     = m_msgQueueSender;
	tArgs->msgQueueReceiver   = m_msgQueueReceiver;
	tArgs->msgQueueDispatcher = m_msgQueueDispatcher;
	tArgs->arg = 0;

	ALOGI("%s: create thread: sender\n", __func__);
	m_senderThread = std::thread(&m_msgManager->sender, (void *)tArgs);
	ALOGI("%s: create thread: receiver\n", __func__);
	m_receiverThread = std::thread(&m_msgManager->receiver, (void *)tArgs);
	ALOGI("%s: create thread: dispatcher\n", __func__);
	m_dispatcherThread = std::thread(&m_msgManager->dispatcher, (void *)tArgs);

	ALOGI("%s: end\n", __func__);
}

void Service::stop()
{
	ALOGW("%s\n", __func__);

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
}

void Service::registerEventCallback(EventCallback *callback)
{
	ALOGI("%s\n", __func__);
	if (callback == NULL)
	{
		ALOGI("%s callback is null\n", __func__);
	}
	m_eventCallback = callback;
}

void Service::onRequest(MsgQueueType msg)
{
	ALOGI("%s: cmdId=%d\n", __func__, msg.header.cmdId);
	m_msgQueueSender->write(msg);
}

void Service::handleMsg(MsgQueueType msg)
{
	ALOGI("%s: cmdId=%d\n", __func__, msg.header.cmdId);
	ALOGI("%s: processing ****************************************\n", __func__);
	onResponse(msg);
}

void Service::onResponse(MsgQueueType msg)
{
	ALOGI("%s: cmdId=%d\n", __func__, msg.header.cmdId);
	ALOGI("%s: cmdId=%d callback end\n", __func__, msg.header.cmdId);
	if (m_eventCallback != NULL)
	{
		m_eventCallback->onResponse(msg);
	}
}

/*-----------Service--end-----------*/


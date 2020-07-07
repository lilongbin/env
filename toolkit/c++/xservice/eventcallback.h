/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* File name   : eventcallback.h
* Author      : longbin
*******************************************/

#ifndef __EVENTCALLBACK_H__
#define __EVENTCALLBACK_H__

#include "comm.h"

/* class EventCallback */
class EventCallback
{
public:
	EventCallback() {}
	virtual ~EventCallback() {}
	virtual void onResponse(MsgQueueType msg) {}
};

#endif


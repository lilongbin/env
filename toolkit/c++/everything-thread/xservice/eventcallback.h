/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* 
* File name   : eventcallback.h
* Author      : longbin
* Created date: 2020-01-03 16:43:42
* Description : 
*
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


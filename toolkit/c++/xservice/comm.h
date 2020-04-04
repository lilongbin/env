/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* 
* File name   : comm.h
* Author      : longbin
* Created date: 2020-01-03 13:25:34
* Description : 
*
*******************************************/

#ifndef __COMM_H__
#define __COMM_H__

#include <iostream>

#define ALOGI while(0) printf
#define ALOGW while(0) printf
#define ALOGE while(0) printf
#define ALOGD while(0) printf

typedef struct MsgHeaderType {
	uint32_t seqId;
	uint16_t funId;
	uint32_t cmdId;
	uint16_t payloadLength;
} MsgHeaderType;

typedef struct MsgQueueType
{
	MsgHeaderType        header;
	std::vector<uint8_t> vpayload;
} MsgQueueType;

#endif


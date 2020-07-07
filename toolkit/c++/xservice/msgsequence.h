/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* File name   : msgsquence.h
* Author      : longbin
*******************************************/

#ifndef __MSG_SEQUENCE_H__
#define __MSG_SEQUENCE_H__

#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <atomic>
#include <condition_variable>
#include <algorithm>

#include "comm.h"
#include "singleton.h"

/* class MsgSequenceID */
class MsgSequenceID:public Singleton<MsgSequenceID>
{
	friend class Singleton<MsgSequenceID>;
public:
	~MsgSequenceID();
	uint32_t getSequenceID();
	bool initSequenceID();

private:
	MsgSequenceID();

	using MuxGuard = std::lock_guard<std::mutex>;
	mutable std::mutex mLock;
	uint32_t m_sequenceID;
};

#endif

/*****************************************
* Copyright (C) 2019 * Ltd. All rights reserved.
* 
* File name   : sequenceId.cpp
* Author      : longbin
* Created date: 2019-07-25 18:54:12
* Description : 
*
*******************************************/

#include <iostream>

#include <string>
#include <map>
#include <vector>

#include "sequenceId.hpp"

Header_SequenceID::Header_SequenceID()
{
	m_sequenceId = 0;
}

Header_SequenceID::~Header_SequenceID()
{
	m_sequenceId = 0;
	/* destory mutex */
}

unsigned int Header_SequenceID::getSequenceId()
{
	unsigned int id = 0;

	if (0 == m_sequenceId)
	{
		(void)initSequenceId();
	}
	/* mutex lock */
    m_mutex.lock();
	id = m_sequenceId++;
	/* mutex unlock */
    m_mutex.unlock();

	return id;
}

bool Header_SequenceID::initSequenceId()
{
	bool ret = false;
	/* mutex attr */
	/* mutex init */
	/* mutex create */
	return ret;
}


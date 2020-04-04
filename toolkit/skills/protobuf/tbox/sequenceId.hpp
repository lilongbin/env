/*****************************************
* Copyright (C) 2019 * Ltd. All rights reserved.
* 
* File name   : sequenceId.hpp
* Author      : longbin
* Created date: 2019-07-25 18:47:48
* Description : 
*
*******************************************/


#ifndef _SEQUENCEID_H
#define _SEQUENCEID_H
/*
#ifdef __cplusplus
extern "C"
{
#endif
*/
#include "singleton.hpp"
#include <iostream>
#include <thread>
#include <mutex>

/*
 * usage:
 * Header_SequenceID * m_sequence = Header_SequenceID::getInstance();
 * id = m_sequence->getSequenceId();
 * */
class Header_SequenceID:public Singleton<Header_SequenceID>
{
	friend class Singleton<Header_SequenceID>;
	public:
	~Header_SequenceID();
	unsigned int getSequenceId();
	bool initSequenceId();

	private:
	Header_SequenceID();
	std::mutex m_mutex;
	unsigned int m_sequenceId;
};

/*
#ifdef __cplusplus
}
#endif
*/
#endif //SEQUENCEID_H

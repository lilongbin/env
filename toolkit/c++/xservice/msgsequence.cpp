/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* 
* File name   : msgsequence.cpp
* Author      : longbin
* Created date: 2020-01-03 13:40:08
* Description : 
*
*******************************************/

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include "comm.h"
#include "msgsequence.h"
#include "log.h"

/*-----------MsgSequenceID--begin-----------*/
MsgSequenceID::MsgSequenceID()
{
   m_sequenceID = 0;
   (void)initSequenceID();
}

MsgSequenceID::~MsgSequenceID()
{
   m_sequenceID = 0;
}

uint32_t MsgSequenceID::getSequenceID()
{
   uint32_t tmp;
   MuxGuard g(mLock);
   tmp = m_sequenceID;
   m_sequenceID++;

   return tmp;
}

bool MsgSequenceID::initSequenceID()
{
   bool ret = false;
   return ret;
}
/*-----------MsgSequenceID--end-----------*/


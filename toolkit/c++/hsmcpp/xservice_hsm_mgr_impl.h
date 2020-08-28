/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* 
* File name   : xservice_hsm_mgr.h
* Author      : longbin
* Created date: 2020-08-05 14:14:45
* Description : 
*
*******************************************/
#ifndef __XSERVICE_HSM_MGR_IMPL_H__
#define __XSERVICE_HSM_MGR_IMPL_H__

#include "hsm_engine.h"

std::string getEventName(int event);

void XService_HSM_Start(void);
bool XService_HSM_ProcMessage(int event, const void * pdata, const size_t data_size);

#endif //__XSERVICE_HSM_MGR_IMPL_H__


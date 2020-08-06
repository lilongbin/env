/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* 
* File name   : xservice_hsm_mgr.h
* Author      : longbin
* Created date: 2020-08-05 14:14:45
* Description : 
*
*******************************************/
#ifndef __XSERVICE_HSM_MGR_H__
#define __XSERVICE_HSM_MGR_H__

#   include "hsm_engine.h"

#   ifdef __cplusplus
extern "C"
{ /* ! Inclusion of header files should NOT be inside the extern "C" block */
#   endif /* __cplusplus */

void XService_HSM_Shutdown(void);
void XService_HSM_Start(void);
HSM_Statechart_T * XService_HSM_Get(void);
bool_t XService_Send_Msg_To_HSM(int event_id, const void * data, size_t data_size);

#   ifdef __cplusplus
}         /* extern "C" */
#   endif /* __cplusplus */

#endif

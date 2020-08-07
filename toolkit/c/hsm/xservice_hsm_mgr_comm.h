/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* 
* File name   : xservice_hsm_mgr_comm.h
* Author      : longbin
* Created date: 2020-08-07 10:09:08
* Description : 
*
*******************************************/

#ifndef __XSERVICE_HSM_MGR_COMM_H__
#define __XSERVICE_HSM_MGR_COMM_H__

#  if 0
#    define XSERVICE_DEBUG_STATE_CHART 1
#    define DBG_ASSERT(x) assert((x))
#  else
#    ifdef XSERVICE_DEBUG_STATE_CHART
#      undef XSERVICE_DEBUG_STATE_CHART
#    endif
#    define DBG_ASSERT(x)
#  endif

#   ifdef __cplusplus
extern "C"
{ /* ! Inclusion of header files should NOT be inside the extern "C" block */
#   endif /* __cplusplus */

const char * event2str(const int event_id);

#   ifdef __cplusplus
}         /* extern "C" */
#   endif /* __cplusplus */

#endif


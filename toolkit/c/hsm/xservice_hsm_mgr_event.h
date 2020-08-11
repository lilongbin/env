/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* 
* File name   : xservice_hsm_mgr_event.h
* Author      : longbin
* Created date: 2020-08-05 14:30:41
* Description : 
*
*******************************************/

#ifndef __XSERVICE_HSM_MGR_EVENT_H__
#define __XSERVICE_HSM_MGR_EVENT_H__

enum __xservice_hsm_event_tag {
    EVT_TypedKeySign = 0,
    EVT_TypedKeyNumber0,
    EVT_TypedKeyNumber1_9,
    EVT_TypedKeyDot,
    EVT_TypedKeyEnter,
    EVT_TypedKeyPound,
    EVT_TypedKeyOther,
} XServiceHsmEvent;

#define EVENT2STR(id) case (id): { return #id; }
#define EVENT2STRS \
    EVENT2STR(EVT_TypedKeySign) \
    EVENT2STR(EVT_TypedKeyNumber0) \
    EVENT2STR(EVT_TypedKeyNumber1_9) \
    EVENT2STR(EVT_TypedKeyDot) \
    EVENT2STR(EVT_TypedKeyEnter) \
    EVENT2STR(EVT_TypedKeyPound) \
    EVENT2STR(EVT_TypedKeyOther) \
/* XServiceHsmEvent end */

#endif


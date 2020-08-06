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
    eTypedKeySign = 0,
    eTypedKeyNumber0,
    eTypedKeyNumber1_9,
    eTypedKeyDot,
    eTypedKeyEnter,
    eTypedKeyOther,
} XServiceHsmEvent;

#define EVENT2STR(id) case (id): { return #id; }
#define EVENT2STRS \
    EVENT2STR(eTypedKeySign) \
    EVENT2STR(eTypedKeyNumber0) \
    EVENT2STR(eTypedKeyNumber1_9) \
    EVENT2STR(eTypedKeyDot) \
    EVENT2STR(eTypedKeyEnter) \
    EVENT2STR(eTypedKeyOther) \
/* XServiceHsmEvent end */
#endif

/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* 
* File name   : xservice_hsm_mgr_declare.h
* Author      : longbin
* Created date: 2020-08-05 14:21:42
* Description : 
*
*******************************************/

#ifndef __XSERVICE_HSM_MGR_DECLARE_H__
#define __XSERVICE_HSM_MGR_DECLARE_H__

#include "hsm_engine.h"
#include "xservice_hsm_mgr_event.h"

#define STATECHART_DEFN()\
HSM_BEGIN(XSERVICE_MGR_HSM)\
  /*=================== STATE: Init ===============================*/\
  HSM_INITIAL_STATE(Init, HSM_TOP, HSM_NO_ACTION, STATE_EMPTY)\
  /*=================== STATE: STATE_EMPTY ===============================*/\
  HSM_SIMPLE_STATE(STATE_EMPTY, HSM_TOP, prompt_none, HSM_NO_ACTION)\
    /*HSM_INTERNAL_TRANSITION(eTypedKeyEnter, HSM_NO_GUARD, clear)*/\
    HSM_TRANSITION(eTypedKeySign, HSM_NO_GUARD, append, STATE_SIGNED)\
    HSM_TRANSITION(eTypedKeyNumber0, HSM_NO_GUARD, append, STATE_LEAD0)\
    HSM_TRANSITION(eTypedKeyNumber1_9, HSM_NO_GUARD, append, STATE_INTEGER)\
  HSM_END_STATE(STATE_EMPTY)\
  /*=================== STATE: STATE_TYPING ===============================*/\
  HSM_COMPOSITE_STATE(STATE_TYPING, HSM_TOP, HSM_NO_INITIAL_STATE, HSM_NO_HISTORY_STATE, HSM_NO_ACTION, HSM_NO_ACTION)\
    HSM_TRANSITION(eTypedKeyEnter, HSM_NO_GUARD, clear, STATE_EMPTY)\
  HSM_END_STATE(STATE_TYPING)\
  /*=================== STATE: STATE_SIGNED ===============================*/\
  HSM_SIMPLE_STATE(STATE_SIGNED, STATE_TYPING, prompt_signed, HSM_NO_ACTION)\
    HSM_TRANSITION(eTypedKeyNumber0, HSM_NO_GUARD, append, STATE_LEAD0)\
    HSM_TRANSITION(eTypedKeyNumber1_9, HSM_NO_GUARD, append, STATE_INTEGER)\
  HSM_END_STATE(STATE_SIGNED)\
  /*=================== STATE: STATE_LEAD0 ===============================*/\
  HSM_SIMPLE_STATE(STATE_LEAD0, STATE_TYPING, prompt_lead0, HSM_NO_ACTION)\
    HSM_TRANSITION(eTypedKeyDot, HSM_NO_GUARD, append, STATE_DOT)\
  HSM_END_STATE(STATE_LEAD0)\
  /*=================== STATE: STATE_INTEGER ===============================*/\
  HSM_SIMPLE_STATE(STATE_INTEGER, STATE_TYPING, prompt_integer, HSM_NO_ACTION)\
    HSM_INTERNAL_TRANSITION(eTypedKeyNumber0, HSM_NO_GUARD, append)\
    HSM_INTERNAL_TRANSITION(eTypedKeyNumber1_9, HSM_NO_GUARD, append)\
    HSM_TRANSITION(eTypedKeyDot, HSM_NO_GUARD, append, STATE_DOT)\
  HSM_END_STATE(STATE_INTEGER)\
  /*=================== STATE: STATE_DOT ===============================*/\
  HSM_SIMPLE_STATE(STATE_DOT, STATE_TYPING, prompt_dot, HSM_NO_ACTION)\
    HSM_INTERNAL_TRANSITION(eTypedKeyNumber0, HSM_NO_GUARD, append)\
    HSM_INTERNAL_TRANSITION(eTypedKeyNumber1_9, HSM_NO_GUARD, append)\
  HSM_END_STATE(STATE_DOT)\
\
HSM_END(XSERVICE_MGR_HSM)


#include "hsm_declare_statechart.h"

#endif


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

#include "xservice_hsm_mgr_event.h"
#include "hsm_engine.h"

#define STATECHART_DEFN()\
HSM_BEGIN(XSERVICE_MGR_HSM)\
  /*=================== STATE: Init ===============================*/\
  HSM_INITIAL_STATE(Init, HSM_TOP, HSM_NO_ACTION, STATE_EMPTY)\
  /*=================== STATE: STATE_EMPTY ===============================*/\
  HSM_SIMPLE_STATE(STATE_EMPTY, HSM_TOP, prompt_none, HSM_NO_ACTION)\
    HSM_TRANSITION(eTypedKeySign, HSM_NO_GUARD, append_signed, STATE_SIGNED)\
    HSM_TRANSITION(eTypedKeyNumber0, HSM_NO_GUARD, append_lead0, STATE_LEAD0)\
    HSM_TRANSITION(eTypedKeyNumber1_9, HSM_NO_GUARD, append_int, STATE_INTEGER)\
    HSM_INTERNAL_TRANSITION(eTypedKeyEnter, HSM_NO_GUARD, prompt_none)\
    HSM_INTERNAL_TRANSITION(eTypedKeyDot, HSM_NO_GUARD, notice)\
    HSM_INTERNAL_TRANSITION(eTypedKeyOther, HSM_NO_GUARD, notice)\
  HSM_END_STATE(STATE_EMPTY)\
  /*=================== STATE: STATE_TYPING ===============================*/\
  HSM_COMPOSITE_STATE(STATE_TYPING, HSM_TOP, HSM_NO_INITIAL_STATE, HSM_NO_HISTORY_STATE, HSM_NO_ACTION, HSM_NO_ACTION)\
    HSM_TRANSITION(eTypedKeyEnter, HSM_NO_GUARD, resulted, STATE_EMPTY)\
    HSM_INTERNAL_TRANSITION(eTypedKeySign, HSM_NO_GUARD, notice)\
    HSM_INTERNAL_TRANSITION(eTypedKeyOther, HSM_NO_GUARD, notice)\
    HSM_INTERNAL_TRANSITION(eTypedKeyDot, HSM_NO_GUARD, notice)\
  HSM_END_STATE(STATE_TYPING)\
  /*=================== STATE: STATE_SIGNED ===============================*/\
  HSM_SIMPLE_STATE(STATE_SIGNED, STATE_TYPING, prompt_signed, HSM_NO_ACTION)\
    HSM_TRANSITION(eTypedKeyNumber0, HSM_NO_GUARD, append_lead0, STATE_LEAD0)\
    HSM_TRANSITION(eTypedKeyNumber1_9, HSM_NO_GUARD, append_int, STATE_INTEGER)\
  HSM_END_STATE(STATE_SIGNED)\
  /*=================== STATE: STATE_LEAD0 ===============================*/\
  HSM_SIMPLE_STATE(STATE_LEAD0, STATE_TYPING, prompt_lead0, HSM_NO_ACTION)\
    HSM_TRANSITION(eTypedKeyDot, HSM_NO_GUARD, append_decimal, STATE_DECIMAL)\
    HSM_INTERNAL_TRANSITION(eTypedKeyNumber0, HSM_NO_GUARD, notice)\
    HSM_INTERNAL_TRANSITION(eTypedKeyNumber1_9, HSM_NO_GUARD, notice)\
  HSM_END_STATE(STATE_LEAD0)\
  /*=================== STATE: STATE_INTEGER ===============================*/\
  HSM_SIMPLE_STATE(STATE_INTEGER, STATE_TYPING, prompt_integer, HSM_NO_ACTION)\
    HSM_INTERNAL_TRANSITION(eTypedKeyNumber0, HSM_NO_GUARD, append_int)\
    HSM_INTERNAL_TRANSITION(eTypedKeyNumber1_9, HSM_NO_GUARD, append_int)\
    HSM_TRANSITION(eTypedKeyDot, HSM_NO_GUARD, append_decimal, STATE_DECIMAL)\
  HSM_END_STATE(STATE_INTEGER)\
  /*=================== STATE: STATE_DECIMAL ===============================*/\
  HSM_SIMPLE_STATE(STATE_DECIMAL, STATE_TYPING, prompt_dot, HSM_NO_ACTION)\
    HSM_INTERNAL_TRANSITION(eTypedKeyNumber0, HSM_NO_GUARD, append_decimal)\
    HSM_INTERNAL_TRANSITION(eTypedKeyNumber1_9, HSM_NO_GUARD, append_decimal)\
  HSM_END_STATE(STATE_DECIMAL)\
\
HSM_END(XSERVICE_MGR_HSM)


#include "hsm_declare_statechart.h"

#endif

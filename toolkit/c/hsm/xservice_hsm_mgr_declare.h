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
#include "xservice_hsm_mgr_comm.h"
#include "hsm_engine.h"

#define STATECHART_DEFN()\
HSM_BEGIN(XSERVICE_MGR_HSM)\
  /*=================== STATE: Init ===============================*/\
  HSM_INITIAL_STATE(Init, HSM_TOP, prompt_none, STATE_EMPTY)\
  /*=================== STATE: STATE_RESTART ===============================*/\
  HSM_JUNCTION_STATE(STATE_RESTART, HSM_TOP)\
    HSM_TRANSITION(HSM_NO_EVENT, HSM_NO_GUARD, prompt_reset, STATE_EMPTY)\
  HSM_END_STATE(STATE_RESTART)\
  /*=================== STATE: STATE_EMPTY ===============================*/\
  HSM_SIMPLE_STATE(STATE_EMPTY, HSM_TOP, HSM_NO_ACTION, HSM_NO_ACTION)\
    HSM_TRANSITION(EVT_TypedKeySign, HSM_NO_GUARD, proc_signed, STATE_SIGNED)\
    HSM_TRANSITION(EVT_TypedKeyNumber0, HSM_NO_GUARD, proc_lead0, STATE_LEAD0)\
    HSM_TRANSITION(EVT_TypedKeyNumber1_9, HSM_NO_GUARD, proc_int, STATE_INTEGER)\
    HSM_INTERNAL_TRANSITION(EVT_TypedKeyEnter, HSM_NO_GUARD, prompt_reset)\
    HSM_INTERNAL_TRANSITION(EVT_TypedKeyDot, HSM_NO_GUARD, onError)\
    HSM_INTERNAL_TRANSITION(EVT_TypedKeyOther, HSM_NO_GUARD, onError)\
  HSM_END_STATE(STATE_EMPTY)\
  /*=================== STATE: STATE_TYPING ===============================*/\
  HSM_COMPOSITE_STATE(STATE_TYPING, HSM_TOP, HSM_NO_INITIAL_STATE, HSM_NO_HISTORY_STATE, HSM_NO_ACTION, HSM_NO_ACTION)\
    HSM_TRANSITION(EVT_TypedKeyEnter, HSM_NO_GUARD, resulted, STATE_RESTART)\
    HSM_INTERNAL_TRANSITION(EVT_TypedKeySign, HSM_NO_GUARD, onError)\
    HSM_INTERNAL_TRANSITION(EVT_TypedKeyOther, HSM_NO_GUARD, onError)\
    HSM_INTERNAL_TRANSITION(EVT_TypedKeyDot, HSM_NO_GUARD, onError)\
  HSM_END_STATE(STATE_TYPING)\
  /*=================== STATE: STATE_SIGNED ===============================*/\
  HSM_SIMPLE_STATE(STATE_SIGNED, STATE_TYPING, HSM_NO_ACTION, HSM_NO_ACTION)\
    HSM_TRANSITION(EVT_TypedKeyNumber0, HSM_NO_GUARD, proc_lead0, STATE_LEAD0)\
    HSM_TRANSITION(EVT_TypedKeyNumber1_9, HSM_NO_GUARD, proc_int, STATE_INTEGER)\
  HSM_END_STATE(STATE_SIGNED)\
  /*=================== STATE: STATE_LEAD0 ===============================*/\
  HSM_SIMPLE_STATE(STATE_LEAD0, STATE_TYPING, HSM_NO_ACTION, HSM_NO_ACTION)\
    HSM_TRANSITION(EVT_TypedKeyDot, HSM_NO_GUARD, proc_dot, STATE_DECIMAL)\
    HSM_INTERNAL_TRANSITION(EVT_TypedKeyNumber0, HSM_NO_GUARD, onError)\
    HSM_INTERNAL_TRANSITION(EVT_TypedKeyNumber1_9, HSM_NO_GUARD, onError)\
  HSM_END_STATE(STATE_LEAD0)\
  /*=================== STATE: STATE_INTEGER ===============================*/\
  HSM_SIMPLE_STATE(STATE_INTEGER, STATE_TYPING, HSM_NO_ACTION, HSM_NO_ACTION)\
    HSM_INTERNAL_TRANSITION(EVT_TypedKeyNumber0, HSM_NO_GUARD, proc_int)\
    HSM_INTERNAL_TRANSITION(EVT_TypedKeyNumber1_9, HSM_NO_GUARD, proc_int)\
    HSM_TRANSITION(EVT_TypedKeyDot, HSM_NO_GUARD, proc_dot, STATE_DECIMAL)\
  HSM_END_STATE(STATE_INTEGER)\
  /*=================== STATE: STATE_DECIMAL ===============================*/\
  HSM_SIMPLE_STATE(STATE_DECIMAL, STATE_TYPING, HSM_NO_ACTION, HSM_NO_ACTION)\
    HSM_INTERNAL_TRANSITION(EVT_TypedKeyNumber0, HSM_NO_GUARD, proc_decimal)\
    HSM_INTERNAL_TRANSITION(EVT_TypedKeyNumber1_9, HSM_NO_GUARD, proc_decimal)\
  HSM_END_STATE(STATE_DECIMAL)\
\
HSM_END(XSERVICE_MGR_HSM)

#ifdef XSERVICE_DEBUG_STATE_CHART
#   define HSM_GENERATE_STATE_NAMES
#   define HSM_GENERATE_GUARD_NAMES
#   define HSM_GENERATE_ACTION_NAMES
#else
#   define HSM_GENERATE_STATE_NAMES
#endif

#include "hsm_declare_statechart.h"

#endif


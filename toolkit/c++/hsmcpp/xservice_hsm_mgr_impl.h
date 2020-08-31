/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* 
* File name   : xservice_hsm_mgr.h
* Author      : longbin
* Created date: 2020-08-24 14:14:45
* Description : 
*
*******************************************/
#ifndef __XSERVICE_HSM_MGR_IMPL_H__
#define __XSERVICE_HSM_MGR_IMPL_H__

#include "hsm_engine.h"

/**
 * The following was auto-generated by gen_hsm;
 * it declares the events used by statecharts within the model.
 */
#define MODEL_EVENT_IDS\
   X(EVT_TypedKeyDot)\
   X(EVT_TypedKeyEnter)\
   X(EVT_TypedKeyNumber0)\
   X(EVT_TypedKeyNumber1_9)\
   X(EVT_TypedKeyOther)\
   X(EVT_TypedKeyPound)\
   X(EVT_TypedKeySign)

#undef X
#define X(evt) evt,
typedef enum
{
   MODEL_EVENT_IDS
   MODEL_EVENT_IDS_NUM_EVENTS
}
Model_Event_Ids_T;


/**
 * The following was auto-generated by gen_hsm;
 * it declares the statechart Default_StateMachine for the HSM engine.
 */

#define STATECHART_DEFN()\
HSM_BEGIN(XSERVICE_MGR_HSM)\
  /*=================== STATE: Init ===============================*/\
  HSM_INITIAL_STATE(Init, HSM_TOP, prompt_none, STATE_EMPTY)\
  /*=================== STATE: STATE_READING ===============================*/\
  HSM_COMPOSITE_STATE(STATE_READING, HSM_TOP, HSM_NO_INITIAL_STATE, STATE_SHALLOW_HIST, HSM_NO_ACTION, HSM_NO_ACTION)\
    HSM_COMPLETION_TRANSITION(HSM_NO_ACTION, STATE_DEEP_HIST)\
    HSM_TRANSITION(EVT_TypedKeyPound, HSM_NO_GUARD, onInterupt2, STATE_DEEP_HIST)\
  HSM_END_STATE(STATE_READING)\
  /*=================== STATE: STATE_WORKING ===============================*/\
  HSM_COMPOSITE_STATE(STATE_WORKING, HSM_TOP, HSM_NO_INITIAL_STATE, STATE_DEEP_HIST, HSM_NO_ACTION, HSM_NO_ACTION)\
    HSM_TRANSITION(EVT_TypedKeyPound, HSM_NO_GUARD, onInterupt1, STATE_SHALLOW_HIST)\
  HSM_END_STATE(STATE_WORKING)\
  /*=================== STATE: STATE_CLOSE ===============================*/\
  HSM_FINAL_STATE(STATE_CLOSE, STATE_READING)\
  /*=================== STATE: STATE_SHALLOW_HIST ===============================*/\
  HSM_SHALLOW_HISTORY_STATE(STATE_SHALLOW_HIST, STATE_READING, STATE_OPEN)\
  /*=================== STATE: STATE_OPEN ===============================*/\
  HSM_JUNCTION_STATE(STATE_OPEN, STATE_READING)\
    HSM_TRANSITION(HSM_NO_EVENT, HSM_NO_GUARD, proc_open, STATE_PAGEX)\
  HSM_END_STATE(STATE_OPEN)\
  /*=================== STATE: STATE_PAGEX ===============================*/\
  HSM_SIMPLE_STATE(STATE_PAGEX, STATE_READING, HSM_NO_ACTION, HSM_NO_ACTION)\
    HSM_INTERNAL_TRANSITION(EVT_TypedKeyNumber0, HSM_NO_GUARD, proc_reading)\
    HSM_INTERNAL_TRANSITION(EVT_TypedKeyNumber1_9, HSM_NO_GUARD, proc_reading)\
    HSM_INTERNAL_TRANSITION(EVT_TypedKeyOther, HSM_NO_GUARD, proc_reading)\
    HSM_INTERNAL_TRANSITION(EVT_TypedKeySign, HSM_NO_GUARD, proc_reading)\
    HSM_TRANSITION(EVT_TypedKeyEnter, HSM_NO_GUARD, proc_close, STATE_CLOSE)\
  HSM_END_STATE(STATE_PAGEX)\
  /*=================== STATE: STATE_DEEP_HIST ===============================*/\
  HSM_DEEP_HISTORY_STATE(STATE_DEEP_HIST, STATE_WORKING, STATE_RESTART)\
  /*=================== STATE: STATE_RESTART ===============================*/\
  HSM_JUNCTION_STATE(STATE_RESTART, STATE_WORKING)\
    HSM_TRANSITION(HSM_NO_EVENT, HSM_NO_GUARD, prompt_reset, STATE_EMPTY)\
  HSM_END_STATE(STATE_RESTART)\
  /*=================== STATE: STATE_TYPING ===============================*/\
  HSM_COMPOSITE_STATE(STATE_TYPING, STATE_WORKING, HSM_NO_INITIAL_STATE, HSM_NO_HISTORY_STATE, HSM_NO_ACTION, HSM_NO_ACTION)\
    HSM_INTERNAL_TRANSITION(EVT_TypedKeyDot, HSM_NO_GUARD, onError)\
    HSM_INTERNAL_TRANSITION(EVT_TypedKeyOther, HSM_NO_GUARD, onError)\
    HSM_INTERNAL_TRANSITION(EVT_TypedKeySign, HSM_NO_GUARD, onError)\
    HSM_TRANSITION(EVT_TypedKeyEnter, HSM_NO_GUARD, resulted, STATE_RESTART)\
  HSM_END_STATE(STATE_TYPING)\
  /*=================== STATE: STATE_EMPTY ===============================*/\
  HSM_SIMPLE_STATE(STATE_EMPTY, STATE_WORKING, HSM_NO_ACTION, HSM_NO_ACTION)\
    HSM_INTERNAL_TRANSITION(EVT_TypedKeyDot, HSM_NO_GUARD, onError)\
    HSM_INTERNAL_TRANSITION(EVT_TypedKeyEnter, HSM_NO_GUARD, prompt_reset)\
    HSM_INTERNAL_TRANSITION(EVT_TypedKeyOther, HSM_NO_GUARD, onError)\
    HSM_TRANSITION(EVT_TypedKeyNumber0, HSM_NO_GUARD, proc_lead0, STATE_LEAD0)\
    HSM_TRANSITION(EVT_TypedKeyNumber1_9, HSM_NO_GUARD, proc_int, STATE_INTEGER)\
    HSM_TRANSITION(EVT_TypedKeySign, HSM_NO_GUARD, proc_signed, STATE_SIGNED)\
  HSM_END_STATE(STATE_EMPTY)\
  /*=================== STATE: STATE_DECIMAL ===============================*/\
  HSM_SIMPLE_STATE(STATE_DECIMAL, STATE_TYPING, HSM_NO_ACTION, HSM_NO_ACTION)\
    HSM_INTERNAL_TRANSITION(EVT_TypedKeyNumber0, HSM_NO_GUARD, proc_decimal)\
    HSM_INTERNAL_TRANSITION(EVT_TypedKeyNumber1_9, HSM_NO_GUARD, proc_decimal)\
  HSM_END_STATE(STATE_DECIMAL)\
  /*=================== STATE: STATE_INTEGER ===============================*/\
  HSM_SIMPLE_STATE(STATE_INTEGER, STATE_TYPING, HSM_NO_ACTION, HSM_NO_ACTION)\
    HSM_INTERNAL_TRANSITION(EVT_TypedKeyNumber0, HSM_NO_GUARD, proc_int)\
    HSM_INTERNAL_TRANSITION(EVT_TypedKeyNumber1_9, HSM_NO_GUARD, proc_int)\
    HSM_TRANSITION(EVT_TypedKeyDot, HSM_NO_GUARD, proc_dot, STATE_DECIMAL)\
  HSM_END_STATE(STATE_INTEGER)\
  /*=================== STATE: STATE_LEAD0 ===============================*/\
  HSM_SIMPLE_STATE(STATE_LEAD0, STATE_TYPING, HSM_NO_ACTION, HSM_NO_ACTION)\
    HSM_INTERNAL_TRANSITION(EVT_TypedKeyNumber0, HSM_NO_GUARD, onError)\
    HSM_INTERNAL_TRANSITION(EVT_TypedKeyNumber1_9, HSM_NO_GUARD, onError)\
    HSM_TRANSITION(EVT_TypedKeyDot, HSM_NO_GUARD, proc_dot, STATE_DECIMAL)\
  HSM_END_STATE(STATE_LEAD0)\
  /*=================== STATE: STATE_SIGNED ===============================*/\
  HSM_SIMPLE_STATE(STATE_SIGNED, STATE_TYPING, HSM_NO_ACTION, HSM_NO_ACTION)\
    HSM_TRANSITION(EVT_TypedKeyNumber0, HSM_NO_GUARD, proc_lead0, STATE_LEAD0)\
    HSM_TRANSITION(EVT_TypedKeyNumber1_9, HSM_NO_GUARD, proc_int, STATE_INTEGER)\
  HSM_END_STATE(STATE_SIGNED)\
\
HSM_END(XSERVICE_MGR_HSM)

/*
 * The following prototypes were auto-generated by gen_hsm;
 * these are the action functions for StateMachine Default_StateMachine.
 */
void onError(const HSM_StateChart_T& statechart);
void onInterupt1(const HSM_StateChart_T& statechart);
void onInterupt2(const HSM_StateChart_T& statechart);
void proc_close(const HSM_StateChart_T& statechart);
void proc_decimal(const HSM_StateChart_T& statechart);
void proc_dot(const HSM_StateChart_T& statechart);
void proc_int(const HSM_StateChart_T& statechart);
void proc_lead0(const HSM_StateChart_T& statechart);
void proc_open(const HSM_StateChart_T& statechart);
void proc_reading(const HSM_StateChart_T& statechart);
void proc_signed(const HSM_StateChart_T& statechart);
void prompt_none(const HSM_StateChart_T& statechart);
void prompt_reset(const HSM_StateChart_T& statechart);
void resulted(const HSM_StateChart_T& statechart);

/*
 * The following prototypes were auto-generated by gen_hsm;
 * these are the guard functions for StateMachine Default_StateMachine.
 */


//auto generate from state of STATECHART_DEFN
#include "hsm_helper_declare.h"


typedef struct XServiceHsmObject_T {
    std::string working_data;
    std::string extra_data;
} XServiceHsmObject_T;

#undef X
#define X(id) case (id): { return #id; }
#define EVENT2STRS MODEL_EVENT_IDS
std::string getEventName(int event);

#endif //__XSERVICE_HSM_MGR_IMPL_H__


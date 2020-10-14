/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* 
* File name   : hsm_helper_define.h
* Author      : longbin
* Created date: 2020-08-31 11:49:23
* Description : 
*
*******************************************/

#ifndef STATECHART_DEFN
#   error STATECHART_DEFN must be defined prior to including this file.
#endif

#ifndef STATECHART_DEFN1
#   define STATECHART_DEFN1()
#endif
#ifndef STATECHART_DEFN2
#   define STATECHART_DEFN2()
#endif
#ifndef STATECHART_DEFN3
#   define STATECHART_DEFN3()
#endif
#ifndef STATECHART_DEFN4
#   define STATECHART_DEFN4()
#endif
#ifndef STATECHART_DEFN5
#   define STATECHART_DEFN5()
#endif
#ifndef STATECHART_DEFN6
#   define STATECHART_DEFN6()
#endif
#ifndef STATECHART_DEFN7
#   define STATECHART_DEFN7()
#endif
#ifndef STATECHART_DEFN8
#   define STATECHART_DEFN8()
#endif
#ifndef STATECHART_DEFN9
#   define STATECHART_DEFN9()
#endif


#undef HSM_BEGIN
#undef HSM_COMPLETION_TRANSITION
#undef HSM_COMPOSITE_STATE
#undef HSM_DEEP_HISTORY_STATE
#undef HSM_FINAL_STATE
#undef HSM_SHALLOW_HISTORY_STATE
#undef HSM_INITIAL_STATE
#undef HSM_JUNCTION_STATE
#undef HSM_SIMPLE_STATE
#undef HSM_TRANSITION
#undef HSM_INTERNAL_TRANSITION
#undef HSM_END_STATE
#undef HSM_END

#define HSM_BEGIN(statechart_name) typedef enum statechart_name##_States_Tag {

#define HSM_INITIAL_STATE(state, parent, action, next_state)              state,
#define HSM_COMPOSITE_STATE(state, parent, initial, history, entry, exit) state,
#define HSM_JUNCTION_STATE(state, parent)                                 state,
#define HSM_DEEP_HISTORY_STATE(state, parent, default_state)              state,
#define HSM_SHALLOW_HISTORY_STATE(state, parent, default_state)           state,
#define HSM_SIMPLE_STATE(state, parent, entry, exit)                      state,
#define HSM_FINAL_STATE(state, parent)                                    state,
#define HSM_END_STATE(state)

#define HSM_COMPLETION_TRANSITION(action, next_state)
#define HSM_TRANSITION(event, guard, action, next_state)
#define HSM_INTERNAL_TRANSITION(event, guard, action)

#define HSM_END(statechart_name) } statechart_name##_States_T;

/*##############*/
STATECHART_DEFN()  /* declare/define the state enumeration type */
STATECHART_DEFN1()
STATECHART_DEFN2()
STATECHART_DEFN3()
STATECHART_DEFN4()
STATECHART_DEFN5()
STATECHART_DEFN6()
STATECHART_DEFN7()
STATECHART_DEFN8()
STATECHART_DEFN9()
/*##############*/


#undef HSM_BEGIN
#undef HSM_COMPLETION_TRANSITION
#undef HSM_COMPOSITE_STATE
#undef HSM_DEEP_HISTORY_STATE
#undef HSM_FINAL_STATE
#undef HSM_SHALLOW_HISTORY_STATE
#undef HSM_INITIAL_STATE
#undef HSM_JUNCTION_STATE
#undef HSM_SIMPLE_STATE
#undef HSM_TRANSITION
#undef HSM_INTERNAL_TRANSITION
#undef HSM_END_STATE
#undef HSM_END

#define HSM_BEGIN(statechart_name)\
HSM_State_Definition_T get_##statechart_name##_Defn() {\
    HSM_State_Definition_T statechart_name##_Defn;\
    std::string statechartName = #statechart_name;\
    HSM_StateList_T stateList;\
    HSM_State_T *pstateObj = NULL;

#define HSM_INITIAL_STATE(state, parent, action, next_state)\
    HSM_Initial_State stateObj_##state{state, #state, parent, action, #action, next_state};\
    pstateObj = &stateObj_##state;\
    stateList.push_back(*pstateObj);

#define HSM_COMPOSITE_STATE(state, parent, initial, history, entry, exit)\
    HSM_Composite_State stateObj_##state{state, #state, parent, initial, history, entry, exit, #entry, #exit};\
    pstateObj = &stateObj_##state;

#define HSM_JUNCTION_STATE(state, parent)\
    HSM_Juncton_State stateObj_##state{state, #state, parent};\
    pstateObj = &stateObj_##state;

#define HSM_DEEP_HISTORY_STATE(state, parent, default_state)\
    HSM_Deep_History_State stateObj_##state{state, #state, parent, default_state};\
    pstateObj = &stateObj_##state;\
    stateList.push_back(*pstateObj);

#define HSM_SHALLOW_HISTORY_STATE(state, parent, default_state)\
    HSM_Shallow_History_State stateObj_##state{state, #state, parent, default_state};\
    pstateObj = &stateObj_##state;\
    stateList.push_back(*pstateObj);

#define HSM_SIMPLE_STATE(state, parent, entry, exit)\
    HSM_Simple_State stateObj_##state{state, #state, parent, entry, exit, #entry, #exit};\
    pstateObj = &stateObj_##state;

#define HSM_FINAL_STATE(state, parent)\
    HSM_Final_State stateObj_##state{state, #state, parent};\
    pstateObj = &stateObj_##state;\
    stateList.push_back(*pstateObj);

#define HSM_END_STATE(state)\
    stateList.push_back(*pstateObj);

#define HSM_COMPLETION_TRANSITION(action, next_state)\
    if (NULL != pstateObj) (void)pstateObj->addCompleteTrans(action, next_state, #action);

#define HSM_TRANSITION(event, guard, action, next_state)\
    if (NULL != pstateObj) (void)pstateObj->addExternalTrans(event, guard, action, next_state, #guard, #action);

#define HSM_INTERNAL_TRANSITION(event, guard, action)\
    if (NULL != pstateObj) (void)pstateObj->addInternalTrans(event, guard, action, #guard, #action);

#define HSM_END(statechart_name)\
    statechart_name##_Defn.statechartName = statechartName;\
    statechart_name##_Defn.stateList = stateList;\
    return statechart_name##_Defn;}

/*##############*/
STATECHART_DEFN()  /* define the state */
STATECHART_DEFN1()
STATECHART_DEFN2()
STATECHART_DEFN3()
STATECHART_DEFN4()
STATECHART_DEFN5()
STATECHART_DEFN6()
STATECHART_DEFN7()
STATECHART_DEFN8()
STATECHART_DEFN9()
/*##############*/


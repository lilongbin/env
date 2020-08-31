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

#define HSM_BEGIN(statechart_name)\
HSM_State_Definition_T get_##statechart_name##_Defn() {\
    HSM_State_Definition_T statechart_name##_Defn;\
    std::string statechartName = #statechart_name;\
    std::vector<HSM_State_T> stateList;\
    HSM_State_T state_tmp;\
    std::vector<HSM_Transition_T> state_Trans_tmp;

#define HSM_INITIAL_STATE(state_name,parent,action,next_state)\
    state_tmp = HSM_State_T{HSM_SKIND_INITIAL, state_name, #state_name, parent, HSM_NO_INITIAL_STATE, HSM_NO_HISTORY_STATE, HSM_NO_ACTION, HSM_NO_ACTION, "no entry", "no exit"};\
    state_tmp.addExternalTrans({HSM_NO_EVENT,HSM_NO_GUARD,action,next_state,HSM_NO_HISTORY_STATE, "no guard", #action});\
    stateList.push_back(state_tmp);

#define HSM_COMPOSITE_STATE(state_name,parent,initial,history,entry,exit)\
    state_tmp = HSM_State_T{HSM_SKIND_COMPOSITE, state_name, #state_name, parent, initial, history, entry, exit, #entry, #exit};

#define HSM_JUNCTION_STATE(state_name,parent)\
    state_tmp = HSM_State_T{HSM_SKIND_JUNCTION, state_name, #state_name, parent, HSM_NO_INITIAL_STATE, HSM_NO_HISTORY_STATE, HSM_NO_ACTION, HSM_NO_ACTION, "no entry", "no exit"};

#define HSM_DEEP_HISTORY_STATE(state_name,parent,default_state)\
    state_tmp = HSM_State_T{HSM_SKIND_DEEP_HISTORY, state_name, #state_name, parent, HSM_NO_INITIAL_STATE, HSM_NO_HISTORY_STATE, HSM_NO_ACTION, HSM_NO_ACTION, "no entry", "no exit"};\
    state_tmp.addExternalTrans({HSM_NO_EVENT,HSM_NO_GUARD,HSM_NO_ACTION,default_state,default_state, "no guard", "no action"});\
    stateList.push_back(state_tmp);

#define HSM_SHALLOW_HISTORY_STATE(state_name,parent,default_state)\
    state_tmp = HSM_State_T{HSM_SKIND_SHALLOW_HISTORY, state_name, #state_name, parent, HSM_NO_INITIAL_STATE, HSM_NO_HISTORY_STATE, HSM_NO_ACTION, HSM_NO_ACTION, "no entry", "no exit"};\
    state_tmp.addExternalTrans({HSM_NO_EVENT,HSM_NO_GUARD,HSM_NO_ACTION,default_state,default_state, "no guard", "no action"});\
    stateList.push_back(state_tmp);

#define HSM_SIMPLE_STATE(state_name,parent,entry,exit)\
    state_tmp = HSM_State_T{HSM_SKIND_SIMPLE, state_name, #state_name, parent, HSM_NO_INITIAL_STATE, HSM_NO_HISTORY_STATE, entry, exit, #entry, #exit};

#define HSM_FINAL_STATE(state_name,parent)\
    state_tmp = HSM_State_T{HSM_SKIND_FINAL, state_name, #state_name, parent, HSM_NO_INITIAL_STATE, HSM_NO_HISTORY_STATE, HSM_NO_ACTION, HSM_NO_ACTION, "no entry", "no exit"};\
    stateList.push_back(state_tmp);

#define HSM_END_STATE(state_name)\
    stateList.push_back(state_tmp);

#define HSM_COMPLETION_TRANSITION(action, next_state)\
    state_tmp.addCompleteTrans({HSM_COMPLETION_EVENT,HSM_NO_GUARD,action,next_state,HSM_NO_HISTORY_STATE, "no guard", #action});

#define HSM_TRANSITION(event,guard,action,next_state)\
    state_tmp.addExternalTrans({event, guard, action, next_state, HSM_NO_HISTORY_STATE, #guard, #action});

#define HSM_INTERNAL_TRANSITION(event,guard,action)\
    state_tmp.addInternalTrans({event,guard,action, HSM_SAME_STATE, HSM_NO_HISTORY_STATE, #guard, #action});

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


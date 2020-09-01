/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* 
* File name   : hsm_helper_declare.h
* Author      : longbin
* Created date: 2020-08-31 11:44:34
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
        HSM_State_Definition_T get_##statechart_name##_Defn();

#define HSM_INITIAL_STATE(state_name,parent,action,next_state)
#define HSM_COMPOSITE_STATE(state_name,parent,initial,history,entry,exit)
#define HSM_JUNCTION_STATE(state_name,parent)
#define HSM_DEEP_HISTORY_STATE(state_name,parent,default_state)
#define HSM_SHALLOW_HISTORY_STATE(state_name,parent,default_state)
#define HSM_SIMPLE_STATE(state_name,parent,entry,exit)
#define HSM_FINAL_STATE(state_name,parent)
#define HSM_END_STATE(state_name)

#define HSM_COMPLETION_TRANSITION(action, next_state)
#define HSM_TRANSITION(event,guard,action,next_state)
#define HSM_INTERNAL_TRANSITION(event,guard,action)

#define HSM_END(statechart_name);

/*##############*/
STATECHART_DEFN()  /* Declare the state variable */
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


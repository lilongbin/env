/*===========================================================================*/
/**
 * @file hsm_define_statechart.h
 *
 * Contains the C preprocessor sequence that translates a STATECHART_DEFN() 
 * MACRO into definitions for the C objects that represent a statechart:
 * the transition tables, the state table, and the state definition structure.
 *
 * %full_filespec:hsm_define_statechart.h~kok_basa#18:incl:kok_aud#1 %
 * @version %version:kok_basa#18 %
 * @author  %derived_by:dzq92s %
 * @date    %date_modified:Wed May 30 08:42:13 2012 %
 *
 *------------------------------------------------------------------------------
 *
 * Copyright 2007 Technologies, Inc., All Rights Reserved.
 * Confidential
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 *
 *   This file contains the X MACRO definitions that translate the 
 *   STATECHART_DEFN() MACRO into definitions of the objects for the
 *   statechart. There is a corresponding hsm_declare_statechart.h that declares
 *   the publicly visible types and objects of the statechart for use by other
 *   modules.
 *
 *   Note that unlike most header files, this one DOES NOT have ifndef/endif
 *   directives to prevent multiple inclusion. This allows multiple statechart
 *   definitions to be created in a single translation unit. This file defines a
 *   sequence of preprocessor statements (including undef) that need to be
 *   performed each time this file is included, even if it has been previously
 *   included in the same translation unit.
 *
 * @section ABBR ABBREVIATIONS:
 *   - hsm = Hierarchical State Machine
 *
 * @section TRACE TRACEABILITY INFO:
 *   - Design Document(s):
 *     - SDD_10016705_State_Machine_Engine.doc
 *
 *   - Requirements Document(s):
 *     - SRS_10016705_State_Machine.doc
 *
 *   - Applicable Standards (in order of precedence: highest first):
 *     - <a href="http://kogespw1.delcoelect.com:8508/passthru/consumer?name=SW%20REF%20264.15D&doctype=K8DBSDoc">
 *       SW REF 264.15D "C Coding Standards" [12-Mar-2006]</a>
 *
 * @section DFS DEVIATIONS FROM STANDARDS:
 *   - None
 *
 * @ingroup state_engine
 * @{
 */
/*==========================================================================*/

#ifndef STATECHART_DEFN
#   error STATECHART_DEFN must be defined prior to including this file.
#endif
/*
 * For very large statecharts, the single STATECHART_DEFN MACRO is not
 * sufficient because MACROs expand to a single line of source code and
 * compilers have a limit on source line length. If this situation is
 * encountered, then these subsequent MACROs can be used to spread the
 * definition over multiple lines. If these are used, they must be
 * "undefined" in the same place the STATECHART_DEFN MACRO would have
 * to be undefined (when expanding multiple statechart definitionss within
 * a single compilation unit).
 */
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


/* *INDENT-OFF* */

/*#############################################################################
 * <statechart_name>_State_Names definition
 *
 * The following logic defines a table containing strings for the state
 * names.
 *
 *############################################################################*/
#ifdef HSM_GENERATE_STATE_NAMES
#   undef HSM_BEGIN
#   undef HSM_COMPLETION_TRANSITION
#   undef HSM_COMPOSITE_STATE
#   undef HSM_DEEP_HISTORY_STATE
#   undef HSM_FINAL_STATE
#   undef HSM_SHALLOW_HISTORY_STATE
#   undef HSM_INITIAL_STATE
#   undef HSM_JUNCTION_STATE
#   undef HSM_SIMPLE_STATE
#   undef HSM_TRANSITION
#   undef HSM_INTERNAL_TRANSITION
#   undef HSM_END_STATE
#   undef HSM_END

#   define HSM_BEGIN(statechart_name) char const * const statechart_name##_State_Names[] = {

#   define HSM_COMPLETION_TRANSITION(action, next_state)
#   define HSM_COMPOSITE_STATE(state_name,parent,initial,history,entry,exit) #state_name,
#   define HSM_DEEP_HISTORY_STATE(state_name,parent,default_state)    #state_name,
#   define HSM_FINAL_STATE(state_name,parent)                         #state_name,
#   define HSM_SHALLOW_HISTORY_STATE(state_name,parent,default_state) #state_name,
#   define HSM_INITIAL_STATE(state_name,parent,action,next_state)     #state_name,
#   define HSM_JUNCTION_STATE(state_name,parent)                      #state_name,
#   define HSM_SIMPLE_STATE(state_name,parent,entry,exit)             #state_name,

#   define HSM_TRANSITION(event,guard,action,next_state)
#   define HSM_INTERNAL_TRANSITION(event,guard,action)
#   define HSM_END_STATE(state_name)

#   define HSM_END(statechart_name) };

/*==============*/
STATECHART_DEFN()   /* Define the state name table */
STATECHART_DEFN1()
STATECHART_DEFN2()
STATECHART_DEFN3()
STATECHART_DEFN4()
STATECHART_DEFN5()
STATECHART_DEFN6()
STATECHART_DEFN7()
STATECHART_DEFN8()
STATECHART_DEFN9()
/*==============*/
#endif

/*=============================================================================
 * Statechart transition tables.
 *
 * The following logic defines the transition tables for the statechart. Each
 * state has an associated TRANSITION_T[] table defining any transitions
 * internal to or from the state. The name of the transition table for
 * a state is derived from the "state_name" argument, which is actually the
 * enumeration tag associated with the state. The name formed is
 * "<state_name>_Transitions". This table is later referenced in the STATE_T for
 * the state within the state table. Note that a state must have at least one
 * transition defined, since otherwise an attempt to build an array with zero
 * entries would result in a compilation error. A special transition that will
 * not ever be taken has been defined for this and can be used:
 * HSM_NULL_TRANSITION().
 *
 *============================================================================*/

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

#define HSM_BEGIN(statechart_name)

#ifdef HSM_GENERATE_ACTION_NAMES
#   define HSM_COMPLETION_TRANSITION(action, next_state)\
       {HSM_COMPLETION_EVENT, HSM_NO_GUARD, action, next_state, NULL, "", #action},
#else
#   define HSM_COMPLETION_TRANSITION(action, next_state)\
       {HSM_COMPLETION_EVENT, HSM_NO_GUARD, action, next_state, NULL, "", ""},
#endif /* HSM_GENERATE_ACTION_NAMES */

#define HSM_COMPOSITE_STATE(state_name,parent,initial,history,entry,exit)\
  const HSM_Transition_T state_name##_Transitions[]={

#define HSM_DEEP_HISTORY_STATE(state_name,parent,default_state)\
  HSM_State_Id_T state_name##_History=default_state;\
  const HSM_Transition_T state_name##_Transitions[]=\
     {{HSM_NO_EVENT,HSM_NO_GUARD,HSM_NO_ACTION,default_state,&state_name##_History, "", ""}};

#define HSM_FINAL_STATE(state_name,parent)

#define HSM_SHALLOW_HISTORY_STATE(state_name,parent,default_state)\
  HSM_State_Id_T state_name##_History=default_state;\
  const HSM_Transition_T state_name##_Transitions[]=\
     {{HSM_NO_EVENT,HSM_NO_GUARD,HSM_NO_ACTION,default_state,&state_name##_History, "", ""}};

#ifdef HSM_GENERATE_ACTION_NAMES
#   define HSM_INITIAL_STATE(state_name,parent,action,next_state)\
     const HSM_Transition_T state_name##_Transitions[]=\
                        {{HSM_NO_EVENT,HSM_NO_GUARD,action,next_state,NULL, "", #action}};
#else
#   define HSM_INITIAL_STATE(state_name,parent,action,next_state)\
     const HSM_Transition_T state_name##_Transitions[]=\
                        {{HSM_NO_EVENT,HSM_NO_GUARD,action,next_state,NULL, "", ""}};
#endif /* HSM_GENERATE_ACTION_NAMES */

#define HSM_JUNCTION_STATE(state_name,parent)\
  const HSM_Transition_T state_name##_Transitions[]={

#define HSM_SIMPLE_STATE(state_name,parent,entry,exit)\
  const HSM_Transition_T state_name##_Transitions[]={


#ifdef HSM_GENERATE_GUARD_NAMES

#   ifdef HSM_GENERATE_ACTION_NAMES
#      define HSM_TRANSITION(event,guard,action,next_state)\
          {event, guard, action, next_state, NULL, #guard, #action},

#      define HSM_INTERNAL_TRANSITION(event,guard,action)\
          {event, guard, action, HSM_SAME_STATE, NULL, #guard, #action},
#   else
#      define HSM_TRANSITION(event,guard,action,next_state)\
          {event, guard, action, next_state, NULL, #guard, ""},

#      define HSM_INTERNAL_TRANSITION(event,guard,action)\
          {event, guard, action, HSM_SAME_STATE, NULL, #guard, ""},
#endif /* HSM_GENERATE_ACTION_NAMES */

#else

#   ifdef HSM_GENERATE_ACTION_NAMES
#      define HSM_TRANSITION(event,guard,action,next_state)\
          {event, guard, action, next_state, NULL, "", #action},

#      define HSM_INTERNAL_TRANSITION(event,guard,action)\
          {event, guard, action, HSM_SAME_STATE, NULL, "", #action},
#   else
#      define HSM_TRANSITION(event,guard,action,next_state)\
          {event, guard, action, next_state, NULL, "", ""},

#      define HSM_INTERNAL_TRANSITION(event,guard,action)\
          {event, guard, action, HSM_SAME_STATE, NULL, "", ""},
#endif /* HSM_GENERATE_ACTION_NAMES */

#endif /* HSM_GENERATE_GUARD_NAMES */

#define HSM_END_STATE(state_name) };

#define HSM_END(statechart_name)

/*==============*/
STATECHART_DEFN()   /* Define the transition tables */
STATECHART_DEFN1()
STATECHART_DEFN2()
STATECHART_DEFN3()
STATECHART_DEFN4()
STATECHART_DEFN5()
STATECHART_DEFN6()
STATECHART_DEFN7()
STATECHART_DEFN8()
STATECHART_DEFN9()
/*==============*/

/*=============================================================================
 * <statechart_name>_Table[], <statechart_name>_Defn
 *
 * The following logic defines (fills in) the state table. The name of the
 * variable is formed using the "statechart_name" argument to HSM_BEGIN and
 * HSM_END MACROs in STATECHART_DEFN: "<statechart_name>_Table".
 *
 * In addition, a const HSM_State_Defn_T structure is created containing
 * a pointer to the state table and the number of states in the table.
 * The name of this variable is "<statechart_name>_Defn".
 *
 *============================================================================*/

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
HSM_State_T const statechart_name##_Table[statechart_name##_NUM_STATES]={

#define HSM_COMPLETION_TRANSITION(action, next_state)

#ifdef HSM_GENERATE_ACTION_NAMES
#   define HSM_COMPOSITE_STATE(state_name,parent,initial,history,entry,exit)\
  {HSM_COMPOSITE_ID, state_name##_Transitions,\
   Num_Elems(state_name##_Transitions), entry, exit, parent, initial, history,\
   #entry, #exit},
#else
#   define HSM_COMPOSITE_STATE(state_name,parent,initial,history,entry,exit)\
  {HSM_COMPOSITE_ID, state_name##_Transitions,\
   Num_Elems(state_name##_Transitions), entry, exit, parent, initial, history, "", ""},
#endif /* HSM_GENERATE_ACTION_NAMES */

#define HSM_DEEP_HISTORY_STATE(state_name,parent,default_state)\
  {HSM_DEEP_HISTORY_ID, state_name##_Transitions,\
   Num_Elems(state_name##_Transitions),\
   NULL, NULL, parent, HSM_NO_INITIAL_STATE, HSM_NO_HISTORY_STATE, "", ""},

#define HSM_FINAL_STATE(state_name,parent)\
  {HSM_FINAL_ID, NULL, 0, NULL, NULL, parent,\
   HSM_NO_INITIAL_STATE, HSM_NO_HISTORY_STATE, "", ""},

#define HSM_SHALLOW_HISTORY_STATE(state_name,parent,default_state)\
  {HSM_SHALLOW_HISTORY_ID,state_name##_Transitions,\
   Num_Elems(state_name##_Transitions),\
   NULL, NULL, parent, HSM_NO_INITIAL_STATE, HSM_NO_HISTORY_STATE, "", ""},

#define HSM_INITIAL_STATE(state_name,parent,action,next_state)\
  {HSM_INITIAL_ID,state_name##_Transitions,Num_Elems(state_name##_Transitions),\
   NULL, NULL, parent, HSM_NO_INITIAL_STATE, HSM_NO_HISTORY_STATE, "", ""},

#define HSM_JUNCTION_STATE(state_name,parent)\
  {HSM_JUNCTION_ID,state_name##_Transitions,Num_Elems(state_name##_Transitions),\
   NULL, NULL, parent, HSM_NO_INITIAL_STATE, HSM_NO_HISTORY_STATE, "", ""},

#ifdef HSM_GENERATE_ACTION_NAMES
#   define HSM_SIMPLE_STATE(state_name,parent,entry,exit)\
  {HSM_SIMPLE_ID, state_name##_Transitions,Num_Elems(state_name##_Transitions),\
   entry, exit, parent, HSM_NO_INITIAL_STATE, HSM_NO_HISTORY_STATE, #entry, #exit},
#else
#   define HSM_SIMPLE_STATE(state_name,parent,entry,exit)\
  {HSM_SIMPLE_ID, state_name##_Transitions,Num_Elems(state_name##_Transitions),\
   entry, exit, parent, HSM_NO_INITIAL_STATE, HSM_NO_HISTORY_STATE, "", ""},
#endif /* HSM_GENERATE_ACTION_NAMES */

#define HSM_TRANSITION(event, guard, action, next_state)
#define HSM_INTERNAL_TRANSITION(event,guard,action)
#define HSM_END_STATE(state_name)


#ifdef HSM_GENERATE_STATE_NAMES

#   define HSM_END(statechart_name)\
};\
HSM_State_Defn_T const statechart_name##_Defn =\
{statechart_name##_Table, statechart_name##_HISTORY_COUNT, statechart_name##_NUM_STATES, #statechart_name, statechart_name##_State_Names};

#else

#   define HSM_END(statechart_name)\
};\
HSM_State_Defn_T const statechart_name##_Defn =\
{statechart_name##_Table, statechart_name##_HISTORY_COUNT, statechart_name##_NUM_STATES, #statechart_name, NULL};

#endif /* HSM_GENERATE_STATE_NAMES */

/*==============*/
STATECHART_DEFN()  /* Define <statechart_name>_Table */
STATECHART_DEFN1()
STATECHART_DEFN2()
STATECHART_DEFN3()
STATECHART_DEFN4()
STATECHART_DEFN5()
STATECHART_DEFN6()
STATECHART_DEFN7()
STATECHART_DEFN8()
STATECHART_DEFN9()
/*==============*/

/* *INDENT-ON* */

/*===========================================================================*/
/*!
 * @file hsm_define_statechart.h
 *
 * @section RH REVISION HISTORY (top to bottom: last revision to first revision)
 *
 * - 30-May-2012 Kirk Bailey
 *   - Fixed generation of entry/exit action function names.
 *
 * - 30-Sep-2011 Kirk Bailey
 *   - Added option to generate action names for trace info.
 *
 * - 02-Sep-2010 Kirk Bailey
 *   - Added option to generate guard names for trace info.
 *
 * - 20-mar-2009 kirk bailey
 *   -Fixed use of HSM_GENERATE_STATE_NAMES so that the state name table is not
 *    created unless the symbol is defined (task kok_aud#47605).
 *
 * - 07-jul-2008 kirk bailey
 *   -Added support for state names.
 *
 * - 13-mar-2008 kirk bailey
 *   -Added extra STATECHART_DEFNx() MACROs to support very big statecharts.
 *
 * - 25-feb-2008 kirk bailey
 *   -Added creation of "<statechart_name>_Defn".
 *
 * - 15-nov-2007 kirk bailey
 *   - Added final states and completion transitions.
 *   - Converted to new Doyxgen format.
 *
 * - 09-aug-2007 kirk bailey
 *    -Changed type usage to be MISRA compliant.
 *
 * - 09-jul-2007 kirk bailey
 *    -Updated comments.
 *
 * - 31-mar-2007 kirk bailey
 *    -Added deep history.
 *
 * - 28-mar-2007 kirk bailey
 *    -More name changes.
 *
 * - 26-mar-2007 kirk bailey
 *    -Added SIMPLE and COMPOSITE states. Renamed some MACROs.
 *
 * - 24-mar-2007 kirk bailey
 *    -Created initial file.
 */
/*===========================================================================*/
/** @} doxygen end group */

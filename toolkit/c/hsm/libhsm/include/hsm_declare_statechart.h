/*===========================================================================*/
/**
 * @file hsm_declare_statechart.h
 *
 * Contains the C preprocessor sequence that translates a STATECHART_DEFN() 
 * MACRO into extern declarations for the publicly visible parts of the
 * statechart: the enum type that lists its states, its state table, and
 * its state definition structure.
 *
 * %full_filespec:hsm_declare_statechart.h~11:incl:kok_aud#1 %
 * @version %version:11 %
 * @author  %derived_by:dzq92s %
 * @date    %date_modified:Wed Jun 10 12:29:30 2009 %
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
 *   STATECHART_DEFN() MACRO into declarations of its publicly accessible parts.
 *   There is a corresponding hsm_define_statechart.h that defines (provides
 *   values for) the statechart's objects.
 *
 *   Note that unlike most header files, this one DOES NOT have ifndef/endif
 *   directives to prevent multiple inclusion. This allows multiple statechart
 *   declarations to be accessed in a single translation unit. This file defines
 *   a sequence of preprocessor statements (including undef) that need to be
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
 * <statechart_name>_States_T declaration
 *
 * The following logic declares the enumeration type containing
 * the states of the statechart. The name of the enumeration is
 * formed using the "statechart_name" argument to HSM_BEGIN in STATECHART_DEFN:
 * "<statechart_name>_States_T".
 *
 *############################################################################*/

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

#define HSM_COMPLETION_TRANSITION(action, next_state)
#define HSM_COMPOSITE_STATE(state_name,parent,initial,history,entry,exit) state_name,
#define HSM_DEEP_HISTORY_STATE(state_name,parent,default_state)    state_name,
#define HSM_FINAL_STATE(state_name,parent)                         state_name,
#define HSM_SHALLOW_HISTORY_STATE(state_name,parent,default_state) state_name,
#define HSM_INITIAL_STATE(state_name,parent,action,next_state)     state_name,
#define HSM_JUNCTION_STATE(state_name,parent)                      state_name,
#define HSM_SIMPLE_STATE(state_name,parent,entry,exit)             state_name,

#define HSM_TRANSITION(event,guard,action,next_state)
#define HSM_INTERNAL_TRANSITION(event,guard,action)
#define HSM_END_STATE(state_name)

#define HSM_END(statechart_name) statechart_name##_NUM_STATES} statechart_name##_States_T;

/*##############*/
STATECHART_DEFN()  /* Declare the state enumeration type */
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

/*#############################################################################
 * <statechart_name>_Table[]
 *
 * The following logic declares the table of states for the statechart to be
 * an extern const. The table will be defined (values filled it) in a separate
 * C file.  The name of the table is formed using the "statechart_name" 
 * argument to HSM_BEGIN in STATECHART_DEFN:  "<statechart_name>_Table".
 *
 * In addition, an extern const HSM_State_Defn_T structure is declared which
 * will contain a pointer to the state table and the number of states in the
 * table. The name of this variable is <statechart_name>_Defn.
 *
 *############################################################################*/

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

#ifdef HSM_GENERATE_STATE_NAMES

/** Designates the beginning of a statechart definition. */
#   define HSM_BEGIN(statechart_name)\
extern HSM_State_T const statechart_name##_Table[statechart_name##_NUM_STATES];\
extern HSM_State_Defn_T const statechart_name##_Defn;\
extern char const * const statechart_name##_State_Names[];

#else

/** Designates the beginning of a statechart definition. */
#   define HSM_BEGIN(statechart_name)\
extern HSM_State_T const statechart_name##_Table[statechart_name##_NUM_STATES];\
extern HSM_State_Defn_T const statechart_name##_Defn;

#endif /* HSM_GENERATE_STATE_NAMES */

#define HSM_COMPLETION_TRANSITION(action, next_state)
#define HSM_COMPOSITE_STATE(state_name,parent,initial,history,entry,exit)
#define HSM_DEEP_HISTORY_STATE(state_name,parent,default_state)
#define HSM_FINAL_STATE(state_name,parent)
#define HSM_SHALLOW_HISTORY_STATE(state_name,parent,default_state)
#define HSM_INITIAL_STATE(state_name,parent,action,next_state)
#define HSM_JUNCTION_STATE(state_name,parent)
#define HSM_SIMPLE_STATE(state_name,parent,entry,exit)
#define HSM_TRANSITION(event,guard,action,next_state)
#define HSM_INTERNAL_TRANSITION(event,guard,action)
#define HSM_END_STATE(state_name)
#define HSM_END(statechart_name)

/*##############*/
STATECHART_DEFN()  /* Declare <statechart_name>_Table */
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

/*#############################################################################
 * <statechart_name>_HISTORY_COUNT, <statechart_name>_PERSISTENT_SIZE
 *
 * The following logic declares enumeration tags named 
 * <statechart_name>_HISTORY_COUNT and <statechart_name>_PERSISTENT_SIZE. The
 * former is the number of the history states in the statechart; the latter
 * is the size of the data area required to save the statechart's context in
 * persistent storage (which depends on the number of history states).
 *
 * In addition, the type <statechart_name>_Persistent_Data_T is declared and
 * is a structure that can hold the persistent data for the statechart. This
 * varies from statechart to statechart since the size depends upon the
 * number of history states.
 *
 *############################################################################*/

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
typedef enum statechart_name##_Persistent_Size_Tag { statechart_name##_HISTORY_COUNT = (0

#define HSM_COMPLETION_TRANSITION(action, next_state)
#define HSM_COMPOSITE_STATE(state_name,parent,initial,history,entry,exit) + 0
#define HSM_DEEP_HISTORY_STATE(state_name,parent,default_state)           + 1
#define HSM_FINAL_STATE(state_name,parent)                                + 0
#define HSM_SHALLOW_HISTORY_STATE(state_name,parent,default_state)        + 1
#define HSM_INITIAL_STATE(state_name,parent,action,next_state)            + 0
#define HSM_JUNCTION_STATE(state_name,parent)                             + 0
#define HSM_SIMPLE_STATE(state_name,parent,entry,exit)                    + 0

#define HSM_TRANSITION(event,guard,action,next_state)
#define HSM_INTERNAL_TRANSITION(event,guard,action)
#define HSM_END_STATE(state_name)

#define HSM_END(statechart_name)\
), statechart_name##_PERSISTENT_SIZE = HSM_PERSISTENT_SIZE(statechart_name##_HISTORY_COUNT)\
} statechart_name##_Persistent_Size_T;\
typedef struct statechart_name##_Persistent_Data_Tag {\
   uint8_t data[statechart_name##_PERSISTENT_SIZE];\
} statechart_name##_Persistent_Data_T;

/*##############*/
STATECHART_DEFN()  /* Declare the state enumeration type */
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

/* *INDENT-ON* */

/*===========================================================================*/

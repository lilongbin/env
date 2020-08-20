#ifndef HSM_ENGINE_H
#   define HSM_ENGINE_H
/*===========================================================================*/
/**
 * @file hsm_engine.h
 *
 * Defines the public API to the hierarchical state machine engine.
 *
 * %full_filespec:hsm_engine.h~kok_basa#28:incl:kok_aud#1 %
 * @version %version:kok_basa#28 %
 * @author  %derived_by:dzq92s %
 * @date    %date_modified:Wed May 30 15:50:12 2012 %
 *
 *------------------------------------------------------------------------------
 *
 * Copyright 2011, 2012 Technologies, Inc., All Rights Reserved.
 * Confidential
 *
 *------------------------------------------------------------------------------
 *
 *@section DESC DESCRIPTION:
 *
 * This module implements a general purpose engine for executing UML
 * statecharts. At initialization, it accepts a const table that defines
 * the states and transitions within the statechart. It uses this information
 * to initialize the statechart. After this is completed, the engine can
 * accept events for the statechart and execute the appropriate actions.
 *
 * Currently, this module supports the following features of the UML Statechart:
 *  - Composite (OR) states
 *  - Entry actions (implemented as a function)
 *  - Exit actions (implemented as a function)
 *  - Transition actions (implemented as a function)
 *  - Guard conditions (implemented as a Boolean function)
 *  - Internal Transitions (implemented as a function)
 *  - Initial pseudostates (handles default transition for composite states)
 *  - Shallow history pseudostates
 *  - Deep history pseudostates
 *  - Junction pseudostates
 *  - Final states (within composites, not at top level of statechart)
 *  - Completion Transitions (from composite states containing a final state)
 *
 * Currently, this module does NOT support the following features of the
 * UML Statechart:
 *  - Activities
 *  - Concurrent (AND) states
 *  - Choice, fork, join pseudostates
 *
 * A statechart is defined by declaring the MACRO STATECHART_DEFN(). This
 * MACRO is a sequence of other HSM_*() MACRO invocations that declare the
 * states and transitions within a statechart. This MACRO is expanded
 * using the X MACRO technique in two contexts: 1) declaration, and 2)
 * definition.
 *
 * The declaration context for expanding STATECHART_DEFN() is for declaring
 * the publicly accessible parts of the statechart. It is accomplished by
 * including the header file hsm_declare_statechart.h after STATECHART_DEFN()
 * has been defined. The publicly accessible declarations it creates are:
 * 
 *  - The enum typedef that enumerates the statechart's states.
 *  - The const HSM_State_T array containing the statechart definition.
 *  - The const HSM_State_Defn_T structure containing a pointer to the state
 *    array and the number of states it contains.
 *
 * This expansion can occur in a header file so as to make these objects
 * available to multiple C files. The HSM_State_T array and its size are
 * needed to pass to the HSM_Start() function.
 *
 * A typical header file for declaring statechart objects would have
 * the following layout:
 *
 * @code
 * #   include "hsm_engine.h"
 * #   include ".. header file that defines events for statechart ..."
 *
 * #   define STATECHART_DEFN() \
 *  HSM_BEGIN(Statechart_Name) \
 *  ... HSM_*() macros to define contents of statechart ...
 *  HSM_END(Statechart_Name)
 *
 * #   include "hsm_declare_statechart.h" <- declares the statechart objects
 * @endcode
 *
 * The definition context in which STATECHART_DEFN() is expanded is in the
 * .c file in which the statechart tables are actually to be generated
 * (the contents of the tables defined) and the state definition structure
 * instantiated. It is accomplished by defining STATECHART_DEFN() (by including
 * the header file in which the aforementioned declarations were made) and then
 * including the header file hsm_define_statechart.h. This header file contains
 * the X MACRO definitions that will translate STATECHART_DEFN() into the table
 * definitions for the statechart and the state definition structure.
 *
 * A typical .c file for defining the statechart objects would have
 * the following layout:
 *
 * @code
 * #include ".. the header file from above with STATECHART_DEFN() ..."
 *
 * ... Either include header files that declare, or provide function prototypes
 *     for the action functions and guard functions ...
 *
 * #include "hsm_define_statechart.h" <- creates the statechart objects
 * @endcode
 *
 *
 * @section ABBR ABBREVIATIONS:
 *   - HSM = hierarchical state machine
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
 * @defgroup hsm_engine Hierarchical State Machine Execution Engine
 * @{
 */
/*==========================================================================*/

/*===========================================================================*
 * Header Files
 *===========================================================================*/
//#   include "dbg_trace.h"
//#   include "hsm_engine_acfg.h"
//#   include "reuse.h"
#include "hsm_comm.h"

#   ifdef __cplusplus
extern "C"
{ /* ! Inclusion of header files should NOT be inside the extern "C" block */
#   endif /* __cplusplus */
/*===========================================================================*
 * Exported Preprocessor #define Constants
 *===========================================================================*/

/**
 * This revision is used to keep track of the format of the binary data used
 * to store a statechart's context; it allows the engine to detect what format
 * the data is in.
 */
#   define HSM_PERSISTENT_DATA_REVISION (2)

/**
 * Used internally to identify a "completion event"; a pseudo-event that
 * "triggers" an HSM_COMPLETION_TRANSITION.
 */
#   define HSM_COMPLETION_EVENT (-2)

/**
 * Specifies the ELSE guard condition - a guard that evaluates to true if
 * all others evaluate to false.
 */
#   define HSM_ELSE HSM_NO_GUARD

/** Use for the action function when the transition has no action. */
#   define HSM_NO_ACTION ((HSM_Action_T)NULL)

/**
 * Used for the event id for triggerless transitions, like those emanating from
 * pseudostates.
 */
#   define HSM_NO_EVENT (-1)

/** Use for the guard function when the transition has no guard. */
#   define HSM_NO_GUARD ((HSM_Guard_T)NULL)

/** Use for the initial state of a composite state that has no initial state. */
#   define HSM_NO_INITIAL_STATE HSM_NO_STATE

/** Use for the history state of a composite state that has no history state. */
#   define HSM_NO_HISTORY_STATE HSM_NO_STATE

/** Value that does not match the value of any state in the statechart. */
#   define HSM_NO_STATE (-1)

/** Used internally for internal transitions. */
#   define HSM_SAME_STATE (-2)

/** Use for the parent state if the state is at the top of the statechart. */
#   define HSM_TOP (-3)


/*===========================================================================*
 * Exported Preprocessor #define MACROS
 *===========================================================================*/

/**
 * In the rare situation that a state has no outgoing transition, use this for
 * the required single transition. This transition will never be taken.
 */
#   define HSM_NULL_TRANSITION()\
        HSM_TRANSITION(HSM_NO_EVENT, HSM_NO_GUARD, HSM_NO_ACTION, HSM_NO_STATE)

/**
 * Users of the statechart engine define their statechart by defining this
 * MACRO to contain the list of supported statechart components that make
 * up their statechart. This MACRO is expanded multiple times within other
 * header files within this module using the X MACRO technique. This technique
 * uses the C preprocessor to construct all of the objects that are required
 * for the statechart and ensures that they are consistent with the one another.
 *
 * The STATECHART_DEFN() must start with HSM_BEGIN() and end with HSM_END().
 * In between are entries for each of the states within the statechart; each
 * state type has its own definition MACRO to control its unique properties. The
 * transitions for the statechart accompany the state from which they emanate,
 * and the list of transitions for a state is terminated with an
 * HSM_END_STATE().
 *
 * The following is a simple example of such a definition for a simple
 * statechart with two simple states: S1, and S2.
 *
 * @code
 *
 * #define STARTECHART_DEFN()\
 * HSM_BEGIN(My_Table)\
 * \
 *    HSM_INITIAL_STATE(I0, HSM_TOP, HSM_NO_ACTION, S1)\
 * \
 *    HSM_SIMPLE_STATE(S1, HSM_TOP, S1_Entry, S1_Exit)\
 *       HSM_INTERNAL_TRANSITION(EVT_001, G_S1,     Action_S1)\
 *       HSM_TRANSITION         (EVT_001, HSM_ELSE, Action_S1_S2, S2)\
 *    HSM_END_STATE(S2)\
 * \
 *    HSM_SIMPLE_STATE(S2, HSM_TOP, S2_Entry, S2_Exit)\
 *       HSM_TRANSITION(EVT_002, HSM_NO_GUARD, Action_S2_S1, S1)\
 *    HSM_END_STATE(S2)\
 * \
 * HSM_END(My_Table)
 *
 * @endcode 
 *
 * @note
 *
 *   - Like all MACROs, STATECHART_DEFN() must be defined on a single "logical"
 *     line; use a backslash ('\') followed by a new-line character to split
 *     the definition across multiple physical lines.
 *
 *   - The first entry in in the STATECHART_DEFN() following the HSM_BEGIN()
 *     MACRO must be an HSM_INITIAL() MACRO designating the initial state
 *     for the statechart.
 *
 *   - The value of the "statechart_name" argument to HSM_BEGIN() and HSM_END()
 *     must be identical for the information generated by the C preprocessor
 *     to compile.
 *
 * Refer to the "See also" list for the list of supported statechart constructs,
 * each of which has a more detailed discussion of its use.
 *
 * @see HSM_BEGIN, HSM_COMPOSITE_STATE, HSM_END, HSM_END_STATE, 
 *      HSM_INITIAL_STATE, HSM_INTERNAL_TRANSITION, HSM_JUNCTION_STATE,
 *      HSM_SHALLOW_HISTORY_STATE, HSM_SIMPLE_STATE, HSM_TRANSITION
 */
#   define STATECHART_DEFN()
#   undef  STATECHART_DEFN

/**
 * This must be the first MACRO listed in the STATECHART_DEFN() MACRO.
 * The entire statechart definition is found between HSM_BEGIN() and HSM_END().
 * Typically, each HSM_*() MACRO that is part of the STATECHART_DEFN() will
 * appear on a new line, which means the previous line must use the backslash
 * continuation character ('\') followed by a new-line character.
 *
 * @param statechart_name
 *    This MACRO parameter is the "name" of the statechart and is used to
 *    form the symbol names of the enumeration tags and publicly accessible
 *    statechart objects. These are:
 *       - The enumeration tag whose value is the number of states in the
 *         statechart (statechart_name_NUM_STATES).
 *       - The enumeration type that lists all of the states in the statechart
 *         (statechart_name_States_T).
 *       - The const statechart state table (statechart_name_Table)
 *       - The const state definition structure (statechart_name_Defn)
 *
 * As an example, HSM_Begin(My_Chart) would cause the following objects
 * to be declared/defined:
 *
 * @code
 * typedef enum My_Chart_States_Tag
 * {
 *   ... list of states in chart ...
 *   My_Chart_NUM_STATES
 * }
 * My_Chart_States_T;
 *
 * extern HSM_State_T const My_Chart_Table[My_Chart_NUM_STATES];
 * extern HSM_State_Defn_T const My_Chart_Defn;
 * @endcode
 *
 * @note
 *
 *   - The value of the "statechart_name" argument to HSM_BEGIN() and HSM_END()
 *     must be identical for the information generated by the C preprocessor
 *     to compile.
 *
 * @see HSM_END
 *
 */
#   define HSM_BEGIN(statechart_name)

/**
 * This MACRO defines a completion transition: a transition from a composite
 * state that is taken due to the composite state's final state being entered.
 *
 * @param action
 *   HSM_Action_T function executed when the transition is taken; HSM_NO_ACTION
 *   should be used if there is no action function.
 *
 * @param next_state
 *   The identifier for the target state of the transition.
 *
 * @note
 *
 *   - There is no event associated with a completion transition; it is
 *     triggered by the entry into the composite's final state.
 *
 *   - A composite state containing a final state must have an outgoing
 *     completion transition; otherwise, the statechart is ill-formed and
 *     a run-time error will occur. A composite state's completion transition
 *     must be the first transition listed in its transition list.
 *
 *   - A guard condition is not allowed on the completion transition to help
 *     prevent the creation of a situation where the statechart becomes
 *     "stuck" in the final state. Multiple paths from the completion
 *     transition can be implemented by having the completion transition
 *     target a junction state which has multiple transitions from it. If
 *     this is done, there should always an enabled path out of the
 *     junction state; otherwise, a run-time error will occur.
 *
 * @see HSM_FINAL_STATE
 */
#   define HSM_COMPLETION_TRANSITION(action, next_state)\
        HSM_TRANSITION(HSM_COMPLETION_EVENT, HSM_NO_GUARD, action, next_state)

#   define HSM_COMPOSITE_COMPLETION_TRANSITION(current_state,action,next_state)\
        HSM_COMPLETION_TRANSITION(action,next_state)

/**
 * This MACRO defines a composite state - a state containing other states.
 *
 * @param state_name
 *    The name of the composite state; this identifier becomes part of the enum
 *    created for the statechart's list of states. This identifier is used
 *    by other states to reference this state; for example, another state with
 *    a transition to this state must use this identifier as part of its
 *    HSM_TRANSITION() definition.
 *
 * @param parent
 *    The identifier for the composite state that is a parent of this state;
 *    HSM_TOP is used if this state has no explicit parent (is at the top
 *    level).
 *
 * @param initial
 *    The identifier for the initial state for this composite state. If this
 *    composite state is the target of a transition, then there must be an
 *    HSM_INITIAL_STATE() for this composite state that defines the target
 *    of such a transition. If there is no such transition, use
 *    HSM_NO_INITIAL_STATE for this argument.
 *
 * @param history
 *    The identifier for the history state for this composite state. A composite
 *    state is not required to have a history state. If it does not have one,
 *    then HSM_NO_HISTORY_STATE() should be used for this argument. If there
 *    is a history state for this composite state (only one is allowed), then
 *    this argument must identify it.
 *
 * @param entry
 *    HSM_Action_T function executed any time the state is entered; 
 *    HSM_NO_ACTION should be used if there is no entry action.
 *
 * @param exit
 *    HSM_Action_T function executed any time the state is exited; 
 *    HSM_NO_ACTION should be used if there is no exit action.
 *
 * @note
 *
 *   - The HSM_COMPOSITE_STATE() MACRO must be followed by a list of all
 *     transitions out of the state (either HSM_TRANSITION(), or
 *     HSM_INTERNAL_TRANSITION()). The order of the transition list should
 *     not be important, since only zero or one should be enabled for any
 *     given event. The exception to this is an HSM_COMPLETION_TRANSITION; if
 *     the composite state has one of these, it must be the first one in its
 *     list of transitions. At least one transition definition is required. In
 *     the rare case that the state has no outgoing or internal transitions
 *     (all transitions are due to a parent state), use HSM_NULL_TRANSITION()
 *     to create the single transition entry (it represents a transition that
 *     will never be taken).
 *
 *   - After all outgoing or internal transitions for the state are defined,
 *     an HSM_END_STATE() MACRO must occur (with the same "state_name" argument)
 *     to terminate the definition of the state.
 *
 * @see HSM_COMPLETION_TRANSITION, HSM_END_STATE, HSM_FINAL_STATE,
 *      HSM_INTERNAL_TRANSITION, HSM_NO_HISTORY_STATE,
 *      HSM_NO_INITIAL_STATE, HSM_NULL_TRANSITION, HSM_TRANSITION
 */
#   define HSM_COMPOSITE_STATE(state_name,parent,initial,history,entry,exit)

/**
 * This MACRO defines a deep history pseudostate for a composite state. A
 * transition to the deep history state is semantically equivalent to a
 * transition to the last simple substate of the composite state, at any
 * nesting level, that was last active. If no state within the composite state
 * had previously been active, then the default transition from the deep 
 * history state is taken. Only one history state is allowed per composite
 * state; it can be deep or shallow history. A statechart is never "in" a
 * history state, it only  transitions through a history state during a single
 * run-to-completion compound transition from one non-pseudostate to another.
 *
 * @param state_name
 *    The name of the history state; this identifier becomes part of the enum
 *    created for the statechart's list of states. This identifier is used
 *    by other states to reference this state; for example, a composite state
 *    that has this state as its history state must use this identifier as
 *    its "history" argument.
 *
 * @param parent
 *   The identifier for the composite state that is a parent of this state;
 *   this cannot be HSM_TOP for a history state.
 *
 * @param default_state
 *   The identifier for the default target state of the transition out of
 *   this state.
 *
 * @note
 *
 *   - No guard conditions are allowed for transitions out of history
 *     states; they are required to always be "enabled".
 *
 *   - No action is allowed for the default transition because it can
 *     lead to confusion. For example, it is not clear whether such an action
 *     is meant to be performed: 1) Only when the default transition is taken,
 *     2) Any time a transition through the history state is taken, 3) Any time
 *     a transition through the history state is taken to the state that is
 *     the target of the default transition, etc.
 *
 *   - The default_state transition is only meaningful for a transition to
 *     the history state if the containing composite state has never been
 *     active before.
 *
 *   - Because only a single outgoing transition is allowed, there is no
 *     HSM_END_STATE() MACRO required or allowed for a history state (unlike
 *     some of the other state types that can have multiple outgoing
 *     transitions).
 *
 * @see HSM_SHALLOW_HISTORY_STATE
 */
#   define HSM_DEEP_HISTORY_STATE(state_name,parent,default_state)

/**
 * This is the last MACRO in the STATECHART_DEFN() expansion; it designates
 * the end of the list of states and transitions for the statechart.
 *
 * @param statechart_name
 *    This MACRO parameter is the "name" of the statechart.
 *
 * @note
 *
 *   - The value of the "statechart_name" argument to HSM_BEGIN() and HSM_END()
 *     must be identical for the information generated by the C preprocessor
 *     to compile.
 *
 * @see HSM_BEGIN
 */
#   define HSM_END(statechart_name)

/**
 * This MACRO is used to terminate the definitions of states that are allowed to
 * have multiple outgoing or internal transitions.
 *
 * @param state_name
 *    The name of the state definition being terminated; this MUST match the
 *    identifier in the corresponding HSM_COMPOSITE_STATE(),
 *    HSM_JUNCTION_STATE(), or HSM_SIMPLE_STATE() MACRO that initiated the
 *    state's definition.
 *
 * @note
 *
 *   - The state_name argument MUST match the identifier in the corresponding
 *     HSM_COMPOSITE_STATE(), HSM_JUNCTION_STATE(), or HSM_SIMPLE_STATE() MACRO
 *     that initiated the state's definition or the information generated by
 *     the C preprocessor will not compile.
 * 
 *   - At least one transition (HSM_INTERNAL_TRANSITION() or HSM_TRANSITION())
 *     must occur between the state's initial definition and the corresponding
 *     HSM_END_STATE().
 *
 * @see HSM_COMPOSITE_STATE, HSM_INTERNAL_TRANSITION, HSM_JUNCTION_STATE,
 *      HSM_NULL_TRANSITION, HSM_SIMPLE_STATE, HSM_TRANSITION
 */
#   define HSM_END_STATE(state_name)

/**
 * This MACRO defines a final state - a state within a composite state that
 * represents a common exit point. Entry to a composite state's final state
 * causes the composite state's completion transition to be taken. A final
 * state has no outgoing transitions; the outgoing transition is implied and
 * is represented by the enclosing composite state's completion transition.
 * If the composite state does not have a completion transition, but has a
 * final state then this is an ill-formed statechart and a runtime error will
 * occur.
 *
 * @param state_name
 *    The name of the final state; this identifier becomes part of the enum
 *    created for the statechart's list of states. This identifier is used
 *    by other states to reference this state; for example, another state with
 *    a transition to this state must use this identifier as part of its
 *    HSM_TRANSITION() definition.
 *
 * @param parent
 *    The identifier for the composite state that is a parent of this state;
 *    HSM_TOP is not a valid parent for a final state.
 *
 * @see HSM_COMPLETION_TRANSITION
 */
#   define HSM_FINAL_STATE(state_name,parent)

/**
 * This MACRO defines an initial pseudostate for the statechart or for a
 * composite state. If a composite state is the target of a transition, then
 * it is required to have an initial state which designates the path to the
 * child state that will be the effective target of that transition. A
 * statechart is never "in" an initial state, it only transitions through an
 * initial state during a single run-to-completion compound transition from one
 * non-pseudostate to another.
 *
 * @param state_name
 *    The name of the initial state; this identifier becomes part of the enum
 *    created for the statechart's list of states. This identifier is used
 *    by other states to reference this state; for example, a composite state
 *    that has this state as its initial state must use this identifier as
 *    its "initial" argument.
 *
 * @param parent
 *   The identifier for the state that is a parent of this state; HSM_TOP is
 *   used if this state has no explicit parent (is at the top level).
 *
 * @param action
 *   HSM_Action_T function executed when the transition from this initial
 *   state is made; HSM_NO_ACTION should be used if there is no action
 *   function.
 *
 * @param next_state
 *   The identifier for the target state of the transition out of this state.
 *
 * @note
 *
 *   - No guard conditions are allowed for transitions out of initial
 *     states; they are required to always be "enabled".
 *
 *   - Only a single transition out of the initial state to "next_state"
 *     is supported.
 *
 *   - Because only a single outgoing transition is allowed, there is no
 *     HSM_END_STATE() MACRO required or allowed for an initial state (unlike
 *     some of the other state types that can have multiple outgoing
 *     transitions).
 */
#   define HSM_INITIAL_STATE(state_name,parent,action,next_state)

/**
 * This MACRO defines an internal transition; this is a transition that
 * consumes an event and performs some action but does not result in a change
 * in state.
 *
 * @param event
 *    The identifier for the event that can trigger the transition.
 *
 * @param guard
 *    The HSM_Guard_T that defines the guard function that enables or
 *    disables the transition. If the transition is always enabled, use
 *    HSM_NO_GUARD. If the transition represents an ELSE transition (one taken
 *    only after all other transition's guards evaluate to false), then use
 *    HSM_ELSE_GUARD.
 *
 * @param action
 *   HSM_Action_T function executed when the transition is taken; HSM_NO_ACTION
 *   should be used if there is no action function.
 *
 * @see HSM_NO_ACTION, HSM_NO_GUARD
 */
#   define HSM_INTERNAL_TRANSITION(event,guard,action)

/**
 * This MACRO defines a junction state - an intermediate pseudostate used to
 * chain together different action sequences while transitioning between
 * "real" states. A statechart is never "in" a junction state, it only
 * transitions through a junction state during a single run-to-completion
 * compound transition from one non-pseudostate to another.
 *
 * @param state_name
 *    The name of the junction state; this identifier becomes part of the enum
 *    created for the statechart's list of states. This identifier is used
 *    by other states to reference this state; for example, another state with
 *    a transition to this state must use this identifier as part of its
 *    HSM_TRANSITION() definition.
 *
 * @param parent
 *    The identifier for the composite state that is a parent of this state;
 *    HSM_TOP is used if this state has no explicit parent (is at the top
 *    level).
 *
 * @note
 *
 *   - The HSM_JUNCTION_STATE() MACRO must be followed by a list of all
 *     transitions out of the state.
 *
 *   - Because a junction state is only a transient state, only HSM_TRANSITION()
 *     definitions (not HSM_INTERNAL_TRANSITION() definitions) should be used.
 *     The order of the transition list should not be important, since only
 *     zero or one of them should be enabled.
 *
 *   - The "event" for any HSM_TRANSITION() out of a junction state should
 *     be HSM_NO_EVENT since these transitions only represent secondary (or
 *     later) links in a compound transition. The event that triggers
 *     the first link in the chain is associated with the transition that
 *     emanates from a non-pseudostate.
 *
 *   - After all outgoing transitions for the state are defined, an
 *     HSM_END_STATE() MACRO must be used (with the same "state_name" argument)
 *     to terminate the definition of the state.
 *
 * @see HSM_END_STATE, HSM_TRANSITION
 */
#   define HSM_JUNCTION_STATE(state_name,parent)

/**
 * Calculates the size of the data area needed to save the context of the
 * statechart in persistent storage.
 *
 * @return the size, in bytes, required to store the context
 *
 * @param  [in] num_history_states number of history states in chart.
 */
#   define HSM_PERSISTENT_SIZE(num_history_states)\
(sizeof(uint16_t) + ((num_history_states + 1) * sizeof(HSM_State_Id_T))) 

/**
 * This MACRO defines a shallow history pseudostate for a composite state. A
 * transition to the shallow history state is semantically equivalent to a
 * transition to whatever direct substate of the composite state was last
 * active (or to the default transition from the history state if the composite
 * state was not previously active). Only one history state is allowed per
 * composite state. A statechart is never "in" a history state, it only
 * transitions through a history state during a single run-to-completion
 * compound transition from one non-pseudostate to another.
 *
 * @param state_name
 *    The name of the history state; this identifier becomes part of the enum
 *    created for the statechart's list of states. This identifier is used
 *    by other states to reference this state; for example, a composite state
 *    that has this state as its history state must use this identifier as
 *    its "history" argument.
 *
 * @param parent
 *   The identifier for the composite state that is a parent of this state;
 *   this cannot be HSM_TOP for a history state.
 *
 * @param default_state
 *   The identifier for the default target state of the transition out of
 *   this state.
 *
 * @note
 *
 *   - No guard conditions are allowed for transitions out of history
 *     states; they are required to always be "enabled".
 *
 *   - No action is allowed for the default transition because it can
 *     lead to confusion. For example, it is not clear whether such an action
 *     is meant to be performed: 1) Only when the default transition is taken,
 *     2) Any time a transition through the history state is taken, 3) Any time
 *     a transition through the history state is taken to the state that is
 *     the target of the default transition, etc.
 *
 *   - The default_state transition is only meaningful for a transition to
 *     the history state if the containing composite state has never been
 *     active before.
 *
 *   - Because only a single outgoing transition is allowed, there is no
 *     HSM_END_STATE() MACRO required or allowed for a history state (unlike
 *     some of the other state types that can have multiple outgoing
 *     transitions).
 *
 * @see HSM_DEEP_HISTORY_STATE
 */
#   define HSM_SHALLOW_HISTORY_STATE(state_name,parent,default_state)

/**
 * This MACRO defines a simple state - one with no children.
 *
 * @param state_name
 *    The name of the simple state; this identifier becomes part of the enum
 *    created for the statechart's list of states. This identifier is used
 *    by other states to reference this state; for example, another state with
 *    a transition to this state must use this identifier as part of its
 *    HSM_TRANSITION() definition.
 *
 * @param parent
 *    The identifier for the composite state that is a parent of this state;
 *    HSM_TOP is used if this state has no explicit parent (is at the top
 *    level).
 *
 * @param entry
 *    HSM_Action_T function executed any time the state is entered; 
 *    HSM_NO_ACTION should be used if there is no entry action.
 *
 * @param exit
 *    HSM_Action_T function executed any time the state is exited; 
 *    HSM_NO_ACTION should be used if there is no exit action.
 *
 * @note
 *
 *   - The HSM_SIMPLE_STATE() MACRO must be followed by a list of all
 *     transitions out of or internal to the state (either HSM_TRANSITION(), or
 *     HSM_INTERNAL_TRANSITION()). The order of the transition list should
 *     not be important, since only zero or one should be enabled for any
 *     given event. At least one transition definition is required. In the
 *     rare case that the state has no outgoing or internal transitions
 *     (all transitions are due to a parent state), use HSM_NULL_TRANSITION()
 *     to create the single transition entry (it represents a transition that
 *     will never be taken).
 *
 *   - After all outgoing or internal transitions for the state are defined,
 *     an HSM_END_STATE() MACRO must be used (with the same "state_name"
 *     argument) to terminate the definition of the state.
 *
 * @see HSM_END_STATE, HSM_INTERNAL_TRANSITION, HSM_NULL_TRANSITION,
 *      HSM_TRANSITION
 */
#   define HSM_SIMPLE_STATE(state_name,parent,entry,exit)

/**
 * This MACRO defines a transition from one state to another state.
 *
 * @param event
 *    The identifier for the event that can trigger the transition.
 *
 * @param guard
 *    The HSM_Guard_T that defines the guard function that enables or
 *    disables the transition. If the transition is always enabled, use
 *    HSM_NO_GUARD. If the transition represents an ELSE transition (one taken
 *    only after all the other transition's guards evaluate to false), then use
 *    HSM_ELSE_GUARD.
 *
 * @param action
 *   HSM_Action_T function executed when the transition is taken; HSM_NO_ACTION
 *   should be used if there is no action function.
 *
 * @param next_state
 *   The identifier for the target state of the transition.
 *
 * @see HSM_NO_ACTION, HSM_NO_GUARD
 */
#   define HSM_TRANSITION(event,guard,action,next_state)

#   define HSM_EXTERNAL_TRANSITION(event,guard,current_state,action,next_state)\
        HSM_TRANSITION(event,guard,action,next_state)

/*===========================================================================*
 * Exported Type Declarations
 *===========================================================================*/

/* *INDENT-OFF* */

/**
 * Values that identify the type of state represented by an HSM_State_T.
 * These values are for internal use (by the MACROs that automatically
 * create the statechart objects).
 */
typedef enum
{
   HSM_COMPOSITE_ID,       /**< composite state (contains children) */
   HSM_DEEP_HISTORY_ID,    /**< deep history pseudostate */
   HSM_FINAL_ID,           /**< final state */
   HSM_INITIAL_ID,         /**< initial pseudostate */
   HSM_JUNCTION_ID,        /**< junction pseudostate */
   HSM_SHALLOW_HISTORY_ID, /**< shallow history pseudostate */
   HSM_SIMPLE_ID           /**< simple state (no children) */
}
HSM_State_Kind_T;

/**
 * Uniquely identifies states within a statechart. Negative values are used
 * for special cases that don't represent actual states.
 */
typedef int16_t HSM_State_Id_T;

/* Allow forward reference by action & guard functions to HSM_Statechart_T. */
struct HSM_Statechart_Tag;

/**
 * Type of pointer to action function.
 */
typedef void (*HSM_Action_T)(struct HSM_Statechart_Tag const *statechart);

/**
 * Type of pointer to function used for debug callback.
 */
typedef void (*HSM_DbgFunc_T)(struct HSM_Statechart_Tag const *statechart);

/**
 * Type of pointer to guard function.
 */
typedef bool_t(*HSM_Guard_T)(struct HSM_Statechart_Tag const *statechart);

/**
 * Each transition between states or internal transition within the statechart
 * is defined by an object of this type; these are automatically created by
 * the C preprocessor during the X MACRO expansion of the STATECHART_DEFN().
 * Transitions are associated with their source state - the state from which
 * they emanate. Each state must have at least one transition since an array
 * of HSM_Transition_T is created for each state. The definition
 * HSM_NULL_TRANSITION() can be used if there really is no transition. Such
 * can be the case if all transitions are handled by parent states.
 *
 * @note
 * - The transition objects should be treated as anonymous types; none of the
 *   fields should ever be modified by any code other than the statechart
 *   engine.
 */
typedef struct
{
   HSM_Event_T event;    /**< Event to respond to */
   HSM_Guard_T guard;    /**< Guard function */
   HSM_Action_T action;  /**< Action function */
   HSM_State_Id_T target_state; /**< Target state ID or default state for history */
   HSM_State_Id_T *history; /**< NULL except for transitions from history states */
   char const *guard_name;  /**< Name of guard function (if enabled) */
   char const *action_name; /**< Name of action function (if enabled) */
}
HSM_Transition_T;


/**
 * Each state in the statechart is described by an HSM_State_T object. These are
 * automatically created by the C preprocessor during the X MACRO expansion of
 * the STATECHART_DEFN().
 *
 * @note
 * - The state objects should be treated as anonymous types; none of the
 *   fields should ever be modified by any code other than the statechart
 *   engine.
 */
typedef struct
{
   /** Identifies what type of state this is. */
   HSM_State_Kind_T state_type;
   /** Transitions emanating from, or internal to, this state. */
   HSM_Transition_T const *transition_table;
   /** Number of entries in the transition table */
   uint8_t transition_count;
   /** Entry action function pointer. */
   HSM_Action_T entry_action;
   /** Exit action function pointer. */
   HSM_Action_T exit_action;
   /** State id of parent state containing this state. */
   HSM_State_Id_T parent_state;
   /** HSM_NO_INITIAL_STATE or id of state that is initial state for composite.*/
   HSM_State_Id_T initial_state;
   /** HSM_NO_HISTORY_STATE or id of state that is history state for composite*/
   HSM_State_Id_T history_state;
   /** Name of entry action function (if enabled). */
   char_t const *entry_action_name;
   /** Name of exit action function (if enabled). */
   char_t const *exit_action_name;
}
HSM_State_T;

/**
 * This structure is used to define the states of the statechart.
 */
typedef struct HSM_State_Defn_Tag
{
   HSM_State_T const *state_table;  /**< The list of states in the statechart.*/
   uint16_t history_state_count;    /**< Number of history states in the statechart. */
   uint16_t state_count;            /**< The number of states. */
   char const * statechart_name;    /**< Name of the statechart. */
   char const * const *state_names; /**< (Configurable) List of state names. */
}
HSM_State_Defn_T;

/**
 * Function pointer that can optionally be provided by a module to translate
 * its event values into names for dbg_trace.
 */
typedef char const *(*HSM_Event_Name_Func_T)(const HSM_Event_T event);

/**
 * This structure controls the debug trace facility for a statechart instance.
 * The debug level can be individually set for each statechart, thereby
 * allowing debugging to be focused on selected statechart instances. Each
 * statechart can also be provided a unique debug identifier to allow the
 * trace messages to be associated with the instance, even when there are
 * multiple instances being driven by the same statechart declaration.
 *
 * @note The lifetime of the string pointed to by chart_id must be greater
 *       than the lifetime of the statechart object it controls since the
 *       HSM engine will reference the string for trace messages. A NULL
 *       is allowed for chart_id, but will mean that the trace messages
 *       for the statechart will instead use the statechart name (which
 *       is sufficient unless there are multiple instances that you need
 *       to differentiate).
 */
typedef struct HSM_Debug_Control_Tag
{
   char const *chart_id;       /**< Unique identifier place in trace messages. */
   int dbg_module_id; /**< Module whose trace level controls this statechart. */
   int log_level; /**< Trace level to use (if dbg_module_id not used). */
   HSM_DbgFunc_T debug_func;  /**< Optional callback function for advanced debugging */
   bool_t perform_check;       /**< If true in HSM_Begin, check for legal UML statechart */
   /**
     * If non-NULL, get_event_name is called to obtain event names for trace
     * output instead of the global callback function HSM_Get_Event_Name. This
     * allows statecharts to have truly private events instead of using global
     * system events.
     */
   HSM_Event_Name_Func_T get_event_name;
}
HSM_Debug_Control_T;

/**
 * Each statechart is described by an HSM_Statechart_T object. The client must
 * define the object of type HSM_Statechart_T and call HSM_Start() to initialize
 * it and cause the statechart to enter its initial state.
 *
 * @note
 * - The statechart object should be treated as an anonymous type; none of the
 *   fields should ever be modified by any code other than the statechart
 *   engine.
 * - Any change to format of this structure should result in a change to the
 *   HSM_PERSISTENT_DATA_REVISION since the binary format of persistent data
 *   has been affected.
 */
typedef struct HSM_Statechart_Tag
{
   /** Number of transitions in the latest compound transition (trans_chain). */
   uint8_t chain_length;
   /** This field is used to track whether HSM_Save() has been called for the
    *  statechart, and HSM_Restore() has not yet been called. In this situation
    *  further calls to HSM_Process_Event() should not occur since such a call
    *  invalidates the saved context. */
   bool_t save_has_occurred;
   /** Used to detect and flag illegal recursive calls to HSM_Process_Event. */
   bool_t processing_event;
   /** Current state of the statechart. */
   HSM_State_Id_T current_state;
   /** Previous state of the statechart; used to detect state changes. */
   HSM_State_Id_T previous_state;
   /** Latest received event id. */
   HSM_Event_T event;
   /** Pointer to data structure containing data associated with event. */
   void const *event_data;
   /** Can be used to point to the object whose state is being represented. */
   void *this_ptr;
   /** The list and number of states in the statechart. */
   HSM_State_Defn_T states;
   /** Controls debug trace for this statechart instance. */
   HSM_Debug_Control_T dbg;
   /** List of transitions last taken. */
   HSM_Transition_T const *trans_chain[HSM_MAX_TRANSITION_CHAIN];
}
HSM_Statechart_T;

/* *INDENT-ON* */


/*===========================================================================*
 * Exported Const Object Declarations
 *===========================================================================*/


/*===========================================================================*
 * Exported Function Prototypes
 *===========================================================================*/

/**
 * Starts a statechart, causing it to enter its initial state.
 *
 * @param [in,out] statechart
 *   HSM_Statechart_T object that is used to hold the statechart data.
 *
 * @pre
 *   - statechart != NULL
 *   - A call to HSM_Begin() must be preceded by a call to HSM_Init().
 */
void HSM_Begin(HSM_Statechart_T *statechart);

/**
 * Checks the statechart definition to make sure it is well-formed (legal).
 * Illegal statechart definitions trigger Error Management (EM) asserts,
 * which would typically (depending on system configuration) mean that
 * the function would not return.
 *
 * @param [in] state_defn The list and number of states in the statechart.
 */
void HSM_Check_Statechart_Defn(HSM_State_Defn_T const *state_defn);

/**
 * Configures the debug settings for the specified statechart. This call sets
 * up the future debug trace settings for the statechart instance.
 *
 * @param [in,out] statechart
 *   HSM_Statechart_T object that is used to hold the statechart data.
 *
 * @param [in] dbg_ctrl
 *   HSM_Debug_Control_T object that specifies the new settings for the
 *   statechart's debug trace activity. A NULL is allowed and has the
 *   effect of disabling any further debug trace for the statechart.
 *
 *  @note
 *
 *   - The lifetime of the string pointed to by dbg_ctrl->chart_id must
 *     be greater than the lifetime of the statechart object it controls
 *     since the HSM engine will reference the string for trace messages.
 *
 * @pre
 *   - statechart != NULL
 */
void HSM_Control_Debug(HSM_Statechart_T *statechart,
                       HSM_Debug_Control_T const *dbg_ctrl);

/**
 * Returns the statechart's debug callback function.
 *
 * @return The HSM_DbgFunc_T for the statechart.
 *
 * @param [in] statechart
 *   HSM_Statechart_T object that is used to hold the statechart data.
 *
 * @pre
 *   - statechart != NULL
 */
HSM_DbgFunc_T HSM_Get_Callback(HSM_Statechart_T const *statechart);

/**
 * Returns the name of the specified state in the statechart.
 *
 * @return A pointer to a string containing the state's name.
 *
 * @note
 *   State names are only available for a statechart if it is compiled with
 *   the appropriate option (HSM_GENERATE_STATE_NAMES); otherwise, the
 *   names returned for its states will be the string "unknown".
 *
 * @param [in] statechart
 *   HSM_Statechart_T object that is used to hold the statechart data.
 *
 * @param [in] state
 *   The state index (from the statechart's enumeration of states) of
 *   the state whose name is to be returned.
 *
 * @pre
 *   - statechart != NULL
 *   - state >= 0
 *   - state < number of states in statechart
 *
 * @post
 *   - pointer returned != NULL
*/
char const *HSM_Get_State_Name(HSM_Statechart_T const *statechart,
                               HSM_State_Id_T state);

/**
 * Initializes the statechart structure without starting the statechart; this
 * call must precede a call to HSM_Begin().
 *
 * @param [in,out] statechart
 *   The caller provides the HSM_Statechart_T object that is used to
 *   hold the statechart data. The structure is initialized by this call.
 * 
 * @param [in] state_defn
 *   The list and number of states in the statechart. The information is
 *   copied from this structure into the statechart structure, which allows
 *   the object pointed to to have a temporary lifetime (it can be a local
 *   variable on the stack).
 *
 * @param [in] this_ptr
 *   A pointer that can be used to point to a data structure representing the
 *   object which is controlled by the statechart. This can be used to provide
 *   access to the object to the action and guard functions which otherwise
 *   would have to access static or global variables.
 *
 * @param [in] dbg_ctrl
 *   Optional pointer to a structure that can specify debug information for
 *   this statechart. Use NULL if no debugging or initial checking of the
 *   statechart is required.
 *
 * @note 
 *
 *   - The dbg_ctrl structure contents are copied into the statechart
 *     object by this call meaning that subsequent changes to the
 *     dbg_ctrl structure will not affect the statechart object. To
 *     alter the statechart object's settings, see HSM_Control_Debug().
 *
 *   - The lifetime of the string pointed to by dbg_ctrl->chart_id must
 *     be greater than the lifetime of the statechart object it controls
 *     since the HSM engine will reference the string for trace messages.
 *
 * @pre
 *   - statechart != NULL
 *   - state_defn != NULL
 *   - state_defn->state_table !- NULL
 *   - state_defn->state_count > 1 (minimum statechart is an initial state
 *                                and a simple state)
 */
void HSM_Init(HSM_Statechart_T *statechart,
              HSM_State_Defn_T const *state_defn,
              void *this_ptr,
              HSM_Debug_Control_T const *dbg_ctrl);

/**
 * Returns an indication of whether the statechart is in the specified state.
 *
 * @return true if in the state; false otherwise.
 *
 * @param [in] statechart
 *   HSM_Statechart_T object that is used to hold the statechart data.
 *
 * @param [in] state
 *   The state index (from the statechart's enumeration of states) to be
 *   checked.
 *
 * @pre
 *   - statechart != NULL
 *
 * @note
 *
 * - The statechart can be "in" multiple states at once since when in
 *   a simple state, the statechart is also "in" any ancestral states.
  */
bool_t HSM_Is_In_State(HSM_Statechart_T const *statechart,
                       HSM_State_Id_T state);

/**
 * Processes the specified event for the statechart.
 *
 * @return true if the event was consumed; false, if the event was not
 *         consumed.
 *
 * @param [in,out] statechart
 *   HSM_Statechart_T object that is used to hold the statechart data. On
 *   entry it describes the current state, on exit any new state.
 * 
 * @param [in] event
 *   The integral value that identifies the event to the statechart.
 * 
 * @param [in] event_data
 *   Pointer to data structure containing data associated with the event. Can
 *   be NULL.
 *
 * @pre
 *   - statechart != NULL
 *   - event >= 0
 *   - statechart->save_has_occurred == false
 */
bool_t HSM_Process_Event(HSM_Statechart_T * statechart,
                         HSM_Event_T event,
                         const void *event_data);

/**
 * Restores the statechart's context from the specified buffer; this restores
 * the statechart's context to what it was when HSM_Save() was called.
 *
 * @return The number of bytes read; this will be equal to
 *         statechart_name_PERSISTENT_SIZE if the context was successfully
 *         restored, but will be zero if the buffer contained an invalid
 *         context. If zero is returned, the statechart context could not be
 *         restored and the statechart object cannot be used unless HSM_Start()
 *         is called to initialize it.
 *
 * @param [out] statechart
 *   HSM_Statechart_T object whose context is restored.
 * 
 * @param [in] buffer
 *   The data buffer from which the statechart's context is read.
 *
 * @param [in] space_available
 *   The amount of space available in the buffer; must be greater than or equal
 *   to the amount of space required to retrieve the statechart's context.
 * 
 * @param [in] state_defn
 *   The list and number of states in the statechart.
 *
 * @param [in] this_ptr
 *   A pointer that can be used to point to a data structure representing the
 *   object which is controlled by the statechart..
 *
 * @param [in] dbg_ctrl
 *   Optional pointer (can be NULL) to a structure controlling debugging of
 *   the statechart instance.
 *
 * @pre
 *    - statechart != NULL
 *    - buffer != NULL
 *    - state_defn != NULL
 *    - state_defn->state_table !- NULL
 *    - state_defn->state_count > 1 (minimum statechart is an initial state
 *                                   and a simple state)
 *
 * @note
 *
 *    - If space_available < statechart_name_PERSISTENT_SIZE, then the buffer
 *      does not contain enough data to restore the statechart's context. The
 *      statechart's context is not restored, and zero is returned.
 *
 *    - If the persistent data revision of the data in the buffer is not
 *      compatible with the statechart engine, then the context is not restored
 *      and zero is returned.
 *
 *    - Only upon the return of a non-zero value has the statechart object's
 *      context been restored, making it usable.
 *
 *    - Any required restoring of the persistent data of the object being
 *      controlled by the statechart must also be performed by the calling
 *      logic before any additional use of the statechart; e.g., calls to
 *      HSM_Process_Event().
 *
 *  @see HSM_Init(), HSM_Start()
 */
size_t HSM_Restore(HSM_Statechart_T *statechart,
                   uint8_t const *buffer,
                   size_t space_available,
                   HSM_State_Defn_T const *state_defn,
                   void *this_ptr,
                   HSM_Debug_Control_T const *dbg_ctrl);

/**
 * Saves the statechart's context into the specified buffer so that it can
 * later be restored via a call to HSM_Restore().
 *
 * @return The number of bytes written; this will be equal to zero if
 *         insufficient room was provided by the caller or will be equal to
 *         statechart_name_PERSISTENT_SIZE if the call succeeded.
 *
 * @param [in, out] statechart
 *   HSM_Statechart_T object that holds the statechart data.
 * 
 * @param [out] buffer
 *   The data buffer into which the statechart's context is written.
 *
 * @param [in] space_available
 *   The amount of space available in the buffer, which must be greater than or
 *   equal to the amount of space required to save the statechart's context.
 *
 * @pre
 *    - statechart != NULL
 *    - buffer != NULL
 *
 * @post
 *    - return value (amount of data written) == 
 *         statechart_name_PERSISTENT_SIZE (success) or 0 (failure)
 *    - statechart->save_has_occurred == true (success) or false (failure)
 *
 * @note
 *
 *    - If the statechart's context is successfully saved via this call, no
 *      further calls to HSM_Process_Event() should be made for the statechart
 *      until its context is restored via HSM_Restore(); otherwise, the saved
 *      context may no longer match the current context. Failure to adhere to
 *      this requirement will lead to a trace warning message.
 *
 *    - Any data in the object being controlled by the statechart must also be
 *      saved by the caller; this call only handles the statechart object.
 */
size_t HSM_Save(HSM_Statechart_T *statechart, uint8_t *buffer, size_t space_available);

/**
 * Sets the statechart's debug callback function.
 *
 * @return The previously set HSM_DbgFunc_T for the statechart.
 *
 * @param [in,out] statechart
 *   HSM_Statechart_T object that is used to hold the statechart data.
 *
 * @param [in] debug_func
 *   Callback function pointer (can be NULL) that allows the caller
 *   to specify a callback that will be made whenever an event is processed
 *   for the statechart. This can be used to debug or log the statechart's
 *   activity.
 *
 * @pre
 *   - statechart != NULL
 */
HSM_DbgFunc_T HSM_Set_Callback(HSM_Statechart_T * statechart,
                                HSM_DbgFunc_T debug_func);

/**
 * Initializes the statechart, checks the its states' definitions (optional),
 * enters its initial state, and prepares it for receiving events.
 *
 * @note Use of HSM_Init() and HSM_Begin is now the recommended replacement
 *       for this function.
 *
 * @param [in,out] statechart
 *   The caller provides the HSM_Statechart_T object that is used to
 *   hold the statechart data. This structure is initialized and updated
 *   to reflect the initial state of the statechart.
 * 
 * @param [in] state_defn
 *   The list and number of states in the statechart. The information is
 *   copied from this structure into the statechart structure, which allows
 *   the object pointed to to have a temporary lifetime (it can be a local
 *   variable on the stack).
 *
 * @param [in] this_ptr
 *   A pointer that can be used to point to a data structure representing the
 *   object which is controlled by the statechart. This can be used to provide
 *   access to the object to the action and guard functions which otherwise
 *   would have to access static or global variables.
 *
 * @param [in] perform_check
 *   If true, checks are performed on the state_table to check for a well-formed
 *   statechart. Any problems will result in an assert being triggered.
 *   Such checks need only be performed when a statechart's definition has
 *   been modified from a previously checked definition.
 *
 * @param [in] debug_func
 *   Optional callback function pointer (can be NULL) that allows the caller
 *   to specify a callback that will be made whenever an event is processed
 *   for the statechart. This can be used to debug or log the statechart's
 *   activity.
 *
 * @pre
 *   - statechart != NULL
 *   - state_defn != NULL
 *   - state_defn->state_table !- NULL
 *   - state_defn->state_count > 1 (minimum statechart is an initial state
 *                                and a simple state)
 */
void HSM_Start(HSM_Statechart_T *statechart,
               HSM_State_Defn_T const *state_defn,
               void *this_ptr,
               const HSM_Debug_Control_T *dbg_ctrl);


/*===========================================================================*
 * Exported Inline Function Definitions and #define Function-Like Macros
 *===========================================================================*/

#   ifdef __cplusplus
}         /* extern "C" */
#   endif /* __cplusplus */
/*===========================================================================*/
/*!
 * @file hsm_engine.h
 *
 * @section RH REVISION HISTORY (top to bottom: last revision to first revision)
 *
 * - 30-May-2012 Kirk Bailey Rev 28
 *   - Added logic to detect, log, and ignore recursive calls to HSM_Process_Event.
 *
 * - 30-Sep-2011 Kirk Bailey Rev 27
 *   - Added support for action function names (for debug trace).
 *
 * - 04-Mar-2011 Kirk Bailey Rev 26
 *   - Task 23873: Expanded debug level support to track another module's level.
 *
 * - 14-Jan-2011 Kirk Bailey Rev 25
 *   - Task 19973: Added support for per-statechart event naming for trace.
 *
 * - 02-Sep-2010 Kirk Bailey
 *   - Added ability to generate guard function names for debug trace.
 *
 * - 01-Sep-2010 Kirk Bailey Rev 23
 *   - Added HSM_Init() and HSM_Begin() to support debug trace for individual
 *     statechart instance.
 *
 * - 25-Jul-2010 Kirk Bailey Rev 20
 *   - Replaced "bool" with "bool_t".
 *
 * - 07-jul-2008 kirk bailey
 *   -Added support for state names.
 *
 * - 21-feb-2008 kirk bailey
 *   - Added HSM_Check_Statechart_Defn to public API.
 *   - Added HSM_State_Defn_T to API and changed HSM_Start to use it.
 *
 * - 15-nov-2007 kirk bailey
 *   - Added final states and completion transitions.
 *   - Converted to new Doyxgen format.
 *
 * - 09-aug-2007 kirk bailey
 *   - Changed type usage to be MISRA compliant.
 *   - Moved HSM_MAX_TRANSITION_CHAIN to hsm_engine_acfg.h.
 *
 * - 10-jul-2007 kirk bailey
 *   - Improved comments.
 *
 * - 05-apr-2007 kirk bailey
 *   - Added callback function and HSM_Is_In_State.
 *
 * - 02-apr-2007 kirk bailey
 *   - Added event data pointer.
 *
 * - 31-mar-2007 kirk bailey
 *   - Added this_ptr. Made statechart an argument for guard & action functions.
 *   - Added deep history.
 *
 * - 28-mar-2007 kirk bailey
 *   - Renamed hsm_engine and converted to BASA naming convention.
 *
 * - 24-mar-2007 kirk bailey
 *   - Added state_type to HSM_State_T struct to support alternate states. Added
 *   - definitions to support new types. Renamed some MACROs.
 *
 * - 19-mar-2007 kirk bailey
 *   - Added HSM_No_Guard.
 *
 * - 16-mar-2007 kirk bailey
 *   - Removed non-functional default state logic.
 *
 * - 15-mar-2007 kirk bailey
 *   - Created initial file from copy of Dave Johnson's state_engine.h.
 */
/*===========================================================================*/
/** @} doxygen end group */
#endif /* HSM_ENGINE_H */

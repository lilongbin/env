/*===========================================================================*/
/**
 * @file hsm_engine.c
 *
 * This is the main source file for the statechart engine. The application
 * writer should not change any part of this file.
 *
 * %full_filespec:hsm_engine.c~kok_basa#30:csrc:kok_aud#1 %
 * @version %version:kok_basa#30 %
 * @author  %derived_by:dzq92s %
 * @date    %date_modified:Thu May 31 09:47:26 2012 %
 *
 *------------------------------------------------------------------------------
 *
 * Copyright 2011, 2012 Technologies, Inc., All Rights Reserved.
 * Confidential
 *
 *------------------------------------------------------------------------------
 *
 * @section DESC DESCRIPTION:
 *
 *   The logic in this file executes statecharts defined by clients of this
 *   module.
 *
 * @section ABBR ABBREVIATIONS:
 *   - hsm = Hierarchical State Machine.
 *   - lca = least common ancestor
 *
 * @section TRACE TRACEABILITY INFO:
 *   - Design Document(s):
 *     - SDD_10016705_State_Machine_Check.doc
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
 * @ingroup hsm_engine
 * @{
 */
/*==========================================================================*/

/*===========================================================================*
 * Header Files
 *===========================================================================*/
#include "hsm_check.h"
#include "hsm_engine.h"
#include "hsm_engine_cfg.h"
#include "hsm_comm.h"
#include <string.h>

/*===========================================================================*
 * Local Preprocessor #define Constants
 *===========================================================================*/

/*===========================================================================*
 * Local Preprocessor #define MACROS
 *===========================================================================*/

//EM_FILENUM(HSM_ENGINE_MODULE_ID, 1); /**< define file for assert handling */

/*===========================================================================*
 * Local Type Declarations
 *===========================================================================*/

/*===========================================================================*
 * Exported Const Object Definitions
 *===========================================================================*/

/*===========================================================================*
 * Local Object Definitions
 *===========================================================================*/

/*===========================================================================*
 * Local Function Prototypes
 *===========================================================================*/
static bool_t Is_State_Ancestor(HSM_Statechart_T const *statechart,
        HSM_State_Id_T ref_st, HSM_State_Id_T confirming_st);

static HSM_State_Id_T Get_LeastCommonAncestor(HSM_Statechart_T const *statechart,
        HSM_State_Id_T s1, HSM_State_Id_T s2);

static HSM_State_Id_T Get_Target_State(HSM_Statechart_T const *statechart,
        HSM_Transition_T const *transition);

static char const *Get_Valid_Event_Name(HSM_Statechart_T const *statechart,
        HSM_Event_T event_id);

static void Add_To_Transition_Chain(HSM_Statechart_T *statechart,
        HSM_Transition_T const *transition);

static void Complete_Transition_Chain(HSM_Statechart_T *statechart,
        HSM_State_T const *p_target,
        HSM_Transition_T const ** p_next_transition);

static const HSM_Transition_T *Find_Enabled_Transition(HSM_Statechart_T *statechart,
        HSM_State_T const *ptr_st, const HSM_Event_T event);

static void Build_Transition_Chain(HSM_Statechart_T *statechart, HSM_Event_T event);

static void Log_Trace_For_Guard(HSM_Statechart_T const *statechart,
        HSM_Transition_T const *p_trans, bool_t guard_was_true);

static void Log_Trace_Action(HSM_Statechart_T const *statechart, char const *action_name);

static void Execute_Entry_Actions(HSM_Statechart_T *statechart,
        HSM_State_Id_T target_state, HSM_State_Id_T lca);

static void Execute_Exit_Actions(HSM_Statechart_T *statechart,
        HSM_State_Id_T src_state, HSM_State_Id_T lca);

static void Take_Transitions(HSM_Statechart_T *statechart);

static void Initialize_Debug_Control(HSM_Statechart_T *statechart,
        HSM_Debug_Control_T const *dbg_ctrl);

static void Initialize_Statechart_Object(HSM_Statechart_T *statechart,
        HSM_State_Defn_T const *state_defn,
        void *this_ptr, HSM_Debug_Control_T const *dbg_ctrl);

static void Update_Dbg_Trace_Level(HSM_Statechart_T *statechart);

/*===========================================================================*
 * Local Inline Function Definitions and Function-Like Macros
 *===========================================================================*/

/*===========================================================================*
 * Function Definitions
 *===========================================================================*/

/**
 * This function verify whether confirming_st is an ancestor of the ref_st.
 *
 * @return true if confirming_st is an ancestor of ref_st; otherwise false.
 *
 * @param [in] p_states The list and number of states in the statechart.
 *
 * @param [in] ref_st the state being checked for being a descendent of confirming_st.
 *
 * @param [in] confirming_st the state being checked for being an ancestor of ref_st.
 *
 * @pre
 *   - p_states != NULL
 *   - 0 <= ref_st < p_states->state_count
 *   - (HSM_TOP == confirming_st) || (0 <= confirming_st < p_states->state_count)
 */
static bool_t Is_State_Ancestor(HSM_Statechart_T const *statechart,
        HSM_State_Id_T ref_st, HSM_State_Id_T confirming_st)
{
    bool_t is_ancestor = false;
    PBC_Internal(statechart != NULL, "NULL statechart");
    HSM_State_Defn_T const *state_defn = &statechart->def_states;

    /* PRQA S 0505 1 *//* Suppress QAC NULL ptr message (checked above). */
    PBC_Internal(state_defn->state_table != NULL, "NULL state table");
    PBC_Internal_2((ref_st >= 0) && (ref_st < state_defn->state_count),
            "(%s) reference state out of range: %d",
            (char*)state_defn->statechart_name, (int)ref_st);

    if (HSM_TOP == confirming_st)
    {
        is_ancestor = true;
    }
    else
    {
        PBC_Internal_2((confirming_st >= 0) && (confirming_st < state_defn->state_count),
                "(%s) State out of range: %d",
                (char*)state_defn->statechart_name, (int)confirming_st);
        for (;;)
        {
            if (state_defn->state_table[ref_st].parent_state == confirming_st)
            {
                is_ancestor = true;
                break; /* out of for loop, return true */
            }

            if (state_defn->state_table[ref_st].parent_state == HSM_TOP)
            {
                break; /* out of for loop, return false */
            }

            ref_st = state_defn->state_table[ref_st].parent_state;
            PBC_Internal_2((ref_st >= 0) && (ref_st < state_defn->state_count),
                    "(%s) parent of reference state out of range: %d",
                    (char*)state_defn->statechart_name, (int)ref_st);
        }
    }

    return is_ancestor;
}

/**
 * This function returns the least common ancestor (LCA) of states s1 and s2. 
 *
 * @param [in] statechart
 *    Data structure describing the statechart.
 *
 * @param [in] s1 state 1
 *
 * @param [in] s2 state 2
 *
 * @pre
 *   - statechart != NULL
 *   - 0 <= s1 < statechart->def_states.state_count
 *   - 0 <= s2 < statechart->def_states.state_count
 *
 * @post
 *   - (lca == HSM_TOP) || (0 <= lca < statechart->def_states.state_count)
 *
 * Conceptually, the LCA of two states is the most deeply nested ancestor that
 * contains both states. There are some unique situations that require special
 * consideration (see notes below). As examples:
 *
 *  - If s1 is s2, s1's parent is returned.
 *  - If s1 is an ancestor of s2, s1 is returned.
 *  - If s2 is an ancestor of s1, s2 is returned
 *
 * If the returned value is HSM_TOP, it means that the common ancestor is the
 * top level of the statechart.
 *
 * @note
 *
 * - When s1 is an ancestor of s2, the conceptual view of LCA might lead to
 *   the conclusion that s1's parent is the LCA, not s1. In the context
 *   that LCA is used (transitions between s1 and s2), neither a transition from
 *   s1 to s2 nor from s2 to s1 involve exiting s1. Because that is the
 *   context in which LCA is used, s1 (not s1's parent) is considered the LCA.
 */
static HSM_State_Id_T Get_LeastCommonAncestor(HSM_Statechart_T const *statechart,
        HSM_State_Id_T s1, HSM_State_Id_T s2)
{
    HSM_State_T const *state_table;
    HSM_State_Id_T lca = s1;

    PBC_Internal(statechart != NULL, "NULL statechart");
    /* PRQA S 0505 1 *//* Suppress QAC NULL ptr message (checked above). */
    PBC_Require_2((s1 >= 0) && (s1 < statechart->def_states.state_count),
            "(%s) Illegal state: %d",
            (char*)statechart->def_states.statechart_name, (int)s1
            );
    PBC_Require_2((s2 >= 0) && (s2 < statechart->def_states.state_count),
            "(%s) Illegal state: %d",
            (char*)statechart->def_states.statechart_name, (int)s2
            );
    state_table = statechart->def_states.state_table;

    if (s1 == s2)
    {
        lca = (HSM_State_Id_T) state_table[s1].parent_state;
    }
    else
    {
        for (;;)
        {
            /* Is s2 an ancestor of s1 ? */
            if (Is_State_Ancestor(statechart, s1, s2))
            {
                lca = s2;
                break; /* out of for loop, return s2 */
            }
            /* Is s1 an ancestor of s2 ? */
            else if (Is_State_Ancestor(statechart, s2, s1))
            {
                lca = s1;
                break; /* out of for loop, return s1 */
            }
            /* If s1's parent and s2's parent are the same, we have a match */
            else if (state_table[s1].parent_state == state_table[s2].parent_state)
            {
                lca = (HSM_State_Id_T) state_table[s1].parent_state;
                break; /* out of for loop */
            }
            else
            {
                /* If s1 the not top-most state, move up a level */
                if (state_table[s1].parent_state != HSM_TOP)
                {
                    s1 = state_table[s1].parent_state;
                }

                /* If s2 the not top-most state, move up a level */
                if (state_table[s2].parent_state != HSM_TOP)
                {
                    s2 = state_table[s2].parent_state;
                }
            }

            PBC_Internal_2((s1 >= 0) && (s1 < statechart->def_states.state_count),
                    "(%s) Illegal state: %d",
                    (char*)statechart->def_states.statechart_name, (int)s1
                    );
            PBC_Internal_2((s2 >= 0) && (s2 < statechart->def_states.state_count),
                    "(%s) Illegal state: %d",
                    (char*)statechart->def_states.statechart_name, (int)s2
                    );
        } /* for (;;) */
    } /* else */

    PBC_Ensure_2((lca == HSM_TOP)
            || ((lca >= 0) && (lca < statechart->def_states.state_count)),
            "(%s) Illegal LCA: %d",
            (char*)statechart->def_states.statechart_name, (int)lca
            );

    return lca;
    /* PRQA S 4700 1 *//* Suppress QAC STMIF metric message. */
}

/**
 * Returns the target state of a transition. Because of history states, this is
 * not necessarily the transition->target_state field.
 *
 * @return the target state of the transition.
 *
 * @param [in] statechart
 *    Data structure describing the statechart.
 *
 * @param [in] transition
 *    The HSM_Transition_T whose target_state is to be returned.
 *
 * @pre
 *   - statechart != NULL
 *   - transition != NULL
 *
 * @post
 *   - (0 < target state (return value) < statechart->def_states.state_count) ||
 *     (HSM_SAME_STATE == target_state)
 */
static HSM_State_Id_T Get_Target_State(HSM_Statechart_T const *statechart,
        HSM_Transition_T const *transition)
{
    HSM_State_Id_T target_state;

    PBC_Internal(statechart != NULL, "NULL statechart");
    PBC_Internal(transition != NULL, "NULL transition");

    /* PRQA S 0505 1 *//* Suppress QAC NULL ptr message (checked above). */
    if (transition->history != NULL)
    {
        target_state = *transition->history;
    }
    else
    {
        target_state = transition->target_state;
    }

    /* PRQA S 0505 1 *//* Suppress QAC NULL ptr message (checked above). */
    PBC_Ensure_2((HSM_SAME_STATE == target_state)
            || ((target_state > 0) && (target_state < statechart->def_states.state_count)),
            "(%s) Illegal target state: %d",
            (char*)statechart->def_states.statechart_name, (int)target_state
            );

    return target_state;
}

/**
 * Translates an event value into a string for dbg_trace. The result will come
 * from either calling the statechart's unique get_event_name function pointer
 * (to support localized event values) or from the global HSM_Get_Event_Name
 * callout.
 *
 * @return a pointer to the string for the event.
 *
 * @param [in] statechart
 *    Data structure describing the statechart.
 * @param [in] event_id ID for event whose name is to be provided.
 *
 * @pre
 *   - statechart != NULL
 */
static char const *Get_Valid_Event_Name(HSM_Statechart_T const *statechart,
        HSM_Event_T event_id)
{
    char const *event_name;
    PBC_Internal(statechart != NULL, "NULL statechart");

    if (statechart->dbg.get_event_name != NULL)
    {
        event_name = statechart->dbg.get_event_name(event_id);
    } else {
        event_name = "unknown"; //HSM_Get_Event_Name(event_id);
    }

    if (NULL == event_name)
    {
        Tr_Fault_2("NULL event name provided for chart %s, event %d", statechart->dbg.chart_label, (int)event_id);
        event_name = "unknown";
    }

    return event_name;
}

/**
 * Adds the specified transition to the transition chain and builds the rest of
 * the chain. A completed chain ends at a simple state. If prior to that, no
 * enabled transition is found out of an intermediate state (due to a guard
 * returning false), then the chain is broken and there is no matched valid
 * transition. If a complete chain is formed, then the function will return
 * with the transition sequence in statechart->trans_chain and the number of
 * transitions in statechart->chain_length. If the chain is broken, then the
 * function will return with statechart->chain_length equal to zero.
 *
 * Note that this function and Find_Enabled_Transition form a recursive loop;
 * however, the recursion is bounded by the HSM_MAX_TRANSITION_CHAIN constant.
 *
 * @param [in,out] statechart
 *    Data structure describing the statechart.
 *
 * @param [in] transition
 *    The first HSM_Transition_T to add to the chain of transitions.
 *
 * @pre
 *   - statechart != NULL
 *   - transition != NULL
 *
 * @see HSM_MAX_TRANSITION_CHAIN
 */
static void Add_To_Transition_Chain(HSM_Statechart_T *statechart,
        HSM_Transition_T const *transition)
{
    HSM_State_Id_T target_state;
    bool_t done = false;
    HSM_Transition_T const *final_trans = NULL;

    PBC_Internal(statechart != NULL, "NULL statechart");
    PBC_Internal(transition != NULL, "NULL transition");

    while ((transition != NULL) && (!done))
    {
        /* PRQA S 0505 1 *//* Suppress QAC NULL ptr message (checked above). */
        PBC_Require_1(statechart->chain_length < HSM_MAX_TRANSITION_CHAIN,
                "Maximum configured chain of transitions exceeded for %s",
                (char*)statechart->def_states.statechart_name);

        statechart->trans_chain[statechart->chain_length++] = transition;

        target_state = Get_Target_State(statechart, transition);

        if (HSM_SAME_STATE == target_state)
        {
            PBC_Require_1(statechart->chain_length == 1,
                    "Internal transitions cannot be chained (%s)",
                    (char*)statechart->def_states.statechart_name);
            /*
             * An internal transition represents a completed chain.
             */
            done = true;
        }
        else /* Not an internal transition */
        {
            HSM_State_T const *p_target;

            PBC_Require_2((target_state > 0) &&
                    (target_state < statechart->def_states.state_count),
                    "(%s) Illegal target state: %d",
                    (char*)statechart->def_states.statechart_name, (int)target_state);

            p_target = &statechart->def_states.state_table[target_state];

            if (HSM_FINAL_ID == p_target->state_type)
            {
                HSM_State_Id_T parent = statechart->def_states.state_table[target_state].parent_state;
                PBC_Require_2((parent > 0) && (parent < statechart->def_states.state_count),
                        "(%s) Illegal parent state: %d",
                        (char*)statechart->def_states.statechart_name, (int)parent);
                final_trans = transition; /* keep track that we had a final state */
                /*
                 * The first transition of the final state's parent is required to
                 * be its completion transition.
                 */
                transition = statechart->def_states.state_table[parent].transition_table;
                PBC_Ensure_3(HSM_COMPLETION_EVENT==transition->event,
                        "(%s) Missing completion transition for state '%s' (%d)",
                        (char*)statechart->def_states.statechart_name,
                        (char*)HSM_Get_State_Name(statechart, parent), (int)parent);
            }
            else
            {
                /*
                 * Check for end of the chain and/or get its next transition.
                 */
                Complete_Transition_Chain(statechart, p_target, &transition);
                done = true;

            } /* if (HSM_FINAL_ID == p_target->state_type)                       */

        } /* if (HSM_SAME_STATE == target_state)                                */

    } /*  while ((transition != NULL) && !done)                                */

    if (NULL == transition)
    {
        /*
         * A compound transition could not be completed. Invalidate the partial
         * chain that had been built to signal that no path was found.
         */
        /* PRQA S 0505 1 *//* Suppress QAC NULL ptr message (checked above). */
        statechart->chain_length = 0;
        /*
         * The statechart is considered ill-formed if a transition into a final
         * state occurred with no enabled transition out of it.
         */
        PBC_Ensure_3(NULL == final_trans,
                "(%s) No path out of a final state '%s' (%d)",
                (char*)statechart->def_states.statechart_name,
                (char*)HSM_Get_State_Name(statechart, final_trans->target_state),
                (int)final_trans->target_state
                );
    }
}

/**
 * Completes a transition chain to the specified target. A transition chain
 * always ends on a simple state; so, if the targeted state is not a simple
 * state then there are additional links in the chain.
 *
 * @param [in,out] statechart
 *    Data structure describing the statechart.
 *
 * @param [in] p_target
 *    The HSM_State_T that is the target of a transition.
 *
 * @param [in,out] p_next_transition
 *    If false is returned, the pointer to the next transition in the chain is
 *    written to *p_next_transition.
 *
 * @pre
 *   - statechart != NULL
 *   - p_target != NULL
 *   - p_next_transition != NULL
 *   - p_target->state_type != HSM_FINAL_ID
 */
static void Complete_Transition_Chain(HSM_Statechart_T *statechart,
        HSM_State_T const *p_target, HSM_Transition_T const **p_next_transition)
{
    PBC_Internal(statechart != NULL, "NULL statechart");
    PBC_Internal_1(p_target != NULL, "(%s) NULL target",
            (char*)statechart->def_states.statechart_name);
    PBC_Internal_1(p_next_transition != NULL, "(%s) NULL location for next transition",
            (char*)statechart->def_states.statechart_name);
    /* PRQA S 0505 1 *//* Suppress QAC NULL ptr message (checked above). */
    PBC_Internal_1(p_target->state_type != HSM_FINAL_ID, "(%s) Target is final state",
            (char*)statechart->def_states.statechart_name);
    /*
     * If the target is a composite state, then the "effective" target is the
     * composite's initial state.
     */
    if (HSM_COMPOSITE_ID == p_target->state_type)
    {
        /* PRQA S 0505 1 *//* Suppress QAC NULL ptr message (checked above). */
        PBC_Require_1((p_target->initial_state > 0) &&
                (p_target->initial_state < statechart->def_states.state_count),
                "(%s) Illegal initial state",
                (char*)statechart->def_states.statechart_name
                );
        p_target = &statechart->def_states.state_table[p_target->initial_state];
    }
    /*
     * A transition chain only ends on a simple state.
     */
    /* PRQA S 0505 1 *//* Suppress QAC NULL ptr message (checked above). */
    if (p_target->state_type != HSM_SIMPLE_ID)
    {
        /*
         * Recursively search for an enabled transition out of this intermediate
         * target state within the compound transition. Note that the recursion
         * is bounded by the HSM_MAX_TRANSITION_CHAIN setting.
         */
        /* PRQA S 0505 1 *//* Suppress QAC NULL ptr message (checked above). */
        *p_next_transition = Find_Enabled_Transition(statechart, p_target, HSM_NO_EVENT);
    }
}

/**
 * Searches for a matched valid transition internal to or out of the specified state
 * that is triggered by the specified event.
 *
 * @return NULL if the state has no matched valid transition for the specified event;
 *         otherwise, a pointer to the HSM_Transition_T corresponding to a matched
 *         valid transition.
 *
 * @param [in,out] statechart
 *    Data structure describing the statechart.
 *
 * @param [in,out] ptr_st
 *    Data structure describing the state to be considered.
 *
 * @param [in] event The event trigger.
 *
 * @pre
 *   - statechart != NULL
 *   - ptr_st != NULL
 *
 * @note
 *   - This function will also be used to evaluate transitions out of
 *     pseudostates. In this situation, the event will be HSM_NO_EVENT and this
 *     function will simply evaluate the guard condition.
 *   - This function and Add_To_Transition_Chain form a recursive loop; however, the
 *     recursion is bounded by the HSM_MAX_TRANSITION_CHAIN constant.
 */
static HSM_Transition_T const *Find_Enabled_Transition(HSM_Statechart_T *statechart,
        HSM_State_T const *ptr_st, const HSM_Event_T event)
{
    HSM_Transition_T const *candidate = NULL;
    HSM_Transition_T const *else_candidate = NULL;
    size_t trans_index;

    PBC_Internal(statechart != NULL, "NULL statechart");
    PBC_Internal(ptr_st != NULL, "NULL state pointer");

    /* PRQA S 0505 1 *//* Suppress QAC NULL ptr message (checked above). */
    for (trans_index = 0; trans_index < ptr_st->transition_count; trans_index++)
    {
        HSM_Transition_T const *p_trans = &ptr_st->transition_table[trans_index];

        if ((HSM_NO_EVENT == event) || (p_trans->event == event))
        {
            HSM_Guard_T guard = p_trans->guard;

            if (HSM_ELSE == guard)
            {
                /* Remember this transition but keep looking */
                else_candidate = p_trans;
            }
            else if (guard(statechart))
            {
                /*
                 * Found a non-ELSE transition with a "true" guard. Need
                 * to make sure its transition chain is not broken further on.
                 */
                Log_Trace_For_Guard(statechart, p_trans, true);
                Add_To_Transition_Chain(statechart, p_trans);
                if (statechart->chain_length != 0) /* chain not broken */
                {
                    candidate = p_trans;
                    break; /* Out of the for loop */
                }
            }
            else
            {
                Log_Trace_For_Guard(statechart, p_trans, false);
            }
        } /* if ((HSM_NO_EVENT == event) ||...  */
    } /* for (trans_index = 0; trans_index < ptr_st->transition_count;... */

    /*
     * If an ELSE guard was encountered and no other matched valid transition was
     * found, then try to complete a transition through the ELSE path.
     */
    if ((else_candidate != NULL) && (NULL == candidate))
    {
        candidate = else_candidate;
        Add_To_Transition_Chain(statechart, candidate);
        if (0 == statechart->chain_length)
        {
            candidate = NULL; /* No candidate found */
        }
    }

    return candidate;
}

/**
 * Builds a chain (list) of transitions triggered by the specified event. The
 * list is placed in statechart->trans_chain, and the number of links in the
 * chain of transitions is written to statechart->chain_length. If the event
 * does not lead to a transition, statechart->chain_length will be zero. Note
 * that an internal transition will result in a chain of length one: the
 * transition is an internal one.
 *
 * @param [in,out] statechart
 *    Data structure describing the statechart.
 *
 * @param [in] event
 *    The event received by the statechart.
 *
 * @pre
 *   - statechart != NULL
 *   - (event == HSM_NO_EVENT) || (event >= 0)
 *
 * @note
 *   - On exit, statechart->chain_length contains the number of transitions in
 *     the group transition triggered by this event, and these transitions are
 *     found in statechart->trans_chain.
 *   - The maximum length of the transition chain is a compile-time constant to
 *     the state engine (HSM_MAX_TRANSITION_CHAIN).
 *
 * @see HSM_MAX_TRANSITION_CHAIN
 *
 * main step:
 * Build_Transition_Chain(statechart, HSM_NO_EVENT):
 * //(Searches for an enabled transition internal to or out of the specified state by the specified event)
 * while ((transition == NULL) && (src_state != HSM_TOP)) {
 *   HSM_State_T const *p_src = &statechart->def_states.state_table[src_state];
 *   transition = Find_Enabled_Transition(statechart, p_src, event) {
 *     for (trans_index = 0; trans_index < ptr_st->transition_count; trans_index++) {
 *         HSM_Transition_T const *p_trans = &ptr_st->transition_table[trans_index];
 *         if ((HSM_NO_EVENT == event) || (p_trans->event == event)) {
 *           if (guard(statechart)) {
 *               //add detailed simple state as the target to transition chain
 *               Add_To_Transition_Chain(statechart, p_trans) {
 *                 statechart->trans_chain[statechart->chain_length++] = transition;
 *                 if (target == simple_state) {
 *                   done;
 *                 } else {
 *                   Complete_Transition_Chain(statechart, p_target, &transition) {
 *                     if (target == composite) {
 *                       target = target.intial_state;
 *                     }
 *                     if (target != simple_state) {
 *                         *p_next_transition = Find_Enabled_Transition(statechart, p_target, HSM_NO_EVENT);
 *                     }
 *                   }
 *                 }
 *               } //end of Add_To_Transition_Chain
 *           } //end of if (guard(statechart))
 *         }
 *     } //end of for loop
 *   }
 *
 *   if (NULL == transition) {
 *     src_state = p_src->parent_state;
 *     statechart->chain_length = 0;
 *   }
 * } //end of while loop
 *
 */
static void Build_Transition_Chain(HSM_Statechart_T *statechart,
        HSM_Event_T event)
{
    HSM_Transition_T const *transition = NULL;
    HSM_State_Id_T src_state;

    PBC_Internal(statechart != NULL, "NULL statechart");
    PBC_Internal_2((event == HSM_NO_EVENT) || (event >= 0),
            "(%s) Illegal event: %d",
            (char*)statechart->def_states.statechart_name, (int)event);

    /* PRQA S 0505 1 *//* Suppress QAC NULL ptr message (checked above). */
    src_state = statechart->current_state;
    statechart->chain_length = 0;

    while ((transition == NULL) && (src_state != HSM_TOP))
    {
        HSM_State_T const *p_src = &statechart->def_states.state_table[src_state];
        /*
         * Look for an enabled internal transition or a transition from the current
         * state that matches the event.
         */
        transition = Find_Enabled_Transition(statechart, p_src, event);

        if (NULL == transition)
        {
            /*
             * If the transition hasn't been found, then the search is repeated for the
             * parent state and will continue until either an matched valid transition is
             * found or the top of the statechart is reached.
             */
            src_state = p_src->parent_state;
            statechart->chain_length = 0; /* clear out any accumulated partial path */
        }

    } /*  while ((transition == NULL) && (src_state != HSM_TOP)) */
}

/**
 * Outputs any enabled trace message for guard functions
 *
 * @param [in] statechart Data structure describing the statechart.
 *
 * @param [in] p_trans Pointer to transition structure containing the guard.
 *
 * @param [in] guard_was_true true if guard returned true, false otherwise.
 */
static void Log_Trace_For_Guard(HSM_Statechart_T const *statechart,
        HSM_Transition_T const *p_trans, bool_t guard_was_true)
{
    if (statechart->dbg.log_level >= TR_LVL_INFO_MID)
    {
        char const *event_name = Get_Valid_Event_Name(statechart, statechart->event);

        if (guard_was_true)
        {
            if (*event_name != '\0')
            {
                Tr_Info_Mid_4( "%s guard %s returned TRUE for event %s while in state %s",
                        (char*)statechart->dbg.chart_label,
                        (char*)p_trans->guard_name, (char*)event_name,
                        HSM_Get_State_Name(statechart, statechart->current_state));
            }
            else
            {
                Tr_Info_Mid_4( "%s guard %s returned TRUE for event %d while in state %s",
                        (char*)statechart->dbg.chart_label,
                        (char*)p_trans->guard_name, (int)statechart->event,
                        HSM_Get_State_Name(statechart, statechart->current_state));
            }
        }
        else
        {
            if (*event_name != '\0')
            {
                Tr_Info_Mid_4( "%s guard %s returned FALSE for event %s while in state %s",
                        (char*)statechart->dbg.chart_label,
                        (char*)p_trans->guard_name, (char*)event_name,
                        HSM_Get_State_Name(statechart, statechart->current_state));
            }
            else
            {
                Tr_Info_Mid_4( "%s guard %s returned FALSE for event %d while in state %s",
                        (char*)statechart->dbg.chart_label,
                        (char*)p_trans->guard_name, (int)statechart->event,
                        HSM_Get_State_Name(statechart, statechart->current_state));
            }
        }
    }
}

/**
 * Issues a trace statement for an action function that is going to be executed.
 *
 * The trace message will only be issued if the name is non-NULL, not the empty
 * string, and the debug trace level for the statechart is TR_LVL_INFO_LO.
 *
 * @param [in] statechart Data structure describing the statechart.
 * @param [in] name Name of action function.
 */
static void Log_Trace_Action(HSM_Statechart_T const *statechart,
        char const *action_name)
{
    if ((action_name != NULL) && (*action_name != '\0')
            && (statechart->dbg.log_level >= TR_LVL_INFO_LO))
    {
        Tr_Info_Lo_1("Executing action (%s())", action_name);
    }
}

/**
 * Performs the entry actions starting from the target state's ancestor that is
 * a direct substate of the LCA and proceeding inward until the target state is
 * reached.
 *
 * @param [in,out] statechart
 *    Data structure describing the statechart.
 *
 * @param [in] target_state
 *    The most deeply nested state that is the destination of a transition and
 *    is therefore the last one whose entry action is to be performed.
 *
 * @param [in] lca
 *    The LCA of the source and target states; entry actions start with the
 *    ancestor of the target state contained in the common ancestor.
 *
 * @pre
 *   - statechart != NULL
 *   - target_state is a valid state
 *   - lca is a valid state and is an ancestor of target_state
 *
 * @note 
 *   - The entry action of the LCA is NOT executed.
 */
static void Execute_Entry_Actions(HSM_Statechart_T *statechart,
        HSM_State_Id_T target_state, HSM_State_Id_T lca)
{
    HSM_State_T const *state_table;
    HSM_State_Id_T entry_states[HSM_MAX_NESTING_LEVELS] =  { 0 };
    HSM_State_Id_T state_index = 0;

    PBC_Internal(statechart != NULL, "NULL statechart");
    /* PRQA S 0505 1 *//* Suppress QAC NULL ptr message (checked above). */
    PBC_Internal_2((target_state >= 0) &&
            (target_state < statechart->def_states.state_count),
            "(%s) Illegal target state: %d",
            (char*)statechart->def_states.statechart_name, (int)target_state
            );
    PBC_Internal_2((HSM_TOP==lca)
            || ((lca >= 0) && (lca < statechart->def_states.state_count)),
            "(%s) Illegal LCA state: %d",
            (char*)statechart->def_states.statechart_name, (int)lca
            );

    /* PRQA S 0505 1 *//* Suppress QAC NULL ptr message (checked above). */
    state_table = statechart->def_states.state_table;

    while (target_state != lca)
    {
        PBC_Require_3(state_index < HSM_MAX_NESTING_LEVELS,
                "(%s) Maximum nesting level exceeded by state '%s' (%d)",
                (char*)statechart->def_states.statechart_name,
                (char*)HSM_Get_State_Name(statechart, target_state), (int)target_state
                );
        entry_states[state_index++] = target_state;
        target_state = state_table[target_state].parent_state;
    }

    while (state_index > 0)
    {
        HSM_State_Id_T parent_of_target;

        state_index--;
        target_state = entry_states[state_index];

        if (state_table[target_state].entry_action != NULL)
        {
            Log_Trace_Action(statechart, state_table[target_state].entry_action_name);
            state_table[target_state].entry_action(statechart);
        }
        /*
         * Any time a new state is entered, its parent's shallow history state
         * must be updated to reflect the new target. This is how the parent's
         * shallow history state always has the value of the last direct substate
         * that was active. If instead, the target is the composite's final state,
         * then the history state must be "reset" to its default transition.
         */
        parent_of_target = state_table[target_state].parent_state;
        if (parent_of_target != HSM_TOP)
        {
            HSM_State_Id_T parent_history;

            parent_history = state_table[parent_of_target].history_state;

            if ((parent_history != HSM_NO_HISTORY_STATE)
                    && (parent_history != target_state))
            {
                HSM_State_Kind_T history_type;

                history_type = state_table[parent_history].state_type;

                PBC_Require_3((HSM_DEEP_HISTORY_ID == history_type)
                        || (HSM_SHALLOW_HISTORY_ID == history_type),
                        "(%s) Illegal history state type for %s (%d)",
                        (char*)statechart->def_states.statechart_name,
                        (char*)HSM_Get_State_Name(statechart, parent_history),
                        (int)parent_history
                        );

                if (HSM_FINAL_ID == state_table[target_state].state_type)
                {
                    //reset history state to its default state
                    *(state_table[parent_history].transition_table->history)
                        = state_table[parent_history].transition_table->target_state;
                }
                else if (HSM_SHALLOW_HISTORY_ID == history_type)
                {
                    //update history state to current state
                    *(state_table[parent_history].transition_table->history) = target_state;
                }
            }
        }
    }
    /* PRQA S 4700 1 *//* Suppress QAC STMIF metric message. */
}

/**
 * Performs the exit actions for all states from src_state until the
 * lca state is reached.
 *
 * @param [in,out] statechart
 *    Data structure describing the statechart.
 *
 * @param [in] src_state
 *    The most deeply nested state that is the first one whose exit action is
 *    to be performed.
 *
 * @param [in] lca
 *    The LCA of the source and target states; once this state is reached,
 *    exit actions stop.
 *
 * @pre
 *   - statechart != NULL
 *   - src_state is a valid state
 *   - lca is a valid state and is an ancestor of src_state
 *
 * @note 
 *   - The exit action of the LCA is NOT executed.
 */
static void Execute_Exit_Actions(HSM_Statechart_T *statechart,
        HSM_State_Id_T src_state, HSM_State_Id_T lca)
{
    HSM_State_T const *state_table;

    PBC_Internal(statechart != NULL, "NULL statechart");
    /* PRQA S 0505 1 *//* Suppress QAC NULL ptr message (checked above). */
    PBC_Internal_2((src_state >= 0) && (src_state < statechart->def_states.state_count),
            "(%s) Illegal source state: %d",
            (char*)statechart->def_states.statechart_name, (int)src_state
            );
    PBC_Internal_2((HSM_TOP==lca)
            || ((lca >= 0) && (lca < statechart->def_states.state_count)),
            "(%s) Illegal LCA: %d",
            (char*)statechart->def_states.statechart_name, (int)lca
            );
    /* PRQA S 0505 1 *//* Suppress QAC NULL ptr message (checked above). */
    state_table = statechart->def_states.state_table;

    while (src_state != lca)
    {
        /*
         * Any time a state is exited, if the parent state has a deep history
         * state, then that deep history state must be updated to the current
         * state of the statechart. This is how the deep history state will
         * always have a record of the most deeply nested substate. If the
         * state being exited is the final state of the history state's composite
         * state then the history state must be "reset" to its default transition.
         */
        HSM_State_Id_T parent_of_src = state_table[src_state].parent_state;
        HSM_State_Kind_T src_type = state_table[src_state].state_type;
        if (parent_of_src != HSM_TOP)
        {
            HSM_State_Id_T parent_history;

            parent_history = state_table[parent_of_src].history_state;

            if (parent_history != HSM_NO_HISTORY_STATE)
            {
                HSM_State_Kind_T history_type;

                history_type = state_table[parent_history].state_type;

                PBC_Require_3((HSM_DEEP_HISTORY_ID == history_type)
                        || (HSM_SHALLOW_HISTORY_ID == history_type),
                        "(%s) Illegal history state type for %s (%d)",
                        (char*)statechart->def_states.statechart_name,
                        (char*)HSM_Get_State_Name(statechart, parent_history),
                        (int)parent_history
                        );

                if (HSM_FINAL_ID == state_table[src_state].state_type)
                {
                    //reset history state to its default state
                    *(state_table[parent_history].transition_table->history)
                        = state_table[parent_history].transition_table->target_state;
                }
                else if ((HSM_DEEP_HISTORY_ID == history_type)
                        && ((HSM_SIMPLE_ID == src_type) || (HSM_COMPOSITE_ID == src_type)))
                {
                    //update history state to current state
                    *(state_table[parent_history].transition_table->history) = statechart->current_state;
                }
            }
        }

        if (state_table[src_state].exit_action != NULL)
        {
            Log_Trace_Action(statechart, state_table[src_state].exit_action_name);
            state_table[src_state].exit_action(statechart);
        }

        src_state = state_table[src_state].parent_state;
        PBC_Internal_3((HSM_TOP == lca) || (src_state != HSM_TOP),
                "(%s) Reached top of statechart without encountering LCA '%s' (%d)",
                (char*)statechart->def_states.statechart_name,
                (char*)HSM_Get_State_Name(statechart, lca), (int)lca
                );
    }
    /* PRQA S 4700 1 *//* Suppress QAC STMIF metric message. */
}

/**
 * Performs the list transitions previously created by Build_Transition_Chain.
 *
 * @param [in,out] statechart
 *    Data structure describing the statechart.
 *
 * @pre
 *   - statechart != NULL
 *
 * @see Build_Transition_Chain()
 *
 * main step:
 * Take_Transitions(statechart):
 * for (trans_index = 0; trans_index < statechart->chain_length; trans_index++) {
 *     if (HSM_SAME_STATE == target_state) {
 *         if (transition->action != NULL) {
 *             transition->action(statechart);
 *         }
 *     } else {
 *         lca = Get_LeastCommonAncestor(statechart, src_state, target_state);
 *         Execute_Exit_Actions(statechart, src_state, lca) {
 *             while (src_state != lca) {
 *                 if (parent_of_src != HSM_TOP) {
 *                     if (parent_history != HSM_NO_HISTORY_STATE) {
 *                         if (HSM_FINAL_ID == state_table[src_state].state_type)
 *                         {
 *                             //reset history state to its default state
 *                             *(state_table[parent_history].transition_table->history)
 *                                 = state_table[parent_history].transition_table->target_state;
 *                         }
 *                         else if ((HSM_DEEP_HISTORY_ID == history_type)
 *                                 && ((HSM_SIMPLE_ID == src_type) || (HSM_COMPOSITE_ID == src_type)))
 *                         {
 *                             //update history state to current state
 *                             *(state_table[parent_history].transition_table->history) = statechart->current_state;
 *                         }
 *                     }
 *                 }
 *                 if (state_table[src_state].exit_action != NULL) {
 *                     state_table[src_state].exit_action(statechart);
 *                 }
 *                 src_state = state_table[src_state].parent_state;
 *             } //end of while loop
 *         }
 *         if (transition->action != NULL) {
 *             transition->action(statechart);
 *         }
 *         Execute_Entry_Actions(statechart, target_state, lca) {
 *             reverse_target_state_list(state_table, target_state, entry_states, lca){
 *                 while (target_state != lca) {
 *                     entry_states[state_index++] = target_state;
 *                     target_state = state_table[target_state].parent_state;
 *                 }
 *             }
 *             while (state_index > 0) {
 *                 state_index--;
 *                 target_state = entry_states[state_index];
 *                 if (state_table[target_state].entry_action != NULL) {
 *                     state_table[target_state].entry_action(statechart);
 *                 }
 *                 if (parent_of_target != HSM_TOP) {
 *                     if ((parent_history != HSM_NO_HISTORY_STATE) && (parent_history != target_state)) {
 *                         if (HSM_FINAL_ID == state_table[target_state].state_type)
 *                         {
 *                             //reset history state to its default state
 *                             *(state_table[parent_history].transition_table->history)
 *                                 = state_table[parent_history].transition_table->target_state;
 *                         }
 *                         else if (HSM_SHALLOW_HISTORY_ID == history_type)
 *                         {
 *                             //update history state to current state
 *                             *(state_table[parent_history].transition_table->history) = target_state;
 *                         }
 *                     }
 *                 }
 *             } //end of while loop
 *         }
 *         src_state = target_state;
 *     }
 * } //end of for loop
 *
 */
static void Take_Transitions(HSM_Statechart_T *statechart)
{
    uint16_t trans_index;
    HSM_State_Id_T src_state;
    HSM_DbgFunc_T debug_func;

    PBC_Internal(statechart != NULL, "NULL statechart");
    src_state = statechart->current_state;
    debug_func = statechart->dbg.debug_func;

    if (debug_func != NULL)
    {
        debug_func(statechart);
    }

    /* PRQA S 0505 1 *//* Suppress QAC NULL ptr message (checked above). */
    for (trans_index = 0; trans_index < statechart->chain_length; trans_index++)
    {
        HSM_Transition_T const *transition = statechart->trans_chain[trans_index];
        HSM_State_Id_T target_state = Get_Target_State(statechart, transition);

        if (HSM_SAME_STATE == target_state) /* internal transition */
        {
            PBC_Internal_1(1 == statechart->chain_length,
                    "(%s) Chained internal transition",
                    (char*)statechart->def_states.statechart_name);

            if (statechart->dbg.log_level >= TR_LVL_INFO_HI)
            {
                Tr_Info_Hi_4( "%s handling internal transition in state %s for event %d(%s)",
                        (char*)statechart->dbg.chart_label,
                        HSM_Get_State_Name(statechart, src_state),
                        (int)statechart->event,
                        Get_Valid_Event_Name(statechart, statechart->event));
            }

            /* Skip exit and entry action for internal transition */
            if (transition->action != NULL)
            {
                Log_Trace_Action(statechart, transition->action_name);
                transition->action(statechart);
            }
        }
        else
        {
            /* Execute the exit actions, transition action and entry actions */
            HSM_State_Id_T lca;

            lca = Get_LeastCommonAncestor(statechart, src_state, target_state);

            if (statechart->dbg.log_level >= TR_LVL_INFO_HI)
            {
                Tr_Info_Hi_4("%s transitioning into state %s for event %d(%s)",
                        (char*)statechart->dbg.chart_label,
                        HSM_Get_State_Name(statechart, target_state),
                        (int)statechart->event,
                        Get_Valid_Event_Name(statechart, statechart->event));
            }

            Execute_Exit_Actions(statechart, src_state, lca);

            if (transition->action != NULL)
            {
                Log_Trace_Action(statechart, transition->action_name);
                transition->action(statechart);
            }

            Execute_Entry_Actions(statechart, target_state, lca);

            src_state = target_state;
        }

    } /* for (trans_index = 0; trans_index < statechart->chain_length;...) */

    statechart->current_state = src_state;
}

/**
 * Transfers/translates the specified dbg_ctrl structure into the statechart
 * object. A NULL pointer is interpreted in a manner to turn off tracing for
 * the statechart. In addition, a NULL chart_label is converted into the
 * compile-time name generated for the statechart.
 *
 * @param [in] statechart
 *    Data structure describing the statechart.
 *
 * @param [in] dbg_ctrl
 *    Optional pointer to a data structure specifying the debug settings
 *    for the statechart.
 *
 * @pre
 *   - statechart != NULL
 */
static void Initialize_Debug_Control(HSM_Statechart_T *statechart,
        HSM_Debug_Control_T const *dbg_ctrl)
{
    PBC_Internal(statechart != NULL, "NULL statechart");

    if (dbg_ctrl != NULL)
    {
        statechart->dbg = *dbg_ctrl;

        if (NULL == statechart->dbg.chart_label)
        {
            statechart->dbg.chart_label = statechart->def_states.statechart_name;
        }
    }
    else
    {
        memset(&statechart->dbg, 0, sizeof(statechart->dbg));
        statechart->dbg.chart_label = statechart->def_states.statechart_name;
        statechart->dbg.log_level = TR_LVL_WARN;
    }
}

/**
 * Initializes a statechart object prior to its being used to process events.
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
 * @pre
 *   - statechart != NULL
 *   - state_defn != NULL
 *   - state_defn->state_table !- NULL
 *   - state_defn->state_count > 1 (minimum statechart is an initial state
 *                                and a simple state)
 */
static void Initialize_Statechart_Object(HSM_Statechart_T *statechart,
        HSM_State_Defn_T const *state_defn,
        void *this_ptr, HSM_Debug_Control_T const *dbg_ctrl)
{
    PBC_Require(statechart != NULL, "NULL statechart");
    PBC_Require(state_defn != NULL, "NULL state definition");
    /* PRQA S 0505 3 *//* Suppress QAC NULL ptr message (checked above). */
    PBC_Require(state_defn->state_table != NULL, "NULL state table");
    PBC_Require_1(state_defn->state_count > 1,
            "Illegal state count for %s", (char*)state_defn->statechart_name);
    PBC_Require_1(HSM_INITIAL_ID == state_defn->state_table[0].state_type,
            "First state is not an initial state for %s", (char*)state_defn->statechart_name);

    memset(statechart, 0, sizeof(*statechart));
    statechart->event = HSM_NO_EVENT;
    statechart->this_ptr = this_ptr;
    /* PRQA S 0505 1 *//* Suppress QAC NULL ptr message (checked above). */
    statechart->def_states = *state_defn;

    Initialize_Debug_Control(statechart, dbg_ctrl);
}

/**
 * Updates the trace level for the statechart to the level of whatever module
 * the statechart has been told to track. This allows a module's statecharts
 * to keep the same trace level even when they change at run time.
 *
 * @param [in,out] statechart
 *    Data structure describing the statechart.
 *
 * @pre
 *   - statechart != NULL
 */
static void Update_Dbg_Trace_Level(HSM_Statechart_T *statechart)
{
    PBC_Internal(statechart != NULL, "NULL statechart");

    if ((statechart->dbg.dbg_module_id == 0))
    {
        statechart->dbg.log_level = 0;
    }
}

/*===========================================================================*
 *
 * Please refer to the detailed description in hsm_engine.h
 *
 *===========================================================================*/
void HSM_Control_Debug(HSM_Statechart_T *statechart,
        HSM_Debug_Control_T const *dbg_ctrl)
{
    PBC_Require(statechart != NULL, "NULL statechart");

    Initialize_Debug_Control(statechart, dbg_ctrl);
}

/*===========================================================================*
 *
 * Please refer to the detailed description in hsm_engine.h
 *
 *===========================================================================*/
HSM_DbgFunc_T HSM_Get_DbgFunc(HSM_Statechart_T const *statechart)
{
    HSM_DbgFunc_T current_callback;

    PBC_Require(statechart != NULL, "NULL statechart");
    /* PRQA S 0505 1 *//* Suppress QAC NULL ptr message (checked above). */
    current_callback = statechart->dbg.debug_func;

    return current_callback;
}


/*===========================================================================*
 *
 * Please refer to the detailed description in hsm_engine.h
 *
 *===========================================================================*/
HSM_DbgFunc_T HSM_Set_DbgFunc(HSM_Statechart_T *statechart,
        HSM_DbgFunc_T debug_func)
{
    HSM_DbgFunc_T current_callback;

    PBC_Require(statechart != NULL, "NULL statechart");
    /* PRQA S 0505 1 *//* Suppress QAC NULL ptr message (checked above). */
    current_callback = statechart->dbg.debug_func;
    statechart->dbg.debug_func = debug_func;

    return current_callback;
}

/*===========================================================================*
 *
 * Please refer to the detailed description in hsm_engine.h
 *
 *===========================================================================*/
char const *HSM_Get_State_Name(HSM_Statechart_T const *statechart,
        HSM_State_Id_T state)
{
    char const *name = "unknown";
    PBC_Require(statechart != NULL, "NULL statechart");
    PBC_Require(state >= 0, "Negative state");
    PBC_Require_1(state < statechart->def_states.state_count,
            "(%s) State out of range",
            (char*)statechart->def_states.statechart_name);

    const HSM_State_Defn_T *state_defn = &statechart->def_states;
    if (state_defn->state_names != NULL)
    {
        name = state_defn->state_names[state];
        PBC_Ensure_2(name != NULL,
                "(%s) NULL state name of state (%d)",
                (char*)state_defn->statechart_name, state);
    }

    return name;
}

/*===========================================================================*
 *
 * Please refer to the detailed description in hsm_engine.h
 *
 *===========================================================================*/
bool_t HSM_Is_In_State(HSM_Statechart_T const *statechart,
        HSM_State_Id_T state)
{
    bool_t is_in_it = false;

    PBC_Require(statechart != NULL, "NULL statechart");

    /* PRQA S 0505 1 *//* Suppress QAC NULL ptr message (checked above). */
    if ((state >= 0) && (state < statechart->def_states.state_count))
    {
        if (state == statechart->current_state)
        {
            is_in_it = true;
        }
        else if (Is_State_Ancestor(statechart, statechart->current_state, state))
        {
            is_in_it = true;
        }
    }

    return is_in_it;
}

/*===========================================================================*
 *
 * Please refer to the detailed description in hsm_engine.h
 *
 *===========================================================================*/
size_t HSM_Restore(HSM_Statechart_T *statechart,
        uint8_t const *buffer, size_t space_available,
        HSM_State_Defn_T const *state_defn, void *this_ptr,
        HSM_Debug_Control_T const *dbg_ctrl)
{
    uint8_t const * const buffer_start = buffer;
    bool_t is_ok = true;
    size_t size_needed;
    HSM_State_Id_T target;
    /*
     * Establish initial values for all of the statechart object that are not
     * part of the persistent data.
     */
    Initialize_Statechart_Object(statechart, state_defn, this_ptr, dbg_ctrl);

    PBC_Require_1(buffer != NULL, "No buffer for restoring HSM data for %s",
            (char*)statechart->def_states.statechart_name);

    size_needed = HSM_PERSISTENT_SIZE(statechart->def_states.history_state_count);

    if (space_available < size_needed)
    {
        Tr_Warn_3("Context for %s requires %d bytes, but only %d were provided",
                (char*)statechart->dbg.chart_label,
                (int)size_needed, (int)space_available);

        is_ok = false;
    }
    /*
     * Read and check persistent data revision
     */
    if (is_ok)
    {
        uint16_t rev;
        memcpy(&rev, buffer, sizeof(rev));
        buffer += sizeof(rev);

        if (HSM_PERSISTENT_DATA_REVISION != rev)
        {
            Tr_Warn_2("Invalid revision of data for %s: %d", (char*)statechart->dbg.chart_label, (int)rev);
            is_ok = false;
        }
    } /* is_ok */
    /*
     * Read and check current state.
     */
    if (is_ok)
    {
        memcpy(&target, buffer, sizeof(target));
        buffer += sizeof(target);

        if (target < statechart->def_states.state_count)
        {
            statechart->current_state = target;
        }
        else
        {
            Tr_Warn_2("Invalid current state for %s: %d", (char*)statechart->dbg.chart_label, (int)target);
            is_ok = false;
        }
    } /* is_ok */
    /*
     * Read and check history state targets.
     */
    if (is_ok)
    {
        uint16_t history_states_left = statechart->def_states.history_state_count;
        HSM_State_Id_T state_index = 0; /* used to search for history states */

        while ((history_states_left > 0)
                && (state_index < statechart->def_states.state_count)
                && (is_ok))
        {
            /* read in and verify target of next history state */
            memcpy(&target, buffer, sizeof(target));
            buffer += sizeof(target);

            if (target >= statechart->def_states.state_count)
            {
                Tr_Warn_2("Invalid target for history state of %s: %d", (char*)statechart->dbg.chart_label, (int)target);
                is_ok = false;
            }
            else /* Find the next history state and assign target to it */
            {
                while (state_index < statechart->def_states.state_count)
                {
                    HSM_State_Kind_T state_type = statechart->def_states.state_table[state_index].state_type;

                    if ((HSM_DEEP_HISTORY_ID == state_type)
                            || (HSM_SHALLOW_HISTORY_ID == state_type))
                    {
                        *statechart->def_states.state_table[state_index].transition_table->history = target;
                        history_states_left--;
                        state_index++;
                        break; /* out of while */
                    }

                    state_index++;

                } /* while (state_index < statechart->def_states.state_count) */

            } /* if (target >= statechart->def_states.state_count) */

        } /* while ((history_states_left > 0) && ... */

        if (history_states_left != 0)
        {
            Tr_Warn_2("Unable to find %d history states for %s",
                    (int)history_states_left,
                    (char*)statechart->dbg.chart_label);
            is_ok = false;
        }

    } /* if (is_ok) */

    if (is_ok)
    {
        return (size_t) (buffer - buffer_start);
    }
    else
    {
        return 0; /* If any error is encountered, indicate nothing was read */
    }
}

/*===========================================================================*
 *
 * Please refer to the detailed description in hsm_engine.h
 *
 *===========================================================================*/
size_t HSM_Save(HSM_Statechart_T *statechart,
        uint8_t *buffer, size_t space_available)
{
    uint8_t * const buffer_start = buffer;
    size_t size_needed;

    PBC_Require(statechart != NULL, "NULL statechart");
    PBC_Require_1(buffer != NULL, "No buffer to save HSM data for %s",
            (char*)statechart->def_states.statechart_name);

    size_needed = HSM_PERSISTENT_SIZE(statechart->def_states.history_state_count);

    if (space_available < size_needed)
    {
        Tr_Warn_3("Context for %s requires %d bytes, but only %d were provided",
                (char*)statechart->dbg.chart_label, (int)size_needed, (int)space_available);
    }
    else
    {
        uint16_t rev = HSM_PERSISTENT_DATA_REVISION;
        uint16_t history_states_left = statechart->def_states.history_state_count;
        HSM_State_Id_T state_index = 0;
        /*
         * Save persistent data revision, current state.
         */
        memcpy(buffer, &rev, sizeof(rev));
        buffer += sizeof(rev);
        memcpy(buffer, &statechart->current_state, sizeof(statechart->current_state));
        buffer += sizeof(statechart->current_state);
        /*
         * Save all current history state target destinations.
         */
        while ((history_states_left > 0)
                && (state_index < statechart->def_states.state_count))
        {
            HSM_State_Kind_T state_type = statechart->def_states.state_table[state_index].state_type;

            if ((HSM_DEEP_HISTORY_ID == state_type)
                    || (HSM_SHALLOW_HISTORY_ID == state_type))
            {
                HSM_State_Id_T target = *statechart->def_states.state_table[state_index].transition_table->history;

                PBC_Require_1(target < statechart->def_states.state_count, "Illegal history target in %s",
                        (char*)statechart->def_states.statechart_name);

                memcpy(buffer, &target, sizeof(target));
                buffer += sizeof(target);
                history_states_left--;
            }
            state_index++;
        }
        PBC_Ensure_2(0 == history_states_left,
                "Unable to find %d history states for %s",
                (int)history_states_left,
                (char*)statechart->def_states.statechart_name);
        statechart->save_has_occurred = true;
    }

    return (size_t) (buffer - buffer_start);
}

/*===========================================================================*
 *
 * Please refer to the detailed description in hsm_engine.h
 *
 *===========================================================================*/
static void HSM_Init(HSM_Statechart_T *statechart,
        HSM_State_Defn_T const *state_defn,
        void *this_ptr,
        HSM_Debug_Control_T const *dbg_ctrl)
{
    HSM_State_Id_T state;

    Initialize_Statechart_Object(statechart, state_defn, this_ptr, dbg_ctrl);

    if (statechart->dbg.perform_check)
    {
        HSM_Check_Statechart_Defn(state_defn);
    }
    /*
     * Initialize all history states to their defaults, which are found in
     * the transition's target_state field.
     */
    for (state = 0; state < state_defn->state_count; state++)
    {
        if ((HSM_DEEP_HISTORY_ID == state_defn->state_table[state].state_type)
                || (HSM_SHALLOW_HISTORY_ID == state_defn->state_table[state].state_type))
        {
            //init history state to its default state
            *state_defn->state_table[state].transition_table->history =
                state_defn->state_table[state].transition_table->target_state;
        }
    }
}

/*===========================================================================*
 *
 * Please refer to the detailed description in hsm_engine.h
 *
 *===========================================================================*/
static void HSM_Begin(HSM_Statechart_T *statechart)
{
    PBC_Require( (0 == statechart->current_state) && (0 == statechart->previous_state),
            "Illegal attempt call to HSM_Begin() for an active statechart");

    Update_Dbg_Trace_Level(statechart);

    if (statechart->dbg.log_level >= TR_LVL_INFO_HI)
    {
        Tr_Info_Hi_1("%s starting", (char*)statechart->dbg.chart_label);
    }

    Build_Transition_Chain(statechart, HSM_NO_EVENT);

    PBC_Require_1(statechart->chain_length > 0,
            "(%s) No path through initial transition",
            (char*)statechart->def_states.statechart_name);

    Take_Transitions(statechart);
}

/*===========================================================================*
 *
 * Please refer to the detailed description in hsm_engine.h
 *
 *===========================================================================*/
bool_t HSM_Process_Event(HSM_Statechart_T *statechart,
        HSM_Event_T event, void const *event_data)
{
    HSM_DbgFunc_T debug_func;
    bool_t event_handled = false;

    PBC_Require(statechart != NULL, "NULL statechart");
    PBC_Require(event >= 0, "Illegal event value");
    /*
     * If the context for the statechart has been saved via HSM_Save() and not
     * restored via HSM_Restore(), then the saved context may be invalidated
     * by further even delivers. This would mean that a subsequent HSM_Restore()
     * would "throw away" any state changes since the HSM_Save() occurred.
     */
    if (statechart->save_has_occurred)
    {
        Tr_Warn_1("Event delivery to %s invalidates previously saved context", (char*)statechart->dbg.chart_label);
    }

    if (statechart->processing_event)
    {
        Tr_Fault_3( "Illegal recursive call to HSM_Process_Event() for %s while processing %d(%s)",
                (char*)statechart->dbg.chart_label,
                (int)statechart->event,
                Get_Valid_Event_Name(statechart, statechart->event));

        Tr_Fault_3("%s will ignore recursive delivery of %d(%s)",
                (char*)statechart->dbg.chart_label,
                (int)event, Get_Valid_Event_Name(statechart, event));
    }
    else
    {
        statechart->processing_event = true;
        Update_Dbg_Trace_Level(statechart);

        /* PRQA S 0505 1 *//* Suppress QAC NULL ptr message (checked above). */
        statechart->event = event;
        statechart->event_data = event_data;
        statechart->previous_state = statechart->current_state;

        Build_Transition_Chain(statechart, event);

        debug_func = statechart->dbg.debug_func;

        if (statechart->chain_length > 0)
        {
            Take_Transitions(statechart);
            event_handled = true;
        }
        else
        {
            if (debug_func != NULL)
            {
                debug_func(statechart);
            }

            if (statechart->dbg.log_level >= TR_LVL_INFO_LO)
            {
                Tr_Info_Lo_4("%s discarded event %d(%s) in state %s",
                        (char*)statechart->dbg.chart_label,
                        (int)statechart->event,
                        Get_Valid_Event_Name(statechart, statechart->event),
                        HSM_Get_State_Name(statechart, statechart->current_state));
            }
        }
        statechart->processing_event = false;
    }

    return event_handled;
}

/*===========================================================================*
 *
 * Please refer to the detailed description in hsm_engine.h
 *
 *===========================================================================*/
void HSM_Start(HSM_Statechart_T *statechart,
        HSM_State_Defn_T const *state_defn,
        void *this_ptr, const HSM_Debug_Control_T *dbg_ctrl)
{
    PBC_Require(state_defn != NULL, "NULL state definition");
#if 0
    HSM_Debug_Control_T hsm_start_dbg = { 0 };

    PBC_Require(state_defn != NULL, "NULL state definition");
    hsm_start_dbg.chart_label = state_defn->statechart_name;
    hsm_start_dbg.log_level = dbg_ctrl->log_level;
    hsm_start_dbg.debug_func = dbg_ctrl->debug_func;
    hsm_start_dbg.perform_check = dbg_ctrl->perform_check;
#endif

#ifdef HSM_START_DEPRECATED
    Tr_Warn("HSM_Start() has been deprecated. Replace with HSM_Init() and HSM_Begin().");
#endif
    HSM_Init(statechart, state_defn, this_ptr, dbg_ctrl);
    HSM_Begin(statechart);
}

/*===========================================================================*/

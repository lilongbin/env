/*===========================================================================*/
/**
 * @file hsm_check.c
 *
 * This filed contains logic that checks a statechart definition for errors.
 *
 * %full_filespec:hsm_check.c~kok_basa#12:csrc:kok_aud#1 %
 * @version %version:kok_basa#12 %
 * @author  %derived_by:dzq92s %
 * @date    %date_modified:Sun Jul 25 14:40:51 2010 %
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
 *   This file contains the runtime logic that checks the statechart definition
 *   provided to the statechart engine for errors that cannot be detected at
 *   compile time.
 *
 * @section ABBR ABBREVIATIONS:
 *   - hsm = Hierarchical State Machine.
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
 * @ingroup hsm_engine
 * @{
 */
/*==========================================================================*/

/*===========================================================================*
 * Header Files
 *===========================================================================*/
#include "hsm_check.h"
#include "hsm_engine_cfg.h"
//#include "pbc_trace.h"

/*===========================================================================*
 * Local Preprocessor #define Constants
 *===========================================================================*/

/*===========================================================================*
 * Local Preprocessor #define MACROS
 *===========================================================================*/

//EM_FILENUM(HSM_ENGINE_MODULE_ID, 2);    /**< define file for assert handling */

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
static void Check_Composite_State_Defn(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T state);

static void Check_Default_History_State(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T src_state,
        HSM_State_Kind_T src_type,
        const HSM_Transition_T *trans);

static void Check_Final_State_Defn(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T state);

static void Check_For_Valid_Parent(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T state);

static void Check_History_State_Defn(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T state);

static void Check_Incoming_Transitions(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T state);

static void Check_Initial_State_Defn(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T state);

static void Check_Initial_Target(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T src_state,
        const HSM_Transition_T *trans);

static void Check_Junction_State_Defn(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T state);

static void Check_Nesting_Constraint(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T state);

static void Check_Outgoing_Transitions(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T state);

static void Check_Simple_State_Defn(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T state);

static void Check_Target(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T src_state,
        const HSM_Transition_T *trans);

static void Check_Targeted_Composite(HSM_State_Defn_T const *state_defn,
        const HSM_Transition_T *trans);

static void Check_Targeted_Final(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T src_state,
        const HSM_Transition_T *trans);

static void Check_Targeted_History(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T src_state,
        const HSM_Transition_T *trans);
static bool_t Check_Is_Ancestor_State(HSM_State_Defn_T const *p_states,
                           HSM_State_Id_T ref_st, HSM_State_Id_T confirming_st);

static char const *Get_State_Name_From_Defn(HSM_State_Defn_T const *state_defn,
                                         HSM_State_Id_T state);

static uint16_t Get_Nesting_Level(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T state);

/*===========================================================================*
 * Local Inline Function Definitions and Function-Like Macros
 *===========================================================================*/

/*===========================================================================*
 * Function Definitions
 *===========================================================================*/

/**
 * Checks the definition of a composite state.
 *
 * @param [in] state_defn Points to structure defining def_states of the statechart.
 *
 * @param [in] state Index to the state's position in the state_table.
 */
static void Check_Composite_State_Defn(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T state)
{
    const HSM_State_T *ptr_st = &state_defn->state_table[state];
    HSM_State_Id_T history_state = ptr_st->history_state;
    HSM_State_Id_T initial_state = ptr_st->initial_state;
    HSM_State_Id_T parent_state = ptr_st->parent_state;

    Check_For_Valid_Parent(state_defn, state);
    /*
     * Make sure the composite's initial state is legal value.
     */
    PBC_Require_3((HSM_NO_INITIAL_STATE == initial_state) || 
            ((initial_state > 0) && (initial_state < state_defn->state_count)),
            "(%s) Illegal initial state for composite state '%s' (%d)",
            (char*)state_defn->statechart_name,
            (char*)Get_State_Name_From_Defn(state_defn, state), (int)state
            );

    if (initial_state != HSM_NO_INITIAL_STATE)
    {
        /*
         * Make sure any state designated the initial state of this composite
         * state is in fact an initial state and has this composite as its
         * parent.
         */
        HSM_State_Kind_T initial_type =
            state_defn->state_table[initial_state].state_type;
        PBC_Require_3(HSM_INITIAL_ID == initial_type,
                "(%s) Initial state for composite state '%s' (%d) is of wrong type",
                (char*)state_defn->statechart_name,
                (char*)Get_State_Name_From_Defn(state_defn, state), (int)state
                );
        PBC_Require_3(state_defn->state_table[initial_state].parent_state == state,
                "(%s) Parent of initial state '%s' (%d) conflicts with its usage",
                (char*)state_defn->statechart_name,
                (char*)Get_State_Name_From_Defn(state_defn, initial_state),
                (int)initial_state
                );
    }
    else if (parent_state != HSM_TOP)
    {
        /*
         * If a composite state does not have an initial state, then its parent
         * cannot have a shallow history state, because this would be an
         * implied transition to the composite state.
         */
        HSM_State_Id_T parent_hist =
            state_defn->state_table[parent_state].history_state;

        PBC_Require_3((HSM_NO_HISTORY_STATE == parent_hist) ||
                (HSM_DEEP_HISTORY_ID == 
                 state_defn->state_table[parent_hist].state_type),
                "(%s) Composite state '%s' (%d) has no initial state, but its parent has shallow history",
                (char*)state_defn->statechart_name,
                (char*)Get_State_Name_From_Defn(state_defn, state), (int)state
                );
    }
    /*
     * Make sure history state is legal value.
     */
    PBC_Require_3((HSM_NO_HISTORY_STATE == history_state) || 
            ((history_state > 0) && (history_state < state_defn->state_count)),
            "(%s) Illegal history state value for state %s (%d)",
            (char*)state_defn->statechart_name,
            (char*)Get_State_Name_From_Defn(state_defn, state), (int)state
            );
    /*
     * Make sure any state designated the history state of this composite state
     * is in fact a history state and has this composite as its parent.
     */
    if (history_state != HSM_NO_HISTORY_STATE)
    {
        HSM_State_Kind_T history_type =
            state_defn->state_table[history_state].state_type;
        PBC_Require_3((HSM_DEEP_HISTORY_ID == history_type) ||
                (HSM_SHALLOW_HISTORY_ID == history_type),
                "(%s) Composite state '%s' (%d) has history state with wrong type",
                (char*)state_defn->statechart_name,
                (char*)Get_State_Name_From_Defn(state_defn, state), (int)state
                );
        PBC_Require_3(state_defn->state_table[history_state].parent_state == state,
                "(%s) The history state for composite state '%s' (%d) specifies a different parent",
                (char*)state_defn->statechart_name,
                (char*)Get_State_Name_From_Defn(state_defn, state), (int)state
                );
    }
}

/**
 * Checks the default target of a history state to make sure it is valid.
 *
 * @param [in] state_defn Points to structure defining def_states of the statechart.
 *
 * @param [in] src_state The (history) state that is the source transition.
 *
 * @param [in] src_type The type of history state (deep or shallow).
 *
 * @param [in] trans The transition whose target state is to be checked.
 */
static void Check_Default_History_State(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T src_state,
        HSM_State_Kind_T src_type,
        const HSM_Transition_T *trans)
{
    HSM_State_Id_T parent_of_hist;
    parent_of_hist = state_defn->state_table[src_state].parent_state;

    /*
     * A history state's default target must be inside its parent state.
     */
    PBC_Require_3(Check_Is_Ancestor_State(state_defn, trans->target_state, parent_of_hist),
            "(%s) Default target of history state '%s' (%d) is outside its parent state",
            (char*)state_defn->statechart_name,
            (char*)Get_State_Name_From_Defn(state_defn, src_state), (int)src_state
            );

    if (HSM_SHALLOW_HISTORY_ID == src_type)
    {
        HSM_State_Id_T parent_of_target;

        parent_of_target =
            state_defn->state_table[trans->target_state].parent_state;
        /*
         * Because a shallow history state targets another state at its level,
         * so should its default transition.
         */
        PBC_Require_3(parent_of_hist == parent_of_target,
                "(%s) Default target of history state '%s' (%d) is at wrong level",
                (char*)state_defn->statechart_name,
                (char*)Get_State_Name_From_Defn(state_defn, src_state), (int)src_state
                );
    }
}

/**
 * Checks the definition of a final state.
 *
 * @param [in] state_defn Points to structure defining def_states of the statechart.
 *
 * @param [in] state Index to the state's position in the state_defn->state_table.
 */
static void Check_Final_State_Defn(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T state)
{
    const HSM_State_T *ptr_st = &state_defn->state_table[state];
    HSM_State_Id_T parent = ptr_st->parent_state;

    Check_For_Valid_Parent(state_defn, state);

    /* Currently, a final state is not allowed at the top of a statechart. */
    PBC_Require_3(parent != HSM_TOP,
            "(%s) Final state '%s' (%d) not allowed at top of statechart",
            (char*)state_defn->statechart_name,
            (char*)Get_State_Name_From_Defn(state_defn, state), (int)state
            );
    /*
     * The parent of the final state is required to have a completion transition
     * as its first transition.
     */
    if (parent != HSM_TOP)
    {
        HSM_Event_T event;
        HSM_Transition_T const *transitions;

        transitions = state_defn->state_table[parent].transition_table;
        PBC_Require_3(transitions != NULL,
                "(%s) State '%s' (%d) must have a completion transition",
                (char*)state_defn->statechart_name,
                (char*)Get_State_Name_From_Defn(state_defn, parent), (int)parent
                );

        event = transitions->event;

        PBC_Require_3(HSM_COMPLETION_EVENT == event,
                "(%s) First transition from state '%s' (%d) must be a completion transition",
                (char*)state_defn->statechart_name,
                (char*)Get_State_Name_From_Defn(state_defn, parent), (int)parent
                );
    }
}

/**
 * Makes sure a parent state is a composite state.
 *
 * @param [in] state_defn Points to structure defining def_states of the statechart.
 *
 * @param [in] state Index to the state's position in the state_defn->state_table.
 */
static void Check_For_Valid_Parent(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T state)
{
    const HSM_State_T *ptr_st = &state_defn->state_table[state];
    HSM_State_Id_T parent = ptr_st->parent_state;

    if (parent != HSM_TOP)
    {
        HSM_State_Kind_T parent_type;

        PBC_Require_3((parent > 0) && (parent < state_defn->state_count),
                "(%s) Illegal parent for state '%s' (%d)",
                (char*)state_defn->statechart_name,
                (char*)Get_State_Name_From_Defn(state_defn, state), (int)state
                );
        parent_type = state_defn->state_table[ptr_st->parent_state].state_type;

        PBC_Require_3(HSM_COMPOSITE_ID == parent_type,
                "(%s) Type of parent specified for '%s' (%d) conflicts with usage",
                (char*)state_defn->statechart_name,
                (char*)Get_State_Name_From_Defn(state_defn, state), (int)state
                );
    }
}

/**
 * Checks the definition of a history state.
 *
 * @param [in] state_defn Points to structure defining def_states of the statechart.
 *
 * @param [in] state Index to the state's position in the state_defn->state_table.
 */
static void Check_History_State_Defn(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T state)
{
    const HSM_State_T *ptr_st = &state_defn->state_table[state];

    Check_For_Valid_Parent(state_defn, state);
    /*
     * The top state cannot have a history state.
     */
    PBC_Require_3(ptr_st->parent_state != HSM_TOP,
            "(%s) History state '%s' (%d) not allowed at top of chart",
            (char*)state_defn->statechart_name,
            (char*)Get_State_Name_From_Defn(state_defn, state), (int)state
            );
    /*
     * Make sure the parent has this state listed as its history state.
     */
    PBC_Require_3(state == 
            state_defn->state_table[ptr_st->parent_state].history_state,   
            "(%s) Parent of history state '%s' (%d) does not reference it",
            (char*)state_defn->statechart_name,
            (char*)Get_State_Name_From_Defn(state_defn, state), (int)state
            );
}

/**
 * Checks the incoming transitions of a state.
 *
 * @param [in] state_defn Points to structure defining def_states of the statechart.
 *
 * @param [in] state The state whose incoming transitions are to be checked.
 */
static void Check_Incoming_Transitions(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T state)
{
    HSM_State_Id_T src_state;
    const HSM_State_T *ptr_st;
    bool_t was_targeted = false;
    /*
     * Every state should be the explicit target of at least one transition with
     * the following exceptions: 1) Initial states, 2) Composite states that a) do
     * not have an initial state or b) whose parents don't have shallow history.
     */
    for (src_state = 0;
            (!was_targeted) && (src_state < state_defn->state_count);
            src_state++)
    {
        uint16_t i;

        ptr_st = &state_defn->state_table[src_state];

        for (i = 0; (!was_targeted) && (i < ptr_st->transition_count); i++)
        {
            const HSM_Transition_T *trans = &ptr_st->transition_table[i];

            if (state == trans->target_state)
            {
                was_targeted = true;
            }
        }

    } /* for (src_state = 0 ... )                                             */

    if (!was_targeted)
    {
        ptr_st = &state_defn->state_table[state];

        if ((HSM_INITIAL_ID != ptr_st->state_type) &&
                (HSM_COMPOSITE_ID != ptr_st->state_type))
        {
            /*
             * Any non-initial, non-composite state should have an incoming
             * transition. This is issued as a "warning", not an error since
             * it will not necessarily lead to incorrect behavior. It is possible
             * that the state is simply a "place holder" for behavior to be
             * implemented in the future.
             */
            Tr_Warn_3("(%s) No incoming transition for state '%s' (%d)",
                    (char*)state_defn->statechart_name,
                    (char*)Get_State_Name_From_Defn(state_defn, state), (int)state
                    );
        }
        else if (HSM_COMPOSITE_ID == ptr_st->state_type)
        {
            HSM_State_Id_T init_state = ptr_st->initial_state;
            HSM_State_Id_T parent_hist = HSM_NO_HISTORY_STATE;
            /*
             * A composite should have an incoming transition if:
             *   1) It has an initial state  - AND -
             *   2) Its parent does NOT have a shallow history state.
             * Instead of treating this as an "error", a trace message is
             * issued since the initial state may be there by convention
             * and is causing no harm.
             */
            if (ptr_st->parent_state != HSM_TOP)
            {
                parent_hist =
                    state_defn->state_table[ptr_st->parent_state].history_state;
            }

            if ((init_state != HSM_NO_INITIAL_STATE) &&
                    ((HSM_NO_HISTORY_STATE == parent_hist) ||
                     (HSM_SHALLOW_HISTORY_ID !=
                      state_defn->state_table[parent_hist].state_type)))
            {
                Tr_Warn_3("(%s) Unnecessary initial state for composite state '%s' (%d)",
                        (char*)state_defn->statechart_name,
                        (char*)Get_State_Name_From_Defn(state_defn, state), (int)state
                        );
            }
        }
    }
    /* PRQA S 4700 1 */ /* Suppress QAC STAV3 metric message. */
}

/**
 * Checks the definition of an initial state.
 *
 * @param [in] state_defn Points to structure defining def_states of the statechart.
 *
 * @param [in] state Index to the state's position in the state_defn->state_table.
 */
static void Check_Initial_State_Defn(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T state)
{
    const HSM_State_T *ptr_st = &state_defn->state_table[state];

    Check_For_Valid_Parent(state_defn, state);

    if (0 == state)
    {
        /*
         * The first state in the table must be the intial state for the
         * entire statechart; its parent must be HSM_TOP.
         */
        PBC_Require_1(HSM_TOP == ptr_st->parent_state,
                "(%s) First state must be initial state for chart",
                (char*)state_defn->statechart_name);
    }
    else
    {
        /*
         * Make sure the parent has this state listed as its initial state.
         */
        PBC_Require_3(HSM_TOP != ptr_st->parent_state,
                "(%s) Initial state '%s' (%d) cannot be for top of statechart",
                (char*)state_defn->statechart_name,
                (char*)Get_State_Name_From_Defn(state_defn, state), (int)state
                );
        PBC_Require_3(state == 
                state_defn->state_table[ptr_st->parent_state].initial_state, 
                "(%s) Parent specified for initial state '%s' (%d) does not reference it",
                (char*)state_defn->statechart_name,
                (char*)Get_State_Name_From_Defn(state_defn, state), (int)state
                ); 
    }
}

/**
 * Checks the target of an initial state to make sure it is valid.
 *
 * @param [in] state_defn Points to structure defining def_states of the statechart.
 *
 * @param [in] src_state The initial state that is the source transition.
 *
 * @param [in] trans The transition whose target state is to be checked.
 */
static void Check_Initial_Target(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T src_state,
        const HSM_Transition_T *trans)
{
    HSM_State_Id_T parent_of_init;
    parent_of_init = state_defn->state_table[src_state].parent_state;
    /*
     * An initial state should not target its parent or a state not within
     * its parent.
     */
    PBC_Require_3(Check_Is_Ancestor_State(state_defn, trans->target_state, parent_of_init),
            "(%s) Illegal target for initial state '%s' (%d)",
            (char*)state_defn->statechart_name,
            (char*)Get_State_Name_From_Defn(state_defn, src_state), (int)src_state
            );
}

/**
 * Checks the definition of a junction state.
 *
 * @param [in] state_defn Points to structure defining def_states of the statechart.
 *
 * @param [in] state Index to the state's position in the state_defn->state_table.
 */
static void Check_Junction_State_Defn(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T state)
{
    Check_For_Valid_Parent(state_defn, state);
}

/**
 * Makes sure that the nesting level of states does not exceed
 * HSM_MAX_NESTING_LEVELS.
 *
 * @param [in] state_defn Points to structure defining def_states of the statechart.
 *
 * @param [in] state The state whose nesting level is to be checked.
 */
static void Check_Nesting_Constraint(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T state)
{
    uint16_t max_nesting_level = 0;
    const HSM_State_T *ptr_st = &state_defn->state_table[state];

    if (HSM_COMPOSITE_ID == ptr_st->state_type)
    {
        uint16_t nesting_level;

        nesting_level = Get_Nesting_Level(state_defn, state);    
        /*
         * Only the composite states have to be checked as long as the
         * most deeply nested composite state's level is less than
         * HSM_MAX_NESTING_LEVELS.
         */
        PBC_Require_3(nesting_level < HSM_MAX_NESTING_LEVELS,
                "(%s) State '%s' (%d) exceeds maximum nesting level",
                (char*)state_defn->statechart_name,
                (char*)Get_State_Name_From_Defn(state_defn, state), (int)state
                );

        if (nesting_level > max_nesting_level)
        {
            max_nesting_level = nesting_level;
        }
    }
}

/**
 * Checks the outgoing transitions of a state.
 *
 * @param [in] state_defn Points to structure defining def_states of the statechart.
 *
 * @param [in] state The state whose outgoing transitions are to be checked.
 */
static void Check_Outgoing_Transitions(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T state)
{
    uint16_t i;
    uint16_t j;
    const HSM_State_T *ptr_st = &state_defn->state_table[state];
    HSM_State_Kind_T state_type = ptr_st->state_type;
    uint16_t duplicates;
    /*
     * All states except final states must have at least one outgoing
     * transition.
     */
    if (HSM_FINAL_ID == ptr_st->state_type)
    {
        /* PRQA S 0306 1 */ /* Suppress QAC cast message. */
        PBC_Require_3(NULL == ptr_st->transition_table,
                "(%s) Final state '%s' (%d) can't have outgoing transitions",
                (char*)state_defn->statechart_name,
                (char*)Get_State_Name_From_Defn(state_defn, state), (int)state
                );
        PBC_Require_3(0 == ptr_st->transition_count,
                "(%s) Final state '%s' (%d) must have zero outgoing transitions",
                (char*)state_defn->statechart_name,
                (char*)Get_State_Name_From_Defn(state_defn, state), (int)state
                );
    }
    else
    {
        PBC_Require_3(NULL != ptr_st->transition_table,
                "(%s) State '%s' (%d) must have outgoing transitions",
                (char*)state_defn->statechart_name,
                (char*)Get_State_Name_From_Defn(state_defn, state), (int)state
                );
        PBC_Require_3(ptr_st->transition_count > 0,
                "(%s) State '%s' (%d) must have at least one outgoing transition",
                (char*)state_defn->statechart_name,
                (char*)Get_State_Name_From_Defn(state_defn, state), (int)state
                );
    }

    for (i = 0; i < ptr_st->transition_count; i++)
    {
        const HSM_Transition_T *trans = &ptr_st->transition_table[i];

        if (HSM_COMPLETION_EVENT == trans->event)
        {
            Check_Target(state_defn, state, trans);
            /* Completion transitions are only supported for composite states.*/
            PBC_Require_3(state_type == HSM_COMPOSITE_ID,
                    "(%s) State '%s' (%d) not allowed to have a completion transition",
                    (char*)state_defn->statechart_name,
                    (char*)Get_State_Name_From_Defn(state_defn, state), (int)state
                    );
            /* Completion transitions are required to be first in the list */
            PBC_Require_3(0 == i,
                    "(%s) Completion transition fo state '%s' (%d) must be the first transition",
                    (char*)state_defn->statechart_name,
                    (char*)Get_State_Name_From_Defn(state_defn, state), (int)state
                    );
        }
        else if (trans->event != HSM_NO_EVENT)
        {
            Check_Target(state_defn, state, trans);
            /*
             * Only transitions from "real" states can be triggered by a "real"
             * event.
             */
            PBC_Require_3((state_type == HSM_COMPOSITE_ID) || (state_type == HSM_SIMPLE_ID),
                    "(%s) Transitions from state '%s' (%d) cannot be triggered by events",
                    (char*)state_defn->statechart_name,
                    (char*)Get_State_Name_From_Defn(state_defn, state), (int)state
                    );
        }
        else /* trans->event == HSM_NO_EVENT */
        {
            if (HSM_NO_STATE == trans->target_state)
            {
                /*
                 * HSM_NULL_TRANSITION is only for use to satisfy the
                 * requirement that every state have at least one entry
                 * in its transition_table.
                 */
                /* PRQA S 0305 1 */ /* Suppress QAC cast message. */
                PBC_Require_3(HSM_NO_GUARD == trans->guard,
                        "(%s) No guard allowed for dummy transition from state '%s' (%d)",
                        (char*)state_defn->statechart_name,
                        (char*)Get_State_Name_From_Defn(state_defn, state), (int)state
                        );
                PBC_Require_3(1 == ptr_st->transition_count,
                        "(%s) Only one dummy transition allowed from state '%s' (%d)",
                        (char*)state_defn->statechart_name,
                        (char*)Get_State_Name_From_Defn(state_defn, state), (int)state
                        );
            }
            else
            {
                /*
                 * Only transitions from pseudostates should be associated
                 * with HSM_NO_EVENT; transitions from composite and simple
                 * states must be triggered by a "real" event.
                 */
                PBC_Require_3((state_type != HSM_COMPOSITE_ID) && (state_type != HSM_SIMPLE_ID),
                        "(%s) Transitions from state '%s' (%d) must be associated with an event",
                        (char*)state_defn->statechart_name,
                        (char*)Get_State_Name_From_Defn(state_defn, state), (int)state
                        );
                Check_Target(state_defn, state, trans);
            } 
        } /* if (trans->event != HSM_NO_EVENT) */
        /*
         * Check for more than one transition with the same event and the
         * same guard (which may be an ELSE).
         */
        duplicates = 0;

        for (j = 0; j < ptr_st->transition_count; j++)
        {
            if ((trans->guard == ptr_st->transition_table[j].guard) && 
                    (trans->event == ptr_st->transition_table[j].event))
            {
                duplicates++;
            }
        }

        PBC_Require_3(duplicates == 1,
                "(%s) Duplicate transitions from state '%s' (%d)",
                (char*)state_defn->statechart_name,
                (char*)Get_State_Name_From_Defn(state_defn, state), (int)state
                );

    } /* for (i = 0; i < ptr_st->transition_count; i++) */

    /* PRQA S 4700 1 */ /* Suppress QAC STMIF metric message. */
}

/**
 * Checks the definition of a simple state.
 *
 * @param [in] state_defn Points to structure defining def_states of the statechart.
 *
 * @param [in] state Index to the state's position in the state_defn->state_table.
 */
static void Check_Simple_State_Defn(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T state)
{
    Check_For_Valid_Parent(state_defn, state);
}

/**
 * Checks the target of a transition to make sure it is valid.
 *
 * @param [in] state_defn Points to structure defining def_states of the statechart.
 *
 * @param [in] src_state The state that is the source transition.
 *
 * @param [in] trans The transition whose target state is to be checked.
 */
static void Check_Target(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T src_state,
        const HSM_Transition_T *trans)
{
    HSM_State_Kind_T src_type;
    /*
     * Transitions must target a valid state and cannot target an initial state.
     */
    PBC_Require_3((HSM_SAME_STATE == trans->target_state) ||
            ((trans->target_state > 0) &&
             (trans->target_state < state_defn->state_count)),
            "(%s) Illegal target for transition from state '%s' (%d)",
            (char*)state_defn->statechart_name,
            (char*)Get_State_Name_From_Defn(state_defn, src_state), (int)src_state
            );

    src_type = state_defn->state_table[src_state].state_type;

    if (HSM_INITIAL_ID == src_type)
    {
        Check_Initial_Target(state_defn, src_state, trans);
    }
    else if ((HSM_DEEP_HISTORY_ID == src_type) ||
            (HSM_SHALLOW_HISTORY_ID == src_type))
    {
        Check_Default_History_State(state_defn, src_state, src_type, trans);
    }

    if (trans->target_state != HSM_SAME_STATE)
    {
        HSM_State_Kind_T target_type;

        target_type = state_defn->state_table[trans->target_state].state_type;

        PBC_Require_3(target_type != HSM_INITIAL_ID,
                "(%s) Initial state cannot be target for transition from state '%s' (%d)",
                (char*)state_defn->statechart_name,
                (char*)Get_State_Name_From_Defn(state_defn, src_state), (int)src_state
                );

        switch (target_type)
        {
            case HSM_COMPOSITE_ID:
                Check_Targeted_Composite(state_defn, trans);
                break;

            case HSM_DEEP_HISTORY_ID:
            case HSM_SHALLOW_HISTORY_ID:
                Check_Targeted_History(state_defn, src_state, trans);
                break;

            case HSM_FINAL_ID:
                Check_Targeted_Final(state_defn, src_state, trans);
                break;

            default:
                break;

        } /* switch (trans->target_state) */

    } /* if (trans->target_state != HSM_SAME_STATE) */
}

/**
 * Checks the composite state that is the target of a transition.
 *
 * @param [in] state_defn Points to structure defining def_states of the statechart.
 *
 * @param [in] trans The transition whose target state is to be checked.
 */
static void Check_Targeted_Composite(HSM_State_Defn_T const *state_defn,
        const HSM_Transition_T *trans)
{
    /*
     * A targeted composite state must have an initial state.
     */
    HSM_State_Id_T target = trans->target_state;
    HSM_State_Id_T initial;

    initial = state_defn->state_table[target].initial_state;

    PBC_Require_3(initial > 0,
            "(%s) Targeted composite state '%s' (%d) must have an initial state",
            (char*)state_defn->statechart_name,
            (char*)Get_State_Name_From_Defn(state_defn, target), (int)target
            );
}

/**
 * Checks the final state that is the target of a transition.
 *
 * @param [in] state_defn Points to structure defining def_states of the statechart.
 *
 * @param [in] src_state The state that is the source transition.
 *
 * @param [in] trans The transition whose target state is to be checked.
 */
static void Check_Targeted_Final(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T src_state,
        const HSM_Transition_T *trans)
{
    /*
     * The source of a transition to a final state cannot be outside
     * the composite state containing the final state.
     */
    HSM_State_Id_T parent_of_target;
    parent_of_target = state_defn->state_table[trans->target_state].parent_state;
    /* PRQA S 3415 1 */ /* Suppress QAC right hand side effects message. */
    PBC_Require_3((src_state == parent_of_target)
            || (Check_Is_Ancestor_State(state_defn, src_state, parent_of_target)),
            "(%s) A transition from state '%s' (%d) targets a final state outside its scope",
            (char*)state_defn->statechart_name,
            (char*)Get_State_Name_From_Defn(state_defn, src_state), (int)src_state
            );
    /*
     * A completion transition for a composite state cannot target that
     * same composite state's final state because that creates an infinite
     * loop.
     */
    PBC_Require_3((src_state != parent_of_target) || (trans->event != HSM_COMPLETION_EVENT),
            "(%s) Completion transition from state '%s' (%d) targets its final state",
            (char*)state_defn->statechart_name,
            (char*)Get_State_Name_From_Defn(state_defn, src_state), (int)src_state
            );
    /*
     * The first transition from a composite state containing a final
     * state must be a completion transition.
     */
    PBC_Require_3(HSM_COMPLETION_EVENT ==
            state_defn->state_table[parent_of_target].transition_table[0].event,
            "(%s) First transition from state '%s' (%d) must be a completion transition",
            (char*)state_defn->statechart_name,
            (char*)Get_State_Name_From_Defn(state_defn, parent_of_target), (int)parent_of_target
            );
}

/**
 * Checks the history state that is the target of a transition.
 *
 * @param [in] state_defn Points to structure defining def_states of the statechart.
 *
 * @param [in] src_state The state that is the source transition.
 *
 * @param [in] trans The transition whose target state is to be checked.
 */
static void Check_Targeted_History(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T src_state,
        const HSM_Transition_T *trans)
{
    HSM_State_Id_T parent_of_hist;

    parent_of_hist = state_defn->state_table[trans->target_state].parent_state;
    /*
     * The source of the transition to a history state cannot be the
     * composite state enclosing the history state or within it (unless it
     * is the composite's initial state).
     */
    PBC_Require_3(parent_of_hist != src_state,
            "(%s) Transitions from composite state '%s' (%d) cannot target its history state",
            (char*)state_defn->statechart_name,
            (char*)Get_State_Name_From_Defn(state_defn, src_state), (int)src_state
            );
    if (Check_Is_Ancestor_State(state_defn, src_state, parent_of_hist))
    {
        PBC_Require_3(src_state == state_defn->state_table[parent_of_hist].initial_state,
                "(%s) State '%s' (%d) cannot target the history state that tracks it",
                (char*)state_defn->statechart_name,
                (char*)Get_State_Name_From_Defn(state_defn, src_state), (int)src_state
                );
    }
}

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
static bool_t Check_Is_Ancestor_State(HSM_State_Defn_T const *state_defn, HSM_State_Id_T ref_st, HSM_State_Id_T confirming_st)
{
    bool_t is_ancestor = false;

    PBC_Internal(state_defn != NULL, "NULL state definition");
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
 * Returns the name of the specified state in the statechart.
 *
 * @return A pointer to a string containing the state's name.
 *
 * @note
 *   State names are only available for a statechart if it is compiled with
 *   the appropriate option (HSM_GENERATE_STATE_NAMES); otherwise, the
 *   names returned for its states will be the string "unknown".
 *
 * @param [in] state_defn
 *   HSM_State_Defn_T that contains the definitions of the states..
 *
 * @param [in] state
 *   The state index (from the statechart's enumeration of states) of
 *   the state whose name is to be returned.
 *
 * @pre
 *   - state_defn != NULL
 *   - state >= 0
 *   - state < number of states in statechart
 *
 * @post
 *   - pointer returned != NULL
 */
static char const *Get_State_Name_From_Defn(HSM_State_Defn_T const *state_defn, HSM_State_Id_T state)
{
    char const *name = "unknown";

    PBC_Internal(state_defn != NULL, "NULL state definition");
    PBC_Internal(state >= 0, "Negative state value");
    PBC_Internal_1(state < state_defn->state_count, "(%s) State out of range", (char*)state_defn->statechart_name);

    if (state_defn->state_names != NULL)
    {
        name = state_defn->state_names[state];
        PBC_Ensure_1(name != NULL, "(%s) NULL state name", (char*)state_defn->statechart_name);
    }

    return name;
}

/**
 * Returns the nesting level of the specified state. States at the top
 * of the statechart (parent = HSM_TOP) have a nesting level of one (1),
 * substates have a nesting level one greater than their parent state.
 *
 * @return the nesting level (1..HSM_MAX_NESTING_LEVELS) of the state.
 *
 * @param [in] state_defn Points to structure defining def_states of the statechart.
 *
 * @param [in] state
 *    State within the statechart for which to calculate the nesting level.
 *
 * @post
 *   - nesting level <= HSM_MAX_NESTING_LEVELS
 */
static uint16_t Get_Nesting_Level(HSM_State_Defn_T const *state_defn,
        HSM_State_Id_T state)
{
    uint16_t nesting_level = 1;
    HSM_State_Id_T parent_state = state_defn->state_table[state].parent_state;

    while (parent_state != HSM_TOP)
    {
        parent_state = state_defn->state_table[parent_state].parent_state;
        nesting_level++;
    }

    PBC_Require_3(nesting_level <= HSM_MAX_NESTING_LEVELS,
            "(%s) State '%s' (%d) exceeds maximum nesting level",
            (char*)state_defn->statechart_name,
            (char*)Get_State_Name_From_Defn(state_defn, state), (int)state
            );

    return nesting_level;
}

/*===========================================================================*
 *
 * Please refer to the detailed description in hsm_engine.h
 *
 *===========================================================================*/
void HSM_Check_Statechart_Defn(HSM_State_Defn_T const *state_defn)
{
    HSM_State_Id_T state;

    /* PRQA S 0306 2 */ /* Suppress QAC cast message. */
    PBC_Require(state_defn != NULL, "NULL state definition");
    /* PRQA S 0505 2 */ /* Suppress QAC NULL ptr message (checked above). */
    PBC_Require(state_defn->state_table != NULL, "NULL state table");
    PBC_Require_1(state_defn->state_count > 1, "(%s) Invalid state count",
            (char*)state_defn->statechart_name);
    PBC_Require_1(HSM_INITIAL_ID == state_defn->state_table[0].state_type,
            "(%s) First state in table not an initial state",
            (char*)state_defn->statechart_name);

    for (state = 0; state < state_defn->state_count; state++)
    {
        switch (state_defn->state_table[state].state_type)
        {
            case HSM_COMPOSITE_ID:
                Check_Composite_State_Defn(state_defn, state);
                break;

            case HSM_DEEP_HISTORY_ID:
            case HSM_SHALLOW_HISTORY_ID:
                Check_History_State_Defn(state_defn, state);
                break;

            case HSM_FINAL_ID:
                Check_Final_State_Defn(state_defn, state);
                break;

            case HSM_INITIAL_ID:
                Check_Initial_State_Defn(state_defn, state);
                break;

            case HSM_JUNCTION_ID:
                Check_Junction_State_Defn(state_defn, state);
                break;

            case HSM_SIMPLE_ID:
                Check_Simple_State_Defn(state_defn, state);
                break;

            default:
                /* illegal state_type */
                PBC_Failed_3("(%s) State '%s' (%d) has an illegal type",
                        (char*)state_defn->statechart_name,
                        (char*)Get_State_Name_From_Defn(state_defn, state), (int)state);
                break;
        } /* switch (state_defn->state_type) */

        Check_Outgoing_Transitions(state_defn, state);

        Check_Incoming_Transitions(state_defn, state);

        Check_Nesting_Constraint(state_defn, state);

    } /* for (state = 0; state < state_defn->state_count; state++)         */

    /* PRQA S 4700 1 */ /* Suppress QAC STCALL metric message. */
}

/*===========================================================================*/
/*!
 * @file hsm_check.c
 *
 * @section RH REVISION HISTORY (top to bottom: last revision to first revision)
 *
 * - 25-Jul-2010 Kirk Bailey Rev 12
 *   - Replaced "bool" with "bool_t".
 *
 * - 18-jun-2009 kirk bailey
 *   -Added statechart name to trace output.
 *
 * - 13-aug-2008 kirk bailey
 *   -Converted from EM to PBC usage.
 *
 * - 21-feb-2008 kirk bailey
 *   - Renamed hsm_Check_Statechart_Defn (moved to public API).
 *   - Changed check for incoming transitions from an error to a warning.
 *
 * - 15-nov-2007 kirk bailey
 *   - Added final states and completion transitions.
 *   - Converted to new Doyxgen format.
 *
 * - 20-aug-2007 kirk bailey
 *   -Fixed QAC MISRA issues.
 *
 * - 08-aug-2007 kirk bailey
 *   - Added logic to make sure HSM_MAX_NESTING_LEVELS constraint is not
 *     violated.
 *   - Changed type usage to be MISRA compliant.
 *
 * - 26-apr-2007 kirk bailey
 *   -Added argument to EM_REQUIRE_FAILED().
 *
 * - 05-apr-2007 kirk bailey
 *   -Improved checks for composite states, transitions, and guards.
 *
 * - 04-apr-2007 kirk bailey
 *   -Created file.
 */
/*===========================================================================*/
/** @} doxygen end group */

/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* 
* File name   : hsm_check.h
* Author      : longbin
* Created date: 2020-08-31 17:06:00
* Description : 
*
*******************************************/

#ifndef __HSM_CHECK_H__
#define __HSM_CHECK_H__

#include "hsm_comm.h"
#include "hsm_state.h"

#if 0
#    define mprint(...) printf(__VA_ARGS__)
#else
#    define mprint(...)
#endif

class HSM_Check
{
private:
    std::vector<HSM_State_T> mStateList;
    std::vector<std::string> mInfo;

public:
    HSM_Check(const HSM_StateList_T &stateList) {
        mStateList = stateList;
    }

    ~HSM_Check() {}

    bool check() {
        return checkStateList();
    }

    std::string getInfo() {
        std::string info;
        std::vector<std::string>::iterator it;
        for (it = mInfo.begin(); it != mInfo.end(); it++) {
            info += *it;
            info += "\n";
        }
        return info;
    }

private:
    bool checkStateList() {
        bool ret = false;
        HSM_State_T state;
        HSM_StateListIterator_T slit = mStateList.begin();
        for (; slit != mStateList.end(); slit++) {
            state = *slit;
            HSM_State_Kind_T type = state.type();
            /* check state */
            switch (type) {
                case HSM_ST_KIND_INVALID:
                    ret = false;
                    mprint("%s invalid state type of state:%s, type:%d\n", __func__, state.name().c_str(), type);
                    break;
                case HSM_ST_KIND_COMPOSITE:
                    ret = checkCompositeState(state);
                    break;
                case HSM_ST_KIND_DEEP_HISTORY:
                    ret = checkDeepHistoryState(state);
                    break;
                case HSM_ST_KIND_FINAL:
                    ret = checkFinalState(state);
                    break;
                case HSM_ST_KIND_INITIAL:
                    ret = checkInitialState(state);
                    break;
                case HSM_ST_KIND_JUNCTION:
                    ret = checkJunctionState(state);
                    break;
                case HSM_ST_KIND_SHALLOW_HISTORY:
                    ret = checkShallowHistoryState(state);
                    break;
                case HSM_ST_KIND_SIMPLE:
                    ret = checkSimpleState(state);
                    break;
                default:
                    ret = false;
                    mprint("%s invalid state type of state:%s, type:%d\n", __func__, state.name().c_str(), type);
                    break;
            }
            if (!ret) {
                addInfo(std::to_string(__LINE__)+" check state failed. @state:"+state.name());
                break;
            }
            /* check transition */
            ret = checkStateTransition(state);
            if (!ret) {
                addInfo(std::to_string(__LINE__)+" check state transition failed. @state:"+state.name());
                break;
            } else {
                addInfo(std::to_string(__LINE__)+" check state and transition ok, @state:"+state.name());
            }
        }
        return ret;
    }

    bool checkStateTransition(const HSM_State_T &istate) {
        bool ret = false;
        HSM_State_T state = istate;
        do {
            ret = checkIncomingTransition(state);
            if (!ret) {
                addInfo(std::to_string(__LINE__)+" invalid incoming transition, @state:"+state.name());
                break;
            }
            ret = checkOutgoingTransition(state);
            if (!ret) {
                addInfo(std::to_string(__LINE__)+" invalid outgoing transition, @state:"+state.name());
                break;
            }
            ret = checkNestingConstraint(state);
            if (!ret) {
                addInfo(std::to_string(__LINE__)+" invalid nesting transition, @state:"+state.name());
                break;
            }
        } while (0);
        return ret;
    }

    bool checkIncomingTransition(const HSM_State_T &istate) {
        HSM_State_T target = istate;
        mprint("%s state:%s\n", __func__, target.name().c_str());
        bool was_targeted = false;
        /*
         * Every state should be the explicit target of at least one transition with
         * the following exceptions: 1) Initial states, 2) Composite states that a) do
         * not have an initial state or b) whose parents don't have shallow history.
         */
        HSM_State_T source;
        HSM_StateListIterator_T slit = mStateList.begin();
        for (; slit != mStateList.end(); slit++) {
            source = *slit;
            std::vector<HSM_Transition_T>::const_iterator it_trans = source.transitionTable().begin();
            for (; it_trans != source.transitionTable().end(); it_trans++) {
                HSM_Transition_T trans = *it_trans;
                if (trans.targetId == target.id()) {
                    addInfo(std::to_string(__LINE__)+" "+target.name()+" was targeted by state:"+source.name());
                    was_targeted = true;
                    break;
                }
            } //end for trans loop
            if (was_targeted) {
                return true;
            }
        } //end for state list loop

        //not targeted
        if ((HSM_ST_KIND_INITIAL != target.type())
                && (HSM_ST_KIND_COMPOSITE != target.type())) {
            //warning
            addInfo(std::to_string(__LINE__)+" warning: No incoming transition for state:"+target.name());
            return true;
        }

        if (HSM_ST_KIND_COMPOSITE == target.type()) {
            /*
             * A composite should have an incoming transition if:
             *   1) It has an initial state  - AND -
             *   2) Its parent does NOT have a shallow history state.
             * Instead of treating this as an "error", a trace message is
             * issued since the initial state may be there by convention
             * and is causing no harm.
             */
            HSM_State_Id_T initialId = target.initialId();
            HSM_State_Id_T parentHistId = HSM_NO_HISTORY_STATE;
            HSM_State_Kind_T parentHistType = HSM_ST_KIND_INVALID;
            if (HSM_TOP != target.parentId()) {
                HSM_State_T parent = getStateById(target.parentId());
                parentHistId = parent.historyId();
                if (HSM_NO_HISTORY_STATE != parentHistId) {
                    parentHistType = getStateById(parentHistId).type();
                }
            }
            if ((HSM_NO_INITIAL_STATE != initialId)
                    && ((HSM_NO_HISTORY_STATE == parentHistId) || HSM_ST_KIND_SHALLOW_HISTORY != parentHistType)) {
                // warning Unnecessary initial state for composite state
                addInfo(std::to_string(__LINE__)+" warning: unnecessary initial state:"
                        + getStateById(initialId).name()
                        + " for composite state:"+target.name());
            }
        }
        return true;
    }

    bool checkOutgoingTransition(const HSM_State_T &istate) {
        bool ret = true;
        HSM_State_T state = istate;
        mprint("%s state:%s\n", __func__, state.name().c_str());
        do {
            //All states except final states must have at least one outgoing transition
            if (HSM_ST_KIND_FINAL == state.type()) {
                //final state cannot have outgoing transition
                if (state.transitionTable().size() != 0) {
                    addInfo(std::to_string(__LINE__)+" final state cannot have outgoing transition, @state:"+state.name());
                    ret = false; 
                    break;
                }
            } else {
                if (state.transitionTable().size() == 0) {
                    addInfo(std::to_string(__LINE__)+" nun final state should have outgoing transition, @state:"+state.name());
                    ret = false; 
                    break;
                }
            }
            std::vector<HSM_Transition_T>::const_iterator it_trans = state.transitionTable().begin();
            for (; it_trans != state.transitionTable().end(); it_trans++) {
                HSM_Transition_T trans = *it_trans;
                if (HSM_COMPLETION_EVENT == trans.event) {
                    if (HSM_ST_KIND_COMPOSITE != state.type()) {
                        addInfo(std::to_string(__LINE__)+" not allowed to have a completion transition, @state:"+state.name());
                        ret = false;
                        break;
                    }
                    ret = hasValidTarget(state, trans);
                    if (!ret) {
                        addInfo(std::to_string(__LINE__)+" invalid target, @state:"+state.name());
                        break;
                    }
                } else if (HSM_NO_EVENT != trans.event) {
                    if ((HSM_ST_KIND_COMPOSITE != state.type())
                        && (HSM_ST_KIND_SIMPLE != state.type())) {
                        //Only transitions from "real" states can be triggered by a "real" event.
                        addInfo(std::to_string(__LINE__)+" not allowed to have a event transition, @state:"+state.name());
                        ret = false;
                        break;
                    }
                    ret = hasValidTarget(state, trans);
                    if (!ret) {
                        addInfo(std::to_string(__LINE__)+" invalid target if event, @state:"+state.name());
                        break;
                    }
                } else {
                    //HSM_NO_EVENT == trans.event
                    if (HSM_NO_STATE == trans.targetId) {
                        if (!trans.guard) {
                            addInfo(std::to_string(__LINE__)+" no guard allowed for dummy transition, @state:"+state.name());
                            ret = false;
                            break;
                        }
                        if (1 != state.transitionTable().size()) {
                            //Only one dummy transition allowed
                            addInfo(std::to_string(__LINE__)+" only one dummy transition allowed, @state:"+state.name());
                            ret = false;
                            break;
                        }
                    } else {
                        /*
                         * Only transitions from pseudostates should be associated
                         * with HSM_NO_EVENT; transitions from composite and simple
                         * states must be triggered by a "real" event.
                         */
                        if ((HSM_ST_KIND_COMPOSITE == state.type())
                                || (HSM_ST_KIND_SIMPLE == state.type())) {
                            addInfo(std::to_string(__LINE__)+" not allowed HSM_NO_EVENT transition, @state:"+state.name());
                            ret = false;
                            break;
                        }
                        ret = hasValidTarget(state, trans);
                        if (!ret) {
                            addInfo(std::to_string(__LINE__)+" invalid target of transition, @state:"+state.name());
                            ret = false;
                            break;
                        }
                    }
                }
                if (!ret) {
                    break;
                }
                //Check for more than one transition with the same event and the
                //same guard (which may be an ELSE).
                int duplicates = 0;
                std::vector<HSM_Transition_T>::const_iterator it_trans2 = state.transitionTable().begin();
                //addInfo(std::to_string(__LINE__)+" checking transitions, @state:"+state.name()+", event:"+std::to_string(trans.event)+", guard:"+trans.guardName);
                for (it_trans2 = state.transitionTable().begin();
                        it_trans2 != state.transitionTable().end();
                        it_trans2++) {
                    HSM_Transition_T trans2 = *it_trans2;
                    if ((trans.event == trans2.event)
                            && (trans.guard.target<HSM_Guard_T>() == trans2.guard.target<HSM_Guard_T>())) {
                        //the same event and same guard
                        duplicates ++;
                        if (duplicates > 1) {
                            addInfo(std::to_string(__LINE__)+" duplicate transitions, @state:"+state.name()+", event:"+std::to_string(trans.event)+", guard:"+trans.guardName);
                        }
                    }
                }
                if (duplicates != 1) {
                    addInfo(std::to_string(__LINE__)+" duplicate transitions, @state:"+state.name()+", event:"+std::to_string(trans.event)+", duplicates:"+std::to_string(duplicates));
                    ret = false;
                    break;
                }
            } //end for loop
            //check transition failed
            if (!ret) {
                break;
            }
        } while (0);
        return ret;
    }

    bool checkNestingConstraint(const HSM_State_T &istate) {
        bool ret = true;
        HSM_State_T state = istate;
        int level = 0;
        mprint("%s state:%s\n", __func__, state.name().c_str());
        do {
            if (HSM_ST_KIND_COMPOSITE == state.type()) {
                level = getNestingLevel(state);
                if (level > HSM_MAX_NESTING_LEVELS) {
                    addInfo(std::to_string(__LINE__)+" the most deeply nested composite state's level out of range, @state:"+state.name());
                    ret = false;
                    break;
                }
            }
        } while (0);
        return ret;
    }

    bool checkCompositeState(const HSM_State_T &istate) {
        bool ret = true;
        HSM_State_T state = istate;
        mprint("%s state:%s\n", __func__, state.name().c_str());
        do {
            ret = inStateList(state.id());
            if (!ret) {
                addInfo(std::to_string(__LINE__)+" not in state list, @state:"+state.name());
                break;
            }
            //has valid parent
            ret = hasValidParent(state);
            if (!ret) {
                addInfo(std::to_string(__LINE__)+" parent state invalid, @state:"+state.name());
                break;
            }
            //has valid initial
            ret = hasValidInitial(state);
            if (!ret) {
                addInfo(std::to_string(__LINE__)+" initial state invalid, @state:"+state.name());
                break;
            }
            //has valid history
            ret = hasValidHistory(state);
            if (!ret) {
                addInfo(std::to_string(__LINE__)+" history state invalid, @state:"+state.name());
                break;
            }
        } while (0);
        return ret;
    }

    bool checkDeepHistoryState(const HSM_State_T &istate) {
        bool ret = true;
        HSM_State_T history = istate;
        mprint("%s history:%s\n", __func__, history.name().c_str());
        do {
            ret = inStateList(history.id());
            if (!ret) {
                addInfo(std::to_string(__LINE__)+" not in state list, @state:"+history.name());
                break;
            }
            //has valid parent
            ret = hasValidParent(history);
            if (!ret) {
                addInfo(std::to_string(__LINE__)+" parent state invalid, @state:"+history.name());
                break;
            }
            //top state cannot have history state
            if (HSM_TOP == history.parentId()) {
                addInfo(std::to_string(__LINE__)+" top state cannot have history state, @state:"+history.name());
                ret = false;
                break;
            }
            //Make sure the parent has this state listed as its history state.
            if (getStateById(history.parentId()).historyId() != history.id()) {
                addInfo(std::to_string(__LINE__)+" the history state isnot its parent's history, @state:"+history.name());
                ret = false;
                break;
            }
        } while (0);
        return ret;
    }

    bool checkFinalState(const HSM_State_T &istate) {
        bool ret = true;
        HSM_State_T state = istate;
        mprint("%s state:%s\n", __func__, state.name().c_str());
        do {
            ret = inStateList(state.id());
            if (!ret) {
                addInfo(std::to_string(__LINE__)+" not in state list, @state:"+state.name());
                break;
            }
            //has valid parent
            ret = hasValidParent(state);
            if (!ret) {
                addInfo(std::to_string(__LINE__)+" parent state invalid, @state:"+state.name());
                break;
            }
            //the parent of final state should have a completion transition as its first transition.
            if (HSM_TOP == state.parentId()) {
                addInfo(std::to_string(__LINE__)+" final state should have not top parent, @state:"+state.name());
                ret = false;
                break;
            }
            HSM_State_T parent = getStateById(state.parentId());
            std::vector<HSM_Transition_T> transTable = parent.transitionTable();
            if (transTable.empty()) {
                addInfo(std::to_string(__LINE__)+" parent cannot have empty transition, @state:"+state.name());
                ret = false;
                break;
            }
            HSM_Transition_T trans = transTable[0];
            if (HSM_COMPLETION_EVENT != trans.event) {
                addInfo(std::to_string(__LINE__)+" parent need complete event to final state, @state:"+state.name());
                ret = false;
                break;
            }
        } while (0);
        return ret;
    }

    bool checkInitialState(const HSM_State_T &istate) {
        bool ret = true;
        HSM_State_T state = istate;
        mprint("%s state:%s\n", __func__, state.name().c_str());
        do {
            ret = inStateList(state.id());
            if (!ret) {
                addInfo(std::to_string(__LINE__)+" not in state list, @state:"+state.name());
                break;
            }
            //has valid parent
            ret = hasValidParent(state);
            if (!ret) {
                addInfo(std::to_string(__LINE__)+" parent state invalid, @state:"+state.name());
                break;
            }
            if (0 == state.id()) {
                //The first state in the table must be the intial state for the entire statechart;
                //its parent must be HSM_TOP.
                if (HSM_TOP != state.parentId()) {
                    addInfo(std::to_string(__LINE__)+" parent of statechart's entry/initial state must be top state, @state:"+state.name());
                    ret = false;
                    break;
                }
            } else {
                //Make sure the parent has this state listed as its initial state.
                if (HSM_TOP == state.parentId()) {
                    addInfo(std::to_string(__LINE__)+" normal initial state should have valid parent, @state:"+state.name());
                    ret = false;
                    break;
                }
                if (getStateById(state.parentId()).initialId() != state.id()) {
                    addInfo(std::to_string(__LINE__)+" initial not listed by its parent, @state:"+state.name());
                    ret = false;
                    break;
                }
            }
        } while (0);
        return ret;
    }

    bool checkJunctionState(const HSM_State_T &istate) {
        bool ret = true;
        HSM_State_T state = istate;
        mprint("%s state:%s\n", __func__, state.name().c_str());
        do {
            ret = inStateList(state.id());
            if (!ret) {
                addInfo(std::to_string(__LINE__)+" not in state list, @state:"+state.name());
                break;
            }
            //has valid parent
            ret = hasValidParent(state);
            if (!ret) {
                addInfo(std::to_string(__LINE__)+" parent state invalid, @state:"+state.name());
                break;
            }
            //has one or more incoming
            //has one or more outgoing
        } while (0);
        return ret;
    }

    bool checkShallowHistoryState(const HSM_State_T &istate) {
        bool ret = true;
        HSM_State_T history = istate;
        mprint("%s history:%s\n", __func__, history.name().c_str());
        do {
            ret = inStateList(history.id());
            if (!ret) {
                addInfo(std::to_string(__LINE__)+" not in state list, @state:"+history.name());
                break;
            }
            //has valid parent
            ret = hasValidParent(history);
            if (!ret) {
                addInfo(std::to_string(__LINE__)+" parent state invalid, @state:"+history.name());
                break;
            }
            //top state cannot have history state
            if (HSM_TOP == history.parentId()) {
                addInfo(std::to_string(__LINE__)+" top state cannot have history state, @state:"+history.name());
                ret = false;
                break;
            }
            //Make sure the parent has this state listed as its history state.
            if (getStateById(history.parentId()).historyId() != history.id()) {
                addInfo(std::to_string(__LINE__)+" history not listed by its parent, @state:"+history.name());
                ret = false;
                break;
            }
        } while (0);
        return ret;
    }

    bool checkSimpleState(const HSM_State_T &istate) {
        bool ret = true;
        HSM_State_T state = istate;
        mprint("%s state:%s\n", __func__, state.name().c_str());
        do {
            ret = inStateList(state.id());
            if (!ret) {
                addInfo(std::to_string(__LINE__)+" not in state list, @state:"+state.name());
                break;
            }
            //has valid parent
            ret = hasValidParent(state);
            if (!ret) {
                addInfo(std::to_string(__LINE__)+" parent state invalid, @state:"+state.name());
                break;
            }
        } while (0);
        return ret;
    }

    bool hasValidParent(const HSM_State_T &istate) {
        HSM_State_T state = istate;
        HSM_State_Id_T parentId = state.parentId();
        mprint("%s state:%s\n", __func__, state.name().c_str());
        if (!isValidParentId(parentId)) {
            return false;
        }
        if (HSM_TOP != parentId) {
            /* parent type should be composite */
            if (HSM_ST_KIND_COMPOSITE != getStateById(parentId).type()) {
                return false;
            }
        }
        return true;
    }

    bool hasValidInitial(const HSM_State_T &istate) {
        HSM_State_T state = istate;
        HSM_State_Id_T initialId = state.initialId();
        mprint("%s state:%s\n", __func__, state.name().c_str());
        if (!isValidInitialId(initialId)) {
            return false;
        }
        if (HSM_NO_INITIAL_STATE != initialId) {
            //has initial state
            /* make sure that initial state type is initial and its parent is the composite state */
            HSM_State_T initial = getStateById(initialId);
            if (HSM_ST_KIND_INITIAL != initial.type()) {
                return false;
            }
            if (state.id() != initialId) {
                return false;
            }
        } else {
            //HSM_NO_INITIAL_STATE
            /*
             * If a composite state does not have an initial state, then its parent
             * cannot have a shallow history state, because this would be an
             * implied transition to the composite state.
             */
            if (HSM_TOP != state.parentId()) {
                HSM_State_T parent = getStateById(state.parentId());
                if (HSM_NO_HISTORY_STATE != parent.historyId()) {
                    HSM_State_T parentHist = getStateById(parent.historyId());
                    /* if parent has history, cannot be shallow history */
                    if (HSM_ST_KIND_SHALLOW_HISTORY == parentHist.type()) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    bool hasValidHistory(const HSM_State_T &istate) {
        HSM_State_T state = istate;
        HSM_State_Id_T historyId = state.historyId();
        mprint("%s state:%s\n", __func__, state.name().c_str());
        if (!isValidHistoryId(historyId)) {
            return false;
        }
        if (HSM_NO_HISTORY_STATE != historyId) {
            /* make sure that history state type is deep/shallow history
             * and its parent is the composite state
             */
            HSM_State_T history = getStateById(historyId);
            if ((HSM_ST_KIND_DEEP_HISTORY != history.type())
                   && (HSM_ST_KIND_SHALLOW_HISTORY != history.type())) {
                return false;
            }
            if (state.id() != history.parentId()) {
                return false;
            }
        }
        return true;
    }

    bool hasValidTarget(const HSM_State_T &target, const HSM_Transition_T &transition) {
        bool ret = true;
        HSM_State_T state = target;
        HSM_Transition_T trans = transition;
        do {
            ret = isValidTargetId(trans.targetId);
            if (!ret) {
                addInfo(std::to_string(__LINE__)+" invalid target id, @state:"+state.name());
                break;
            }
            if (HSM_ST_KIND_INITIAL == state.type()) {
                ret = isTargetOfInitialValid(state, trans);
                if (!ret) {
                    addInfo(std::to_string(__LINE__)+" invalid target of initial, @state:"+state.name());
                    break;
                }
            } else if ((HSM_ST_KIND_DEEP_HISTORY == state.type())
                    || (HSM_ST_KIND_SHALLOW_HISTORY == state.type())) {
                ret = isDefaultOfHistoryValid(state, trans);
                if (!ret) {
                    addInfo(std::to_string(__LINE__)+" invalid default history, @state:"+state.name());
                    break;
                }
            }

            if (HSM_SAME_STATE == trans.targetId) {
                continue;
            }
            /* external transition */
            HSM_State_T target = getStateById(trans.targetId);
            switch (target.type()) {
                case HSM_ST_KIND_INITIAL:
                    //initial state cannot be target for transition
                    addInfo(std::to_string(__LINE__)+" not allowed target initial state, @state:"+state.name());
                    ret = false;
                    break;
                case HSM_ST_KIND_COMPOSITE:
                    ret = isTargetCompositeValid(state, trans);
                    break;
                case HSM_ST_KIND_DEEP_HISTORY:
                    ret = isTargetDeepHistoryValid(state, trans);
                    break;
                case HSM_ST_KIND_SHALLOW_HISTORY:
                    ret = isTargetShallowHistoryValid(state, trans);
                    break;
                case HSM_ST_KIND_FINAL:
                    ret = isTargetFinalValid(state, trans);
                    break;
                case HSM_ST_KIND_SIMPLE:
                    break;
                default:
                    break;
            }
            if (!ret) {
                break;
            }
        } while (0);
        return true;
    }

    bool isTargetOfInitialValid(const HSM_State_T &state, const HSM_Transition_T &transition) {
        bool ret = false;
        HSM_State_T initial = state;
        HSM_Transition_T trans = transition;
        do {
            //an initial state's target should within its parent, and cannot be parent
            ret = isAncestorState(trans.targetId, initial.parentId());
            if (!ret) {
                addInfo(std::to_string(__LINE__)+" invalid target of initial state, @state:"+initial.name());
                break;
            }
        } while (0);
        return ret;
    }

    bool isDefaultOfHistoryValid(const HSM_State_T &state, const HSM_Transition_T &transition) {
        bool ret = true;
        HSM_State_T history = state;
        HSM_Transition_T trans = transition;
        do {
            //A history state's default target must be inside its parent state.
            ret = isAncestorState(trans.targetId, history.parentId());
            if (!ret) {
                addInfo(std::to_string(__LINE__)+" invalid target of history state, @state:"+history.name());
                break;
            }
            if (HSM_ST_KIND_SHALLOW_HISTORY == history.type()) {
                //shallow history state targets another state at its same level
                HSM_State_T target = getStateById(trans.targetId);
                if (history.parentId() != target.parentId()) {
                    addInfo(std::to_string(__LINE__)+" shallow history and its target should at the same level, @state:"+history.name());
                    ret = false;
                    break;
                }
            }
        } while (0);
        return ret;
    }

    bool isTargetCompositeValid(const HSM_State_T &state, const HSM_Transition_T &transition) {
        bool ret = true;
        (void)state;
        HSM_Transition_T trans = transition;
        HSM_State_T composite = getStateById(trans.targetId);
        do {
            //A targeted composite state must have an initial state.
            ret = hasValidInitial(composite); 
            if (!ret) {
                addInfo(std::to_string(__LINE__)+" A targeted composite state must have an initial state, @state:"+composite.name());
                break;
            }
        } while (0);
        return ret; 
    }

    bool isTargetDeepHistoryValid(HSM_State_T &src, HSM_Transition_T &transition) {
        bool ret = true;
        HSM_State_T source = src;
        HSM_Transition_T trans = transition;
        HSM_State_T history = getStateById(trans.targetId);
        do {
            /*
             * The source of the transition to a history state cannot be the
             * composite state enclosing the history state or within it (unless it
             * is the composite's initial state).
             */
            ret = isValidParentId(history.parentId());
            if (!ret) {
                addInfo(std::to_string(__LINE__)+" invalid parent of history, @state:"+history.name());
                break;
            }
            if (HSM_TOP == history.parentId()) {
                addInfo(std::to_string(__LINE__)+" history should within composite, @state:"+history.name());
                ret = false;
                break;
            }
            if (source.id() == history.parentId()) {
                ret = false;
                addInfo(std::to_string(__LINE__)+" composite cannot target its history, @state:"+source.name());
                break;
            }
            if (isAncestorState(source.id(), history.parentId())) {
                HSM_State_T parentHist = getStateById(history.parentId());
                if (parentHist.initialId() != source.id()) {
                    addInfo(std::to_string(__LINE__)+" cannot target the history state that tracks it, @state:"+source.name());
                    ret = false;
                    break;
                }
            }
        } while (0);
        return ret; 
    }

    bool isTargetShallowHistoryValid(HSM_State_T &src, HSM_Transition_T &transition) {
        bool ret = true;
        HSM_State_T source = src;
        HSM_Transition_T trans = transition;
        HSM_State_T history = getStateById(trans.targetId);
        do {
            /*
             * The source of the transition to a history state cannot be the
             * composite state enclosing the history state or within it (unless it
             * is the composite's initial state).
             */
            ret = isValidParentId(history.parentId());
            if (!ret) {
                addInfo(std::to_string(__LINE__)+" invalid parent of history, @state:"+history.name());
                break;
            }
            if (HSM_TOP == history.parentId()) {
                addInfo(std::to_string(__LINE__)+" history should within composite, @state:"+history.name());
                ret = false;
                break;
            }
            if (source.id() == history.parentId()) {
                ret = false;
                addInfo(std::to_string(__LINE__)+" composite cannot target its history, @state:"+source.name());
                break;
            }
            if (isAncestorState(source.id(), history.parentId())) {
                HSM_State_T parentHist = getStateById(history.parentId());
                if (parentHist.initialId() != source.id()) {
                    addInfo(std::to_string(__LINE__)+" cannot target the history state that tracks it, @state:"+source.name());
                    ret = false;
                    break;
                }
            }
        } while (0);
        return ret; 
    }

    bool isTargetFinalValid(const HSM_State_T &src, const HSM_Transition_T &transition) {
        bool ret = true;
        HSM_State_T source = src;
        HSM_Transition_T trans = transition;
        HSM_State_T finally = getStateById(trans.targetId);
        do {
            //to final state outside its scope
            if ((source.id() != finally.parentId())
                    && (!isAncestorState(source.id(), finally.parentId()))) {
                addInfo(std::to_string(__LINE__)+" to final state outside its scope, @state:"+source.name());
                ret = false;
                break;
            }
            /*
             * A completion transition for a composite state cannot target that
             * same composite state's final state because that creates an infinite
             * loop.
             */
            if ((source.id() == finally.parentId())
                    && (HSM_COMPLETION_EVENT == trans.event)) {
                ret = false;
                //Completion transition cannot target its final state
                addInfo(std::to_string(__LINE__)+" Completion transition cannot target its final state, @state:"+source.name());
                break;
            }
            /*
             * The transitions from a composite state containing a final state
             * must have a completion transition.
             */
            HSM_State_T parent = getStateById(finally.parentId());
            const HSM_TransitionList_T transTable = parent.transitionTable();
            HSM_TransChainIterator_T it_trans;
            ret = false;
            for (it_trans = transTable.begin(); it_trans != transTable.end(); it_trans++) {
                trans = *it_trans;
                if (HSM_COMPLETION_EVENT == trans.event) {
                    //find out complete transition
                    ret = true;
                    break;
                }
            }
            if (true != ret) {
                addInfo(std::to_string(__LINE__)+" transitions must have a complete transition, @state:"+parent.name());
                break;
            }
        } while (0);
        return ret; 
    }

    bool isValidParentId(const HSM_State_Id_T parentId) {
        mprint("%s parentId:%d\n", __func__, parentId);
        if ((inStateList(parentId)) || (HSM_TOP == parentId)) {
            return true;
        }
        return false;
    }

    bool isValidInitialId(const HSM_State_Id_T initialId) {
        mprint("%s initialId:%d\n", __func__, initialId);
        if ((inStateList(initialId)) || (HSM_NO_INITIAL_STATE == initialId)) {
            return true;
        }
        return false;
    }

    bool isValidHistoryId(const HSM_State_Id_T historyId) {
        mprint("%s historyId:%d\n", __func__, historyId);
        if ((inStateList(historyId)) || (HSM_NO_HISTORY_STATE == historyId)) {
            return true;
        }
        return false;
    }

    bool isValidTargetId(const HSM_State_Id_T targetId) {
        mprint("%s targetId:%d\n", __func__, targetId);
        if ((inStateList(targetId))
                /*|| (HSM_NO_STATE == targetId)*/
               || (HSM_SAME_STATE == targetId)) {
            return true;
        }
        return false;
    }

    bool isAncestorState(const HSM_State_Id_T ref, const HSM_State_Id_T toConfirm) {
        bool isAncestor = false;
        HSM_State_Id_T sourceId = ref;
        if (HSM_TOP == sourceId) {
            return false;
        }
        if (HSM_TOP == toConfirm) {
            return true;
        }
        HSM_State_T source;
        HSM_State_Id_T parentId;
        while (true) {
            source = getStateById(sourceId);
            parentId = source.parentId();
            if (toConfirm == parentId) {
                isAncestor = true;
                break;
            }
            if (HSM_TOP == parentId) {
                break;
            }
            sourceId = parentId;
        }
        return isAncestor;
    }

    HSM_State_T &getStateById(const HSM_State_Id_T stateid) {
        assert(inStateList(stateid));
        return mStateList[stateid];
    }

    bool inStateList(const HSM_State_Id_T stateid) {
        //mprint("%s stateid:%d\n", __func__, stateid);
        if ((stateid >= 0) && ((size_t)stateid < mStateList.size())) {
            if (mStateList[stateid].id() == stateid) {
                return true;
            }
        }
        return false;
    }

    uint32_t getNestingLevel(const HSM_State_T &istate) {
        uint32_t level = 1;
        HSM_State_T state = istate;
        HSM_State_Id_T parentId = state.parentId();
        while (HSM_TOP != parentId) {
            level += 1;
            parentId = state.parentId();
            if ((parentId > 0)
                && ((size_t)parentId < mStateList.size())) {
                state = mStateList[parentId];
            } else if (HSM_TOP == parentId) {
                // upto top level
            } else {
                mprint("%s invalid parentId of state:%s\n", __func__, state.name().c_str());
                assert(0);
            }
        }
        return level;
    }

    void addInfo(const std::string &info) {
        mInfo.push_back(info);
    }
};

#endif //__HSM_CHECK_H__


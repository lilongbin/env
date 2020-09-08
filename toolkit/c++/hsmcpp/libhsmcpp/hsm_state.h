/*****************************************
 * Copyright (C) 2020 * Ltd. All rights reserved.
 * 
 * File name   : HSM_State.h
 * Author      : longbin
 * Created date: 2020-08-24 15:09:22
 * Description : 
 *
 *******************************************/

#ifndef __HSM_STATE_H__
#define __HSM_STATE_H__

#include "hsm_comm.h"
#include <cassert>
#include <string>
#include <stdexcept>

class HSM_State_T;
typedef std::vector<HSM_State_T> HSM_StateList_T;
typedef std::vector<HSM_State_T>::const_iterator HSM_StateListIterator_T;

typedef struct {
    HSM_StateList_T stateList;
    std::string     statechartName;
} HSM_State_Definition_T;


class HSM_State_T
{
private:
    HSM_State_Kind_T mStateType;
    HSM_State_Id_T   mStateId;
    std::string      mStateName;
    HSM_State_Id_T   mParentStateId;
    HSM_State_Id_T   mInitialStateId;
    HSM_State_Id_T   mHistoryStateId;
    HSM_Action_T     mEntryAction;
    HSM_Action_T     mExitAction;
    std::string      mEntryName;
    std::string      mExitName;
    HSM_TransitionList_T mTransitionTable;

public:
    HSM_State_T() {
        mStateType = HSM_ST_KIND_INVALID;
        mStateId = HSM_TOP;
        mStateName = "HSM_TOP";
        mParentStateId = HSM_NO_STATE;
        mInitialStateId = HSM_NO_INITIAL_STATE;
        mHistoryStateId = HSM_NO_HISTORY_STATE;
        mEntryAction = HSM_NO_ACTION;
        mExitAction = HSM_NO_ACTION;
        mEntryName = "";
        mExitName = "";
    }

    HSM_State_T(HSM_State_Kind_T type, HSM_State_Id_T id, std::string name,
            HSM_State_Id_T parent, HSM_State_Id_T initial, HSM_State_Id_T history,
            HSM_Action_T entry, HSM_Action_T exit,
            std::string entryname, std::string exitname) {
        mStateType = type;
        mStateId = id;
        mStateName = name;
        mParentStateId = parent;
        mInitialStateId = initial;
        mHistoryStateId = history;
        mEntryAction = entry;
        mExitAction = exit;
        mEntryName = entryname;
        mExitName = exitname;
        std::cout <<"new state, type:"<<type<<",id:"<<id<<",name:"<<name
            <<",parent:"<<parent<<",initial:"<<initial<<",history:"<<history
            <<",entry:"<<entryname<<",exit:"<<exitname
            << std::endl;
    }

    ~HSM_State_T() {
    }

    HSM_State_T (HSM_State_T &state) {
        this->mStateType = state.mStateType;
        this->mStateId = state.mStateId;
        this->mStateName = state.mStateName;
        this->mEntryAction = state.mEntryAction;
        this->mExitAction = state.mExitAction;
        this->mParentStateId = state.mParentStateId;
        this->mInitialStateId = state.mInitialStateId;
        this->mHistoryStateId = state.mHistoryStateId;
        this->mEntryName = state.mEntryName;
        this->mExitName = state.mExitName;
        this->mTransitionTable = state.mTransitionTable;
    }

    HSM_State_T (const HSM_State_T &state) {
        this->mStateType = state.mStateType;
        this->mStateId = state.mStateId;
        this->mStateName = state.mStateName;
        this->mEntryAction = state.mEntryAction;
        this->mExitAction = state.mExitAction;
        this->mParentStateId = state.mParentStateId;
        this->mInitialStateId = state.mInitialStateId;
        this->mHistoryStateId = state.mHistoryStateId;
        this->mEntryName = state.mEntryName;
        this->mExitName = state.mExitName;
        this->mTransitionTable = state.mTransitionTable;
    }

    HSM_State_T& operator= (const HSM_State_T &state) {
        this->mStateType = state.mStateType;
        this->mStateId = state.mStateId;
        this->mStateName = state.mStateName;
        this->mEntryAction = state.mEntryAction;
        this->mExitAction = state.mExitAction;
        this->mParentStateId = state.mParentStateId;
        this->mInitialStateId = state.mInitialStateId;
        this->mHistoryStateId = state.mHistoryStateId;
        this->mEntryName = state.mEntryName;
        this->mExitName = state.mExitName;
        this->mTransitionTable = state.mTransitionTable;
        return *this;
    }

    HSM_State_Kind_T type() {
        return mStateType;
    }

    HSM_State_Id_T id() {
        return mStateId;
    }

    const std::string & name() {
        return mStateName;
    }

    const HSM_Action_T entryAction() {
        return mEntryAction;
    }

    const HSM_Action_T exitAction() {
        return mExitAction;
    }

    HSM_State_Id_T parentId() {
        return mParentStateId;
    }

    HSM_State_Id_T initialId() {
        return mInitialStateId;
    }

    HSM_State_Id_T historyId() {
        return mHistoryStateId;
    }

    const std::string & entryName() {
        return mEntryName;
    }

    const std::string & exitName() {
        return mExitName;
    }

    const HSM_TransitionList_T &transitionTable() {
        return mTransitionTable;
    }

    bool addInternalTrans(const HSM_Transition_T &trans) {
        HSM_Transition_T transition {};
        /* has event, has guard, no history, do action, no next_state */
        transition.event = trans.event;
        transition.guard = trans.guard;
        transition.action = trans.action;
        transition.targetId = HSM_SAME_STATE; /* next_state */
        if ((HSM_ST_KIND_DEEP_HISTORY == mStateType)
            || (HSM_ST_KIND_SHALLOW_HISTORY == mStateType)) {
            transition.historyId = trans.historyId;//only valid for history state
        } else {
            transition.historyId = HSM_NO_HISTORY_STATE;
        }
        transition.guardName = trans.guardName;
        transition.actionName = trans.actionName;
        return addTransition(transition);
    }

    bool addExternalTrans(const HSM_Transition_T &trans) {
        HSM_Transition_T transition {};
        /* has event, has guard, no history, do action, has next_state */
        transition.event = trans.event;
        transition.guard = trans.guard;
        transition.action = trans.action;
        transition.targetId = trans.targetId; /* next_state */
        if ((HSM_ST_KIND_DEEP_HISTORY == mStateType)
            || (HSM_ST_KIND_SHALLOW_HISTORY == mStateType)) {
            transition.historyId = trans.historyId;//only valid for history state
        } else {
            transition.historyId = HSM_NO_HISTORY_STATE;
        }
        transition.guardName = trans.guardName;
        transition.actionName = trans.actionName;
        return addTransition(transition);
    }

    bool addCompleteTrans(const HSM_Transition_T &trans) {
        HSM_Transition_T transition {};
        /* no event, no guard, no history, do action, has next_state */
        transition.event = HSM_COMPLETION_EVENT;
        transition.guard = HSM_NO_GUARD;
        transition.action = trans.action;
        transition.targetId = trans.targetId; /* next_state */
        transition.historyId = HSM_NO_HISTORY_STATE;
        transition.guardName = "no guard";
        transition.actionName = trans.actionName;
        return addTransition(transition);
    }

    HSM_State_Id_T getHistoryDefaultTargetId() {
        HSM_State_Id_T targetId;
        if ((HSM_ST_KIND_DEEP_HISTORY == mStateType)
                || (HSM_ST_KIND_SHALLOW_HISTORY == mStateType)) {
            if (mTransitionTable.size() != 1) {
                printf("%s transition size invalid of history state:%s\n", __func__, mStateName.c_str());
                assert(0);
            }
            targetId = mTransitionTable[0].historyId; //user initialized value
            //printf("%s history id is %d\n", __func__, targetId);
        } else {
            printf("%s failed, not history state:%s\n", __func__, mStateName.c_str());
            assert(0);
        }
        return targetId;
    }

    void saveHistoryState(HSM_State_Id_T stateid) {
        if ((HSM_ST_KIND_DEEP_HISTORY == mStateType)
                || (HSM_ST_KIND_SHALLOW_HISTORY == mStateType)) {
            if (mTransitionTable.size() != 1) {
                printf("%s transition size invalid of history state:%s\n", __func__, mStateName.c_str());
                assert(0);
            }
            mTransitionTable[0].targetId = stateid; //save target id for jump back
            //printf("%s target id set to %d\n", __func__, stateid);
        } else {
            printf("%s failed, not history state:%s\n", __func__, mStateName.c_str());
            assert(0);
        }
    }

private:
    bool addTransition(const HSM_Transition_T &trans) {
        mTransitionTable.push_back(trans);
        return true;
    }

    void setTransTable(HSM_TransitionList_T &transTable) {
        mTransitionTable = transTable;
    }
};

#endif //__HSM_STATE_H__


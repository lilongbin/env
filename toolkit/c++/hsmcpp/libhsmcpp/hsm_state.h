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
protected:
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

    virtual ~HSM_State_T() {
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

    HSM_State_T & operator= (const HSM_State_T &state) {
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

    virtual HSM_State_Kind_T type() {
        return mStateType;
    }

    virtual HSM_State_Id_T id() {
        return mStateId;
    }

    virtual const std::string & name() {
        return mStateName;
    }

    virtual const HSM_Action_T entryAction() {
        return mEntryAction;
    }

    virtual const HSM_Action_T exitAction() {
        return mExitAction;
    }

    virtual HSM_State_Id_T parentId() {
        return mParentStateId;
    }

    virtual HSM_State_Id_T initialId() {
        return mInitialStateId;
    }

    virtual HSM_State_Id_T historyId() {
        return mHistoryStateId;
    }

    virtual const std::string & entryName() {
        return mEntryName;
    }

    virtual const std::string & exitName() {
        return mExitName;
    }

    virtual const HSM_TransitionList_T &transitionTable() {
        return mTransitionTable;
    }

    virtual bool addInternalTrans(const HSM_Event_T event, const HSM_Guard_T guard, HSM_Action_T action,
                                        std::string guardName, std::string actionName) {
        (void)event; (void)guard; (void)action; (void)guardName; (void)actionName;
        return false;
    }

    virtual bool addExternalTrans(const HSM_Event_T event, const HSM_Guard_T guard, HSM_Action_T action, HSM_State_Id_T next_state,
                                        std::string guardName, std::string actionName) {
        (void)event; (void)guard; (void)action; (void)next_state; (void)guardName; (void)actionName;
        return false;
    }

    virtual bool addCompleteTrans(const HSM_Action_T action, HSM_State_Id_T next_state, std::string actionName) {
        (void)action; (void)next_state; (void)actionName;
        return false;
    }

    HSM_State_Id_T getHistoryDefaultTargetId() {
        HSM_State_Id_T targetId;
        if ((HSM_ST_KIND_DEEP_HISTORY == mStateType)
                || (HSM_ST_KIND_SHALLOW_HISTORY == mStateType)) {
            if (mTransitionTable.empty()) {
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

    void updateHistoryTargetId(HSM_State_Id_T stateid) {
        if ((HSM_ST_KIND_DEEP_HISTORY == mStateType)
                || (HSM_ST_KIND_SHALLOW_HISTORY == mStateType)) {
            if (mTransitionTable.empty()) {
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

protected:
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

    bool addTransition(const HSM_Transition_T &trans) {
        mTransitionTable.push_back(trans);
        return true;
    }
};


class HSM_Initial_State:public HSM_State_T {
public:
    HSM_Initial_State() {}
    HSM_Initial_State(HSM_State_Id_T id, std::string name,
            HSM_State_Id_T parent, HSM_Action_T action, std::string action_name,
            HSM_State_Id_T next_state)
            :HSM_State_T(HSM_ST_KIND_INITIAL, id, name, parent, HSM_NO_INITIAL_STATE, HSM_NO_HISTORY_STATE, HSM_NO_ACTION, HSM_NO_ACTION, "no-entry", "no-exit") {
            addTransition({HSM_NO_EVENT, HSM_NO_GUARD, action, next_state, HSM_NO_HISTORY_STATE, "no-guard", action_name});
    }
    HSM_Initial_State (HSM_Initial_State &state):HSM_State_T(state) {    }
    HSM_Initial_State (const HSM_Initial_State &state):HSM_State_T(state) {    }
    HSM_Initial_State & operator= (const HSM_Initial_State &state) {
        HSM_State_T::operator=(state);
        return *this;
    }
};

class HSM_Composite_State:public HSM_State_T {
public:
    HSM_Composite_State() {}
    HSM_Composite_State(HSM_State_Id_T id, std::string name,
            HSM_State_Id_T parent, HSM_State_Id_T initial, HSM_State_Id_T history,
            HSM_Action_T entry, HSM_Action_T exit,
            std::string entryname, std::string exitname)
            :HSM_State_T(HSM_ST_KIND_COMPOSITE, id, name, parent, initial, history, entry, exit, entryname, exitname) {
    }
    HSM_Composite_State (HSM_Composite_State &state):HSM_State_T(state) {    }
    HSM_Composite_State (const HSM_Composite_State &state):HSM_State_T(state) {    }
    HSM_Composite_State & operator= (const HSM_Composite_State &state) {
        HSM_State_T::operator=(state);
        return *this;
    }

    bool addInternalTrans(const HSM_Event_T event, const HSM_Guard_T guard, HSM_Action_T action,
                                        std::string guardName, std::string actionName) {
        /* has event, has guard, no history, has action, no next_state */
        return addTransition({event, guard, action, HSM_SAME_STATE, HSM_NO_HISTORY_STATE, guardName, actionName});
    }

    bool addExternalTrans(const HSM_Event_T event, const HSM_Guard_T guard, HSM_Action_T action, HSM_State_Id_T next_state,
                                        std::string guardName, std::string actionName) {
        /* has event, has guard, no history, has action, has next_state */
        return addTransition({event, guard, action, next_state, HSM_NO_HISTORY_STATE, guardName, actionName});
    }

    bool addCompleteTrans(const HSM_Action_T action, HSM_State_Id_T next_state, std::string actionName) {
        /* no event, no guard, no history, has action, has next_state */
        return addTransition({HSM_COMPLETION_EVENT, HSM_NO_GUARD, action, next_state, HSM_NO_HISTORY_STATE, "no-guard", actionName});
    }
};

class HSM_Juncton_State:public HSM_State_T {
public:
    HSM_Juncton_State() {}
    HSM_Juncton_State(HSM_State_Id_T id, std::string name,
            HSM_State_Id_T parent)
            :HSM_State_T(HSM_ST_KIND_JUNCTION, id, name, parent, HSM_NO_INITIAL_STATE, HSM_NO_HISTORY_STATE, HSM_NO_ACTION, HSM_NO_ACTION, "no-entry", "no-exit") {
    }
    HSM_Juncton_State (HSM_Juncton_State &state):HSM_State_T(state) {    }
    HSM_Juncton_State (const HSM_Juncton_State &state):HSM_State_T(state) {    }
    HSM_Juncton_State & operator= (const HSM_Juncton_State &state) {
        HSM_State_T::operator=(state);
        return *this;
    }

    bool addExternalTrans(const HSM_Event_T event, const HSM_Guard_T guard, HSM_Action_T action, HSM_State_Id_T next_state,
                                        std::string guardName, std::string actionName) {
        /* has event, has guard, no history, has action, has next_state */
        return addTransition({event, guard, action, next_state, HSM_NO_HISTORY_STATE, guardName, actionName});
    }
};

class HSM_Deep_History_State:public HSM_State_T {
public:
    HSM_Deep_History_State() {}
    HSM_Deep_History_State(HSM_State_Id_T id, std::string name,
            HSM_State_Id_T parent, HSM_State_Id_T default_state)
            :HSM_State_T(HSM_ST_KIND_DEEP_HISTORY, id, name, parent, HSM_NO_INITIAL_STATE, HSM_NO_HISTORY_STATE, HSM_NO_ACTION, HSM_NO_ACTION, "no-entry", "no-exit") {
            HSM_Transition_T transition {HSM_NO_EVENT, HSM_NO_GUARD, HSM_NO_ACTION, default_state, default_state, "no-guard", "no-action"};
            addTransition(transition);
    }
    HSM_Deep_History_State (HSM_Deep_History_State &state):HSM_State_T(state) {    }
    HSM_Deep_History_State (const HSM_Deep_History_State &state):HSM_State_T(state) {    }
    HSM_Deep_History_State & operator= (const HSM_Deep_History_State &state) {
        HSM_State_T::operator=(state);
        return *this;
    }
};

class HSM_Shallow_History_State:public HSM_State_T {
public:
    HSM_Shallow_History_State() {}
    HSM_Shallow_History_State(HSM_State_Id_T id, std::string name,
            HSM_State_Id_T parent, HSM_State_Id_T default_state)
            :HSM_State_T(HSM_ST_KIND_SHALLOW_HISTORY, id, name, parent, HSM_NO_INITIAL_STATE, HSM_NO_HISTORY_STATE, HSM_NO_ACTION, HSM_NO_ACTION, "no-entry", "no-exit") {
            HSM_Transition_T transition {HSM_NO_EVENT, HSM_NO_GUARD, HSM_NO_ACTION, default_state, default_state, "no-guard", "no-action"};
            addTransition(transition);
    }
    HSM_Shallow_History_State (HSM_Shallow_History_State &state):HSM_State_T(state) {    }
    HSM_Shallow_History_State (const HSM_Shallow_History_State &state):HSM_State_T(state) {    }
    HSM_Shallow_History_State & operator= (const HSM_Shallow_History_State &state) {
        HSM_State_T::operator=(state);
        return *this;
    }
};

class HSM_Simple_State:public HSM_State_T {
public:
    HSM_Simple_State() {}
    HSM_Simple_State(HSM_State_Id_T id, std::string name,
            HSM_State_Id_T parent, HSM_Action_T entry, HSM_Action_T exit,
            std::string entryname, std::string exitname)
            :HSM_State_T(HSM_ST_KIND_SIMPLE, id, name, parent, HSM_NO_INITIAL_STATE, HSM_NO_HISTORY_STATE, entry, exit, entryname, exitname) {
    }
    HSM_Simple_State (HSM_Simple_State &state):HSM_State_T(state) {    }
    HSM_Simple_State (const HSM_Simple_State &state):HSM_State_T(state) {    }
    HSM_Simple_State & operator= (const HSM_Simple_State &state) {
        HSM_State_T::operator=(state);
        return *this;
    }

    bool addInternalTrans(const HSM_Event_T event, const HSM_Guard_T guard, HSM_Action_T action,
                                        std::string guardName, std::string actionName) {
        /* has event, has guard, no history, has action, no next_state */
        return addTransition({event, guard, action, HSM_SAME_STATE, HSM_NO_HISTORY_STATE, guardName, actionName});
    }

    bool addExternalTrans(const HSM_Event_T event, const HSM_Guard_T guard, HSM_Action_T action, HSM_State_Id_T next_state,
                                        std::string guardName, std::string actionName) {
        /* has event, has guard, no history, has action, has next_state */
        return addTransition({event, guard, action, next_state, HSM_NO_HISTORY_STATE, guardName, actionName});
    }
};

class HSM_Final_State:public HSM_State_T {
public:
    HSM_Final_State() {}
    HSM_Final_State(HSM_State_Id_T id, std::string name,
            HSM_State_Id_T parent)
            :HSM_State_T(HSM_ST_KIND_FINAL, id, name, parent, HSM_NO_INITIAL_STATE, HSM_NO_HISTORY_STATE, HSM_NO_ACTION, HSM_NO_ACTION, "no-entry", "no-exit") {
    }
    HSM_Final_State (HSM_Final_State &state):HSM_State_T(state) {    }
    HSM_Final_State (const HSM_Final_State &state):HSM_State_T(state) {    }
    HSM_Final_State & operator= (const HSM_Final_State &state) {
        HSM_State_T::operator=(state);
        return *this;
    }
};

class HSM_Dummy_State:public HSM_State_T {
public:
    HSM_Dummy_State() {}
    HSM_Dummy_State(HSM_State_Id_T id, std::string name)
            :HSM_State_T(HSM_ST_KIND_INVALID, id, name, HSM_NO_STATE, HSM_NO_INITIAL_STATE, HSM_NO_HISTORY_STATE, HSM_NO_ACTION, HSM_NO_ACTION, "no-entry", "no-exit") {
    }
    HSM_Dummy_State (HSM_Dummy_State &state):HSM_State_T(state) {    }
    HSM_Dummy_State (const HSM_Dummy_State &state):HSM_State_T(state) {    }
    HSM_Dummy_State & operator= (const HSM_Dummy_State &state) {
        HSM_State_T::operator=(state);
        return *this;
    }
};

#endif //__HSM_STATE_H__


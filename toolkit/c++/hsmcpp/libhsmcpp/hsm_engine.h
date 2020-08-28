/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* 
* File name   : hsm_engine.h
* Author      : longbin
* Created date: 2020-08-24 16:21:58
* Description : 
*
*******************************************/

#ifndef __HSM_ENGINE_H__
#define __HSM_ENGINE_H__

#include "hsm_comm.h"
#include "hsm_state.h"
#include <assert.h>

#define HSM_Log(...)

typedef struct {
    std::vector<HSM_State_T> stateList;
    std::string         statechartName;
} HSM_State_Definition_T;

typedef std::vector<HSM_Transition_T>::iterator HSM_TransChainIterator_T;
typedef std::vector<HSM_State_T>::const_iterator HSM_StateListIterator_T;

class HSM_Engine
{
private:
    HSM_StateChart_T *mpStatechart = NULL;
    HSM_State_Definition_T mStateDfn;
    const HSM_State_T mHsmTopState {HSM_SKIND_INVALID, HSM_TOP, "HSM_TOP", HSM_NO_STATE, HSM_NO_INITIAL_STATE, HSM_NO_HISTORY_STATE, HSM_NO_ACTION, HSM_NO_ACTION, "", ""};
    const HSM_State_T mHsmSameState {HSM_SKIND_INVALID, HSM_SAME_STATE, "HSM_SAME_STATE", HSM_NO_STATE, HSM_NO_INITIAL_STATE, HSM_NO_HISTORY_STATE, HSM_NO_ACTION, HSM_NO_ACTION, "", ""};

public:
    HSM_Engine() {
    }

    HSM_Engine(HSM_Engine &engine) {
        this->mpStatechart = engine.mpStatechart;
    }

    HSM_Engine(const HSM_Engine &engine) {
        this->mpStatechart = engine.mpStatechart;
    }

    HSM_Engine & operator= (const HSM_Engine &engine) {
        this->mpStatechart = engine.mpStatechart;
        return *this;
    }

    ~HSM_Engine() {
    }

    bool start(const HSM_State_Definition_T &stateDfn, void * userObj) {
        HSM_Log("start state chart %s begin\n", stateDfn.statechartName.c_str());
        init();
        //mStateDfn = stateDfn;
        mStateDfn.statechartName = stateDfn.statechartName;
        HSM_StateListIterator_T slit = stateDfn.stateList.begin();
        HSM_State_T state;
        HSM_State_Id_T id = 0;
        for (; slit != stateDfn.stateList.end(); slit++, id++) {
            state = *slit;
            if (state.id() != id) {
                HSM_Log("%s error, status id(%d:%s) and list index are inconsistent, expected id:%d\n",
                        __func__, state.id(), state.name().c_str(), id);
                assert(0);
            }
            mStateDfn.stateList.push_back(state);
        }
        HSM_Log("%s stateList size:%d\n", __func__, (int)mStateDfn.stateList.size());
        assert(0 != mStateDfn.stateList.size());

        mpStatechart->userObj = userObj;
        //build transition chain
        buildTransitionChain(HSM_NO_EVENT);
        //do tansition
        if (0 < transChainSize()) {
            doTransitions();
        }
        HSM_Log("start state chart %s ok\n", stateDfn.statechartName.c_str());
        HSM_Log("%s end, @state:%s\n\n", __func__, currentStateName().c_str());
        return true;
    }

    bool proccessMessage(HSM_Event_T event, std::vector<uint8_t> &data) {
        mpStatechart->event = event;
        mpStatechart->eventdata = data;
        //build transition chain
        buildTransitionChain(event);
        //do transitions
        if (0 < transChainSize()) {
            doTransitions();
        }
        HSM_Log("%s end, @state:%s\n\n", __func__, currentStateName().c_str());
        return true;
    }

    HSM_Event_T getEvent() {
        return mpStatechart->event;
    }

    size_t getEventData(std::vector<uint8_t> &data) {
        data = mpStatechart->eventdata;
        return data.size();
    }

private:
    void init() {
        mpStatechart = new HSM_StateChart_T();
        if (mpStatechart == NULL) {
            std::cout << "construct HSM_StateChart_T failed." << std::endl;
        }
        mpStatechart->currentStateId = 0;//init
    }

    HSM_State_T &getStateById(const HSM_State_Id_T stateid) {
        int size = (int)mStateDfn.stateList.size();
        if (HSM_TOP == stateid) {
            return const_cast<HSM_State_T &>(mHsmTopState);
        } else if (HSM_SAME_STATE == stateid) {
            return const_cast<HSM_State_T &>(mHsmSameState);
        }
        if ((stateid < 0) || (stateid >= size)) {
            HSM_Log("%s state id(%d) out of range:%d\n", __func__, stateid, size);
            assert(0);
        }
        return mStateDfn.stateList[stateid];
    }

    std::string getStateNameById(const HSM_State_Id_T stateid) {
        std::string name;
        if (HSM_TOP == stateid) {
            name = "HSM_TOP";
        } else if (HSM_SAME_STATE == stateid) {
            name = "HSM_SAME_STATE";
        } else {
            name = getStateById(stateid).name();
        }
        return name;
    }

    HSM_State_Id_T currentStateId() {
        return mpStatechart->currentStateId;
    }

    HSM_State_T & currentState() {
        return getStateById(currentStateId());
    }

    bool setCurrentStateId(HSM_State_Id_T id) {
        bool ret = false;
        if ((0 < id) && ((size_t)id < mStateDfn.stateList.size())) {
            mpStatechart->currentStateId = id;
            ret = true;
        } else {
            ret = false;
            HSM_Log("%s invlaid state id:%d\n", __func__, id);
            assert(0);
        }
        //update currentStateName
        mpStatechart->currentStateName = currentStateName();
        return ret;
    }

    std::string currentStateName() {
        return getStateById(currentStateId()).name().c_str();
    }

    size_t transChainSize() {
        return mpStatechart->transChain.size();
    }

    void appendTransChain(const HSM_Transition_T &trans) {
        mpStatechart->transChain.push_back(trans);
    }

    void clearTransChain() {
        mpStatechart->transChain.clear();
    }

    HSM_State_Id_T getTransTargetId(const HSM_Transition_T &trans) {
        HSM_State_Id_T targetId = trans.targetId;
#if 0
        //for history state, has saved current to target, and when need reset will restore from historyId
        if (trans.historyId != HSM_NO_HISTORY_STATE) {
            //if saved history when exit before, goto history first;
            targetId = trans.historyId;
        }
#endif
        if (HSM_NO_STATE == trans.targetId) {
            HSM_Log("%s no target to transfer, targetid:%d\n", __func__, trans.targetId);
            assert(0);
        }
        return targetId;
    }

    void completeTransitionChain(HSM_State_Id_T stateid, const HSM_Transition_T **pnext_trans) {
        HSM_State_T state = getStateById(stateid);
        if (HSM_SKIND_COMPOSITE == state.type()) {
            //if the target is a composite state, need goto its initial state;
            state = getStateById(state.initialId());
            assert(HSM_NO_INITIAL_STATE != stateid);
            if (HSM_SKIND_INITIAL != state.type()) {
                HSM_Log("%s composite's initial state should be INITIAL type\n", __func__);
            }
            assert(HSM_SKIND_INITIAL == state.type());
        }

        /* a transition chain only ends with a simple state. */
        /* if (target==composite && its initial == simple) ignore to add its initial; why?
         * a composite's initial state cannot be a simple type state?
         * this may a question here */
        if (HSM_SKIND_SIMPLE != state.type()) {
            *pnext_trans = matchValidTransition(stateid, HSM_NO_EVENT);
        }
    }

    void addToTransitionChain(const HSM_State_Id_T sourceId, const HSM_Transition_T *ptrans) {
        assert(ptrans != NULL);
        HSM_Log("%s @state:%s, event:%d, target:%s\n",
                __func__, getStateNameById(sourceId).c_str(),
                ptrans->event, getStateNameById(ptrans->targetId).c_str());

        bool done = false;
        HSM_State_T source = getStateById(sourceId);
        const HSM_Transition_T *final_trans = NULL;
        HSM_Transition_T trans {};

        //add detailed simple state as the target to transition chain
        while ((ptrans != NULL) && (true != done)) {
            appendTransChain(*ptrans);
            HSM_State_Id_T targetId = getTransTargetId(*ptrans);
            if ((HSM_SAME_STATE == targetId) || (sourceId == targetId)) {
                /* same state: internal transition */
                done = true;
                HSM_Log("%s done @internal transition for state:%s\n", __func__, source.name().c_str());
                break;
            }
            //not internal transition
            HSM_State_T target = getStateById(targetId);
            if (HSM_SKIND_FINAL == target.type()) {
                HSM_State_Id_T targetparentId = target.parentId();
                final_trans = ptrans; /* keep track that we had a final state */
                //go to parent's out path for a final state
                HSM_State_T parent = getStateById(targetparentId);
                std::vector<HSM_Transition_T> transTable = parent.transitionTable();
                HSM_TransChainIterator_T it_trans = transTable.begin();
                HSM_Log("%s searching complete event of trans for final state:%s", __func__, target.name().c_str());
                //get final's parent's complete event trans
                for (; it_trans != transTable.end(); it_trans++) {
                    trans = *it_trans;
                    if (HSM_COMPLETION_EVENT == trans.event) {
                        ptrans = &trans;
                        break;
                    } else {
                        ptrans = NULL;
                    }
                }
            } else {
                HSM_Log("%s to complete trans for external state:%s", __func__, target.name().c_str());
                completeTransitionChain(targetId, &ptrans);
                done = true;
            }
        } //end for while loop

        if (ptrans == NULL) {
            clearTransChain();
            if (final_trans != NULL) {
                HSM_Log("%s error: No path out of a final state:%d\n", __func__, final_trans->targetId);
            }
        }
    }

    const HSM_Transition_T *matchValidTransition(const HSM_State_Id_T stateid, HSM_Event_T event) {
        HSM_Transition_T candidate {};
        bool candidate_valid = false;
        HSM_Transition_T else_candidate {};
        bool else_candidate_valid = false;
        HSM_Transition_T const *pcandidate = NULL;

        HSM_State_T state = getStateById(stateid);
        HSM_Log("%s for state:%s, event:%d\n", __func__, getStateNameById(stateid).c_str(), event);
        HSM_Transition_T trans {};

        std::vector<HSM_Transition_T> transTable = state.transitionTable();
        for (HSM_TransChainIterator_T it_trans = transTable.begin(); it_trans != transTable.end(); it_trans++) {
            trans = *it_trans;
            if ((HSM_NO_EVENT == event) || (event == trans.event)) {
                HSM_Log("?event:%d, trans.event:%d,action:%s\n", event, trans.event, trans.actionName.c_str());
                HSM_Guard_T guard = trans.guard;
                if (!guard/* guard is nullptr: no guard */) {
                    /* Remember this transition but keep looking */
                    else_candidate = trans;
                    else_candidate_valid = true;
                    HSM_Log("?guard no guard @state:%s, event:%d\n", getStateNameById(stateid).c_str(), event);
                } else if (guard(*mpStatechart)) {
                    // add to trans chain;
                    candidate = trans;
                    HSM_Log("?guard true @state:%s, event:%d\n", getStateNameById(stateid).c_str(), event);
                    addToTransitionChain(stateid, &candidate);
                    if (0 != transChainSize()) {
                        /* chain not broken */
                        candidate_valid = true;
                        pcandidate = &candidate;
                        break;
                    }
                } else {
                    /* log guard return false */
                    HSM_Log("?guard false @state:%s, event:%d\n", getStateNameById(stateid).c_str(), event);
                }
            }
        }

        if ((true != candidate_valid) && (true == else_candidate_valid)) {
            //add to trans chain;
            pcandidate = &else_candidate;
            HSM_Log("?event:%d, else trans.event:%d,action:%s\n",event, pcandidate->event, pcandidate->actionName.c_str());
            addToTransitionChain(stateid, &else_candidate);
            if (0 == transChainSize()) {
                /* chain broken */
                candidate_valid = false;
                pcandidate = NULL;
            }
        }
        HSM_Log("%s %s\n", __func__, (pcandidate!=NULL)?"ok":"failed.");
        return pcandidate;
    }

    bool buildTransitionChain(HSM_Event_T event) {
        //search for a valid transition(inner/outer) by the specified event
        HSM_State_Id_T stateId = currentStateId();
        HSM_State_T state = currentState();
        HSM_Log("%s current state=%d:%s\n", __func__, stateId, state.name().c_str());
        const HSM_Transition_T *ptrans = NULL;
        int maxdepth = 0;

        /* clear transchain first */
        clearTransChain();

        while ((NULL == ptrans) && (stateId != HSM_TOP) && (maxdepth++ <= 30)) {
            //match valid transition
            HSM_Log("%s match transition for state:%d:%s, event:%d begin\n", __func__, stateId, getStateNameById(stateId).c_str(), event);
            ptrans = matchValidTransition(stateId, event);
            if (NULL == ptrans) {
                mpStatechart->transChain.clear();
                state = getStateById(stateId);
                stateId = state.parentId();
            } else {
                HSM_Log("%s match transition for state:%d:%s, event:%d ok\n", __func__, stateId, getStateNameById(stateId).c_str(), event);
                break;
            }
        }
        if (maxdepth >= 30) {
            HSM_Log("%s match valid transition error, too deep maxdepth:%d\n", __func__, maxdepth);
            assert(0);
        }
        return (ptrans != NULL);
    }

    bool excuteTransAction(const HSM_State_Id_T stateid, const HSM_Transition_T &trans) {
        (void)stateid;
        if (trans.action) {
            HSM_Log("#action:%s @state:%s event:%d\n",
                   trans.actionName.c_str(), getStateNameById(stateid).c_str(), trans.event);
            trans.action(*mpStatechart);
        }
        return true;
    }

    bool isStateAncestor(HSM_State_Id_T ref, HSM_State_Id_T toConfirm) {
        bool isAncestor = false;
        if (HSM_TOP == toConfirm) {
            return true;
        }
        HSM_State_T source;
        HSM_State_Id_T parentId;
        while (true) {
            source = getStateById(ref);
            parentId = source.parentId();
            if (toConfirm == parentId) {
                isAncestor = true;
                break;
            }
            if (HSM_TOP == parentId) {
                break;
            }
            ref = parentId;
        }
        HSM_Log("%s %s is ancestor of %s? %s\n",
                __func__, getStateNameById(toConfirm).c_str(),
                getStateNameById(ref).c_str(),
                isAncestor?"yes":"no");
        return isAncestor;
    }

    HSM_State_Id_T getLeastCommonAncestor(HSM_State_Id_T sourceId, HSM_State_Id_T targetId) {
        HSM_State_Id_T lca = sourceId;
        HSM_State_T source;
        HSM_State_T target;

        if (sourceId == targetId) {
            source = getStateById(sourceId);
            lca = source.parentId();
            return lca;
        }

        while (true) {
            source = getStateById(sourceId);
            target = getStateById(targetId);
            if (isStateAncestor(sourceId, targetId)) {
                lca = targetId;
                break;
            } else if (isStateAncestor(targetId, sourceId)) {
                lca = sourceId;
                break;
            } else if (source.parentId() == getStateById(targetId).parentId()) {
                //has common parent
                lca = source.parentId();
                break;
            } else {
                //move up a level
                if (source.parentId() != HSM_TOP) {
                    sourceId = source.parentId();
                }
                if (target.parentId() != HSM_TOP) {
                    targetId = target.parentId();
                }
            }
        }
        if ((lca == HSM_TOP) || (lca > 0)) {
            //ok
        } else {
            HSM_Log("%s failed.\n", __func__);
            assert(0);
        }
        return lca;
    }

    void excuteEntryActions(HSM_State_Id_T targetId, HSM_State_Id_T lca) {
        (void)targetId;
        (void)lca;
        //HSM_Log("%s begin\n", __func__);
        HSM_State_T target;
        HSM_State_Id_T parentId;
        HSM_State_T parent;
        HSM_State_Id_T parentHistId;
        HSM_State_T parentHist;
        HSM_State_Kind_T target_type;
        HSM_State_Kind_T hist_type;
        //rebuilt list from target to its parent endwith lca;
        std::vector<HSM_State_Id_T> entry_states;
        while ((lca != targetId) && (HSM_TOP != targetId)) {
            entry_states.push_back(targetId);
            targetId = getStateById(targetId).parentId();
        }
        //enter into target from lca
        for (std::vector<HSM_State_Id_T>::reverse_iterator rit = entry_states.rbegin(); rit != entry_states.rend(); rit++) {
            targetId = *rit;
            HSM_Log("%s entry state:%s\n", __func__, getStateNameById(targetId).c_str());
            target = getStateById(targetId);
            target_type = target.type();
            parentId = target.parentId();
            if (target.entryAction()) {
                HSM_Log("#entry:%s, @state:%s\n", target.entryName().c_str(), target.name().c_str());
                target.entryAction()(*mpStatechart);
            }
            //save shallow history
            parent = getStateById(parentId);
            parentHistId = parent.historyId();
            if ((parentId != HSM_TOP) && (parentHistId != parentId)) {
                if (parentHistId != HSM_NO_HISTORY_STATE) {
                    parentHist = getStateById(parentHistId);
                    hist_type = parentHist.type();
                    if (target_type == HSM_SKIND_FINAL) {
                        //reset history state to its default state
                        //history has only 1 transiton
                        //mStateDfn.stateList[parentHistId].mTransitionTable[0].historyId = parentHist.mTransitionTable[0].targetId;
                        mStateDfn.stateList[parentHistId].saveHistoryState(parentHist.getHistoryDefaultTargetId());
                        HSM_Log("%s reset shallow history state:%s to default target:%s\n",
                                __func__, parentHist.name().c_str(),
                                getStateNameById(parentHist.getHistoryDefaultTargetId()).c_str());
                    } else if (HSM_SKIND_SHALLOW_HISTORY == hist_type) {
                        //update shallow history state to target state
                        //mStateDfn.stateList[parentHistId].mTransitionTable[0].historyId = targetId;
                        mStateDfn.stateList[parentHistId].saveHistoryState(targetId);
                        HSM_Log("%s save shallow history state:%s to target:%s\n",
                                __func__, parentHist.name().c_str(), getStateNameById(targetId).c_str());
                    }
                }
            }
        }
    }

    void excuteExitActions(HSM_State_Id_T sourceId, HSM_State_Id_T lca) {
        (void)sourceId;
        (void)lca;
        //HSM_Log("%s begin\n", __func__);
        HSM_State_T source = getStateById(sourceId);
        HSM_State_Id_T parentId = source.parentId();
        HSM_State_T parent;
        HSM_State_Id_T parentHistId;
        HSM_State_T parentHist;
        HSM_State_Kind_T sourceType = source.type();
        HSM_State_Kind_T hist_type;
        while (lca != sourceId) {
            source = getStateById(sourceId);
            sourceType = source.type();
            parentId = source.parentId();
            HSM_Log("%s exit state:%s\n", __func__, source.name().c_str());
            if (HSM_TOP != parentId) {
                parent = getStateById(parentId);
                parentHistId = parent.historyId();
                parentHist = getStateById(parentHistId);
                if (parentHistId != HSM_NO_HISTORY_STATE) {
                    hist_type = parentHist.type();
                    if (sourceType == HSM_SKIND_FINAL) {
                        //reset history state to its default state
                        //history has only 1 transiton
                        //mStateDfn.stateList[parentHistId].mTransitionTable[0].historyId = parentHist.mTransitionTable[0].targetId;
                        mStateDfn.stateList[parentHistId].saveHistoryState(parentHist.getHistoryDefaultTargetId());
                        HSM_Log("%s reset history state:%s to default target:%s\n",
                                __func__, parentHist.name().c_str(),
                                getStateById(parentHist.getHistoryDefaultTargetId()).name().c_str());
                    } else if ((HSM_SKIND_DEEP_HISTORY == hist_type)
                                && ((HSM_SKIND_COMPOSITE == sourceType) || (HSM_SKIND_SIMPLE == sourceType))) {
                        //update history state to current state
                        //mStateDfn.stateList[parentHistId].mTransitionTable[0].historyId = currentStateId();
                        mStateDfn.stateList[parentHistId].saveHistoryState(currentStateId());
                        HSM_Log("%s save deep history state:%s to current:%s\n",
                                __func__, parentHist.name().c_str(), currentStateName().c_str());
                    }
                }
            }
            if (source.exitAction()) {
                HSM_Log("#exit:%s, @state:%s\n", source.exitName().c_str(), source.name().c_str());
                source.exitAction()(*mpStatechart);
            }
            sourceId = parentId;
        }
    }

    void doTransitions() {
        HSM_Transition_T trans;
        HSM_State_Id_T targetId;
        HSM_State_Id_T sourceId = currentStateId();
        HSM_State_T target;
        HSM_State_Id_T lca = HSM_TOP;
        std::vector<HSM_Transition_T> transChain = mpStatechart->transChain;
        HSM_Log("\n%s begin current state:%s\n", __func__, getStateNameById(sourceId).c_str());
        HSM_Log(".%s transChainSize:%d\n", __func__, (int)transChainSize());
        for (HSM_TransChainIterator_T it_trans = transChain.begin(); it_trans != transChain.end(); it_trans++) {
            trans = *it_trans;
            targetId = trans.targetId;
            target = getStateById(targetId);
            HSM_Log("--%s current state:%s target:%s\n", __func__, getStateNameById(sourceId).c_str(), target.name().c_str());
            if ((HSM_SAME_STATE/*sourceId*/ == targetId) || (sourceId == targetId)) {
                /* same state: internel transition */
                HSM_Log("----%s internel transition: current state:%s\n", __func__, getStateNameById(sourceId).c_str());
                excuteTransAction(sourceId, trans);
            } else {
                /* Execute the exit actions, transition action and entry actions */
                lca = getLeastCommonAncestor(sourceId, targetId);
                HSM_Log("++++%s external transition: current state:%s target:%s, lca:%s\n",
                        __func__, getStateNameById(sourceId).c_str(), target.name().c_str(),
                        getStateNameById(lca).c_str());
                excuteExitActions(sourceId, lca);
                excuteTransAction(sourceId, trans);
                excuteEntryActions(targetId, lca);
                sourceId = targetId;
            }
        }
        setCurrentStateId(sourceId);
        HSM_Log("%s end current state:%s\n\n", __func__, getStateNameById(sourceId).c_str());
    }
};


#endif //__HSM_ENGINE_H__


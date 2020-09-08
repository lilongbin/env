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
#include "hsm_check.h"
#include "hsm_debug.h"
#include "hsm_log.h"
#include <assert.h>

#if 0
#    define mprint(...) printf(__VA_ARGS__)
#else
#    define mprint(...)
#endif

class HSM_Engine
{
private:
    HSM_StateChart_T *mpStatechart = NULL;
    HSM_State_Definition_T mStateDfn {};
    const HSM_State_T mHsmTopState {HSM_ST_KIND_INVALID, HSM_TOP, "HSM_TOP", HSM_NO_STATE, HSM_NO_INITIAL_STATE, HSM_NO_HISTORY_STATE, HSM_NO_ACTION, HSM_NO_ACTION, "", ""};
    const HSM_State_T mHsmSameState {HSM_ST_KIND_INVALID, HSM_SAME_STATE, "HSM_SAME_STATE", HSM_NO_STATE, HSM_NO_INITIAL_STATE, HSM_NO_HISTORY_STATE, HSM_NO_ACTION, HSM_NO_ACTION, "", ""};
    HSM_Debug m_debug {};
    HSM_Log m_log {};

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

    void setDebug(HSM_Debug &debug) {
        m_debug = debug;
    }

    bool start(const HSM_State_Definition_T &stateDfn, void * userObj) {
        logi("start statechart %s begin", stateDfn.statechartName.c_str());
        //mStateDfn = stateDfn;
        initWorkSpace(stateDfn, userObj);
        checkStateList(stateDfn.stateList);

        //build transition chain
        buildTransitionChain(HSM_NO_EVENT);
        //do tansition
        if (0 < transChainSize()) {
            doTransitions();
        }
        logi("start statechart %s ok, @state:%s", stateDfn.statechartName.c_str(), currentStateName().c_str());
        return true;
    }

    bool proccessMessage(HSM_Event_T event, std::vector<uint8_t> &data) {
        mpStatechart->event = event;
        mpStatechart->eventdata = data;
        logi("#%s @state:%s, event:%d-%s", __func__, currentStateName().c_str(), event, getEventName(event).c_str());
        mprint("\n#%s @state:%s, event:%d-%s\n", __func__, currentStateName().c_str(), event, getEventName(event).c_str());

        //build transition chain
        buildTransitionChain(event);
        //do transitions
        if (0 < transChainSize()) {
            doTransitions();
        }
        logi("#%s end, @state:%s", __func__, currentStateName().c_str());
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
    void initWorkSpace(const HSM_State_Definition_T &stateDfn, void *userObj) {
        mpStatechart = new HSM_StateChart_T();
        if (mpStatechart == NULL) {
            std::cout << "construct HSM_StateChart_T failed." << std::endl;
        }
        mpStatechart->currentStateId = 0;//@initial state

        mpStatechart->userObj = userObj;

        //init state definition object
        mStateDfn.statechartName = stateDfn.statechartName;
        HSM_StateListIterator_T slit;
        HSM_State_T state;
        HSM_State_Id_T id = 0;
        for (slit = stateDfn.stateList.begin();
                slit != stateDfn.stateList.end();
                slit++, id++) {
            state = *slit;
            if (state.id() != id) {
                logi("%s error, status id(%d:%s) and list index are inconsistent, expected id:%d",
                        __func__, state.id(), state.name().c_str(), id);
                assert(0);
            }
            mStateDfn.stateList.push_back(state);
        }
        logi("%s stateList size:%d", __func__, (int)mStateDfn.stateList.size());
        assert(0 != mStateDfn.stateList.size());
    }

    void checkStateList(const HSM_StateList_T &stateList) {
        HSM_Check checker = HSM_Check(stateList);
        bool res = checker.check();
        mprint("check info: %s", checker.getInfo().c_str());
        logi("check info: %s", checker.getInfo().c_str());
        logi("%s result:%s", __func__, res?"ok":"failure");
        assert(res == true);
    }

    std::string getEventName(HSM_Event_T event) {
        if (HSM_NO_EVENT == event) {
            return "HSM_NO_EVENT";
        } else if (HSM_COMPLETION_EVENT == event) {
            return "HSM_COMPLETION_EVENT";
        } else {
            std::string name = "unknown";
            if (m_debug.event_name_func()) {
                name = m_debug.event_name_func()(event);
            }
            return name;
        }
    }

    HSM_State_T &getStateById(const HSM_State_Id_T stateid) {
        int size = (int)mStateDfn.stateList.size();
        if (HSM_TOP == stateid) {
            return const_cast<HSM_State_T &>(mHsmTopState);
        } else if (HSM_SAME_STATE == stateid) {
            return const_cast<HSM_State_T &>(mHsmSameState);
        }
        if ((stateid < 0) || (stateid >= size)) {
            logi("%s state id(%d) out of range:%d", __func__, stateid, size);
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

    std::string currentStateName() {
        return getStateById(currentStateId()).name().c_str();
    }

    HSM_State_Id_T currentStateId() {
        return mpStatechart->currentStateId;
    }

    HSM_State_T & currentState() {
        return getStateById(currentStateId());
    }

    bool setCurrentStateId(HSM_State_Id_T id) {
        bool ret = false;
        logi("%s state:%s -> @state:%s", __func__, currentStateName().c_str(), getStateNameById(id).c_str());
        if ((0 < id) && ((size_t)id < mStateDfn.stateList.size())) {
            mpStatechart->currentStateId = id;
            ret = true;
        } else {
            ret = false;
            logi("%s invlaid state id:%d", __func__, id);
            assert(0);
        }
        //update currentStateName
        mpStatechart->currentStateName = currentStateName();
        return ret;
    }

    bool isStateAncestor(HSM_State_Id_T ref, HSM_State_Id_T toConfirm) {
        bool isAncestor = false;
        if (HSM_TOP == ref) {
            return false;
        }
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
        //logd("?%s is ancestor of %s? %s", getStateNameById(toConfirm).c_str(), getStateNameById(ref).c_str(), isAncestor?"yes":"no");
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

        int maxdepth = HSM_MAX_NESTING_LEVELS;
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
            if (maxdepth-- < 0) {
                logi("%s failed, checked too deep nesting level.", __func__);
            }
        }
        if ((lca == HSM_TOP) || (lca > 0)) {
            //ok
        } else {
            logi("%s failed.", __func__);
            assert(0);
        }
        return lca;
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
        if (HSM_NO_STATE == trans.targetId) {
            logi("%s no target to transfer, targetid:%d", __func__, trans.targetId);
            assert(0);
        }
        return targetId;
    }

    void completeTransitionChain(HSM_State_Id_T stateid, const HSM_Transition_T **pnext_trans) {
        HSM_State_T state = getStateById(stateid);
        if (HSM_ST_KIND_COMPOSITE == state.type()) {
            //if the target is a composite state, need goto its initial state;
            state = getStateById(state.initialId());
            assert(HSM_NO_INITIAL_STATE != stateid);
            if (HSM_ST_KIND_INITIAL != state.type()) {
                logi("%s composite's initial state should be INITIAL type", __func__);
            }
            assert(HSM_ST_KIND_INITIAL == state.type());
        }

        /* a transition chain only ends with a simple state. */
        /* if (target==composite && its initial == simple) ignore to add its initial; why?
         * a composite's initial state cannot be a simple type state?
         * this may a question here */
        if (HSM_ST_KIND_SIMPLE != state.type()) {
            *pnext_trans = matchValidTransition(stateid, HSM_NO_EVENT);
        }
    }

    void addToTransitionChain(const HSM_State_Id_T sourceId, const HSM_Transition_T *ptrans) {
        assert(ptrans != NULL);
        logi("%s source state:%s, event:%d-%s, target:%s",
                __func__, getStateNameById(sourceId).c_str(),
                ptrans->event, getEventName(ptrans->event).c_str(),
                getStateNameById(ptrans->targetId).c_str());

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
                logi("%s done. add internal transition for state:%s", __func__, source.name().c_str());
                break;
            }
            //not internal transition
            HSM_State_T target = getStateById(targetId);
            if (HSM_ST_KIND_FINAL == target.type()) {
                HSM_State_Id_T targetparentId = target.parentId();
                final_trans = ptrans; /* keep track that we had a final state */
                //go to parent's out path for a final state
                HSM_State_T parent = getStateById(targetparentId);
                logi("%s searching complete-event trans for final state:%s", __func__, target.name().c_str());
                //get final's parent's complete event trans
                const HSM_TransitionList_T transTable = parent.transitionTable();
                HSM_TransChainIterator_T it_trans;
                for (it_trans = transTable.begin(); it_trans != transTable.end(); it_trans++) {
                    trans = *it_trans;
                    if (HSM_COMPLETION_EVENT == trans.event) {
                        ptrans = &trans;
                        break; //break for loop, and add to transition chain;
                    } else {
                        ptrans = NULL;
                    }
                } //end for loop
            } else {
                logi("%s to complete trans for external state:%s", __func__, target.name().c_str());
                completeTransitionChain(targetId, &ptrans);
                done = true;
            }
        } //end while loop

        if (ptrans == NULL) {
            clearTransChain();
            if (final_trans != NULL) {
                logi("%s error: No path out of a final state:%d", __func__, final_trans->targetId);
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
        logi("%s for state:%s, event:%d-%s",
                __func__, getStateNameById(stateid).c_str(),
                event, getEventName(event).c_str());
        HSM_Transition_T trans {};

        const HSM_TransitionList_T transTable = state.transitionTable();
        HSM_TransChainIterator_T it_trans;
        for (it_trans = transTable.begin(); it_trans != transTable.end(); it_trans++) {
            trans = *it_trans;
            if ((HSM_NO_EVENT == event) || (event == trans.event)) {
                HSM_Guard_T guard = trans.guard;
                if (!guard/* no guard */) {
                    /* Remember this transition but keep looking */
                    else_candidate = trans;
                    else_candidate_valid = true;
                    logi("?event:%d-%s, guard:HSM_NO_GUARD, action:%s, target:%s, @state:%s",
                            event, getEventName(event).c_str(),
                            trans.actionName.c_str(), getStateNameById(trans.targetId).c_str(),
                            getStateNameById(stateid).c_str());
                } else if (guard(*mpStatechart)) {
                    // add to trans chain;
                    candidate = trans;
                    logi("?event:%d-%s, guard:true %s(), action:%s, target:%s, @state:%s",
                            event, getEventName(event).c_str(), trans.guardName.c_str(),
                            trans.actionName.c_str(), getStateNameById(trans.targetId).c_str(),
                            getStateNameById(stateid).c_str());
                    addToTransitionChain(stateid, &candidate);
                    if (0 != transChainSize()) {
                        /* chain not broken */
                        candidate_valid = true;
                        pcandidate = &candidate;
                        break;
                    }
                } else {
                    /* log guard return false */
                    logi("?event:%d-%s, guard:false %s(), @state:%s",
                            event, getEventName(event).c_str(),
                            trans.guardName.c_str(), getStateNameById(stateid).c_str());
                }
            }
        }

        if ((true != candidate_valid) && (true == else_candidate_valid)) {
            //add to trans chain;
            pcandidate = &else_candidate;
            logi("?event:%d-%s, use backup trans event:%d, action:%s, target:%s",
                    event, getEventName(event).c_str(), pcandidate->event, pcandidate->actionName.c_str(),
                    getStateNameById(pcandidate->targetId).c_str());
            addToTransitionChain(stateid, &else_candidate);
            if (0 == transChainSize()) {
                /* chain broken */
                candidate_valid = false;
                pcandidate = NULL;
            }
        }
        logi("%s %s", __func__, (pcandidate != NULL)?"ok":"failed.");
        return pcandidate;
    }

    bool buildTransitionChain(HSM_Event_T event) {
        //search for a valid transition(inner/outer) by the specified event
        HSM_State_Id_T stateId = currentStateId();
        HSM_State_T state = currentState();
        logi("%s @state:%s, event:%d-%s",
                __func__, state.name().c_str(), event, getEventName(event).c_str());
        const HSM_Transition_T *ptrans = NULL;
        int maxdepth = 0;

        /* clear transchain first */
        clearTransChain();

        while ((NULL == ptrans)
                && (HSM_TOP != stateId)
                && (maxdepth++ < HSM_MAX_NESTING_LEVELS)) {
            //match valid transition
            logi("%s match transition for state:%s, event:%d-%s begin",
                    __func__, getStateNameById(stateId).c_str(),
                    event, getEventName(event).c_str());
            ptrans = matchValidTransition(stateId, event);
            if (NULL == ptrans) {
                mpStatechart->transChain.clear();
                state = getStateById(stateId);
                stateId = state.parentId();
                logd("%s match transition for state:%s, event:%d-%s failed",
                        __func__, getStateNameById(stateId).c_str(),
                        event, getEventName(event).c_str());
            } else {
                logd("%s match transition for state:%s, event:%d-%s ok",
                        __func__, getStateNameById(stateId).c_str(),
                        event, getEventName(event).c_str());
                break;
            }
        }
        if (maxdepth >= HSM_MAX_NESTING_LEVELS) {
            logi("%s match valid transition error, too deep maxdepth:%d", __func__, maxdepth);
            assert(0);
        }
        return (ptrans != NULL);
    }

    void excuteExitActions(HSM_State_Id_T sourceId, HSM_State_Id_T lca) {
        //logi("%s begin", __func__);
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
            logi("%s exit state:%s", __func__, source.name().c_str());
            if (HSM_TOP != parentId) {
                parent = getStateById(parentId);
                parentHistId = parent.historyId();
                parentHist = getStateById(parentHistId);
                if (parentHistId != HSM_NO_HISTORY_STATE) {
                    hist_type = parentHist.type();
                    if (sourceType == HSM_ST_KIND_FINAL) {
                        //reset history state to its default state
                        //history has only 1 transiton (to its default state or saved state)
                        //reset to parentHist.mTransitionTable[0].targetId;
                        mStateDfn.stateList[parentHistId].saveHistoryState(parentHist.getHistoryDefaultTargetId());
                        logi("%s reset target of history state:%s to default target:%s",
                                __func__, parentHist.name().c_str(),
                                getStateById(parentHist.getHistoryDefaultTargetId()).name().c_str());
                    } else if ((HSM_ST_KIND_DEEP_HISTORY == hist_type)
                                && ((HSM_ST_KIND_COMPOSITE == sourceType) || (HSM_ST_KIND_SIMPLE == sourceType))) {
                        //update history state to current state
                        mStateDfn.stateList[parentHistId].saveHistoryState(currentStateId());
                        logi("%s save target of deep history state:%s to current state:%s",
                                __func__, parentHist.name().c_str(), currentStateName().c_str());
                    }
                }
            }
            if (source.exitAction()) {
                logi("#exit:%s, @state:%s", source.exitName().c_str(), source.name().c_str());
                source.exitAction()(*mpStatechart);
            }
            sourceId = parentId;
        }
    }

    bool excuteTransAction(const HSM_State_Id_T stateid, const HSM_Transition_T &trans) {
        (void)stateid;
        if (trans.action) {
            logi("#action:%s @state:%d-%s, event:%d-%s",
                   trans.actionName.c_str(), stateid,
                   getStateNameById(stateid).c_str(),
                   trans.event, getEventName(trans.event).c_str());
            trans.action(*mpStatechart);
        }
        return true;
    }

    void excuteEntryActions(HSM_State_Id_T targetId, HSM_State_Id_T lca) {
        //logi("%s begin", __func__);
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
        std::vector<HSM_State_Id_T>::reverse_iterator rit;
        for (rit = entry_states.rbegin(); rit != entry_states.rend(); rit++) {
            targetId = *rit;
            logi("%s entry state:%s", __func__, getStateNameById(targetId).c_str());
            target = getStateById(targetId);
            target_type = target.type();
            parentId = target.parentId();
            if (target.entryAction()) {
                logi("#entry:%s, @state:%s", target.entryName().c_str(), target.name().c_str());
                target.entryAction()(*mpStatechart);
            }
            //save shallow history
            parent = getStateById(parentId);
            parentHistId = parent.historyId();
            if ((parentId != HSM_TOP) && (parentHistId != parentId)) {
                if (parentHistId != HSM_NO_HISTORY_STATE) {
                    parentHist = getStateById(parentHistId);
                    hist_type = parentHist.type();
                    if (target_type == HSM_ST_KIND_FINAL) {
                        //reset history state to its default state
                        //history has only 1 transiton (to its default state or saved state)
                        //reset to parentHist.mTransitionTable[0].targetId;
                        mStateDfn.stateList[parentHistId].saveHistoryState(parentHist.getHistoryDefaultTargetId());
                        logi("%s reset target of shallow history state:%s to default target:%s",
                                __func__, parentHist.name().c_str(),
                                getStateNameById(parentHist.getHistoryDefaultTargetId()).c_str());
                    } else if (HSM_ST_KIND_SHALLOW_HISTORY == hist_type) {
                        //update shallow history state to target state
                        mStateDfn.stateList[parentHistId].saveHistoryState(targetId);
                        logi("%s save target of shallow history state:%s to target:%s",
                                __func__, parentHist.name().c_str(), getStateNameById(targetId).c_str());
                    }
                }
            }
        }
    }

    void doTransitions() {
        HSM_Transition_T trans {};
        HSM_State_Id_T targetId {};
        HSM_State_Id_T sourceId = currentStateId();
        HSM_State_T target {};
        HSM_State_Id_T lca = HSM_TOP;

        logd("%s begin current state:%s, trans count:%d",
                __func__, getStateNameById(sourceId).c_str(), (int)transChainSize());

        const HSM_TransitionList_T transChain = mpStatechart->transChain;
        showTransChainInfo(sourceId, transChain);
        HSM_TransChainIterator_T it_trans;
        for (it_trans = transChain.begin(); it_trans != transChain.end(); it_trans++) {
            trans = *it_trans;
            targetId = trans.targetId;
            target = getStateById(targetId);
            if ((HSM_SAME_STATE/*sourceId*/ == targetId) || (sourceId == targetId)) {
                /* same state: internel transition */
                mprint("\n--##%s (Internel): @state:%s\n", __func__, getStateNameById(sourceId).c_str());
                logi("--##%s (Internel): @state:%s", __func__, getStateNameById(sourceId).c_str());
                excuteTransAction(sourceId, trans);
            } else {
                /* Execute the exit actions, transition action and entry actions */
                lca = getLeastCommonAncestor(sourceId, targetId);
                mprint("\n++##%s (External): @state:%s --> @target:%s, LCA:%s\n",
                        __func__, getStateNameById(sourceId).c_str(),
                        target.name().c_str(), getStateNameById(lca).c_str());
                logi("++##%s (External): @state:%s --> @target:%s, LCA:%s",
                        __func__, getStateNameById(sourceId).c_str(),
                        target.name().c_str(), getStateNameById(lca).c_str());
                excuteExitActions(sourceId, lca);
                excuteTransAction(sourceId, trans);
                excuteEntryActions(targetId, lca);
                sourceId = targetId;
            }
        }
        setCurrentStateId(sourceId);
        logd("%s end current state:%s\n", __func__, getStateNameById(sourceId).c_str());
    }

    void showTransChainInfo(HSM_State_Id_T stateid, const HSM_TransitionList_T &transChain) {
        HSM_Transition_T trans;
        HSM_State_Id_T targetId;
        HSM_State_T target;
        std::string stateName = getStateNameById(stateid);
        std::string splitLine(70, '#');
        mprint("%s@state:%s\n", splitLine.c_str(), stateName.c_str());
        mprint("%22s  %15s    %15s    %10s\n", "event", "guard", "action", "target");
        logd("%s@state:%s", splitLine.c_str(), stateName.c_str());
        logd("%22s  %15s    %15s    %10s", "event", "guard", "action", "target");
        HSM_TransChainIterator_T it_trans;
        for (it_trans = transChain.begin(); it_trans != transChain.end(); it_trans++) {
            trans = *it_trans;
            targetId = trans.targetId;
            target = getStateById(targetId);
            /* event, guard, action, target */
            mprint("%22s, %15s(), %15s(), %10s\n", getEventName(trans.event).c_str(),
                    trans.guardName.c_str(), trans.actionName.c_str(), target.name().c_str());
            logd("%22s, %15s(), %15s(), %10s", getEventName(trans.event).c_str(),
                    trans.guardName.c_str(), trans.actionName.c_str(), target.name().c_str());
        }
        mprint("%s@target:%s\n", splitLine.c_str(), target.name().c_str());
        logd("%s@target:%s", splitLine.c_str(), target.name().c_str());
    }

    void logi(const char* fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        std::string logs = m_log.log_to_string(4, m_debug.debug_tag().c_str(), fmt, ap);
        va_end(ap);
        if (m_debug.log_func()) {
            m_debug.log_func()(logs);
        }
    }
    void logd(const char* fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        std::string logs = m_log.log_to_string(3, m_debug.debug_tag().c_str(), fmt, ap);
        va_end(ap);
        if (m_debug.log_func()) {
            m_debug.log_func()(logs);
        }
    }
};


#endif //__HSM_ENGINE_H__


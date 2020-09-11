/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* 
* File name   : HSM_Comm.h
* Author      : longbin
* Created date: 2020-08-24 13:55:32
* Description : 
*
*******************************************/

#ifndef __HSM_COMM_H__
#define __HSM_COMM_H__

#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <map>
#include <thread>
#include <atomic>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <functional>

#define HSM_MAX_NESTING_LEVELS 30

#define HSM_TOP (-1)
#define HSM_NO_STATE (-1)
#define HSM_NO_INITIAL_STATE HSM_NO_STATE
#define HSM_NO_HISTORY_STATE HSM_NO_STATE

#define HSM_NO_ACTION ((HSM_Action_T)NULL)
#define HSM_NO_GUARD ((HSM_Guard_T)NULL)

#define HSM_NO_EVENT (-1)
#define HSM_COMPLETION_EVENT (-2)

#define HSM_SAME_STATE (-2)

typedef enum {
   HSM_ST_KIND_INVALID = 0,
   HSM_ST_KIND_COMPOSITE,       /**< composite state (contains children) */
   HSM_ST_KIND_DEEP_HISTORY,    /**< deep history pseudostate */
   HSM_ST_KIND_FINAL,           /**< final state */
   HSM_ST_KIND_INITIAL,         /**< initial pseudostate */
   HSM_ST_KIND_JUNCTION,        /**< junction pseudostate */
   HSM_ST_KIND_SHALLOW_HISTORY, /**< shallow history pseudostate */
   HSM_ST_KIND_SIMPLE           /**< simple state (no children) */
} HSM_State_Kind_T; //State Kind

struct HSM_Transition_T;
struct HSM_StateChart_T;
typedef int32_t HSM_Event_T;
typedef int32_t HSM_State_Id_T;

typedef std::function<void (const HSM_StateChart_T &)> HSM_Action_T;
typedef std::function<bool (const HSM_StateChart_T &)> HSM_Guard_T;
typedef std::function<std::string (const HSM_Event_T &)> HSM_EventNameFunc_T;
typedef std::function<void (const HSM_StateChart_T &)> HSM_DbgFunc_T;
typedef std::function<void (const std::string &logs)> HSM_LogFunc_T;

typedef std::vector<HSM_Transition_T> HSM_TransitionList_T;
typedef std::vector<HSM_Transition_T>::const_iterator HSM_TransChainIterator_T;

typedef struct HSM_Transition_T {
   HSM_Event_T event;    /**< Event to respond to */
   HSM_Guard_T guard;    /**< Guard function */
   HSM_Action_T action;  /**< Action function */
   HSM_State_Id_T targetId; /**< Target state ID or default state for history */
   HSM_State_Id_T historyId; /**< default state for history */
   std::string guardName;  /**< Name of guard function */
   std::string actionName; /**< Name of action function */
} HSM_Transition_T;

typedef struct HSM_StateChart_T {
    HSM_State_Id_T                currentStateId;
    std::string                   currentStateName;
    HSM_State_Id_T                previousStateId;
    //bool                          isProcessingEvt;
    HSM_TransitionList_T          transChain;
    void *                        userObj;
    HSM_Event_T                   event;
    std::vector<uint8_t>          eventdata;
} HSM_StateChart_T;


#endif //__HSM_COMM_H__


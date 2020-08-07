/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* 
* File name   : xservice_hsm_mgr.c
* Author      : longbin
* Created date: 2020-08-05 15:08:52
* Description : 
*
*******************************************/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "xservice_hsm_mgr_impl.h"

/* state chart declare */
#include "xservice_hsm_mgr_declare.h"

typedef struct XServiceHsmObject_T {
    HSM_Statechart_T statechart;
    char user_input[1024];
} XServiceHsmObject_T;
static XServiceHsmObject_T gs_xservice_hsm_object;

static void onError(const HSM_Statechart_T* pstatechart);
static void resulted(const HSM_Statechart_T* pstatechart);
static void proc_signed(const HSM_Statechart_T* pstatechart);
static void proc_lead0(const HSM_Statechart_T* pstatechart);
static void proc_int(const HSM_Statechart_T* pstatechart);
static void proc_dot(const HSM_Statechart_T* pstatechart);
static void proc_decimal(const HSM_Statechart_T* pstatechart);
static void prompt_none(const HSM_Statechart_T* pstatechart);
static void prompt_reset(const HSM_Statechart_T* pstatechart);

static void debugger(const HSM_Statechart_T* pstatechart);

/* state chart define */
#include "hsm_define_statechart.h"


/**
 * This function is called to shutdown the HSM.
 */
void XService_HSM_Shutdown(void) {
    printf("%s\n", __func__);
}

/**
 * This function is called to start the TboxAudioMgr HSM.
 */
void XService_HSM_Start(void) {
    printf("%s\n", __func__);
    HSM_Debug_Control_T dbg_ctrl = {0};

    dbg_ctrl.chart_id = "XServiceMgrHsm";
    dbg_ctrl.dbg_module_id = 1;
#if XSERVICE_DEBUG_STATE_CHART
    dbg_ctrl.debug_func = debugger;
#else
    dbg_ctrl.debug_func = debugger;
    dbg_ctrl.debug_func = NULL;
#endif
    dbg_ctrl.perform_check = false;
    dbg_ctrl.get_event_name = NULL;

    memset(&gs_xservice_hsm_object, 0, sizeof(gs_xservice_hsm_object));
    HSM_Init(&gs_xservice_hsm_object.statechart, &XSERVICE_MGR_HSM_Defn, &gs_xservice_hsm_object, &dbg_ctrl);
    HSM_Begin(&gs_xservice_hsm_object.statechart);
}

HSM_Statechart_T * XService_HSM_Get(void) {
    return &gs_xservice_hsm_object.statechart;
}

bool_t XService_SendMsgToHSM(int event_id, const void * pdata, const size_t data_size) {
    bool_t event_used = true;
    (void)data_size;
    if (pdata == NULL) {
        /* pdata can be empty; */
    }
    //printf("%s evt:%d-%s\n", __func__, event_id, event2str(event_id));
    HSM_Process_Event(&gs_xservice_hsm_object.statechart, event_id, pdata);
    return event_used;
}

static void onError(const HSM_Statechart_T* pstatechart) {
    if (pstatechart == NULL) {
        DBG_ASSERT(0);
        return;
    }
    XServiceHsmObject_T * pobj = (XServiceHsmObject_T *)pstatechart->this_ptr;
    const char *current_state_name = HSM_Get_State_Name(pstatechart, pstatechart->current_state);
    char const* pevt_data = (char const *)(pstatechart->event_data);
    printf("\r[%d-%s] [%s]: Invalid char %d(%c)\n",
            pstatechart->current_state, current_state_name,
            __func__, pevt_data[0], pevt_data[0]);
    printf("\t\thas ignored, please continue input\n");
    printf("%s", pobj->user_input); fflush(stdout);
}

static void resulted(const HSM_Statechart_T* pstatechart) {
    if (pstatechart == NULL) {
        DBG_ASSERT(0);
        return;
    }
    XServiceHsmObject_T * pobj = (XServiceHsmObject_T *)pstatechart->this_ptr;
    const char *current_state_name = HSM_Get_State_Name(pstatechart, pstatechart->current_state);
    printf("\r[%d-%s] [%s]: %s\n",
            pstatechart->current_state, current_state_name,
            __func__, pobj->user_input);
    memset(pobj->user_input, 0, sizeof(pobj->user_input));
}

static void proc_signed(const HSM_Statechart_T* pstatechart) {
    if (pstatechart == NULL) {
        DBG_ASSERT(0);
        return;
    }
    XServiceHsmObject_T * pobj = (XServiceHsmObject_T *)pstatechart->this_ptr;
    const char *current_state_name = HSM_Get_State_Name(pstatechart, pstatechart->current_state);
    char const* pevt_data = (char const *)(pstatechart->event_data);
    strncat(pobj->user_input, pevt_data, 1);
    printf("\r[%d-%s] [%s]: %d(%c) (%s)\n",
            pstatechart->current_state, current_state_name,
            __func__, pevt_data[0], pevt_data[0], pobj->user_input);
    printf("\tuncompleted, you can continue type 0123456789\n");
    printf("%s", pobj->user_input); fflush(stdout);
}

static void proc_lead0(const HSM_Statechart_T* pstatechart) {
    if (pstatechart == NULL) {
        DBG_ASSERT(0);
        return;
    }
    XServiceHsmObject_T * pobj = (XServiceHsmObject_T *)pstatechart->this_ptr;
    const char *current_state_name = HSM_Get_State_Name(pstatechart, pstatechart->current_state);
    char const* pevt_data = (char const *)(pstatechart->event_data);
    strncat(pobj->user_input, pevt_data, 1);
    printf("\r[%d-%s] [%s]: %d(%c) (%s)\n",
            pstatechart->current_state, current_state_name,
            __func__, pevt_data[0], pevt_data[0], pobj->user_input);
    printf("\tvalidity, you can continue type .\n");
    printf("%s", pobj->user_input); fflush(stdout);
}

static void proc_int(const HSM_Statechart_T* pstatechart) {
    if (pstatechart == NULL) {
        DBG_ASSERT(0);
        return;
    }
    XServiceHsmObject_T * pobj = (XServiceHsmObject_T *)pstatechart->this_ptr;
    const char *current_state_name = HSM_Get_State_Name(pstatechart, pstatechart->current_state);
    char const* pevt_data = (char const *)(pstatechart->event_data);
    strncat(pobj->user_input, pevt_data, 1);
    printf("\r[%d-%s] [%s]: %d(%c) (%s)\n",
            pstatechart->current_state, current_state_name,
            __func__, pevt_data[0], pevt_data[0], pobj->user_input);
    printf("\tvalidity, you can continue type 0123456789.\n");
    printf("%s", pobj->user_input); fflush(stdout);
}

static void proc_dot(const HSM_Statechart_T* pstatechart) {
    if (pstatechart == NULL) {
        DBG_ASSERT(0);
        return;
    }
    XServiceHsmObject_T * pobj = (XServiceHsmObject_T *)pstatechart->this_ptr;
    const char *current_state_name = HSM_Get_State_Name(pstatechart, pstatechart->current_state);
    char const* pevt_data = (char const *)(pstatechart->event_data);
    strncat(pobj->user_input, pevt_data, 1);
    printf("\r[%d-%s] [%s]: %d(%c) (%s)\n",
            pstatechart->current_state, current_state_name,
            __func__, pevt_data[0], pevt_data[0], pobj->user_input);
    printf("\tuncompleted, you can continue type 0123456789\n");
    printf("%s", pobj->user_input); fflush(stdout);
}

static void proc_decimal(const HSM_Statechart_T* pstatechart) {
    if (pstatechart == NULL) {
        DBG_ASSERT(0);
        return;
    }
    XServiceHsmObject_T * pobj = (XServiceHsmObject_T *)pstatechart->this_ptr;
    const char *current_state_name = HSM_Get_State_Name(pstatechart, pstatechart->current_state);
    char const* pevt_data = (char const *)(pstatechart->event_data);
    strncat(pobj->user_input, pevt_data, 1);
    printf("\r[%d-%s] [%s]: %d(%c) (%s)\n",
            pstatechart->current_state, current_state_name,
            __func__, pevt_data[0], pevt_data[0], pobj->user_input);
    printf("\tvalidity, you can continue type 0123456789\n");
    printf("%s", pobj->user_input); fflush(stdout);
}

static void prompt_none(const HSM_Statechart_T* pstatechart) {
    if (pstatechart == NULL) {
        DBG_ASSERT(0);
        return;
    }
    (void)pstatechart;
    printf("Welcome, please type +-0123456789\n");
}

static void prompt_reset(const HSM_Statechart_T* pstatechart) {
    if (pstatechart == NULL) {
        DBG_ASSERT(0);
        return;
    }
    (void)pstatechart;
    printf("restart, please type +-0123456789\n");
}

static void debugger(const HSM_Statechart_T* pstatechart) {
    if (pstatechart == NULL) {
        DBG_ASSERT(0);
        return;
    }
    XServiceHsmObject_T * pobj = (XServiceHsmObject_T *)pstatechart->this_ptr;
    (void)pobj;
    const char *current_state_name = HSM_Get_State_Name(pstatechart, pstatechart->current_state);
    const char *event_name = event2str(pstatechart->event);
    printf("\r[%s][%d-%s] [%s] evt:%d-%s\n",
            pstatechart->dbg.chart_id,
            pstatechart->current_state, current_state_name,
            __func__,
            pstatechart->event, event_name);
}


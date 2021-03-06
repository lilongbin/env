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
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include "xservice_hsm_mgr_impl.h"

/* statechart define */
#include "hsm_define_statechart.h"

#define DBG_ASSERT(x) assert(x)

typedef struct XServiceHsmObject_T {
    //HSM_Statechart_T statechart;
    char working_data[1024];
    char extra_data[1024];
} XServiceHsmObject_T;
static XServiceHsmObject_T gs_xservice_hsm_object;
static HSM_Statechart_T gs_statechart;

int __hsm_log_func(int prio, const char* tag, const char *fmt, ...) {
    int val = 0;
    char space[1024] = { 0 };
    (void)prio;
    (void)tag;
    if (tag != NULL) {
        val = snprintf(space, sizeof(space), "%s: ", tag);
    }
    va_list ap;
    va_start(ap, fmt);
    val += vsnprintf(space+val, sizeof(space)-val, fmt, ap);
    printf("%s\n", space);
    va_end(ap);
    return val;
}

const char * getEventName(const int event_id) {
    switch (event_id) {
        EVENT2STRS
        default:
            return "unkown";
    }
}

/*
 * implement declared functions auto-generated by gen_hsm here
 */
void onError(const HSM_Statechart_T* pstatechart) {
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
    printf("%s", pobj->working_data); fflush(stdout);
}

void onInterupt1(const HSM_Statechart_T* pstatechart) {
    if (pstatechart == NULL) {
        DBG_ASSERT(0);
        return;
    }
    XServiceHsmObject_T * pobj = (XServiceHsmObject_T *)pstatechart->this_ptr;
    printf("\n%s entry reading dec:%s, str:%s\n", __func__, pobj->working_data, pobj->extra_data);
}

void onInterupt2(const HSM_Statechart_T* pstatechart) {
    if (pstatechart == NULL) {
        DBG_ASSERT(0);
        return;
    }
    XServiceHsmObject_T * pobj = (XServiceHsmObject_T *)pstatechart->this_ptr;
    printf("\n%s exit reading dec:%s, str:%s\n", __func__, pobj->working_data, pobj->extra_data);
}

void proc_close(const HSM_Statechart_T* pstatechart) {
    if (pstatechart == NULL) {
        DBG_ASSERT(0);
        return;
    }
    XServiceHsmObject_T * pobj = (XServiceHsmObject_T *)pstatechart->this_ptr;
    printf("\n%s %s has been cleared\n", __func__, pobj->extra_data);
}

void proc_decimal(const HSM_Statechart_T* pstatechart) {
    if (pstatechart == NULL) {
        DBG_ASSERT(0);
        return;
    }
    XServiceHsmObject_T * pobj = (XServiceHsmObject_T *)pstatechart->this_ptr;
    const char *current_state_name = HSM_Get_State_Name(pstatechart, pstatechart->current_state);
    char const* pevt_data = (char const *)(pstatechart->event_data);
    strncat(pobj->working_data, pevt_data, 1);
    printf("\r[%d-%s] [%s]: %d(%c) (%s)\n",
            pstatechart->current_state, current_state_name,
            __func__, pevt_data[0], pevt_data[0], pobj->working_data);
    printf("\tvalidity, you can continue type 0123456789\n");
    printf("%s", pobj->working_data); fflush(stdout);
}

void proc_dot(const HSM_Statechart_T* pstatechart) {
    if (pstatechart == NULL) {
        DBG_ASSERT(0);
        return;
    }
    XServiceHsmObject_T * pobj = (XServiceHsmObject_T *)pstatechart->this_ptr;
    const char *current_state_name = HSM_Get_State_Name(pstatechart, pstatechart->current_state);
    char const* pevt_data = (char const *)(pstatechart->event_data);
    strncat(pobj->working_data, pevt_data, 1);
    printf("\r[%d-%s] [%s]: %d(%c) (%s)\n",
            pstatechart->current_state, current_state_name,
            __func__, pevt_data[0], pevt_data[0], pobj->working_data);
    printf("\tuncompleted, you can continue type 0123456789\n");
    printf("%s", pobj->working_data); fflush(stdout);
}

void proc_int(const HSM_Statechart_T* pstatechart) {
    if (pstatechart == NULL) {
        DBG_ASSERT(0);
        return;
    }
    XServiceHsmObject_T * pobj = (XServiceHsmObject_T *)pstatechart->this_ptr;
    const char *current_state_name = HSM_Get_State_Name(pstatechart, pstatechart->current_state);
    char const* pevt_data = (char const *)(pstatechart->event_data);
    strncat(pobj->working_data, pevt_data, 1);
    printf("\r[%d-%s] [%s]: %d(%c) (%s)\n",
            pstatechart->current_state, current_state_name,
            __func__, pevt_data[0], pevt_data[0], pobj->working_data);
    printf("\tvalidity, you can continue type 0123456789.\n");
    printf("%s", pobj->working_data); fflush(stdout);
}

void proc_lead0(const HSM_Statechart_T* pstatechart) {
    if (pstatechart == NULL) {
        DBG_ASSERT(0);
        return;
    }
    XServiceHsmObject_T * pobj = (XServiceHsmObject_T *)pstatechart->this_ptr;
    const char *current_state_name = HSM_Get_State_Name(pstatechart, pstatechart->current_state);
    char const* pevt_data = (char const *)(pstatechart->event_data);
    strncat(pobj->working_data, pevt_data, 1);
    printf("\r[%d-%s] [%s]: %d(%c) (%s)\n",
            pstatechart->current_state, current_state_name,
            __func__, pevt_data[0], pevt_data[0], pobj->working_data);
    printf("\tvalidity, you can continue type .\n");
    printf("%s", pobj->working_data); fflush(stdout);
}

void proc_open(const HSM_Statechart_T* pstatechart) {
    if (pstatechart == NULL) {
        DBG_ASSERT(0);
        return;
    }
    XServiceHsmObject_T * pobj = (XServiceHsmObject_T *)pstatechart->this_ptr;
    memset(pobj->extra_data, 0, sizeof(pobj->extra_data));
    printf("%s please input your string\n", __func__);
}

void proc_reading(const HSM_Statechart_T* pstatechart) {
    if (pstatechart == NULL) {
        DBG_ASSERT(0);
        return;
    }
    XServiceHsmObject_T * pobj = (XServiceHsmObject_T *)pstatechart->this_ptr;
    const char *current_state_name = HSM_Get_State_Name(pstatechart, pstatechart->current_state);
    char const* pevt_data = (char const *)(pstatechart->event_data);
    strncat(pobj->extra_data, pevt_data, 1);
    printf("\r[%d-%s] [%s]: %d(%c) (%s)\n",
            pstatechart->current_state, current_state_name,
            __func__, pevt_data[0], pevt_data[0], pobj->extra_data);
    printf("%s", pobj->extra_data); fflush(stdout);
}

void proc_signed(const HSM_Statechart_T* pstatechart) {
    if (pstatechart == NULL) {
        DBG_ASSERT(0);
        return;
    }
    XServiceHsmObject_T * pobj = (XServiceHsmObject_T *)pstatechart->this_ptr;
    const char *current_state_name = HSM_Get_State_Name(pstatechart, pstatechart->current_state);
    char const* pevt_data = (char const *)(pstatechart->event_data);
    strncat(pobj->working_data, pevt_data, 1);
    printf("\r[%d-%s] [%s]: %d(%c) (%s)\n",
            pstatechart->current_state, current_state_name,
            __func__, pevt_data[0], pevt_data[0], pobj->working_data);
    printf("\tuncompleted, you can continue type 0123456789\n");
    printf("%s", pobj->working_data); fflush(stdout);
}

void prompt_none(const HSM_Statechart_T* pstatechart) {
    if (pstatechart == NULL) {
        DBG_ASSERT(0);
        return;
    }
    (void)pstatechart;
    printf("Welcome, please type +-0123456789\n");
}

void prompt_reset(const HSM_Statechart_T* pstatechart) {
    if (pstatechart == NULL) {
        DBG_ASSERT(0);
        return;
    }
    (void)pstatechart;
    printf("restart, please type +-0123456789\n");
}

void resulted(const HSM_Statechart_T* pstatechart) {
    if (pstatechart == NULL) {
        DBG_ASSERT(0);
        return;
    }
    XServiceHsmObject_T * pobj = (XServiceHsmObject_T *)pstatechart->this_ptr;
    const char *current_state_name = HSM_Get_State_Name(pstatechart, pstatechart->current_state);
    printf("\r[%d-%s] [%s]: %s\n",
            pstatechart->current_state, current_state_name,
            __func__, pobj->working_data);
    memset(pobj->working_data, 0, sizeof(pobj->working_data));
}

bool isEventEnter(const HSM_Statechart_T* pstatechart) {
    if (pstatechart == NULL) {
        DBG_ASSERT(0);
        return false;
    }
    const char *current_state_name = HSM_Get_State_Name(pstatechart, pstatechart->current_state);
    int event = pstatechart->event;
    printf("\r[%d-%s] [%s]: event:%d\n",
            pstatechart->current_state, current_state_name,
            __func__, event);
    return true;
}

/*
 * hsm user define from here
 */
void debugger(const HSM_Statechart_T* pstatechart) {
    if (pstatechart == NULL) {
        DBG_ASSERT(0);
        return;
    }
    XServiceHsmObject_T * pobj = (XServiceHsmObject_T *)pstatechart->this_ptr;
    (void)pobj;
    const char *current_state_name = HSM_Get_State_Name(pstatechart, pstatechart->current_state);
    const char *event_name = getEventName(pstatechart->event);
    printf("\r[%s][%d-%s] [%s] evt:%d-%s\n",
            pstatechart->dbg.chart_label,
            pstatechart->current_state, current_state_name,
            __func__,
            pstatechart->event, event_name);
}

/**
 * This function is called to start the TboxAudioMgr HSM.
 */
void XService_HSM_Start(void) {
    printf("%s\n", __func__);
    HSM_Debug_Control_T dbg_ctrl = {0};

    dbg_ctrl.chart_label = "XServiceMgrHsm";
#if XSERVICE_DEBUG_STATE_CHART
    dbg_ctrl.dbg_module_id = 1;
    dbg_ctrl.debug_func = debugger;
#else
    dbg_ctrl.debug_func = debugger;
    dbg_ctrl.debug_func = NULL;
#endif
    dbg_ctrl.log_level = TR_LVL_FAULT;
    //dbg_ctrl.log_level = TR_LVL_INFO_LO;
    dbg_ctrl.perform_check = true;
    dbg_ctrl.get_event_name = (HSM_Event_Name_Func_T)getEventName;

    memset(&gs_xservice_hsm_object, 0, sizeof(gs_xservice_hsm_object));
    #if 0
    HSM_Init(&gs_statechart, &XSERVICE_MGR_HSM_Defn, &gs_xservice_hsm_object, &dbg_ctrl);
    HSM_Begin(&gs_statechart);
    #else
    HSM_Start(&gs_statechart, &XSERVICE_MGR_HSM_Defn, &gs_xservice_hsm_object, &dbg_ctrl);
    #endif
}

HSM_Statechart_T * XService_HSM_Get(void) {
    return &gs_statechart;
}

bool_t XService_HSM_ProcMessage(int event_id, const void * pdata, const size_t data_size) {
    bool_t event_used = true;
    (void)data_size;
    if (pdata == NULL) {
        /* pdata can be empty; */
    }
    //printf("%s evt:%d-%s\n", __func__, event_id, event2str(event_id));
    HSM_Process_Event(&gs_statechart, event_id, pdata);
    return event_used;
}


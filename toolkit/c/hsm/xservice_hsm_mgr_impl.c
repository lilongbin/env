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
#include "xservice_hsm_mgr_impl.h"

#include "xservice_hsm_mgr_declare.h"

typedef struct XServiceHsmObject_T {
    HSM_Statechart_T statechart;
    char user_input[1024];
} XServiceHsmObject_T;
static XServiceHsmObject_T gs_xservice_hsm_object;

static void notice(HSM_Statechart_T* pstatechart);
static void resulted(HSM_Statechart_T* pstatechart);
static void append_none(HSM_Statechart_T* pstatechart);
static void append_signed(HSM_Statechart_T* pstatechart);
static void append_lead0(HSM_Statechart_T* pstatechart);
static void append_int(HSM_Statechart_T* pstatechart);
static void append_decimal(HSM_Statechart_T* pstatechart);
static void prompt_none(HSM_Statechart_T* pstatechart);
static void prompt_signed(HSM_Statechart_T* pstatechart);
static void prompt_lead0(HSM_Statechart_T* pstatechart);
static void prompt_integer(HSM_Statechart_T* pstatechart);
static void prompt_dot(HSM_Statechart_T* pstatechart);

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

    dbg_ctrl.chart_id = "XServiceMgr";
    dbg_ctrl.dbg_module_id = 1;
    dbg_ctrl.debug_func = NULL;
    dbg_ctrl.perform_check = false;
    dbg_ctrl.get_event_name = NULL;

    memset(&gs_xservice_hsm_object, 0, sizeof(gs_xservice_hsm_object));
    HSM_Init(&gs_xservice_hsm_object.statechart, &XSERVICE_MGR_HSM_Defn, &gs_xservice_hsm_object, &dbg_ctrl);
    HSM_Begin(&gs_xservice_hsm_object.statechart);
}

HSM_Statechart_T * XService_HSM_Get(void) {
    return &gs_xservice_hsm_object.statechart;
}

bool_t XService_SendMsgToHSM(int event_id, const void * data, size_t data_size) {
    bool_t event_used = true;
    switch (event_id) {
        case eTypedKeySign:
            break;
        case eTypedKeyNumber0:
            break;
        case eTypedKeyNumber1_9:
            break;
        case eTypedKeyDot:
            break;
        case eTypedKeyEnter:
            break;
        case eTypedKeyOther:
            break;
        default:
            break;
    }
    HSM_Process_Event(&gs_xservice_hsm_object.statechart, event_id, data);
    return event_used;
}

static void notice(HSM_Statechart_T* pstatechart) {
    XServiceHsmObject_T * pobj = (XServiceHsmObject_T *)pstatechart->this_ptr;
    char const* pevt_data = (char const *)(pstatechart->event_data);
    printf("%s: Invalid char %d(%c), has ignored, please continue input\n", __func__, pevt_data[0], pevt_data[0]);
}

static void resulted(HSM_Statechart_T* pstatechart) {
    XServiceHsmObject_T * pobj = (XServiceHsmObject_T *)pstatechart->this_ptr;
    printf("%s: %s\n", __func__, pobj->user_input);
    memset(pobj->user_input, 0, sizeof(pobj->user_input));
}

static void append_none(HSM_Statechart_T* pstatechart) {
    XServiceHsmObject_T * pobj = (XServiceHsmObject_T *)pstatechart->this_ptr;
    char const* pevt_data = (char const *)(pstatechart->event_data);
    strncat(pobj->user_input, pevt_data, 1);
    /* printf("%s: valid char: %s, you can continue.\n", __func__, pobj->user_input); */
}

static void append_signed(HSM_Statechart_T* pstatechart) {
    XServiceHsmObject_T * pobj = (XServiceHsmObject_T *)pstatechart->this_ptr;
    char const* pevt_data = (char const *)(pstatechart->event_data);
    strncat(pobj->user_input, pevt_data, 1);
    //printf("%s: valid char: %s, you can continue type 0123456789\n", __func__, pobj->user_input);
}

static void append_lead0(HSM_Statechart_T* pstatechart) {
    XServiceHsmObject_T * pobj = (XServiceHsmObject_T *)pstatechart->this_ptr;
    char const* pevt_data = (char const *)(pstatechart->event_data);
    strncat(pobj->user_input, pevt_data, 1);
    //printf("%s: valid char: %s, you should continue type .\n", __func__, pobj->user_input);
}

static void append_int(HSM_Statechart_T* pstatechart) {
    XServiceHsmObject_T * pobj = (XServiceHsmObject_T *)pstatechart->this_ptr;
    char const* pevt_data = (char const *)(pstatechart->event_data);
    strncat(pobj->user_input, pevt_data, 1);
    printf("%s: valid char: %s, you can continue type 0123456789.\n", __func__, pobj->user_input);
}

static void append_decimal(HSM_Statechart_T* pstatechart) {
    XServiceHsmObject_T * pobj = (XServiceHsmObject_T *)pstatechart->this_ptr;
    char const* pevt_data = (char const *)(pstatechart->event_data);
    strncat(pobj->user_input, pevt_data, 1);
    printf("%s: valid char: %s, you can continue type 0123456789\n", __func__, pobj->user_input);
}

static void prompt_none(HSM_Statechart_T* pstatechart) {
    (void)pstatechart;
    printf("Welcome, please type +-0123456789\n");
}

static void prompt_signed(HSM_Statechart_T* pstatechart) {
    (void)pstatechart;
    printf("uncompleted, please type 0123456789\n");
}

static void prompt_lead0(HSM_Statechart_T* pstatechart) {
    (void)pstatechart;
    printf("valid number, you can type .\n");
}

static void prompt_integer(HSM_Statechart_T* pstatechart) {
    (void)pstatechart;
    printf("valid number, you can type 0123456789.\n");
}

static void prompt_dot(HSM_Statechart_T* pstatechart) {
    (void)pstatechart;
    printf("uncompleted, please type 0123456789\n");
}


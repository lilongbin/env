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
#include "xservice_hsm_mgr.h"
#include "xservice_hsm_mgr_declare.h"

typedef struct XServiceObject_T {
    HSM_Statechart_T xservice_statechart;
    char user_input[1024];
} XServiceObject_T;
static XServiceObject_T gs_xservice_object;

static void display(HSM_Statechart_T* statechart);
static void clear(HSM_Statechart_T* statechart);
static void append(HSM_Statechart_T* statechart);
static void prompt_none(HSM_Statechart_T* statechart);
static void prompt_signed(HSM_Statechart_T* statechart);
static void prompt_lead0(HSM_Statechart_T* statechart);
static void prompt_integer(HSM_Statechart_T* statechart);
static void prompt_dot(HSM_Statechart_T* statechart);

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

    memset(&gs_xservice_object, 0, sizeof(gs_xservice_object));
    HSM_Init(&gs_xservice_object.xservice_statechart, &XSERVICE_MGR_HSM_Defn, &gs_xservice_object, &dbg_ctrl);
    HSM_Begin(&gs_xservice_object.xservice_statechart);
}

HSM_Statechart_T * XService_HSM_Get(void) {
    return &(gs_xservice_object.xservice_statechart);
}

bool_t XService_Send_Msg_To_HSM(int event_id, const void * data, size_t data_size) {
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
    HSM_Process_Event(&(gs_xservice_object.xservice_statechart), event_id, data);
    return event_used;
}

static void display(HSM_Statechart_T* statechart) {
    XServiceObject_T * pobj = (XServiceObject_T *)statechart->this_ptr;
    printf("%s %s\n", __func__, gs_xservice_object.user_input);
}

static void clear(HSM_Statechart_T* statechart) {
    XServiceObject_T * pobj = (XServiceObject_T *)statechart->this_ptr;
    printf("%s\n", __func__);
    memset(gs_xservice_object.user_input, 0, sizeof(gs_xservice_object.user_input));
}

static void append(HSM_Statechart_T* statechart) {
    XServiceObject_T * pobj = (XServiceObject_T *)statechart->this_ptr;
    char const* rcv_data = (char const *)(statechart->event_data);

    strncat(pobj->user_input, rcv_data, 1);
    printf("%s %s\n", __func__, gs_xservice_object.user_input);
}

static void prompt_none(HSM_Statechart_T* statechart) {
    printf("Please type +-01-9\n");
}
static void prompt_signed(HSM_Statechart_T* statechart) {
    printf("please type 01-9\n");
}
static void prompt_lead0(HSM_Statechart_T* statechart) {
    printf("please type .\n");
}
static void prompt_integer(HSM_Statechart_T* statechart) {
    printf("please type 01-9\n");
}
static void prompt_dot(HSM_Statechart_T* statechart) {
    printf("please type 01-9\n");
}


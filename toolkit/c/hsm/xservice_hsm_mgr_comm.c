/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* 
* File name   : xservice_hsm_mgr_comm.c
* Author      : longbin
* Created date: 2020-08-07 10:19:01
* Description : 
*
*******************************************/

#include <stdio.h>
#include <stdarg.h>
#include "xservice_hsm_mgr_comm.h"
#include "xservice_hsm_mgr_event.h"

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


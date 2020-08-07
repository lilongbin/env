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
#include "xservice_hsm_mgr_comm.h"
#include "xservice_hsm_mgr_event.h"

const char * event2str(const int event_id) {
    switch (event_id) {
        EVENT2STRS
        default:
            return "unkown";
    }
}


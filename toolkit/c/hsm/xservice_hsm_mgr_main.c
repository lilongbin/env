/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* 
* File name   : xservice_hsm_mgr_main.c
* Author      : longbin
* Created date: 2020-08-05 15:48:16
* Description : 
*
*******************************************/

#include <stdio.h>
#include "xservice_hsm_mgr_event.h"
#include "xservice_hsm_mgr.h"

const char * event2str(int event_id) {
    switch (event_id) {
        EVENT2STRS
        default:
            return "unkown";
    }
}
int main()
{
    XService_HSM_Start();
    int i = 0;
    char ch = 0;
    int evt = eTypedKeyOther;
    for (i = 0; i < 100; i++) {
        ch = getchar();
        if ((ch == '+')
            ||(ch == '-') ) {
            evt = eTypedKeySign;
        } else if (ch == '0') {
            evt = eTypedKeyNumber0;
        } else if ((ch >= '1') && (ch <= '9')) {
            evt = eTypedKeyNumber1_9;
        } else if (ch == '.') {
            evt = eTypedKeyDot;
        } else if ((ch == '\n')||(ch == '\r')) {
            evt = eTypedKeyEnter;
        } else {
            evt = eTypedKeyOther;
        }
        printf("send evt: %d:%s\n", evt, event2str(evt));
        XService_Send_Msg_To_HSM(evt, &ch, 1);
    }
    XService_HSM_Shutdown();
    return 0;
}


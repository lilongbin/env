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
#include <unistd.h>
#include <errno.h>
#include <termios.h>

#include "xservice_hsm_mgr_event.h"
#include "xservice_hsm_mgr_impl.h"

#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif

static struct termios raw_attr;
static struct termios new_attr;
char getch()
{
    char ch = 0;
    int ret = 0;
    (void)tcgetattr(STDIN_FILENO, &raw_attr);
    new_attr = raw_attr;
    new_attr.c_lflag &= ~ICANON;
    new_attr.c_lflag &= ~ECHO;
    new_attr.c_lflag |= ISIG;
    new_attr.c_cc[VMIN] = 1;
    new_attr.c_cc[VTIME] = 0;
    (void)tcsetattr(STDIN_FILENO, TCSANOW, &new_attr);
    ret = read(STDIN_FILENO, &ch, 1);
    (void)tcsetattr(STDIN_FILENO, TCSANOW, &raw_attr);
    if (ret == -1) {
        perror("read");
    }
    return ch;
}

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
    char ch = 0;
    int evt = eTypedKeyOther;
    //int i = 0;
    //for (i = 0; i < 100; i++) {
    for (;;) {
        ch = getch();
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
        //printf("send evt: %d-%s, data:%d-%c\n", evt, event2str(evt), ch, ch);
        XService_SendMsgToHSM(evt, &ch, 1);
    }
    XService_HSM_Shutdown();
    return 0;
}


/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* 
* File name   : xservice_hsm_mgr_main.cpp
* Author      : longbin
* Created date: 2020-08-05 15:48:16
* Description : 
*
*******************************************/

#include <cstdio>
#include <unistd.h>
#include <errno.h>
#include <termios.h>

#include "xservice_hsm_mgr_event.h"
#include "xservice_hsm_mgr_impl.h"

#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif

char getch()
{
    //need #include <termios.h>
    struct termios raw_attr;
    struct termios new_attr;
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

int main()
{
    XService_HSM_Start();
    char ch = 0;
    int evt = EVT_TypedKeyOther;
    //int i = 0;
    //for (i = 0; i < 100; i++) {
    for (;;) {
        ch = getch();
        if ((ch == '+')
            ||(ch == '-') ) {
            evt = EVT_TypedKeySign;
        } else if (ch == '0') {
            evt = EVT_TypedKeyNumber0;
        } else if ((ch >= '1') && (ch <= '9')) {
            evt = EVT_TypedKeyNumber1_9;
        } else if (ch == '.') {
            evt = EVT_TypedKeyDot;
        } else if ((ch == '\n')||(ch == '\r')) {
            evt = EVT_TypedKeyEnter;
        } else if (ch == '#') {
            evt = EVT_TypedKeyPound;
        } else {
            evt = EVT_TypedKeyOther;
        }
        printf("send evt: %d-%s, data:%d-%c\n", evt, getEventName(evt).c_str(), ch, ch);
        XService_HSM_ProcMessage(evt, &ch, sizeof(ch));
    }
    return 0;
}


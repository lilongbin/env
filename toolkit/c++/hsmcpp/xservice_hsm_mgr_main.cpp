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

#include "xservice_hsm_mgr_impl.h"

#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif

/* create HSM engine and state chart */
static HSM_Engine *mHsm = new HSM_Engine();
/* create userObj defined by user */
static XServiceHsmObject_T gs_xservice_hsm_object {};

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

void logfunc(const std::string &logs) {
    (void)logs;
    //std::cout << logs;
}

/**
 * This function is called to start the TboxAudioMgr HSM.
 */
void XService_HSM_Start(void) {
    printf("%s\n", __func__);
    HSM_State_Definition_T xservice_mgr_hsm_defn = get_XSERVICE_MGR_HSM_Defn();

    HSM_Debug debug = HSM_Debug(xservice_mgr_hsm_defn.statechartName, NULL, getEventName, 0, logfunc);
    mHsm->setDebug(debug);

    mHsm->start(xservice_mgr_hsm_defn, Init/* 0 */, &gs_xservice_hsm_object);
}

bool XService_HSM_ProcMessage(int event, const void * pdata, const size_t data_size) {
    bool event_used = true;
    (void)data_size;
    if (pdata == NULL) {
        /* pdata can be empty; */
    }
    std::vector<uint8_t> data((uint8_t *)pdata, (uint8_t *)pdata+data_size);
    printf("%s event:%d-%s\n", __func__, event, getEventName(event).c_str());
    mHsm->processMessage(event, data);
    return event_used;
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
        printf("send evt: %d-%s, data:%d(%c)\n", evt, getEventName(evt).c_str(), ch, ch);
        XService_HSM_ProcMessage(evt, &ch, sizeof(ch));
    }
    return 0;
}


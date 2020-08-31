/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* 
* File name   : hsm_debug.h
* Author      : longbin
* Created date: 2020-08-31 15:35:49
* Description : 
*
*******************************************/

#ifndef __HSM_DEBUG_H__
#define __HSM_DEBUG_H__

#include "hsm_comm.h"

class HSM_Debug
{
private:
    std::string m_debug_tag;
    HSM_DbgFunc_T m_debug_func;
    HSM_EventNameFunc_T m_get_event_name; //std::string getEventName(HSM_Event_T event);
    int m_log_level;
    HSM_LogFunc_T m_log_func; //void log(const std::string &str);

public:
    HSM_Debug() {
        m_debug_tag = __FILE__;
        m_debug_func = NULL;
        m_get_event_name = NULL;
        m_log_level = 0;
        m_log_func = NULL;
    }
    HSM_Debug(std::string tag, HSM_DbgFunc_T debug_func, HSM_EventNameFunc_T eventfunc, int level, HSM_LogFunc_T logfunc) {
        m_debug_tag = tag;
        m_debug_func = debug_func;
        m_get_event_name = eventfunc;
        m_log_level = level;
        m_log_func = logfunc;
    }
    ~HSM_Debug() {}
    std::string debug_tag() {
        return m_debug_tag;
    }
    HSM_DbgFunc_T debug_func() {
        return m_debug_func;
    }
    HSM_EventNameFunc_T event_name() {
        return m_get_event_name;
    }
    int log_level() {
        return m_log_level;
    }
    HSM_LogFunc_T log_func() {
        return m_log_func;
    }
};

#endif //__HSM_DEBUG_H__


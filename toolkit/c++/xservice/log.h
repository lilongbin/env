/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* 
* File name   : log.h
* Author      : longbin
* Created date: 2020-07-07 16:45:04
* Description : 
*
*******************************************/

#include <cstdarg>

#ifdef __cplusplus
extern "C" {
#endif

void set_log_level(int level);
int __print_log(int prio, const char* tag, const char* fmt, ...);

typedef enum LogPriority {
    LOG_UNKNOWN = 0,
    LOG_DEFAULT,    /* only for SetMinPriority() */
    LOG_VERBOSE,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL,
    LOG_SILENT,     /* only for SetMinPriority(); must be last */
} LogPriority;

#define LOG_STR(x) #x
#define LOG_XSTR(s) LOG_STR(s)

#ifndef LOG_TAG
#define LOG_TAG __FILE__ ":" LOG_XSTR(__LINE__)
#endif

#define LOG_PRI(pri, tag, ...) ((void)__print_log(pri, tag, __VA_ARGS__))

#define ALOGE(...) (LOG_PRI(LOG_ERROR,   LOG_TAG, __VA_ARGS__))
#define ALOGW(...) (LOG_PRI(LOG_WARN,    LOG_TAG, __VA_ARGS__))
#define ALOGI(...) (LOG_PRI(LOG_INFO,    LOG_TAG, __VA_ARGS__))
#define ALOGD(...) (LOG_PRI(LOG_DEBUG,   LOG_TAG, __VA_ARGS__))
#define ALOGV(...) (LOG_PRI(LOG_VERBOSE, LOG_TAG, __VA_ARGS__))

#ifdef __cplusplus
}  // extern "C"
#endif


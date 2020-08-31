/*****************************************
 * Copyright (C) 2020 * Ltd. All rights reserved.
 * 
 * File name   : hsm_log.h
 * Author      : longbin
 * Created date: 2020-08-31 16:02:07
 * Description : 
 *
 *******************************************/

#ifndef __HSM_LOG_H__
#define __HSM_LOG_H__

#include <cstdio>
#include <cstdarg>
#include <iostream>
#include <string>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

#include "hsm_comm.h"

#define gettid() syscall(SYS_gettid)

enum {
    LOG_UNKNOWN = 0,
    LOG_DEFAULT,    /* only for SetMinPriority() */
    LOG_VERBOSE,
    LOG_DEBUG,
    LOG_INFO,
    LOG_WARN,
    LOG_ERROR,
    LOG_FATAL,
    LOG_SILENT,     /* only for SetMinPriority(); must be last */
};

class HSM_Log
{
private:
    const char* __priority_name(int priority) {
        int idx;
        /* the first character of each string should be unique */
        static const char* prioritynames[] = { "Verbose", "Debug", "Info", "Warn", "Error", "Assert" };

        idx = (int)priority - (int)LOG_VERBOSE;
        if ((idx < 0)
                || (idx >= (int)(sizeof(prioritynames) / sizeof(prioritynames[0])))) {
            return "?unknown?";
        }
        return prioritynames[idx];
    }

    void __log_appendV(std::string* dst, const char* fmt, va_list ap) {
        // First try with a small fixed size buffer
        char space[1024] {};
        va_list backup_ap;
        va_copy(backup_ap, ap);
        int result = vsnprintf(space, sizeof(space), fmt, backup_ap);
        va_end(backup_ap);
        if (result < static_cast<int>(sizeof(space))) {
            if (result >= 0) {
                // Normal case -- everything fit.
                dst->append(space, result);
                return;
            }
            if (result < 0) {
                // Just an error.
                return;
            }
        }
        int length = result + 1;
        char* buf = new char[length];
        va_copy(backup_ap, ap);
        result = vsnprintf(buf, length, fmt, backup_ap);
        va_end(backup_ap);
        if (result >= 0 && result < length) {
            // It fit
            dst->append(buf, result);
        }
        delete[] buf;
    }

    std::string __log_vprint(int prio, const char* tag, const char* fmt, va_list ap) {
        std::string log_print_str;
        std::string timestamp_millis;
        struct tm *ptm;
        char timeBuf[32] {};
        uint32_t pid, tid;

        struct timespec ts {};
        (void)clock_gettime(CLOCK_REALTIME, &ts);
        time_t when = ts.tv_sec;//time(NULL);

        ptm = localtime(&when);
        strftime(timeBuf, sizeof(timeBuf), "%Y-%m-%d %H:%M:%S", ptm);
        std::string millis = std::to_string(ts.tv_nsec/1000000);
        if (millis.size() < 3) {
            //add lead '0'
            timestamp_millis.assign(3-millis.size(), '0');
        }
        timestamp_millis.append(millis);

        pid = getpid();
        tid = gettid();
        char chpri = __priority_name(prio)[0];

        log_print_str += std::string(timeBuf);
        log_print_str += "." + timestamp_millis + " ";
        log_print_str += std::to_string(pid) + " ";
        log_print_str += std::to_string(tid) + " ";
        log_print_str += std::string(1, chpri) + " ";
        log_print_str += std::string(tag) + ": ";
        __log_appendV(&log_print_str, fmt, ap);
        log_print_str += "\n";

        return log_print_str;
    }

public:
    std::string log_to_string(int prio, const char* tag, const char* fmt, va_list ap) {
        std::string logs = __log_vprint(prio, tag, fmt, ap);
        return logs;
    }

    int __print_log(int prio, const char* tag, const char* fmt, ...) {
        va_list ap;
        va_start(ap, fmt);
        if (tag == NULL) {
            tag = "";
        }
        std::string log = __log_vprint(prio, tag, fmt, ap);
        va_end(ap);
        return (int)log.size();
    }
};

#endif //__HSM_LOG_H__


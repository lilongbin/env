/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* 
* File name   : hsm_comm.h
* Author      : longbin
* Created date: 2020-08-05 12:56:36
* Description : 
*
*******************************************/

#ifndef __HSM_COMM_H__
#define __HSM_COMM_H__

#include <stdio.h>
#include <sys/types.h>

#   define false  (0)
#   define true   (!false)
//#   define NULL   ((void *)0)

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;

typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;

typedef char char_t;
typedef float float32_t;
typedef double float64_t;

#   ifndef __cplusplus
typedef uint8_t bool; /* Deprecated, use bool_t instead */
#   endif /* __cplusplus */
typedef uint8_t bool_t;

#   ifndef MAX
#     define  MAX(a,b) (((a)>(b)) ? (a) : (b))
#   endif

#   ifndef MIN
#      define  MIN(a,b) (((a)<(b)) ? (a) : (b))
#   endif

#   ifndef Num_Elems
#      define Num_Elems(array)    (sizeof(array)/sizeof(array[0]))
#   endif

#   ifndef UNUSED_PARAM
#      define UNUSED_PARAM(argument) (void)(argument)
#   endif

#   define HSM_MAX_TRANSITION_CHAIN 25
typedef int16_t HSM_Event_T;

#ifndef LOG_TAG
#define LOG_STR(x) #x
#define LOG_XSTR(s) LOG_STR(s)
#define LOG_TAG __FILE__ ":" LOG_XSTR(__LINE__)
#endif

extern int __hsm_log_func(int prio, const char *tag, const char *fmt, ...);
#define hsm_log_func(...) __hsm_log_func(0, LOG_TAG, __VA_ARGS__)
//#define hsm_log_func(fmt,...) printf(fmt, __VA_ARGS__)

#if 0
#      define Tr_Fault(msg)
#      define Tr_Fault_1(msg,a1)
#      define Tr_Fault_2(msg,a1,a2)
#      define Tr_Fault_3(msg,a1,a2,a3)
#      define Tr_Fault_4(msg,a1,a2,a3,a4)

#      define Tr_Warn(msg)
#      define Tr_Warn_1(msg,a1)
#      define Tr_Warn_2(msg,a1,a2)
#      define Tr_Warn_3(msg,a1,a2,a3)
#      define Tr_Warn_4(msg,a1,a2,a3,a4)

#      define Tr_Notify(msg)
#      define Tr_Notify_1(msg,a1)
#      define Tr_Notify_2(msg,a1,a2)
#      define Tr_Notify_3(msg,a1,a2,a3)
#      define Tr_Notify_4(msg,a1,a2,a3,a4)

#      define Tr_Info_Hi(msg)
#      define Tr_Info_Hi_1(msg,a1)
#      define Tr_Info_Hi_2(msg,a1,a2)
#      define Tr_Info_Hi_3(msg,a1,a2,a3)
#      define Tr_Info_Hi_4(msg,a1,a2,a3,a4)

#      define Tr_Info_Mid(msg)
#      define Tr_Info_Mid_1(msg,a1)
#      define Tr_Info_Mid_2(msg,a1,a2)
#      define Tr_Info_Mid_3(msg,a1,a2,a3)
#      define Tr_Info_Mid_4(msg,a1,a2,a3,a4)

#      define Tr_Info_Lo(msg)
#      define Tr_Info_Lo_1(msg,a1)
#      define Tr_Info_Lo_2(msg,a1,a2)
#      define Tr_Info_Lo_3(msg,a1,a2,a3)
#      define Tr_Info_Lo_4(msg,a1,a2,a3,a4)
#else
#      define Tr_Fault hsm_log_func
#      define Tr_Fault_1 hsm_log_func
#      define Tr_Fault_2 hsm_log_func
#      define Tr_Fault_3 hsm_log_func
#      define Tr_Fault_4 hsm_log_func

#      define Tr_Warn hsm_log_func
#      define Tr_Warn_1 hsm_log_func
#      define Tr_Warn_2 hsm_log_func
#      define Tr_Warn_3 hsm_log_func
#      define Tr_Warn_4 hsm_log_func

#      define Tr_Notify hsm_log_func
#      define Tr_Notify_1 hsm_log_func
#      define Tr_Notify_2 hsm_log_func
#      define Tr_Notify_3 hsm_log_func
#      define Tr_Notify_4 hsm_log_func

#      define Tr_Info_Hi hsm_log_func
#      define Tr_Info_Hi_1 hsm_log_func
#      define Tr_Info_Hi_2 hsm_log_func
#      define Tr_Info_Hi_3 hsm_log_func
#      define Tr_Info_Hi_4 hsm_log_func

#      define Tr_Info_Mid hsm_log_func
#      define Tr_Info_Mid_1 hsm_log_func
#      define Tr_Info_Mid_2 hsm_log_func
#      define Tr_Info_Mid_3 hsm_log_func
#      define Tr_Info_Mid_4 hsm_log_func

#      define Tr_Info_Lo hsm_log_func
#      define Tr_Info_Lo_1 hsm_log_func
#      define Tr_Info_Lo_2 hsm_log_func
#      define Tr_Info_Lo_3 hsm_log_func
#      define Tr_Info_Lo_4 hsm_log_func
#endif

#   define PBC_Require(expr_, ...) \
if(expr_){}                        \
else {                             \
   hsm_log_func(__VA_ARGS__);            \
}
#   define PBC_Require_1 PBC_Require
#   define PBC_Require_2 PBC_Require
#   define PBC_Require_3 PBC_Require
#   define PBC_Require_4 PBC_Require
#   define PBC_Ensure    PBC_Require
#   define PBC_Ensure_1  PBC_Require
#   define PBC_Ensure_2  PBC_Require
#   define PBC_Ensure_3  PBC_Require
#   define PBC_Ensure_4  PBC_Require
#   define PBC_Internal  PBC_Require
#   define PBC_Internal_1  PBC_Require
#   define PBC_Internal_2  PBC_Require
#   define PBC_Internal_3  PBC_Require
#   define PBC_Internal_4  PBC_Require
#   define PBC_Failed      PBC_Require
#   define PBC_Failed_1    PBC_Require
#   define PBC_Failed_2    PBC_Require
#   define PBC_Failed_3    PBC_Require
#   define PBC_Failed_4    PBC_Require

#define  TR_LVL_SILENT   -1
#define  TR_LVL_FAULT     0
#define  TR_LVL_WARN      1
#define  TR_LVL_NOTIFY    2
#define  TR_LVL_INFO_HI   3
#define  TR_LVL_INFO_MID  4
#define  TR_LVL_INFO_LO   5


#endif


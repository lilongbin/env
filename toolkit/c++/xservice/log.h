/*****************************************
* Copyright (C) 2020 * Ltd. All rights reserved.
* 
* File name   : log.h
* Author      : longbin
* Created date: 2020-07-07 16:45:04
* Description : 
*
*******************************************/

// #include "comm.h"

#define LOG_LEVEL 4

# if DEBUG

#ifndef LOG_TAG
#define LOG_TAG __FILE__
#endif
#define LOG_PRI(pri, pri_str, tag, fmt,...) if(LOG_LEVEL>=pri)printf(pri_str tag fmt"\r\n",##__VA_ARGS__)

#define ALOGE(fmt,...) LOG_PRI(1, "E ", LOG_TAG": ", fmt, ##__VA_ARGS__)
#define ALOGW(fmt,...) LOG_PRI(2, "W ", LOG_TAG": ", fmt, ##__VA_ARGS__)
#define ALOGI(fmt,...) LOG_PRI(3, "I ", LOG_TAG": ", fmt, ##__VA_ARGS__)
#define ALOGD(fmt,...) LOG_PRI(4, "D ", LOG_TAG": ", fmt, ##__VA_ARGS__)
#define ALOGV(fmt,...) LOG_PRI(5, "V ", LOG_TAG": ", fmt, ##__VA_ARGS__)

# else

#define ALOGI(fmt,...)
#define ALOGW(fmt,...)
#define ALOGE(fmt,...)
#define ALOGD(fmt,...)
#define ALOGV(fmt,...)

# endif


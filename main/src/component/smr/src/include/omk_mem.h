/*
  Copyright (c), 2021-2024, Oritek Tech. Co., Ltd.
 */

#ifndef OMK_MEM_H
#define OMK_MEM_H

#include <stdio.h>

#define mem_emerg_trace(fmt, ...)   \
    printf("EMERG [Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define mem_alert_trace(fmt, ...)   \
    printf("ALERT [Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define mem_crit_trace(fmt, ...)    \
    printf("CRIT [Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define mem_err_trace(fmt, ...)     \
    printf("ERR [Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define mem_warn_trace(fmt, ...)    \
    printf("WARN [Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define mem_notice_trace(fmt, ...)  \
    printf("NOTICE [Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define mem_info_trace(fmt, ...)    \
    printf("INFO [Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#define mem_debug_trace(fmt, ...)   \
    printf("DEBUG [Func]:%s [Line]:%d [Info]:" fmt, __FUNCTION__, __LINE__, ##__VA_ARGS__)

#endif /* OMK_MEM_H */

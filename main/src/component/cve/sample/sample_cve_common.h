#ifndef SAMPLE_CVE_COMMON_H
#define SAMPLE_CVE_COMMON_H

#include <stdio.h>
#ifndef OT_AVP_CVE_V200
#include "ot_eis_type.h"
#else
#include "ot_type.h"
#endif

#define RETURN_NONE
#define CVE_MAX_FILE_PATH 256
#define OT_AVP_CVE_MASK_NUM                     25
#define CHECK_RETURN(val, ret)                  \
    do {                                        \
        if (val) {                              \
            return ret;                         \
        }                                       \
    } while(0);

#define CHECK_RETURN_WITH_LOG(val, ret, ...)    \
    do {                                        \
        if (val) {                              \
            printf(__VA_ARGS__);             \
            return ret;                         \
        }                                       \
    } while(0);

#define CHECK_GOTO(val, label)                  \
    do {                                        \
        if (val) {                              \
            goto label;                         \
        }                                       \
    } while(0);

#define CHECK_GOTO_WITH_LOG(val, label, ...)    \
    do {                                        \
        if (val) {                              \
            printf(__VA_ARGS__);             \
            goto label;                         \
        }                                       \
    } while(0);

ot_s32 sample_cve_init();
ot_void sample_cve_deinit();

#endif
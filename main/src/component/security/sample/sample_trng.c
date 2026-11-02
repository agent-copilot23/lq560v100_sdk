/*
  Copyright (c), 2021-2023, Oritek Tech. Co., Ltd.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "sample_utils.h"
#include "ot_omi_cipher.h"
#include "ot_eis_errno.h"
#include "sample_func.h"

#define GENERATE_TIMES  10

ot_s32 sample_trng(ot_void)
{
    sample_log("************ test trng ************\n");
    ot_s32 ret;
    ot_s32 index;
    ot_u32 random_number = 0;

    /* 1. cipher init */
    for (index = 0; index < GENERATE_TIMES; index++) {
        ret = ot_omi_cipher_trng_get_random(&random_number);
        if (ret != OT_SUCCESS) {
            sample_err("ot_omi_cipher_trng_get_random failed!\n");
            goto __EXIT__;
        }
    }
    sample_log("************ test trng success ************\n");
__EXIT__:
    return ret;
}


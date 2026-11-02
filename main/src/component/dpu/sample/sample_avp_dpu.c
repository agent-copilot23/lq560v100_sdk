/*
  Copyright (c), 2001-2024, Oritek Tech. Co., Ltd.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <pthread.h>

#include <unistd.h>
#include <signal.h>
#include "securec.h"
#include "sample_avp_dpu_proc.h"
#include "sample_common_avp_dpu.h"

#define SAMPLE_AVP_DPU_ARG_MAX_NUM 2

static char **g_ch_cmd_argv = OT_NULL;
/* function : to process abnormal case */

static ot_void sample_avp_dpu_handle_sig(ot_s32 signo)
{
    if (signo == SIGINT || signo == SIGTERM) {
        switch (*g_ch_cmd_argv[1]) {
            case '0': {
                sample_avp_dpu_vi_vproc_rect_match_handle_sig();
                break;
                }
            case '1': {
                sample_avp_dpu_file_rect_match_handle_sig();
                break;
                }
            default:
                break;
        }
    }
}

/* function : show usage */
static ot_void sample_avp_dpu_usage(const char *prg_name)
{
    printf("Usage : %s <index> \n", prg_name);
    printf("index:\n");
    printf("\t 0) VI->VPROC->RECT->MATCH.\n");
    printf("\t 1) FILE->RECT->MATCH.\n");
}

static ot_s32 sample_avp_dpu_case(ot_char idx)
{
    ot_s32 ret;
    switch (idx) {
        case '0': {
            ret = sample_avp_dpu_vi_vproc_rect_match();
            sample_avp_dpu_check_exps_return(1, ret, "Not Support yet!\n");
            break;
            }
        case '1': {
            ret = sample_avp_dpu_file_rect_match();
            break;
            }
        default: {
            ret = OT_FAILURE;
            break;
            }
    }
    return ret;
}
/* function : dpu sample */

int main(int argc, char *argv[])
{
    ot_s32 ret;
    ot_s32 idx_len;
    struct sigaction sa;

    if (argc != SAMPLE_AVP_DPU_ARG_MAX_NUM) {
        sample_avp_dpu_usage(argv[0]);
        return OT_FAILURE;
    }
    if (!strncmp(argv[1], "-h", SAMPLE_AVP_DPU_ARG_MAX_NUM)) {
        sample_avp_dpu_usage(argv[0]);
        return OT_SUCCESS;
    }
    g_ch_cmd_argv = argv;

    (ot_void)memset_s(&sa, sizeof(struct sigaction), 0, sizeof(struct sigaction));
    sa.sa_handler = sample_avp_dpu_handle_sig;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    idx_len = (ot_s32)strlen(argv[1]);
    if (idx_len != 1) {
        sample_avp_dpu_usage(argv[0]);
        return OT_FAILURE;
    }

    ret = sample_avp_dpu_case(*argv[1]);
    if (ret != OT_SUCCESS) {
        sample_avp_dpu_usage(argv[0]);
    }
    return 0;
}
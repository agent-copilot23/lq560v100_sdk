#include "ot_avp_npu_rts.h"
#include "file_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef __GNUC__
#include <unistd.h>
#endif
#ifndef SIMULATOR
#include "ot_smr.h"
#ifndef OT_AVP_NPU_V200
#include "ot_eis_media_sys.h"
#endif
#endif

extern ot_s32 sample_npu_async_inference_both_single_graph(int argc, char** argv);
extern ot_s32 sample_npu_async_inference_mix_single_and_multi_graph(int argc, char** argv);


typedef ot_s32 (*sample_process_func)(int argc, char** argv);

typedef struct sample_execute_t{
    ot_s32 index;
    sample_process_func func;
} sample_execute_t;

/* function : show usage */
static ot_void sample_npu_usage(const ot_s8 *name)
{
    printf("  Usage : %s <index> \n", name);
    printf("  index :\n");
    printf("          1) async inference. [single_npu_graph_model + single_npu_graph_model]\n");
    printf("          2) async inference. [single_graph_model + multi_graph_model ]\n");
}

static ot_s32 sample_npu_run_case(int argc, char** argv)
{
    ot_s32 index = atoi(argv[1]);
    static sample_execute_t s_execute[] = {
        {0, NULL},
        {1, sample_npu_async_inference_both_single_graph},
        {2, sample_npu_async_inference_mix_single_and_multi_graph},

    };

    if (index > (sizeof(s_execute) / sizeof(sample_execute_t))) {
        printf("index[%d] is invalid", index);
        return -1;
    }

    return s_execute[index].func(argc, argv);
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        sample_npu_usage(argv[0]);
        return -1;
    }

#ifdef OT_AVP_NPU_V200
    ot_smr_attr smr_attr = {0};
    const char *region_name = (const char *)"anony";

    strcpy((char *)smr_attr.regions[0].region_name, region_name);
    smr_attr.regions[0].addr = (ot_phys_addr)0x80000000;
    smr_attr.regions[0].len = 0x30000000;  // 256M * 3 = 768M
    smr_attr.allocator = OT_SMR_ALLOCATOR_BBAA;
    smr_attr.num = 1;

    if (ot_smr_config(&smr_attr) != 0) {
        printf("SMR already config!\n");
    }
#else
    ot_eis_media_init();
#endif
    ot_smr_init();

    int ret = sample_npu_run_case(argc, argv);

    ot_smr_deinit();

#ifndef OT_AVP_NPU_V200
    ot_eis_media_deinit();
#endif
    if (ret != 0) {
        sample_npu_usage(argv[0]);
        return -1;
    }
    return 0;
}

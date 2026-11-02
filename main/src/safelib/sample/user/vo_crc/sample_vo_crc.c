#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <stdint.h>
#include <time.h>

#include "ot_semu_rt.h"
#include "ot_eis_type.h"
#include "ot_eis_errno.h"
#include "crc32.h"
#include "ot_eis_vo_surface.h"
#include "semu_log.h"
#include "ot_smr.h"

#define sample_pause()                                                            \
    do {                                                                          \
        semu_info("---------------press enter key to exit!---------------\n");    \
        getchar();                                                                \
    } while (0)

static ot_s32 calculate_frame_crc(ot_eis_handle handle, ot_u32 *crc_value)
{
    ot_s32 result = OT_FAILURE;
    ot_void *vitual = OT_NULL;
    ot_eis_video_frame frame = { 0 };
    clock_t start, end; /* 记录crc函数计算时长 */
    result = ot_eis_vo_surface_get_frame(handle, &frame);
    if (result != OT_SUCCESS) {
        semu_error("get surface frame failed by handle[%lld]", (ot_u64)handle);
        return result;
    }
    result = ot_smr_mmap(frame.buff.phys_addr[0], frame.buff.stride[0] * frame.attr.height, OT_FALSE, &vitual);
    if (result != OT_SUCCESS) {
        semu_error("mmap frame failed");
        goto release_frame;
    }
    /* 仅需计算YUV frame中的Y */
    start = clock();
    *crc_value =  crc32((ot_u8 *)vitual, frame.buff.stride[0] * frame.attr.height);
    end = clock();
    semu_info("crc calculate time %ld us\n", ((ot_ulong) (end - start)));

    result = ot_smr_munmap(vitual, frame.buff.stride[0] * frame.attr.height);
release_frame:
    result = ot_eis_vo_surface_release_frame(handle, &frame);

    return result;
}

/* 现方案：使用crc库*/
int main() {
    ot_s32 result = OT_FAILURE;
    ot_u32 layer_id = 0;
    ot_u32 intervel = 2 * 1000 * 1000; /* 检测时长 us， 后续通过接口传递*/
    ot_eis_handle handle = (ot_eis_handle)((uintptr_t)layer_id) /* 暂时假设surface0，后续通过接口传递handle */;
    ot_u32 crc_pre, crc_next;
    semu_info("sample vo crc beging\n");
    result = ot_smr_init();
    if (result != OT_SUCCESS) {
        semu_error("init smr failed, please check\n");
        return result;
    }

    result = ot_eis_vo_init();
    if (result != OT_SUCCESS) {
        semu_error("init vo failed, please check\n");
        return result;
    }

    result = calculate_frame_crc(handle, &crc_pre);
    if (result != OT_SUCCESS) {
        semu_error("calculate pre frame crc failed, please check\n");
        goto deinit_smr;
    }

    usleep(intervel);

    result = calculate_frame_crc(handle, &crc_next);
    if (result != OT_SUCCESS) {
        semu_error("calculate next frame crc failed, please check\n");
        goto deinit_smr;
    }

    if (crc_pre == crc_next) {
        semu_info("frame same error\n");
    }
    semu_info("frame pre_crc[%u], next_crc[%u]\n", crc_pre, crc_next);
    sample_pause();

deinit_smr:
    ot_eis_vo_deinit();
    ot_smr_deinit();

    return result;
}
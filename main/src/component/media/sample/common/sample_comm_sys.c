/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#include "sample_comm.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <signal.h>
#include "ot_eis_video.h"
#include "ot_media_pipe.h"
#include "ot_vrb.h"
#include "ot_buffer_pool.h"
#include "ot_smr.h"

#define OT_ALIGN_UP(x, a)           ((((x) + ((a) - 1)) / (a)) * (a))
#define OT_ALIGN_DOWN(x, a)         (((x) / (a)) * (a))
#define OT_DIV_UP(x, a)             (((x) + ((a) - 1)) / (a))
#define OT_DEFAULT_ALIGN                   32
#define OT_MAX_ALIGN                       1024

#define SAMPLE_VRB_CNT (1)

#define SAMPLE_DEFAULT_OVERLAP (270)

#define SAMPLE_MAXI_NUM_LIMIT (30000)

#define sample_mem_check_open_return() \
    do { \
        if (g_sample_mem_dev <= 0) { \
            g_sample_mem_dev = open("/dev/mem", O_RDWR | O_SYNC); \
            if (g_sample_mem_dev < 0) { \
                perror("open dev/mem error"); \
                return OT_NULL; \
            } \
        } \
    } while (0)

static ot_s32 g_sample_mem_dev = -1;

/* The order of g_sample_pic_size's element must be consistent with the enum value defined in "ot_pic_size". */
static ot_eis_img_size g_sample_pic_size[PIC_BUTT] = {
    { 352,  288  },  /* PIC_CIF */
    { 640,  360  },  /* PIC_360P */
    { 720,  576  },  /* PIC_D1_PAL */
    { 720,  480  },  /* PIC_D1_NTSC */
    { 960,  576  },  /* PIC_960H */
    { 1280, 720  },  /* PIC_720P */
    { 1920, 1080 },  /* PIC_1080P */
    { 720,  480  },  /* PIC_480P */
    { 720,  576  },  /* PIC_576P */
    { 800,  600  },  /* PIC_800X600 */
    { 1024, 768  },  /* PIC_1024X768 */
    { 1280, 960  },  /* PIC_1280X960 */
    { 1280, 1024 },  /* PIC_1280X1024 */
    { 1366, 768  },  /* PIC_1366X768 */
    { 1440, 900  },  /* PIC_1440X900 */
    { 1280, 800  },  /* PIC_1280X800 */
    { 1600, 1200 },  /* PIC_1600X1200 */
    { 1680, 1050 },  /* PIC_1680X1050 */
    { 1920, 1200 },  /* PIC_1920X1200 */
    { 640,  480  },  /* PIC_640X480 */
    { 1920, 2160 },  /* PIC_1920X2160 */
    { 2560, 1440 },  /* PIC_2560X1440 */
    { 2560, 1600 },  /* PIC_2560X1600 */
    { 2592, 1520 },  /* PIC_2592X1520 */
    { 2688, 1520 },  /* PIC_2688X1520 */
    { 2592, 1944 },  /* PIC_2592X1944 */
    { 3840, 2160 },  /* PIC_3840X2160 */
    { 4096, 2160 },  /* PIC_4096X2160 */
    { 3000, 3000 },  /* PIC_3000X3000 */
    { 4000, 3000 },  /* PIC_4000X3000 */
    { 6080, 2800 },  /* PIC_6080X2800 */
    { 7680, 4320 },  /* PIC_7680X4320 */
    { 3840, 8640 }   /* PIC_3840X8640 */
};

ot_void sample_common_get_pic_buf_cfg(const ot_eis_img_attr *img_attr, ot_eis_buf_size_calc_cfg *calc_cfg)
{
    ot_eis_media_get_img_buf_size(img_attr, calc_cfg);
}

ot_s32 sample_common_get_buffer_pool_cfg(const ot_eis_img_attr *img_attr, ot_u32 *blk_size)
{
    ot_eis_buf_size_calc_cfg calc_cfg;

    if(img_attr == OT_NULL) {
        return -1;
    }

    sample_common_get_pic_buf_cfg(img_attr, &calc_cfg);

    *blk_size = calc_cfg.blk_size;
    return 0;
}

ot_s32 sample_common_get_vrb_cfg(ot_u32 pipe_num, ot_u32 width, ot_u32 height, ot_video_buffer_attr *buffer_attr)
{
    ot_eis_buf_size_calc_cfg calc_cfg;
    ot_eis_img_attr img_attr;

    if(buffer_attr == OT_NULL) {
        return -1;
    }

    buffer_attr->cnt = 2;

    img_attr.width         = width;
    img_attr.height        = height;
    img_attr.bit_depth     = OT_EIS_PIXEL_BIT_DEPTH_8;
    img_attr.pixel_fmt  = OT_EIS_IMAGE_FORMAT_BAYER_16BPP;
    img_attr.compress_mode = OT_EIS_IMAGE_COMPRESS_MODE_NONE;
    img_attr.layout  = OT_EIS_IMAGE_LAYOUT_LINEAR;
    sample_common_get_pic_buf_cfg(&img_attr, &calc_cfg);
    buffer_attr->buf_blks[0].size = calc_cfg.blk_size;
    buffer_attr->buf_blks[0].cnt = pipe_num * SAMPLE_VRB_CNT;
    strcpy(buffer_attr->buf_blks[0].region_name, "anony");

    img_attr.pixel_fmt  = OT_EIS_IMAGE_FORMAT_YVU_420_SEMIPLANAR;
    img_attr.compress_mode = OT_EIS_IMAGE_COMPRESS_MODE_NONE;
    img_attr.layout  = OT_EIS_IMAGE_LAYOUT_LINEAR;
    sample_common_get_pic_buf_cfg(&img_attr, &calc_cfg);
    buffer_attr->buf_blks[1].size = calc_cfg.blk_size;
    buffer_attr->buf_blks[1].cnt = pipe_num * SAMPLE_VRB_CNT;
    strcpy(buffer_attr->buf_blks[1].region_name, "anony");

    img_attr.compress_mode = OT_EIS_IMAGE_COMPRESS_MODE_NONE;
    img_attr.layout  = OT_EIS_IMAGE_LAYOUT_LINEAR;
    sample_common_get_pic_buf_cfg(&img_attr, &calc_cfg);
    buffer_attr->buf_blks[2].size = calc_cfg.blk_size;
    buffer_attr->buf_blks[2].cnt = pipe_num * SAMPLE_VRB_CNT;
    strcpy(buffer_attr->buf_blks[2].region_name, "anony");

    return 0;
}

ot_void sample_sys_signal(void (*func)(int))
{
    struct sigaction sa = { 0 };

    sa.sa_handler = func;
    sa.sa_flags = 0;
    sigaction(SIGINT, &sa, OT_NULL);
    sigaction(SIGTERM, &sa, OT_NULL);
}

ot_void *sample_sys_io_mmap(ot_u64 phy_addr, ot_u32 size)
{
    ot_u32 diff;
    ot_u64 page_phy;
    ot_u8 *page_addr = OT_NULL;
    ot_ulong page_size;

    sample_mem_check_open_return();

    /* page_size will be 0 when size is 0 and diff is 0, and then mmap will be error(invalid argument) */
    if (!size) {
        printf("func: %s size can't be 0.\n", __FUNCTION__);
        return OT_NULL;
    }

    /* the mmap address should align with page */
    page_phy = phy_addr & 0xfffffffffffff000ULL;
    diff = phy_addr - page_phy;

    /* the mmap size should be multiples of 1024 */
    page_size = ((size + diff - 1) & 0xfffff000UL) + 0x1000;

    page_addr = mmap((void *)0, page_size, PROT_READ | PROT_WRITE, MAP_SHARED, g_sample_mem_dev, page_phy);
    if (page_addr == MAP_FAILED) {
        perror("mmap error");
        return OT_NULL;
    }
    return (ot_void *)(page_addr + diff);
}

ot_s32 sample_sys_munmap(ot_void *vir_addr, ot_u32 size)
{
    ot_u64 page_addr;
    ot_u32 page_size;
    ot_u32 diff;

    page_addr = (((uintptr_t)vir_addr) & 0xfffffffffffff000ULL);
    diff = (uintptr_t)vir_addr - page_addr;
    page_size = ((size + diff - 1) & 0xfffff000UL) + 0x1000;

    return munmap((ot_void *)(uintptr_t)page_addr, page_size);
}

ot_s32 sample_sys_set_reg(ot_u64 addr, ot_u32 value)
{
    ot_u32 *reg_addr = OT_NULL;
    ot_u32 map_len = sizeof(value);

    reg_addr = (ot_u32 *)sample_sys_io_mmap(addr, map_len);
    if (reg_addr == OT_NULL) {
        return OT_FAILURE;
    }

    *reg_addr = value;

    return sample_sys_munmap(reg_addr, map_len);
}

ot_s32 sample_sys_get_reg(ot_u64 addr, ot_u32 *value)
{
    ot_u32 *reg_addr = OT_NULL;
    ot_u32 map_len;

    if (value == OT_NULL) {
        return OT_EIS_ERRNO_NULL_PTR;
    }

    map_len = sizeof(*value);
    reg_addr = (ot_u32 *)sample_sys_io_mmap(addr, map_len);
    if (reg_addr == OT_NULL) {
        return OT_FAILURE;
    }

    *value = *reg_addr;

    return sample_sys_munmap(reg_addr, map_len);
}

/* get picture size(w*h), according pic_size */
ot_s32 sample_comm_sys_get_pic_size(ot_pic_size pic_size, ot_eis_img_size *size)
{
    if (size == OT_NULL) {
        sample_print("null ptr arg!\n");
        return OT_FAILURE;
    }

    if (pic_size >= PIC_BUTT) {
        sample_print("illegal pic_size!\n");
        return OT_FAILURE;
    }

    size->width = g_sample_pic_size[pic_size].width;
    size->height = g_sample_pic_size[pic_size].height;

    return OT_SUCCESS;
}

ot_pic_size sample_comm_sys_get_pic_enum(const ot_eis_img_size *size)
{
    ot_pic_size i;

    for (i = PIC_CIF; i < PIC_BUTT; i++) {
        if ((g_sample_pic_size[i].width == size->width) &&
            (g_sample_pic_size[i].height == size->height)) {
            return i;
        }
    }

    return PIC_1080P;
}

/* smr vrb init & media init */
ot_s32 sample_comm_sys_init(const ot_video_buffer_attr *vrb_cfg)
{
    ot_s32 ret;

    ot_eis_media_deinit();
    ot_vrb_deinit();
    ot_smr_deinit();

    if (vrb_cfg == OT_NULL) {
        sample_print("input parameter is null, it is invalid!\n");
        return OT_FAILURE;
    }

    ret = ot_vrb_config(vrb_cfg);
    if (ret != OT_SUCCESS) {
        sample_print("ot_vrb_config failed!\n");
        return OT_FAILURE;
    }

    ret = ot_vrb_init();
    if (ret != OT_SUCCESS) {
        sample_print("ot_vrb_init failed!\n");
        return OT_FAILURE;
    }

    ret = ot_eis_media_init();
    if (ret != OT_SUCCESS) {
        sample_print("ot_eis_media_init failed!\n");
        ot_vrb_deinit();
        return OT_FAILURE;
    }

    ret = ot_smr_init();
    if (ret != OT_SUCCESS) {
        sample_print("ot_smr_init failed!\n");
        return OT_FAILURE;
    }

    return OT_SUCCESS;
}

/* smr vrb exit & media exit */
ot_void sample_comm_sys_exit(ot_void)
{
    ot_eis_media_deinit();
    ot_vrb_deinit();
    ot_smr_deinit();
    return;
}

ot_s32 sample_comm_sys_set_vi_vproc_mode(ot_eis_vi_vproc_mode_type mode_type)
{
    ot_u32 i;
    ot_s32 ret;
    ot_eis_vi_vproc_mode_type other_pipe_mode_type;
    ot_eis_vi_vproc_mode vi_vproc_mode;

    if (mode_type == OT_EIS_VI_OFFLINE_VPROC_ONLINE) {
        other_pipe_mode_type = OT_EIS_VI_OFFLINE_VPROC_ONLINE;
    } else {
        other_pipe_mode_type = OT_EIS_VI_OFFLINE_VPROC_OFFLINE;
    }

    vi_vproc_mode.mode[0] = mode_type;
    for (i = 1; i < OT_COMM_EIS_VI_MAX_PIPE_NUM; i++) {
        vi_vproc_mode.mode[i] = other_pipe_mode_type;
    }

    ret = ot_eis_media_set_vi_vproc_mode(&vi_vproc_mode);
    if (ret != OT_SUCCESS) {
        sample_print("set vi vproc mode failed!\n");
        return OT_FAILURE;
    }

    return OT_SUCCESS;
}

ot_s32 sample_comm_sys_set_vi_vproc_init_cfg(ot_eis_vi_vproc_mode_type mode_type)
{
    ot_s32 ret;
    ret = sample_comm_sys_set_vi_vproc_mode(mode_type);
    if (ret != OT_SUCCESS) {
        sample_print("set_vi_vproc_mode failed with %#x!\n", ret);
        return OT_FAILURE;
    }

    ret = ot_eis_media_set_3dnr_pos(OT_EIS_3DNR_POS_VPROC);
    if (ret != OT_SUCCESS) {
        sample_print("set_3dnr_pos failed with %#x!\n", ret);
        return OT_FAILURE;
    }

    return OT_SUCCESS;
}

ot_s32 sample_comm_vi_bind_vo(ot_eis_handle vi_chn_hdl, ot_eis_handle vo_inport_hdl, ot_eis_handle pip_hdl)
{
    ot_eis_media_node_port src_port;
    ot_eis_media_node_port sink_port;

    src_port.node_type = OT_EIS_MODULE_VI;
    src_port.handle = vi_chn_hdl;

    sink_port.node_type = OT_EIS_MODULE_VO;
    sink_port.handle = vo_inport_hdl;

    check_return(ot_media_pipe_attach(pip_hdl, &src_port, &sink_port), "ot_media_pipe_attach(VI-VO)");

    return OT_SUCCESS;
}

ot_s32 sample_comm_vi_un_bind_vo(ot_eis_handle vi_chn_hdl, ot_eis_handle vo_inport_hdl, ot_eis_handle pip_hdl)
{
    ot_eis_media_node_port src_port;
    ot_eis_media_node_port sink_port;

    src_port.node_type = OT_EIS_MODULE_VI;
    src_port.handle = vi_chn_hdl;

    sink_port.node_type = OT_EIS_MODULE_VO;
    sink_port.handle = vo_inport_hdl;

    check_return(ot_media_pipe_detach(pip_hdl, &src_port, &sink_port), "ot_media_pipe_detach(VI-VO)");

    return OT_SUCCESS;
}


ot_s32 sample_comm_vi_bind_venc(ot_eis_handle vi_chn_hdl, ot_eis_handle venc_chn_hdl, ot_eis_handle pip_hdl)
{
    ot_eis_media_node_port src_port;
    ot_eis_media_node_port sink_port;

    src_port.node_type = OT_EIS_MODULE_VI;
    src_port.handle = vi_chn_hdl;

    sink_port.node_type = OT_EIS_MODULE_VENC;
    sink_port.handle = venc_chn_hdl;

    check_return(ot_media_pipe_attach(pip_hdl, &src_port, &sink_port), "ot_media_pipe_attach(VI-VENC)");

    return OT_SUCCESS;
}

ot_s32 sample_comm_vi_un_bind_venc(ot_eis_handle vi_chn_hdl, ot_eis_handle venc_chn_hdl, ot_eis_handle pip_hdl)
{
    ot_eis_media_node_port src_port;
    ot_eis_media_node_port sink_port;

    src_port.node_type = OT_EIS_MODULE_VI;
    src_port.handle = vi_chn_hdl;

    sink_port.node_type = OT_EIS_MODULE_VENC;
    sink_port.handle = venc_chn_hdl;

    check_return(ot_media_pipe_detach(pip_hdl, &src_port, &sink_port), "ot_media_pipe_detach(VI-VENC)");

    return OT_SUCCESS;
}

ot_s32 sample_comm_vi_bind_vproc(ot_eis_handle vi_chn_hdl, ot_eis_handle vproc_pipe_hdl, ot_eis_handle pip_hdl)
{
    ot_eis_media_node_port src_port;
    ot_eis_media_node_port sink_port;

    src_port.node_type = OT_EIS_MODULE_VI;
    src_port.handle = vi_chn_hdl;

    sink_port.node_type = OT_EIS_MODULE_VPROC;
    sink_port.handle = vproc_pipe_hdl;

    check_return(ot_media_pipe_attach(pip_hdl, &src_port, &sink_port), "ot_media_pipe_attach(VI-VPROC)");

    return OT_SUCCESS;
}

ot_s32 sample_comm_vi_un_bind_vproc(ot_eis_handle vi_chn_hdl, ot_eis_handle vproc_pipe_hdl, ot_eis_handle pip_hdl)
{
    ot_eis_media_node_port src_port;
    ot_eis_media_node_port sink_port;

    src_port.node_type = OT_EIS_MODULE_VI;
    src_port.handle = vi_chn_hdl;

    sink_port.node_type = OT_EIS_MODULE_VPROC;
    sink_port.handle = vproc_pipe_hdl;

    check_return(ot_media_pipe_detach(pip_hdl, &src_port, &sink_port), "ot_media_pipe_detach(VI-VPROC)");

    return OT_SUCCESS;
}

ot_s32 sample_comm_vproc_bind_vo(ot_eis_handle vproc_chn_hdl, ot_eis_handle vo_in_port_hdl, ot_eis_handle pip_hdl)
{
    ot_eis_media_node_port src_port;
    ot_eis_media_node_port sink_port;

    src_port.node_type = OT_EIS_MODULE_VPROC;
    src_port.handle = vproc_chn_hdl;

    sink_port.node_type = OT_EIS_MODULE_VO;
    sink_port.handle = vo_in_port_hdl;

    check_return(ot_media_pipe_attach(pip_hdl, &src_port, &sink_port), "ot_media_pipe_attach(VPROC-VO)");

    return OT_SUCCESS;
}

ot_s32 sample_comm_vproc_un_bind_vo(ot_eis_handle vproc_chn_hdl, ot_eis_handle vo_in_port_hdl, ot_eis_handle pip_hdl)
{
    ot_eis_media_node_port src_port;
    ot_eis_media_node_port sink_port;

    src_port.node_type = OT_EIS_MODULE_VPROC;
    src_port.handle = vproc_chn_hdl;

    sink_port.node_type = OT_EIS_MODULE_VO;
    sink_port.handle = vo_in_port_hdl;

    check_return(ot_media_pipe_detach(pip_hdl, &src_port, &sink_port), "ot_media_pipe_detach(VPROC-VO)");

    return OT_SUCCESS;
}


ot_s32 sample_comm_vproc_bind_venc(ot_eis_handle vproc_chn_hdl, ot_eis_handle venc_chn_hdl, ot_eis_handle pip_hdl)
{
    ot_eis_media_node_port src_port;
    ot_eis_media_node_port sink_port;

    src_port.node_type = OT_EIS_MODULE_VPROC;
    src_port.handle = vproc_chn_hdl;

    sink_port.node_type = OT_EIS_MODULE_VENC;
    sink_port.handle = venc_chn_hdl;

    check_return(ot_media_pipe_attach(pip_hdl, &src_port, &sink_port), "ot_media_pipe_attach(VPROC-VENC)");

    return OT_SUCCESS;
}

ot_s32 sample_comm_vproc_un_bind_venc(ot_eis_handle vproc_chn_hdl, ot_eis_handle venc_chn_hdl, ot_eis_handle pip_hdl)
{
    ot_eis_media_node_port src_port;
    ot_eis_media_node_port sink_port;

    src_port.node_type = OT_EIS_MODULE_VPROC;
    src_port.handle = vproc_chn_hdl;

    sink_port.node_type = OT_EIS_MODULE_VENC;
    sink_port.handle = venc_chn_hdl;

    check_return(ot_media_pipe_detach(pip_hdl, &src_port, &sink_port), "ot_media_pipe_detach(VPROC-VENC)");

    return OT_SUCCESS;
}


ot_s32 sample_comm_vproc_bind_vproc(ot_eis_handle vproc_chn_hdl, ot_eis_handle vproc_pipe_hdl, ot_eis_handle pip_hdl)
{
    ot_eis_media_node_port src_port;
    ot_eis_media_node_port sink_port;

    src_port.node_type = OT_EIS_MODULE_VPROC;
    src_port.handle = vproc_chn_hdl;

    sink_port.node_type = OT_EIS_MODULE_VPROC;
    sink_port.handle = vproc_pipe_hdl;
    check_return(ot_media_pipe_attach(pip_hdl, &src_port, &sink_port), "ot_media_pipe_attach(VPROC-VPROC)");

    return OT_SUCCESS;
}

ot_s32 sample_comm_vproc_un_bind_vproc(ot_eis_handle vproc_chn_hdl, ot_eis_handle vproc_pipe_hdl, ot_eis_handle pip_hdl)
{
    ot_eis_media_node_port src_port;
    ot_eis_media_node_port sink_port;

    src_port.node_type = OT_EIS_MODULE_VPROC;
    src_port.handle = vproc_chn_hdl;

    sink_port.node_type = OT_EIS_MODULE_VPROC;
    sink_port.handle = vproc_pipe_hdl;

    check_return(ot_media_pipe_detach(pip_hdl, &src_port, &sink_port), "ot_media_pipe_detach(VPROC-VPROC)");

    return OT_SUCCESS;
}


ot_s32 sample_comm_media_pipe_init(ot_eis_handle *pip_hdl)
{
    ot_s32 ret;

    ret = ot_media_pipe_init();
   if( ret != 0){
       printf("pipe init err! ret[%d]\n", ret);
       return OT_FAILURE;
   }

   ret = ot_media_pipe_create(pip_hdl);
    if( ret != 0){
        printf("pipe create err! ret[%d]\n", ret);
        goto pipe_create_failed;
    }

    return OT_SUCCESS;

pipe_create_failed:
    ot_media_pipe_deinit();
    return ret;
}

ot_void sample_comm_media_pipe_stop(ot_eis_handle pip_hdl)
{
    ot_media_pipe_destroy(pip_hdl);
    ot_media_pipe_deinit();
}

ot_s32 sample_comm_media_create_buf_pool(ot_eis_handle* pool_hdl, ot_eis_img_attr* img_attr, ot_u32 cnt)
{
    ot_eis_buf_size_calc_cfg calc_cfg;
    sample_common_get_pic_buf_cfg(img_attr, &calc_cfg);

    ot_video_buffer_attr attr;
    attr.buf_blks[0].cnt = cnt;
    attr.buf_blks[0].region_name[0] = '\0';
    attr.buf_blks[0].size = calc_cfg.blk_size;
    attr.cnt = 1;

    return ot_buffer_pool_create(pool_hdl, &attr);
}

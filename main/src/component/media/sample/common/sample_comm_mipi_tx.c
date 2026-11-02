/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include "sample_comm.h"
#include "ot_eis_mipi_tx.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif /* end of #ifdef __cplusplus */

// only select one type
// #define MIPI_TX_SHORT_CMD
// #define MIPI_TX_MIX_CMD

#if defined(MIPI_TX_SHORT_CMD)
static ot_eis_mipitx_cmd_info_attr g_set_cmd_info_800X1280[] = {
    /* {devno work_mode lp_clk_en data_type cmd_size cmd} */
    {0, 0, 0, 0x15, 0x50ee, NULL},
    {0, 0, 0, 0x15, 0x85ea, NULL},
    {0, 0, 0, 0x15, 0x55eb, NULL},
    {0, 0, 0, 0x15, 0xa024, NULL},
    {0, 0, 0, 0x15, 0x0030, NULL},
    {0, 0, 0, 0x15, 0x0239, NULL},
    {0, 0, 0, 0x15, 0x073a, NULL},
    {0, 0, 0, 0x15, 0x103b, NULL},
    {0, 0, 0, 0x15, 0x207a, NULL},
    {0, 0, 0, 0x15, 0x007d, NULL},
    {0, 0, 0, 0x15, 0x2090, NULL},
    {0, 0, 0, 0x15, 0x7091, NULL},
    {0, 0, 0, 0x15, 0xF893, NULL},
    {0, 0, 0, 0x15, 0x7495, NULL},
    {0, 0, 0, 0x15, 0x0a97, NULL},
    {0, 0, 0, 0x15, 0x1099, NULL},
    {0, 0, 0, 0x15, 0x60ee, NULL},
    {0, 0, 0, 0x15, 0x0121, NULL},
    {0, 0, 0, 0x15, 0x6227, NULL},
    {0, 0, 0, 0x15, 0x0130, NULL},
    {0, 0, 0, 0x15, 0xF333, NULL},
    {0, 0, 0, 0x15, 0x3f34, NULL},
    {0, 0, 0, 0x15, 0x243a, NULL},
    {0, 0, 0, 0x15, 0x003b, NULL},
    {0, 0, 0, 0x15, 0x203c, NULL},
    {0, 0, 0, 0x15, 0x113d, NULL},
    {0, 0, 0, 0x15, 0x933e, NULL},
    {0, 0, 0, 0x15, 0x5542, NULL},
    {0, 0, 0, 0x15, 0x5543, NULL},
    {0, 0, 0, 0x15, 0x247f, NULL},
    {0, 0, 0, 0x15, 0x2480, NULL},
    {0, 0, 0, 0x15, 0x2086, NULL},
    {0, 0, 0, 0x15, 0x0089, NULL},
    {0, 0, 0, 0x15, 0xaa8a, NULL},
    {0, 0, 0, 0x15, 0x4491, NULL},
    {0, 0, 0, 0x15, 0x3392, NULL},
    {0, 0, 0, 0x15, 0x9b93, NULL},
    {0, 0, 0, 0x15, 0x009a, NULL},
    {0, 0, 0, 0x15, 0x809c, NULL},
    {0, 0, 0, 0x15, 0x155a, NULL},
    {0, 0, 0, 0x15, 0x2f5b, NULL},
    {0, 0, 0, 0x15, 0x385c, NULL},
    {0, 0, 0, 0x15, 0x425d, NULL},
    {0, 0, 0, 0x15, 0x475e, NULL},
    {0, 0, 0, 0x15, 0x1547, NULL},
    {0, 0, 0, 0x15, 0x2f48, NULL},
    {0, 0, 0, 0x15, 0x3849, NULL},
    {0, 0, 0, 0x15, 0x424a, NULL},
    {0, 0, 0, 0x15, 0x474b, NULL},
    {0, 0, 0, 0x15, 0x564c, NULL},
    {0, 0, 0, 0x15, 0x504d, NULL},
    {0, 0, 0, 0x15, 0x644e, NULL},
    {0, 0, 0, 0x15, 0x494f, NULL},
    {0, 0, 0, 0x15, 0x4a50, NULL},
    {0, 0, 0, 0x15, 0x565f, NULL},
    {0, 0, 0, 0x15, 0x5060, NULL},
    {0, 0, 0, 0x15, 0x6461, NULL},
    {0, 0, 0, 0x15, 0x4962, NULL},
    {0, 0, 0, 0x15, 0x4a63, NULL},
    {0, 0, 0, 0x15, 0x4e64, NULL},
    {0, 0, 0, 0x15, 0x3665, NULL},
    {0, 0, 0, 0x15, 0x4c66, NULL},
    {0, 0, 0, 0x15, 0x4767, NULL},
    {0, 0, 0, 0x15, 0x5568, NULL},
    {0, 0, 0, 0x15, 0x4e51, NULL},
    {0, 0, 0, 0x15, 0x3652, NULL},
    {0, 0, 0, 0x15, 0x4c53, NULL},
    {0, 0, 0, 0x15, 0x4754, NULL},
    {0, 0, 0, 0x15, 0x5555, NULL},
    {0, 0, 0, 0x15, 0x5669, NULL},
    {0, 0, 0, 0x15, 0x616a, NULL},
    {0, 0, 0, 0x15, 0x6d6b, NULL},
    {0, 0, 0, 0x15, 0x7f6c, NULL},
    {0, 0, 0, 0x15, 0x5656, NULL},
    {0, 0, 0, 0x15, 0x6157, NULL},
    {0, 0, 0, 0x15, 0x6d58, NULL},
    {0, 0, 0, 0x15, 0x7f59, NULL},
    {0, 0, 0, 0x15, 0x70ee, NULL},
    {0, 0, 0, 0x15, 0x0000, NULL},
    {0, 0, 0, 0x15, 0x0301, NULL},
    {0, 0, 0, 0x15, 0x0002, NULL},
    {0, 0, 0, 0x15, 0x0103, NULL},
    {0, 0, 0, 0x15, 0x1104, NULL},
    {0, 0, 0, 0x15, 0x1305, NULL},
    {0, 0, 0, 0x15, 0x5506, NULL},
    {0, 0, 0, 0x15, 0x0107, NULL},
    {0, 0, 0, 0x15, 0x0008, NULL},
    {0, 0, 0, 0x15, 0x0009, NULL},
    {0, 0, 0, 0x15, 0x000a, NULL},
    {0, 0, 0, 0x15, 0x000b, NULL},
    {0, 0, 0, 0x15, 0x050c, NULL},
    {0, 0, 0, 0x15, 0x050d, NULL},
    {0, 0, 0, 0x15, 0x000e, NULL},
    {0, 0, 0, 0x15, 0x0410, NULL},
    {0, 0, 0, 0x15, 0x0711, NULL},
    {0, 0, 0, 0x15, 0x0012, NULL},
    {0, 0, 0, 0x15, 0x0113, NULL},
    {0, 0, 0, 0x15, 0x0014, NULL},
    {0, 0, 0, 0x15, 0x0015, NULL},
    {0, 0, 0, 0x15, 0x0f16, NULL},
    {0, 0, 0, 0x15, 0x0d17, NULL},
    {0, 0, 0, 0x15, 0x0818, NULL},
    {0, 0, 0, 0x15, 0x0019, NULL},
    {0, 0, 0, 0x15, 0x0529, NULL},
    {0, 0, 0, 0x15, 0x052a, NULL},
    {0, 0, 0, 0x15, 0x1730, NULL},
    {0, 0, 0, 0x15, 0x1731, NULL},
    {0, 0, 0, 0x15, 0x5532, NULL},
    {0, 0, 0, 0x15, 0x0533, NULL},
    {0, 0, 0, 0x15, 0x0534, NULL},
    {0, 0, 0, 0x15, 0x3c35, NULL},
    {0, 0, 0, 0x15, 0x1736, NULL},
    {0, 0, 0, 0x15, 0x1737, NULL},
    {0, 0, 0, 0x15, 0x5538, NULL},
    {0, 0, 0, 0x15, 0x2a39, NULL},
    {0, 0, 0, 0x15, 0x2a3a, NULL},
    {0, 0, 0, 0x15, 0x3c3b, NULL},
    {0, 0, 0, 0x15, 0x3f60, NULL},
    {0, 0, 0, 0x15, 0x3c61, NULL},
    {0, 0, 0, 0x15, 0x2062, NULL},
    {0, 0, 0, 0x15, 0x2163, NULL},
    {0, 0, 0, 0x15, 0x1464, NULL},
    {0, 0, 0, 0x15, 0x1665, NULL},
    {0, 0, 0, 0x15, 0x1066, NULL},
    {0, 0, 0, 0x15, 0x1267, NULL},
    {0, 0, 0, 0x15, 0x0068, NULL},
    {0, 0, 0, 0x15, 0x3c69, NULL},
    {0, 0, 0, 0x15, 0x3c6a, NULL},
    {0, 0, 0, 0x15, 0x3c6b, NULL},
    {0, 0, 0, 0x15, 0x3c6c, NULL},
    {0, 0, 0, 0x15, 0x3c6d, NULL},
    {0, 0, 0, 0x15, 0x3c6e, NULL},
    {0, 0, 0, 0x15, 0x026f, NULL},
    {0, 0, 0, 0x15, 0x3c70, NULL},
    {0, 0, 0, 0x15, 0x3c71, NULL},
    {0, 0, 0, 0x15, 0x3c72, NULL},
    {0, 0, 0, 0x15, 0x3c73, NULL},
    {0, 0, 0, 0x15, 0x3c74, NULL},
    {0, 0, 0, 0x15, 0x3c75, NULL},
    {0, 0, 0, 0x15, 0x3f80, NULL},
    {0, 0, 0, 0x15, 0x3c81, NULL},
    {0, 0, 0, 0x15, 0x2082, NULL},
    {0, 0, 0, 0x15, 0x2183, NULL},
    {0, 0, 0, 0x15, 0x1584, NULL},
    {0, 0, 0, 0x15, 0x1785, NULL},
    {0, 0, 0, 0x15, 0x1186, NULL},
    {0, 0, 0, 0x15, 0x1387, NULL},
    {0, 0, 0, 0x15, 0x0188, NULL},
    {0, 0, 0, 0x15, 0x3c89, NULL},
    {0, 0, 0, 0x15, 0x3c8a, NULL},
    {0, 0, 0, 0x15, 0x3c8b, NULL},
    {0, 0, 0, 0x15, 0x3c8c, NULL},
    {0, 0, 0, 0x15, 0x3c8d, NULL},
    {0, 0, 0, 0x15, 0x3c8e, NULL},
    {0, 0, 0, 0x15, 0x038f, NULL},
    {0, 0, 0, 0x15, 0x3c90, NULL},
    {0, 0, 0, 0x15, 0x3c91, NULL},
    {0, 0, 0, 0x15, 0x3c92, NULL},
    {0, 0, 0, 0x15, 0x3c93, NULL},
    {0, 0, 0, 0x15, 0x3c94, NULL},
    {0, 0, 0, 0x15, 0x3c95, NULL},
    {0, 0, 0, 0x15, 0x00ea, NULL},
    {0, 0, 0, 0x15, 0x00eb, NULL},
    {0, 0, 0, 0x15, 0x00ee, NULL},
};

static ot_s32 mipi_tx_cfg_short_cmd(ot_eis_handle vo_intf_hdl)
{
    ot_s32 i, ret;
    for (i = 0; i < (sizeof(g_set_cmd_info_800X1280) / sizeof(g_set_cmd_info_800X1280[0])); i++) {
        ret = ot_eis_vo_if_set_cmd_info(vo_intf_hdl, &g_set_cmd_info_800X1280[i]);
        if(ret != OT_SUCCESS) {
            printf("ot_eis_vo_if_set_cmd_info ret err: %08x\n", ret);
            return ret;
        }
    }
    return OT_SUCCESS;
}
#endif

#ifdef MIPI_TX_MIX_CMD
static ot_s32 mipi_tx_write_short_cmd(ot_eis_handle vo_intf_hdl, ot_u8 dat0, ot_u8 dat1)
{
    ot_cmd_info_attr attr = {0, 0, 0, 0x15, 0, NULL};
    attr.cmd_size = (dat0 | (dat1 << 8));
    return ot_eis_vo_if_set_cmd_info(vo_intf_hdl, &attr);
}

static ot_s32 mipi_tx_write_long_3cmd(ot_eis_handle vo_intf_hdl, ot_u8 dat0, ot_u8 dat1, ot_u8 dat2)
{
    ot_u8 cmd[3] = {dat0, dat1, dat2};
    ot_cmd_info_attr attr = {0, 0, 0, 0x29, 3, cmd};
    return ot_eis_vo_if_set_cmd_info(vo_intf_hdl, &attr);
}

static ot_s32 mipi_tx_write_long_4cmd(ot_eis_handle vo_intf_hdl, ot_u8 dat0, ot_u8 dat1, ot_u8 dat2, ot_u8 dat3)
{
    ot_u8 cmd[4] = {dat0, dat1, dat2, dat3};
    ot_cmd_info_attr attr = {0, 0, 0, 0x29, 4, cmd};
    return ot_eis_vo_if_set_cmd_info(vo_intf_hdl, &attr);
}

static ot_s32 mipi_tx_write_long_5cmd(ot_eis_handle vo_intf_hdl, ot_u8 dat0, ot_u8 dat1, ot_u8 dat2, ot_u8 dat3, ot_u8 dat4)
{
    ot_u8 cmd[5] = {dat0, dat1, dat2, dat3, dat4};
    ot_cmd_info_attr attr = {0, 0, 0, 0x29, 5, cmd};
    return ot_eis_vo_if_set_cmd_info(vo_intf_hdl, &attr);
}

static ot_s32 mipi_tx_write_long_6cmd(ot_eis_handle vo_intf_hdl, ot_u8 dat0, ot_u8 dat1, ot_u8 dat2, ot_u8 dat3, ot_u8 dat4, ot_u8 dat5)
{
    ot_u8 cmd[6] = {dat0, dat1, dat2, dat3, dat4, dat5};
    ot_cmd_info_attr attr = {0, 0, 0, 0x29, 6, cmd};
    ot_eis_vo_if_set_cmd_info(vo_intf_hdl, &attr);
}

static ot_s32 mipi_tx_cfg_mix_cmd(ot_eis_handle vo_intf_hdl)
{
    ot_s32 ret;
    ret  = mipi_tx_write_short_cmd(vo_intf_hdl, 0xee, 0x50);
    ret |= mipi_tx_write_long_3cmd(vo_intf_hdl, 0xea, 0x85, 0x55);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0x24, 0xa0);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0x30, 0x00);
    ret |= mipi_tx_write_long_4cmd(vo_intf_hdl, 0x39, 0x02, 0x07, 0x10);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0x7a, 0x20);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0x7d, 0x00);
    ret |= mipi_tx_write_long_3cmd(vo_intf_hdl, 0x90, 0x20, 0x70);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0x93, 0xF8);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0x95, 0x74);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0x97, 0x0a);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0x99, 0x10);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0xee, 0x60);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0x21, 0x01);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0x27, 0x62);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0x30, 0x01);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0x33, 0xF3);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0x34, 0x3f);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0x3a, 0x24);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0x3b, 0x00);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0x3c, 0x20);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0x3d, 0x11);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0x3e, 0x93);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0x42, 0x55);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0x43, 0x55);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0x7f, 0x24);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0x80, 0x24);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0x86, 0x20);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0x89, 0x00);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0x8a, 0xaa);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0x91, 0x44);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0x92, 0x33);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0x93, 0x9b);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0x9a, 0x00);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0x9c, 0x80);
    ret |= mipi_tx_write_long_6cmd(vo_intf_hdl, 0x5a, 0x15, 0x2f, 0x38, 0x42, 0x47);
    ret |= mipi_tx_write_long_6cmd(vo_intf_hdl, 0x47, 0x15, 0x2f, 0x38, 0x42, 0x47);
    ret |= mipi_tx_write_long_6cmd(vo_intf_hdl, 0x4c, 0x56, 0x50, 0x64, 0x49, 0x4a);
    ret |= mipi_tx_write_long_6cmd(vo_intf_hdl, 0x5f, 0x56, 0x50, 0x64, 0x49, 0x4a);
    ret |= mipi_tx_write_long_6cmd(vo_intf_hdl, 0x64, 0x4e, 0x36, 0x4c, 0x47, 0x55);
    ret |= mipi_tx_write_long_6cmd(vo_intf_hdl, 0x51, 0x4e, 0x36, 0x4c, 0x47, 0x55);
    ret |= mipi_tx_write_long_5cmd(vo_intf_hdl, 0x69, 0x56, 0x61, 0x6d, 0x7f);
    ret |= mipi_tx_write_long_5cmd(vo_intf_hdl, 0x56, 0x56, 0x61, 0x6d, 0x7f);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0xee, 0x70);
    ret |= mipi_tx_write_long_6cmd(vo_intf_hdl, 0x00, 0x00, 0x03, 0x00, 0x01, 0x11);
    ret |= mipi_tx_write_long_6cmd(vo_intf_hdl, 0x05, 0x13, 0x55, 0x01, 0x00, 0x00);
    ret |= mipi_tx_write_long_6cmd(vo_intf_hdl, 0x0a, 0x00, 0x00, 0x05, 0x05, 0x00);
    ret |= mipi_tx_write_long_6cmd(vo_intf_hdl, 0x10, 0x04, 0x07, 0x00, 0x01, 0x00);
    ret |= mipi_tx_write_long_6cmd(vo_intf_hdl, 0x15, 0x00, 0x0f, 0x0d, 0x08, 0x00);
    ret |= mipi_tx_write_long_3cmd(vo_intf_hdl, 0x29, 0x05, 0x05);
    ret |= mipi_tx_write_long_6cmd(vo_intf_hdl, 0x30, 0x17, 0x17, 0x55, 0x05, 0x05);
    ret |= mipi_tx_write_long_6cmd(vo_intf_hdl, 0x35, 0x3c, 0x17, 0x17, 0x55, 0x2a);
    ret |= mipi_tx_write_long_3cmd(vo_intf_hdl, 0x3a, 0x2a, 0x3c);
    ret |= mipi_tx_write_long_6cmd(vo_intf_hdl, 0x60, 0x3f, 0x3c, 0x20, 0x21, 0x14);
    ret |= mipi_tx_write_long_6cmd(vo_intf_hdl, 0x65, 0x16, 0x10, 0x12, 0x00, 0x3c);
    ret |= mipi_tx_write_long_6cmd(vo_intf_hdl, 0x6a, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c);
    ret |= mipi_tx_write_long_6cmd(vo_intf_hdl, 0x6f, 0x02, 0x3c, 0x3c, 0x3c, 0x3c);
    ret |= mipi_tx_write_long_3cmd(vo_intf_hdl, 0x74, 0x3c, 0x3c);
    ret |= mipi_tx_write_long_6cmd(vo_intf_hdl, 0x80, 0x3f, 0x3c, 0x20, 0x21, 0x15);
    ret |= mipi_tx_write_long_6cmd(vo_intf_hdl, 0x85, 0x17, 0x11, 0x13, 0x01, 0x3c);
    ret |= mipi_tx_write_long_6cmd(vo_intf_hdl, 0x8a, 0x3c, 0x3c, 0x3c, 0x3c, 0x3c);
    ret |= mipi_tx_write_long_6cmd(vo_intf_hdl, 0x8f, 0x03, 0x3c, 0x3c, 0x3c, 0x3c);
    ret |= mipi_tx_write_long_3cmd(vo_intf_hdl, 0x94, 0x3c, 0x3c);
    ret |= mipi_tx_write_long_3cmd(vo_intf_hdl, 0xea, 0x00, 0x00);
    ret |= mipi_tx_write_short_cmd(vo_intf_hdl, 0xee, 0x00);
    if (ret != OT_SUCCESS) {
        printf("mipi_tx_cfg_mix_cmd ret err: %08x\n", ret);
    }
    return ret;
}
#endif

static ot_s32 mipi_tx_cmd_display_start(ot_eis_handle vo_intf_hdl)
{
    ot_s32 ret;
    ot_eis_mipitx_cmd_info_attr cmd = {0};

    cmd.devno = 0;
    cmd.work_mode = OT_MIPITX_WORK_MODE_LP;
    cmd.lp_clk_en = 0;
    cmd.data_type = 0x05;
    cmd.cmd_size = 0x0011; // 0x11 sleep out
    ret = ot_eis_vo_if_set_cmd_info(vo_intf_hdl, &cmd);

    usleep(60000); // delay 60ms
    cmd.cmd_size = 0x0029; // 0x29 - display on
    ret |= ot_eis_vo_if_set_cmd_info(vo_intf_hdl, &cmd);
    if(ret != OT_SUCCESS) {
        sample_print("ot_eis_vo_if_set_cmd_info ret err: %08x\n", ret);
        return ret;
    }

    return OT_SUCCESS;
}

static ot_s32 mipi_tx_cmd_display_stop(ot_eis_handle vo_intf_hdl)
{
    ot_s32 ret;
    ot_eis_mipitx_cmd_info_attr cmd = {0};

    cmd.devno = 0;
    cmd.work_mode = OT_MIPITX_WORK_MODE_LP;
    cmd.lp_clk_en = 0;
    cmd.data_type = 0x05;
    cmd.cmd_size = 0x0028; // 0x28 display off
    ret = ot_eis_vo_if_set_cmd_info(vo_intf_hdl, &cmd);

    usleep(60000); // delay 60ms
    cmd.cmd_size = 0x0010; // 0x10 - sleep in
    ret |= ot_eis_vo_if_set_cmd_info(vo_intf_hdl, &cmd);
    if(ret != OT_SUCCESS) {
        sample_print("ot_eis_vo_if_set_cmd_info ret err: %08x\n", ret);
        return ret;
    }
    return OT_SUCCESS;
}

static ot_void sample_comm_get_mipitx_combo_dev_attr(sample_vo_attr *vo_attr, ot_eis_vo_if_attr *if_attr)
{
    ot_s32 i;
    if (vo_attr->display_attr[0].size.type != OT_EIS_VO_DISPLAY_CUSTOMIZED) {
        return;
    }

    if_attr->dev_attr.devno = 0;
    for (i = 0 ; i < OT_MIPITX_MAX_LANE_NUM; i++) {
        if_attr->dev_attr.lane_id[i] = i;
    }
    if_attr->dev_attr.out_mode = OT_MIPITX_OUT_MODE_DSI_VIDEO;
    if_attr->dev_attr.out_format = OT_MIPITX_OUT_FORMAT_RGB_24BIT;
    if_attr->dev_attr.clklane_continue_mode = OT_MIPITX_CLK_LANE_CONTINUE;

    if_attr->dev_attr.sync_info.hpw = vo_attr->display_attr[0].sync_info.hpw;
    if_attr->dev_attr.sync_info.hbp = vo_attr->display_attr[0].sync_info.hbb - vo_attr->display_attr[0].sync_info.hpw;
    if_attr->dev_attr.sync_info.hact = vo_attr->display_attr[0].sync_info.hact;
    if_attr->dev_attr.sync_info.hfp = vo_attr->display_attr[0].sync_info.hfb;
    if_attr->dev_attr.sync_info.vpw = vo_attr->display_attr[0].sync_info.vpw;
    if_attr->dev_attr.sync_info.vbp = vo_attr->display_attr[0].sync_info.vbb - vo_attr->display_attr[0].sync_info.vpw;
    if_attr->dev_attr.sync_info.vact = vo_attr->display_attr[0].sync_info.vact;
    if_attr->dev_attr.sync_info.vfp = vo_attr->display_attr[0].sync_info.vfb;
    if_attr->dev_attr.pixel_clk = vo_attr->display_attr[0].clk_info.auto_user_sync_info.pixel_clk / 1000 - 300; // KHz
    if_attr->dev_attr.phy_data_rate = (if_attr->dev_attr.pixel_clk * 24 + 4000) / 4 / 1000; // Mbps
    switch (vo_attr->display_mode) {
        case ONE_IN_PORT_MIPI_AV069HDT_960X540:
        case ONE_IN_PORT_MIPI_AV069HDT_1280X720:
            if_attr->dev_attr.video_mode = OT_MIPITX_NON_BURST_MODE_SYNC_PULSES;
            if_attr->dev_attr.phy_data_rate = 350;
            if_attr->dev_attr.pixel_clk = if_attr->dev_attr.phy_data_rate * 1000 / 6;
            break;
        case TWO_IN_PORT_MIPI_AV069HDT_2560X720:
            if_attr->dev_attr.video_mode = OT_MIPITX_NON_BURST_MODE_SYNC_PULSES;
            break;
        default:
            if_attr->dev_attr.video_mode = OT_MIPITX_BURST_MODE;
            break;
    }
}

ot_s32 sample_comm_start_mipi_tx(sample_vo_attr *vo_attr)
{
    ot_s32 ret;
    ot_eis_handle vo_intf_hdl;
    ot_eis_vo_if_attr if_attr = {0};

    ret = ot_eis_vo_if_init();
    if(ret != OT_SUCCESS) {
        printf("ot_eis_vo_if_init ret err: %08x\n", ret);
        return ret;
    }

    ret = ot_eis_vo_if_create(&vo_intf_hdl);
    if(ret != OT_SUCCESS) {
        printf("ot_eis_vo_if_create ret err: %08x\n", ret);
        goto IF_DEINIT;
    }

    sample_comm_get_mipitx_combo_dev_attr(vo_attr, &if_attr);

    ret = ot_eis_vo_if_config(vo_intf_hdl, &if_attr);
    if(ret != OT_SUCCESS) {
        printf("ot_eis_vo_if_config_attr ret err: %08x\n", ret);
        goto IF_DESTROY;
    }

    if (vo_attr->display_mode == ONE_IN_PORT_MIPI_800X1280) {
        sample_comm_gpio_output_ctrl(SCREEN_VDD_GPIO_GRP, SCREEN_VDD_GPIO_PIN, OT_TRUE);
        usleep(150000); // delay 150ms
#ifdef MIPI_TX_MIX_CMD
        ret |= mipi_tx_cfg_mix_cmd(vo_intf_hdl);
#elif defined (MIPI_TX_SHORT_CMD)
        ret |= mipi_tx_cfg_short_cmd(vo_intf_hdl);
#endif
        ret |= mipi_tx_cmd_display_start(vo_intf_hdl);
        if (ret != OT_SUCCESS) {
            printf("mipi tx cfg cmd failed. ret err: %08x\n", ret);
            goto IF_DESTROY;
        }
    }

    ret = ot_eis_vo_if_start(vo_intf_hdl);
    if(ret != OT_SUCCESS) {
        printf("ot_eis_vo_if_start ret err: %08x\n", ret);
        goto IF_DESTROY;
    }
    vo_attr->mipi_tx_hdl = vo_intf_hdl;
    return OT_SUCCESS;
IF_DESTROY:
    ot_eis_vo_if_destroy(vo_intf_hdl);
IF_DEINIT:
    ot_eis_vo_if_deinit();
    return ret;
}

ot_void sample_comm_stop_mipi_tx(sample_vo_attr *vo_attr)
{
    ot_s32 ret;

    if (vo_attr->display_mode == ONE_IN_PORT_MIPI_800X1280) {
        ret = mipi_tx_cmd_display_stop(vo_attr->mipi_tx_hdl);
        if(ret != OT_SUCCESS) {
            printf("mipi_tx_cmd_display_stop ret err: %08x\n", ret);
            return;
        }
        sample_comm_gpio_output_ctrl(SCREEN_VDD_GPIO_GRP, SCREEN_VDD_GPIO_PIN, OT_FALSE);
    }

    ret = ot_eis_vo_if_stop(vo_attr->mipi_tx_hdl);
    if(ret != OT_SUCCESS) {
        printf("ot_eis_vo_if_stop ret err: %08x\n", ret);
        return;
    }

    ret = ot_eis_vo_if_destroy(vo_attr->mipi_tx_hdl);
    if(ret != OT_SUCCESS) {
        printf("ot_eis_vo_if_destroy ret err: %08x\n", ret);
        return;
    }

    (ot_void)ot_eis_vo_if_deinit();
}

#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* end of #ifdef __cplusplus */

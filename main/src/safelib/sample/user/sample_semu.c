#include <stdio.h>
#include <stdlib.h>

#include "ot_semu_rt.h"
#include "ot_eis_type.h"
#include "ot_eis_errno.h"
#include "semu_log.h"

#define NUM_REGISTERS  10
#define sample_pause()                                                            \
    do {                                                                          \
        semu_info("---------------press enter key to exit!---------------\n");    \
        getchar();                                                                \
    } while (0)

static ot_semu_reg semu_reg[NUM_REGISTERS] = {
    {(ot_ulong *)0x11000000, 0xffffffff},   /* 初始值：0xffffffff */
    {(ot_ulong *)0x11000008, 0xc2},         /* 初始值：0xc2 */
    {(ot_ulong *)0x1100000c, 0x0},          /* 初始值：0x0 */
    {(ot_ulong *)0x11000010, 0x1},          /* 初始值：0x1 */
    {(ot_ulong *)0x11000014, 0x0},          /* 初始值：0x0 */
    {(ot_ulong *)0x11000018, 0xffffffff},   /* 初始值：0xffffffff */
    {(ot_ulong *)0x1100001c, 0x0},          /* 初始值：0x0 */
    {(ot_ulong *)0x11000020, 0x0},          /* 初始值：0x0 */
    {(ot_ulong *)0x11000024, 0xffffffff},   /* 初始值：0xffffffff */
    {(ot_ulong *)0x11000028, 0x20}          /* 初始值：0x20 */
};

static ot_void semu_err_callback(ot_semu_sm_type sm_type, ot_s32 err_type, ot_semu_err_info *info)
{
    semu_info("get error,sm_type:%d, err_type:%d\n", sm_type, err_type);
    if (sm_type == OT_SEMU_SM_MIPIRX_ERR_MONITOR) {
        semu_info("mipirx dev_id: %d\n", info->mipirx.dev_id);
        semu_info("mipirx vc_id: %d\n", info->mipirx.vc_id);
        semu_info("mipirx phy_id: %d\n", info->mipirx.phy_id);
    }

    if (sm_type == OT_SEMU_SM_MIPITX_ERR_MONITOR) {
        semu_info("mipitx err %d\n", err_type);
    }

    if (sm_type == OT_SEMU_SM_MIPITX_FPS_ERR_MONITOR) {
        semu_info("mipitx fps err %d\n", err_type);
    }

    if (sm_type == OT_SEMU_SM_I2C_ERR_MONITOR) {
        semu_info("i2c dev_id: %d\n", info->i2c.dev_id);
    }

    if (sm_type == OT_SEMU_SM_SPI_ERR_MONITOR) {
        semu_info("spi dev_id: %d\n", info->spi.dev_id);
    }

    if(sm_type == OT_SEMU_SM_VI_ERR_MONITOR && err_type == OT_SEMU_VI_ERR_RESOLUTION_CHANGE) {
        semu_info("vi get resolution change report dev_id: %d\n", info->vi.dev_id);
    }

    if(sm_type == OT_SEMU_SM_VI_ERR_MONITOR && err_type == OT_SEMU_VI_ERR_RECOVER_OK) {
        semu_info("vi recover OK report\n");
    }

    if(sm_type == OT_SEMU_SM_VO_ERR_MONITOR) {
        semu_info("vo irq user report errtype[%d]\n", err_type);
    }

    if(sm_type == OT_SEMU_SM_REGISTERS_MONITOR) {
        semu_info("Register: %p  Value: 0x%x  Num: %d\n",
                    info->regs.registers->phys_address,
                    info->regs.registers->value,
                    info->regs.num_registers);
    }
}

ot_s32 main(ot_void)
{
    ot_semu_rt_init_cfg init_cfg = {0};
    ot_semu_rt_temper_monitor_cfg temper_cfg = {0};
    ot_semu_rt_vproc_monitor_cfg vproc_cfg = {0};
    ot_semu_rt_ddr_monitor_cfg ddr_cfg = {0};
    ot_semu_rt_video_delay_monitor_cfg vdelay_cfg = {0};
    ot_semu_rt_registers_monitor_cfg registers_cfg = {0};
    ot_semu_rt_mipitx_fps_monitor_cfg mipitx_cfg = {0};

    init_cfg.err_callback = semu_err_callback;
    temper_cfg.range.risk_time = 3;
    temper_cfg.range.fatal_time = 5;
    temper_cfg.range.thres_level_1 = 60;
    temper_cfg.range.thres_level_2 = 90;
    temper_cfg.range.thres_level_3 = 95;
    temper_cfg.range.thres_level_4 = 105;

    temper_cfg.detect_interval_ms = 200;

    //vproc config
    vproc_cfg.detect_interval_ms = 2000; // 2s
    vproc_cfg.loss_thres = 0;

    /* ddr config */
    ddr_cfg.test_addr = (void *)0x50000000;
    ddr_cfg.test_len = 0x100;
    ddr_cfg.detect_interval_ms = 200; // 200ms

    /* video delay config */
    vdelay_cfg.detect_interval_ms = 2000; /* 2s */
    vdelay_cfg.max_delay_thres = 20; /* test for 20ms */

    /* registers config */
    registers_cfg.detect_interval_ms = 2000;
    registers_cfg.reg_set.num_registers = NUM_REGISTERS;
    registers_cfg.reg_set.registers = (ot_semu_reg *)semu_reg;

    //mipitx config
    mipitx_cfg.detect_interval_ms = 2000; // 2s
    mipitx_cfg.fps = 60;
    ot_s32 ret = ot_semu_rt_init(&init_cfg);
    if (ret != OT_SUCCESS) {
        semu_error("semu rt init error\n");
        return -1;
    }

    ret = ot_semu_rt_enable_temper_monitor(OT_TRUE, &temper_cfg);
    if (ret != OT_SUCCESS) {
        semu_error("semu rt enable temper error\n");
        goto err_with_init;
    }

    ret = ot_semu_rt_enable_mipirx_monitor(OT_TRUE);
    if (ret != OT_SUCCESS) {
        semu_error("semu rt enable mipirx error\n");
        goto err_with_temper_enable;
    }

    ret = ot_semu_rt_enable_i2c_monitor(OT_TRUE);
    if (ret != OT_SUCCESS) {
        semu_error("semu rt enable i2c error\n");
        goto err_with_mipirx_enable;
    }

    ret = ot_semu_rt_enable_spi_monitor(OT_TRUE);
    if (ret != OT_SUCCESS) {
        semu_error("semu rt enable spi error\n");
        goto err_with_i2c_enable;
    }

    ret = ot_semu_rt_enable_emmc_monitor(OT_TRUE);
    if (ret != OT_SUCCESS) {
        semu_error("semu rt enable emmc error\n");
        goto err_with_spi_enable;
    }

    ret = ot_semu_rt_enable_fmc_monitor(OT_TRUE);
    if (ret != OT_SUCCESS) {
        semu_error("semu rt enable fmc error\n");
        goto err_with_emmc_enable;
    }

    ret = ot_semu_rt_enable_vproc_monitor(OT_TRUE, &vproc_cfg);
    if (ret != OT_SUCCESS) {
        semu_error("semu rt enable vproc error\n");
        goto err_with_fmc_enable;
    }

    ret = ot_semu_rt_enable_ddr_monitor(OT_TRUE, &ddr_cfg);
    if (ret != OT_SUCCESS) {
        semu_error("semu rt enable ddr error\n");
        goto err_with_vproc_enable;
    }

    ret = ot_semu_rt_enable_video_delay_monitor(OT_TRUE, &vdelay_cfg);
    if (ret != OT_SUCCESS) {
        semu_error("semu rt enable vdelay error\n");
        goto err_with_ddr_enable;
    }

    ret = ot_semu_rt_enable_registers_monitor(OT_TRUE, &registers_cfg);
    if (ret != OT_SUCCESS) {
        semu_error("semu rt enable registers error\n");
        goto err_with_vdelay_enable;
    }

    ret = ot_semu_rt_enable_mipitx_monitor(OT_TRUE);
    if (ret != OT_SUCCESS) {
        semu_error("semu rt enable mipitx error\n");
        goto err_with_registers_enable;
    }

    ret = ot_semu_rt_enable_mipitx_fps_monitor(OT_TRUE, &mipitx_cfg);
    if (ret != OT_SUCCESS) {
        semu_error("semu rt enable mipitx fps error\n");
        goto err_with_mipitx_enable;
    }

    ret = ot_semu_rt_enable_vi_monitor(OT_TRUE);
    if (ret != OT_SUCCESS) {
        semu_error("semu rt enable vi error\n");
        goto err_with_mipitx_fps_enable;
    }

    ret = ot_semu_rt_enable_vo_monitor(OT_TRUE);
    if (ret != OT_SUCCESS) {
        semu_error("semu rt enable vo error\n");
        goto err_with_vi_enable;
    }

    ret = ot_semu_rt_start();
    if (ret != OT_SUCCESS) {
        semu_error("semu rt start error\n");
        goto end;
    }

    sample_pause();

    ret = ot_semu_rt_stop();
    if (ret != OT_SUCCESS) {
        semu_error("semu rt stop error\n");
        goto end;
    }

end:
    ot_semu_rt_enable_vo_monitor(OT_FALSE);

err_with_vi_enable:
    ot_semu_rt_enable_vi_monitor(OT_FALSE);

err_with_mipitx_fps_enable:
    ot_semu_rt_enable_mipitx_fps_monitor(OT_FALSE, OT_NULL);

err_with_mipitx_enable:
    ot_semu_rt_enable_mipitx_monitor(OT_FALSE);

err_with_registers_enable:
    ot_semu_rt_enable_registers_monitor(OT_FALSE, OT_NULL);

err_with_vdelay_enable:
    ot_semu_rt_enable_video_delay_monitor(OT_FALSE, OT_NULL);

err_with_ddr_enable:
    ot_semu_rt_enable_ddr_monitor(OT_FALSE, OT_NULL);

err_with_vproc_enable:
    ot_semu_rt_enable_vproc_monitor(OT_FALSE, OT_NULL);

err_with_fmc_enable:
    ot_semu_rt_enable_fmc_monitor(OT_FALSE);

err_with_emmc_enable:
    ot_semu_rt_enable_emmc_monitor(OT_FALSE);

err_with_spi_enable:
    ot_semu_rt_enable_spi_monitor(OT_FALSE);

err_with_i2c_enable:
    ot_semu_rt_enable_i2c_monitor(OT_FALSE);

err_with_mipirx_enable:
    ot_semu_rt_enable_mipirx_monitor(OT_FALSE);

err_with_temper_enable:
    ot_semu_rt_enable_temper_monitor(OT_FALSE, OT_NULL);

err_with_init:
    ot_semu_rt_deinit();
    return ret;
}

#include <stdio.h>

#include "ot_eis_mipi_rx.h"

#define MIPI_EXT_TYPE_NUM 3

typedef enum {
    COMM_SENSOR_IMX334_8M_30FPS_12BIT,
    COMM_SENSOR_OV03C_2M_60FPS_12BIT,
    COMM_SENSOR_INVALID
} comm_sensor_type;


typedef struct {
    ot_u32 sns_uid;
    comm_sensor_type sns_type;
    ot_bool clk_reset_enable;
    ot_eis_sensor_clk_src sns_clk_src;
    ot_eis_sensor_reset_src sns_reset_src;
    ot_u32 bus_id;
    const ot_eis_vi_if_attr* mipi_attr;
    const ot_eis_vi_intf_ext_attr* mipi_ex_attr;
} sample_comm_sns_attr;


static const ot_eis_vi_if_attr s_mipi_4lane_sensor_imx334_12bit_8m_dev0_attr = {
    .if_type = OT_VI_INTF_TYPE_MIPI,
    .data_rate = OT_VI_INTF_PPC_X1,
    .img_width = 3980,
    .img_height = 2160,
    .mipi_attr = {
        .devno = 0,
        .data_fmt = OT_MIPIRX_DT_RAW_12,
        .wdr_mode = OT_MIPIRX_WDR_NONE,
        .lane_id = {0, 1, 2, 3},
    }
};

static const ot_eis_vi_if_attr s_mipi_4lane_sensor_0x03c_12bit_2m_dev2_attr = {
    .if_type = OT_VI_INTF_TYPE_MIPI,
    .data_rate = OT_VI_INTF_PPC_X1,
    .img_width = 1920,
    .img_height = 1080,
    .mipi_attr = {
        .devno = 2,
        .data_fmt = OT_MIPIRX_DT_RAW_12,
        .wdr_mode = OT_MIPIRX_WDR_NONE,
        .lane_id = {4, 5, 6, 7},
    }
};

static ot_eis_vi_intf_ext_attr g_mipi_ext_data_default_attr = {
    .data_attr = {
            .type_num = MIPI_EXT_TYPE_NUM,
            .data_bit_width = {12, 12, 12},
            .data_type = {0x2c, 0x2c, 0x2c}
    }
};

static const sample_comm_sns_attr s_sns_id_0_attr = {
    .sns_uid = 0,
    .sns_type = COMM_SENSOR_IMX334_8M_30FPS_12BIT,
    .clk_reset_enable = OT_TRUE,
    .sns_clk_src = 0,
    .sns_reset_src = 0,
    .bus_id = 0,
    .mipi_attr = &s_mipi_4lane_sensor_imx334_12bit_8m_dev0_attr,
    .mipi_ex_attr = &g_mipi_ext_data_default_attr,
};

static const sample_comm_sns_attr s_sns_id_1_attr = {
    .sns_uid = 1,
    .sns_type = COMM_SENSOR_OV03C_2M_60FPS_12BIT,
    .clk_reset_enable = OT_TRUE,
    .sns_clk_src = 0,
    .sns_reset_src = 0,
    .bus_id = 0,
    .mipi_attr = &s_mipi_4lane_sensor_0x03c_12bit_2m_dev2_attr,
    .mipi_ex_attr = &g_mipi_ext_data_default_attr,
};


ot_void sample_comm_get_sensor_cfg(ot_u32 sns_uid, const sample_comm_sns_attr **sns_info)
{
    switch(sns_uid) {
        case 0:
            *sns_info = &s_sns_id_0_attr;
            break;

        case 1:
            *sns_info = &s_sns_id_1_attr;
            break;

        default:
            break;
    }
}

ot_s32 vi_intf_start_sensor(ot_u32 sns_uid)
{
    const sample_comm_sns_attr* sns_attr = OT_NULL;
    sample_comm_get_sensor_cfg(sns_uid, &sns_attr);
    ot_s32 ret = ot_eis_vi_if_sensor_clk_enable(sns_attr->sns_clk_src, OT_TRUE);
    if(ret != OT_SUCCESS) {
        printf("ot_eis_vi_if_sensor_clk_enable ret err: %08x\n", ret);
        return OT_FAILURE;
    }

    ret = ot_eis_vi_if_sensor_enable(sns_attr->sns_reset_src, OT_TRUE);
    if(ret != OT_SUCCESS) {
        printf("ot_eis_vi_if_sensor_enable ret err: %08x\n", ret);
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

ot_s32 vi_intf_stop_sensor(ot_u32 sns_uid)
{
    const sample_comm_sns_attr* sns_attr = OT_NULL;
    sample_comm_get_sensor_cfg(sns_uid, &sns_attr);

    ot_s32 ret = ot_eis_vi_if_sensor_enable(sns_attr->sns_reset_src, OT_FALSE);
    if(ret != OT_SUCCESS) {
        printf("ot_eis_vi_if_sensor_enable ret err: %08x\n", ret);
        return OT_FAILURE;
    }

    ret = ot_eis_vi_if_sensor_clk_enable(sns_attr->sns_clk_src, OT_FALSE);
    if(ret != OT_SUCCESS) {
        printf("ot_eis_vi_if_sensor_clk_enable ret err: %08x\n", ret);
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

ot_s32 vi_intf_start_mipi(ot_u32 sns_uid, ot_eis_handle* mipi_hdl)
{
    ot_eis_handle vi_intf_hdl;
    ot_eis_vi_if_init_attr init_attr;
    init_attr.divide_mode = OT_MIPIRX_LANE_LAYOUT_MODE_1;
    ot_s32 ret = ot_eis_vi_if_init(&init_attr);
    if(ret != OT_SUCCESS) {
        printf("ot_eis_vi_if_init ret err: %08x\n", ret);
        goto IF_FAILED;
    }

    const sample_comm_sns_attr* sns_attr = OT_NULL;
    sample_comm_get_sensor_cfg(sns_uid, &sns_attr);
    ret = ot_eis_vi_if_create(&vi_intf_hdl, sns_attr->mipi_attr);
    if(ret != OT_SUCCESS) {
        printf("ot_eis_vi_if_create ret err: %08x\n", ret);
        goto IF_DEINIT;
    }
    ret = ot_eis_vi_if_set_ext_attr(vi_intf_hdl, sns_attr->mipi_ex_attr);
    if(ret != OT_SUCCESS) {
        printf("ot_eis_vi_if_set_ext_attr ret err: %08x\n", ret);
        goto IF_DESTROY;
    }

    ret = ot_eis_vi_if_start(vi_intf_hdl);
    if(ret != OT_SUCCESS) {
        printf("ot_eis_vi_if_start ret err: %08x\n", ret);
        goto IF_DESTROY;
    }
    *mipi_hdl = vi_intf_hdl;
    return OT_SUCCESS;
IF_DESTROY:
    ot_eis_vi_if_destroy(vi_intf_hdl);
IF_DEINIT:
    ot_eis_vi_if_deinit();
IF_FAILED:
    return OT_FAILURE;
}

ot_s32 vi_intf_stop_mipi(ot_eis_handle mipi_hdl)
{
    ot_s32 ret = ot_eis_vi_if_stop(mipi_hdl);
    if(ret != OT_SUCCESS) {
        printf("ot_eis_vi_if_stop ret err: %08x\n", ret);
        return OT_FAILURE;
    }

    ret = ot_eis_vi_if_destroy(mipi_hdl);
    if(ret != OT_SUCCESS) {
        printf("ot_eis_vi_if_destroy ret err: %08x\n", ret);
    }

    ret = ot_eis_vi_if_deinit();
    if(ret != OT_SUCCESS) {
        printf("ot_eis_vi_if_deinit ret err: %08x\n", ret);
        return OT_FAILURE;
    }
    return OT_SUCCESS;
}

int main()
{
    ot_u32 sns_id = 0;
    ot_eis_handle mipi_hdl;
    ot_s32 ret = vi_intf_start_mipi(sns_id, &mipi_hdl);
    if(ret != OT_SUCCESS) {
        printf("vi_intf_start_mipi ret err: %08x\n", ret);
        return OT_FAILURE;
    }

    ret = vi_intf_start_sensor(sns_id);
    if(ret != OT_SUCCESS) {
        printf("vi_intf_start_sensor ret err: %08x\n", ret);
        vi_intf_stop_mipi(mipi_hdl);
        return OT_FAILURE;
    }

    while(1) {
        printf("---------------press enter key to exit!---------------\n");
        getchar();
    }

    ret = vi_intf_stop_mipi(mipi_hdl);
    if(ret != OT_SUCCESS) {
        printf("vi_intf_start_sensor ret err: %08x\n", ret);
        return OT_FAILURE;
    }

    ret = vi_intf_stop_sensor(sns_id);
    if(ret != OT_SUCCESS) {
        printf("vi_intf_start_sensor ret err: %08x\n", ret);
        return OT_FAILURE;
    }
}

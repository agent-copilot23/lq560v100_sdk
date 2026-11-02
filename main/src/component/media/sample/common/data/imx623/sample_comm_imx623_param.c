#include <stdio.h>
#include <sys/time.h>
#include "sample_comm.h"
#include "ot_eis_vi_sns.h"
#include "ot_eis_vi_awb.h"
#include "ot_eis_sns_ctrl.h"
#include "imx623_sensor_alg.h"
#include "imx623_sensor_awb.h"
#include "imx623_sensor_table.h"

static ot_void sample_comm_parse_imx623_alg_param(ot_vi_pipe_id vi_pipe, ot_eis_vi_sns_alg_default *alg)
{
    alg->key = g_imx623_key;
    alg->drc = &g_imx623_cmos_drc;
    alg->demosaic = &g_imx623_cmos_demosaic;
    alg->gamma = &g_imx623_cmos_gamma;
    alg->sharpen = &g_imx623_cmos_yuv_sharpen;
    alg->ldci = &g_imx623_cmos_ldci;
    alg->dpc = &g_imx623_cmos_dpc;
    alg->lsc = &g_imx623_cmos_lsc;
    alg->ge = &g_imx623_cmos_ge;
    alg->anti_false_color = &g_imx623_cmos_anti_false_color;
    alg->bayer_nr = &g_imx623_cmos_bayer_nr;
    alg->expander = &g_imx623_cmos_expander;
    alg->clut = &g_imx623_cmos_clut;
    alg->wdr = &g_imx623_cmos_wdr;
    alg->dehaze = &g_imx623_cmos_dehaze;
    alg->cac = &g_imx623_cmos_cac;
    alg->bshp = &g_imx623_cmos_bayershp;
    alg->acs = &g_imx623_cmos_acs;
    alg->ca = &g_imx623_cmos_ca;
    alg->sns_blc = &g_imx623_cmos_blc;
    alg->noise_calibration = g_imx623_cmos_noise_calibration;
    return;
}

static ot_void sample_comm_parse_imx623_awb_param(ot_vi_pipe_id vi_pipe, ot_eis_vi_awb_sensor_default *sns_awb_param)
{
    *sns_awb_param = g_imx623_awb_cmos;
    return;
}

static ot_void sample_comm_parse_imx623_sensor_table_param(ot_vi_pipe_id vi_pipe, ot_eis_sns_reg_table *sns_table_param)
{
    ot_eis_sns_reg_table  *dkshd_table = sns_table_param;
    ot_eis_sns_reg_table  *adj_table   = sns_table_param + 1;
    ot_eis_sns_reg_table  *pxshd_table = sns_table_param + 2;
    ot_eis_sns_reg_table  *awb_table   = sns_table_param + 3;
    ot_eis_sns_reg_table  *fme_table   = sns_table_param + 4;

    dkshd_table->data_len =  sizeof(g_imx623_dkshd) / sizeof(g_imx623_dkshd[0]);
    dkshd_table->write_flag = 1;
    dkshd_table->reg_data = OT_NULL;
    dkshd_table->reg_data = (ot_eis_sns_reg_data *)&g_imx623_dkshd[0][0];


    adj_table->data_len =  sizeof(g_imx623_adj) / sizeof(g_imx623_adj[0]);
    adj_table->write_flag = 1;
    adj_table->reg_data = OT_NULL;
    adj_table->reg_data =  (ot_eis_sns_reg_data *)&g_imx623_adj[0][0];


    pxshd_table->data_len =  sizeof(g_imx623_pxshd) / sizeof(g_imx623_pxshd[0]);
    pxshd_table->write_flag = 1;
    pxshd_table->reg_data = OT_NULL;
    pxshd_table->reg_data = (ot_eis_sns_reg_data *)&g_imx623_pxshd[0][0];


    awb_table->data_len =  sizeof(g_imx623_awb) / sizeof(g_imx623_awb[0]);
    awb_table->write_flag = 1;
    awb_table->reg_data = OT_NULL;
    awb_table->reg_data = (ot_eis_sns_reg_data *)&g_imx623_awb[0][0];


    fme_table->data_len =  sizeof(g_imx623_fme) / sizeof(g_imx623_fme[0]);
    fme_table->write_flag = 1;
    fme_table->reg_data = OT_NULL;
    fme_table->reg_data = (ot_eis_sns_reg_data *)&g_imx623_fme[0][0];

    return;
}

ot_sns_set_param g_imx623_tuning_obj = {
    .set_alg = sample_comm_parse_imx623_alg_param,
    .set_awb = sample_comm_parse_imx623_awb_param,
    .set_sensor_table = sample_comm_parse_imx623_sensor_table_param,
};

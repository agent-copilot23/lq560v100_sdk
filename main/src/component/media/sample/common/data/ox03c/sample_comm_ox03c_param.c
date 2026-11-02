#include <stdio.h>
#include <sys/time.h>
#include "sample_comm.h"
#include "ot_eis_vi_sns.h"
#include "ot_eis_vi_awb.h"
#include "ot_eis_sns_ctrl.h"
#include "ox03c_sensor_alg.h"
#include "ox03c_sensor_awb.h"
#include "ox03c_sensor_table.h"

static ot_void sample_comm_parse_ox03c_alg_param(ot_vi_pipe_id vi_pipe, ot_eis_vi_sns_alg_default *alg)
{
    alg->key = g_ox03c_key;
    alg->drc = &g_ox03c_cmos_drc;
    alg->demosaic = &g_ox03c_cmos_demosaic;
    alg->gamma = &g_ox03c_cmos_gamma;
    alg->sharpen = &g_ox03c_cmos_yuv_sharpen;
    alg->ldci = &g_ox03c_cmos_ldci;
    alg->dpc = &g_ox03c_cmos_dpc;
    alg->lsc = &g_ox03c_cmos_lsc;
    alg->ge = &g_ox03c_cmos_ge;
    alg->anti_false_color = &g_ox03c_cmos_anti_false_color;
    alg->bayer_nr = &g_ox03c_cmos_bayer_nr;
    alg->expander = &g_ox03c_cmos_expander;
    alg->clut = &g_ox03c_cmos_clut;
    alg->wdr = &g_ox03c_cmos_wdr;
    alg->dehaze = &g_ox03c_cmos_dehaze;
    alg->cac = &g_ox03c_cmos_cac;
    alg->bshp = &g_ox03c_cmos_bayershp;
    alg->acs = &g_ox03c_cmos_acs;
    alg->ca = &g_ox03c_cmos_ca;
    alg->sns_blc = &g_ox03c_cmos_blc;
    alg->noise_calibration = g_ox03c_cmos_noise_calibration;
    return;
}

static ot_void sample_comm_parse_ox03c_awb_param(ot_vi_pipe_id vi_pipe, ot_eis_vi_awb_sensor_default *sns_awb_param)
{
    *sns_awb_param = g_ox03c_awb_cmos;
    return;
}

static ot_void sample_comm_parse_ox03c_sensor_table_param(ot_vi_pipe_id vi_pipe, ot_eis_sns_reg_table *sns_table_param)
{
    ot_eis_sns_reg_table  *dkshd_table = sns_table_param;
    ot_eis_sns_reg_table  *adj_table   = sns_table_param + 1;
    ot_eis_sns_reg_table  *pxshd_table = sns_table_param + 2;
    ot_eis_sns_reg_table  *awb_table   = sns_table_param + 3;
    ot_eis_sns_reg_table  *fme_table   = sns_table_param + 4;

    dkshd_table->data_len =  sizeof(g_ox03c_dkshd) / sizeof(g_ox03c_dkshd[0]);
    dkshd_table->write_flag = 1;
    dkshd_table->reg_data = OT_NULL;
    dkshd_table->reg_data = (ot_eis_sns_reg_data *)&g_ox03c_dkshd[0][0];


    adj_table->data_len =  sizeof(g_ox03c_adj) / sizeof(g_ox03c_adj[0]);
    adj_table->write_flag = 1;
    adj_table->reg_data = OT_NULL;
    adj_table->reg_data =  (ot_eis_sns_reg_data *)&g_ox03c_adj[0][0];


    pxshd_table->data_len =  sizeof(g_ox03c_pxshd) / sizeof(g_ox03c_pxshd[0]);
    pxshd_table->write_flag = 1;
    pxshd_table->reg_data = OT_NULL;
    pxshd_table->reg_data = (ot_eis_sns_reg_data *)&g_ox03c_pxshd[0][0];


    awb_table->data_len =  sizeof(g_ox03c_awb) / sizeof(g_ox03c_awb[0]);
    awb_table->write_flag = 1;
    awb_table->reg_data = OT_NULL;
    awb_table->reg_data = (ot_eis_sns_reg_data *)&g_ox03c_awb[0][0];


    fme_table->data_len =  sizeof(g_ox03c_fme) / sizeof(g_ox03c_fme[0]);
    fme_table->write_flag = 1;
    fme_table->reg_data = OT_NULL;
    fme_table->reg_data = (ot_eis_sns_reg_data *)&g_ox03c_fme[0][0];

    return;
}

ot_sns_set_param g_ox03c_tuning_obj = {
    .set_alg = sample_comm_parse_ox03c_alg_param,
    .set_awb = sample_comm_parse_ox03c_awb_param,
    .set_sensor_table = sample_comm_parse_ox03c_sensor_table_param,
};

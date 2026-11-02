/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#include "scene_setparam_inner.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "securec.h"
#include "ot_eis_vi_ae_api.h"
#include "ot_eis_vproc.h"
#include "ot_eis_vproc_common.h"
#include "ot_eis_vproc_grp.h"
#include "ot_scene_inner.h"
#include "sample_comm.h"
#include "ot_eis_vi_api.h"
#include "ot_scenecomm.h"
#include "ot_eis_vi_alg_api.h"
#include "ot_eis_scene_auto_api.h"


#ifdef __cplusplus
extern "C" {
#endif

#define SET_3DNR

ot_bool g_isp_pause[OT_SCENE_PIPE_MAX_NUM] = {0};
ot_scene_pipe_param g_scene_pipe_param[OT_SCENE_PIPETYPE_NUM] = {0};

ot_scene_pipe_param *get_pipe_params(ot_void)
{
    return g_scene_pipe_param;
}

ot_bool *get_isp_pause(ot_void)
{
    return g_isp_pause;
}

ot_u32 scene_get_level_ltoh(ot_u64 value, ot_u32 count, const ot_u64 *thresh)
{
    ot_u32 level;

    for (level = 0; level < count; level++) {
        if (value <= thresh[level]) {
            break;
        }
    }

    if (level == count) {
        level = count - 1;
    }
    return level;
}

ot_u32 scene_get_level_ltoh_u32(ot_u32 value, ot_u32 count, const ot_u32 *thresh)
{
    ot_u32 level;

    for (level = 0; level < count; level++) {
        if (value <= thresh[level]) {
            break;
        }
    }

    if (level == count) {
        level = count - 1;
    }
    return level;
}

ot_u32 scene_interpulate(ot_u64 middle, ot_u64 left, ot_u64 left_value, ot_u64 right, ot_u64 right_value)
{
    ot_u32 value_tmp, k;

    if (middle <= left) {
        value_tmp = (ot_u32)left_value;
        return value_tmp;
    }
    if (middle >= right) {
        value_tmp = (ot_u32)right_value;
        return value_tmp;
    }

    k = (ot_u32)(right - left);
    value_tmp = (ot_u32)(((right - middle) * left_value + (middle - left) * right_value + (k >> 1)) / k);
    return value_tmp;
}

ot_u32 scene_time_filter(ot_u32 param0, ot_u32 param1, ot_u32 time_cnt, ot_u32 index)
{
    ot_u64 temp;
    ot_u32 value;

    if (param0 > param1) {
        temp = (ot_u64)(param0 - param1) << 8; /* 8 is param to time_cnt */
        temp = ((temp * (index + 1)) / scene_div_0to1(time_cnt)) >> 8; /* 8 is time_cnt to param */
        value = (param0 > ((ot_u32)temp + 1)) ? (param0 - (ot_u32)temp - 1) : 0;
    } else if (param0 < param1) {
        temp = (ot_u64)(param1 - param0) << 8; /* 8 is param to time_cnt */
        temp = ((temp * (index + 1)) / scene_div_0to1(time_cnt)) >> 8; /* 8 is time_cnt to param */
        value = (param0 + (ot_u32)temp + 1) > param1 ? param1 : (param0 + (ot_u32)temp + 1);
    } else {
        value = param0;
    }
    return value;
}

#ifdef SET_3DNR
static void print_para_n(const ot_scene_3dnr *p_x, const t_v200_vproc_adv_iey *pai, const t_v200_vproc_adv_sfy *pas,
    const t_v200_vproc_tfy *pt, const t_v200_vproc_sfy *ps, const t_v200_vproc_iey *pi)
{
    scene_logd("\n\n                             sizeof( X ) = %lu ", sizeof(*p_x));
    scene_logd("\n**************N0**********|***********N1***********|*******N2*********|**************N3******************"
        "*****");
    scene_logd("\n -en          %3u:        |              %3u:      |       %3u:       |               %3u:               "
        "     |", p_x->nry0_en, p_x->nry1_en, p_x->nry2_en, p_x->nry3_en);
    scene_logd("\n -nXsf1       %3u:        |          %3u:    %3u:  |    %3u:    %3u:  |            %3u:    %3u:          "
        "     |", pas[0].sfs1, ps[1].sfs1, ps[1].sbr1,
        ps[2].sfs1, ps[2].sbr1, ps[3].sfs1, ps[3].sbr1); /* 2,3 is index */
    scene_logd("\n -nXsf2       %3u:        |          %3u:%3u:%3u:  |    %3u:%3u:%3u:  |            %3u:%3u:%3u:          "
        "     |", pai[0].aux_ies, _tmprt3x(ps, sfs2, sft2, sbr2));
    scene_logd("\n -nXsf4                   |          %3u:%3u:%3u:  |    %3u:%3u:%3u:  |            %3u:%3u:%3u:          "
        "     |", _tmprt3x(ps, sfs4, sft4, sbr4));
    scene_logd("\n -nXsfs5     %3u:%3u:%3u: |                        |                  |            %3u:%3u:%3u:          "
        "     |", pas[0].sfs5, pas[0].sft5, pas[0].sbr5, ps[4].sfs2, ps[4].sft2, ps[4].sbr2); /* 4 is index */
    scene_logd("\n -nXsf5  %3u:%3u:%3u:%3u: |        %3u:%3u:%3u:%3u:|  %3u:%3u:%3u:%3u:|  %3u:%3u:%3u:%3u: | "
        " %3u:%3u:%3u:%3u: |", pai[0].ies0, pai[0].ies1, pai[0].ies2,
        pai[0].ies3, _tmprt4_4(pi, ies0, ies1, ies2, ies3));
    scene_logd("\n -nXsht5         %3u:%3u: |                        |                  | -nXsht5  %3u:%3u: | "
        "         %3u:%3u: |", pai[0].o_sht, pai[0].u_sht, pi[3].o_sht, /* 3 is index */
        pi[3].u_sht, pi[4].o_sht, pi[4].u_sht); /* 3, 4 is index */
    scene_logd("\n                          |                        |-kmode      %3u:  |              %3u: |              "
        "     |", ps[2].k_mode, ps[3].k_mode); /* 2, 3 is index */

    scene_logd("\n -nXsf3      %3u:%3u:%3u: |                        |                  |                   |              "
        "     |", pas[0].spn3, pas[0].sbn3, pas[0].pbr3);

    scene_logd("\n -nXsf4      %3u:%3u:%3u: |                        |                  |                   |              "
        "     |", pas[0].spn4, pas[0].sbn4, pas[0].pbr4);
    scene_logd("\n -nXsf6  %3u:%3u:%3u:%3u: |        %3u:%3u:%3u:%3u:|  %3u:%3u:%3u:%3u:|  %3u:%3u:%3u:%3u: | "
        " %3u:%3u:%3u:%3u: |", pas[0].spn6, pas[0].sbn6, pas[0].pbr6,
        pas[0].j_mode6, _tmprt4_4(ps, spn, sbn, pbr, j_mode));
    scene_logd("\n -nXsfr6 %3u:%3u:%3u:%3u: |        %3u:%3u:%3u:%3u:|  %3u:%3u:%3u:%3u:|  %3u:%3u:%3u:%3u: | "
        " %3u:%3u:%3u:%3u: |", pas[0].sfr6[0], pas[0].sfr6[1], pas[0].sfr6[2], pas[0].sfr6[3], /* 2.3 is index */
        _tmprt4_4(ps, sfr6[0], sfr6[1], sfr6[2], sfr6[3])); /* 2, 3 is index */
    scene_logd("\n -nXsbr6         %3u:%3u: |             %3u:%3u:   |          %3u:%3u:|          %3u:%3u: | "
        "         %3u:%3u: |", pas[0].sbr6[0], pas[0].sbr6[1], _tmprt2_4x(ps, sbr6[0], sbr6[1]));
    scene_logd("\n                          |                        |                  |                   |              "
        "     |");
    scene_logd("\n -nXsfn  %3u:%3u:%3u:%3u: |        %3u:%3u:%3u:%3u:|  %3u:%3u:%3u:%3u:|  %3u:%3u:%3u:%3u: | "
        " %3u:%3u:%3u:%3u: |",
        pas[0].sfn0, pas[0].sfn1, pas[0].sfn2, pas[0].sfn3, _tmprt4_4(ps, sfn0_0, sfn1_0, sfn2_0, sfn3_0));

    scene_logd("\n -nXsth      %3u:%3u:%3u: |            %3u:%3u:%3u:|      %3u:%3u:%3u:|      %3u:%3u:%3u: | "
        "     %3u:%3u:%3u: |", pas[0].sth1, pas[0].sth2, pas[0].sth3, _tmprt3_4(ps, sth1_0, sth2_0, sth3_0));
    scene_logd("\n *************************|-nX2sfn %3u:%3u:%3u:%3u:|  %3u:%3u:%3u:%3u:|                                  "
        "     |", _tmprt4_2(ps, sfn0_1, sfn1_1, sfn2_1, sfn3_1));
    scene_logd("\n -nX2sf1      %3u:        |-nX2sth     %3u:%3u:%3u:|      %3u:%3u:%3u:|                                  "
        "     |", pas[1].sfs1, _tmprt3z(ps, 1, 2, sth1_1, sth2_1, sth3_1)); /* 2, is index */
    scene_logd("\n -nX2sf2      %3u:        |                        |                  |-nXbIdx         %3u:%3u:          "
        "     |", pai[1].aux_ies, ps[3].bri_idx0, ps[3].bri_idx1); /* 2, 3 is index */
}

static void print_nc0(const ot_scene_3dnr *p_x, const t_v200_vproc_tfy *pt, const t_v200_vproc_adv_iey *pai,
    const t_v200_vproc_adv_sfy *pas, const t_v200_vproc_mdy *pm)
{
    scene_logd("\n                          |-ref          %3u:      |                  "
        "|********************nrc0****************", pt[1].ref_en);
    scene_logd("\n -nX2sf5 %3u:%3u:%3u:%3u: |-preMath      %3u:      |                  |  -nC0en             %3u:       "
        "       |", pai[1].ies0, pai[1].ies1, pai[1].ies2, pai[1].ies3, pt[0].auto_math, p_x->nrc0_en);
    scene_logd("\n -nX2sht5        %3u:%3u: |-preTFS       %3u:      |                  |  -presfc            %3u:       "
        "       |", pai[1].o_sht, pai[1].u_sht, pt[0].tfs0, p_x->nrc0.pre_sfc); /* 1 is index */
    scene_logd("\n                          |-dciW         %3u:      |                  |  -sfc               %3u:       "
        "       |", pm[0].dci_w, p_x->nrc0.sfc); /* 0 is index */
    scene_logd("\n -nX2sf3     %3u:%3u:%3u: |-nXtss        %3u:      |     %3u:%3u:     |  -sfcExt          %3u:%3u:     "
        "       |", pas[1].spn3, pas[1].sbn3, pas[1].pbr3, pt[1].tss0, pt[2].tss0,    /* 2 index */
        pt[2].tss1, p_x->nrc0.sfc_enhance, p_x->nrc0.sfc_ext); /* 2 index */
    scene_logd("\n -nX2sf4     %3u:%3u:%3u: |-nXtfs        %3u:      |     %3u:%3u:     |  -tpc               %3u:       "
        "       |", pas[1].spn4, pas[1].sbn4, pas[1].pbr4, pt[1].tfs0, pt[2].tfs0, /* 2 index */
        pt[2].tfs1, p_x->nrc0.tpc); /* 2 index */
    scene_logd("\n -nX2sf6 %3u:%3u:%3u:%3u: |-nXtfr0   %3u:%3u:%3u:  |   %3u:%3u:%3u:   |  -tfc               %3u:       "
        "       |", pas[1].spn6, pas[1].sbn6, pas[1].pbr6, pas[1].j_mode6,
        _tmprt3z(pt, 1, 2, tfr0[0], tfr0[1], tfr0[2]), p_x->nrc0.tfc); /* 2 index  */
    scene_logd("\n -nX2sfr6%3u:%3u:%3u:%3u: |          %3u:%3u:%3u:  |   %3u:%3u:%3u:   |  -trc               %3u:       "
        "       |", pas[1].sfr6[0], pas[1].sfr6[1], pas[1].sfr6[2], pas[1].sfr6[3], /* 2, 3 index  */
        _t3_(pt, 1, tfr0[3], tfr0[4], tfr0[5]), /*  3,4,5, index */
        _t3_(pt, 2, tfr0[3], tfr0[4], tfr0[5]), p_x->nrc0.trc); /*  2,3,4,5, index */
}

static void print_nc1(const t_v200_vproc_nrc1 *pc, const t_v200_vproc_tfy *pt,
    const t_v200_vproc_adv_sfy *pas, const t_v200_vproc_mdy *pm, const ot_scene_3dnr *p_x)
{
    scene_logd("\n -nX2sbr6        %3u:%3u: |-nXtfr1  %3u:%3u:%3u:   |   %3u:%3u:%3u:   "
        "|********************nrc1***************|",
        pas[1].sbr6[0], pas[1].sbr6[1], _tmprt3z(pt, 1, 2, tfr1[0], tfr1[1], tfr1[2])); /*  2 index */
    scene_logd("\n                          |         %3u:%3u:%3u:   |   %3u:%3u:%3u:   |                                  "
        "     |", _tmprt3z(pt, 1, 2, tfr1[3], tfr1[4], tfr1[5])); /*  2,3,4,5, index */
    scene_logd("\n -nX2sfn %3u:%3u:%3u:%3u: |-maMode     %3u:%3u:    |                  | -nC1en              %3u:         "
        "     |", pas[1].sfn0, pas[1].sfn1, pas[1].sfn2, pas[1].sfn3,  pt[0].math_mode, pt[1].math_mode, p_x->nrc1_en);
    scene_logd("\n -nX2sth     %3u:%3u:%3u: |                        |                  | -nCsf2          %3u:%3u:%3u:     "
        "     |", pas[1].sth1, pas[1].sth2, pas[1].sth3, pc->sfs2, pc->sft2, pc->sbr2);
    scene_logd("\n                          |-advMath    %3u:%3u:    |-mcth       %3u:  |                                  "
        "     |", pm[0].adv_math, pm[1].adv_math, pm[0].mcth);

    scene_logd("\n -mXmath             %3u: |            %3u:%3u:    |     %3u:%3u:     | -nCstr           %3u:| %3u:      "
        "     |", pas[0].bld1,  pm[0].math0, pm[0].math1, pm[1].math0, pm[1].math1, pc->pbr0, pc->pbr1);

    scene_logd("\n -advMode            %3u: |-mXmate     %3u:%3u:    |     %3u:%3u:     |                                  "
        "     |", p_x->adv_mode, pm[0].mate0, pm[0].mate1, pm[1].mate0, pm[1].mate1);
    scene_logd("\n -nXbIdx         %3u:%3u: |-mXmabw     %3u:%3u:    |     %3u:%3u:     |                                  "
        "     |", pas[0].bri_idx, pas[1].bri_idx, pm[0].mabw0, pm[0].mabw1, pm[1].mabw0, pm[1].mabw1);
}

static void print_nrx(const ot_scene_3dnr *p_x)
{
    const t_v200_vproc_adv_iey *pai = p_x->adv_ie_y;
    const t_v200_vproc_adv_sfy *pas = p_x->adv_sf_y;
    const t_v200_vproc_tfy     *pt  = p_x->tfy;
    const t_v200_vproc_sfy     *ps  = p_x->sfy;
    const t_v200_vproc_iey     *pi  = p_x->iey;
    const t_v200_vproc_mdy     *pm  = p_x->mdy;
    const t_v200_vproc_nrc1    *pc  = &p_x->nrc1;

    print_para_n(p_x, pai, pas, pt, ps, pi);
    print_nc0(p_x, pt, pai, pas, pm);

    print_nc1(pc, pt, pas, pm, p_x);

    scene_logd("\n*********************************************************************************************************"
        "******");
    scene_logd("\n -n3bri0    %6u: %6u: %6u: %6u:  -n3bri4  %6u: %6u: %6u: %6u:  ",
        _t4a_0_(ps, 3, bri_str), _t4a_4_(ps, 3, bri_str)); /* 3 index */
    scene_logd("\n -n3bri8    %6u: %6u: %6u: %6u:  -n3bri12 %6u: %6u: %6u: %6u: %6u:  ",
        _t4a_8_(ps, 3, bri_str), _t4a_12_(ps, 3, bri_str), ps[3].bri_str[0x10]); /* 3, 0x10 index */
    scene_logd("\n -n2sbs0   %3u:%3u:%3u:%3u: %3u:%3u:%3u:%3u:    -n2sbs8 %3u:%3u:%3u:%3u: %3u:%3u:%3u:%3u:",
        _t4a_0_(ps, 2, sbs_k), _t4a_4_(ps, 2, sbs_k), _t4a_8_(ps, 2, sbs_k), _t4a_12_(ps, 2, sbs_k)); /* 2 index */
    scene_logd("\n -n2sbs0   %3u:%3u:%3u:%3u: %3u:%3u:%3u:%3u:    -n2sbs8 %3u:%3u:%3u:%3u: %3u:%3u:%3u:%3u:%3u:",
        _t4a_10_(ps, 2, sbs_k), _t4a_14_(ps, 2, sbs_k), _t4a_18_(ps, 2, sbs_k), /* 2 index */
        _t4a_1c_(ps, 2, sbs_k), ps[2].sbs_k[0x20]); /* 2  0x20  index */

    scene_logd("\n -n2sds0   %3u:%3u:%3u:%3u: %3u:%3u:%3u:%3u:    -n2sds8 %3u:%3u:%3u:%3u: %3u:%3u:%3u:%3u:",
        _t4a_0_(ps, 2, sds_k), _t4a_4_(ps, 2, sds_k), _t4a_8_(ps, 2, sds_k), _t4a_12_(ps, 2, sds_k)); /* 2 index */
    scene_logd("\n -n2sds0   %3u:%3u:%3u:%3u: %3u:%3u:%3u:%3u:    -n2sds8 %3u:%3u:%3u:%3u: %3u:%3u:%3u:%3u:%3u:",
        _t4a_10_(ps, 2, sds_k), _t4a_14_(ps, 2, sds_k), _t4a_18_(ps, 2, sds_k), /* 2 index */
        _t4a_1c_(ps, 2, sds_k), ps[2].sds_k[0x20]); /* 2  0x20  index */

    scene_logd("\n -n3sbs0   %3u:%3u:%3u:%3u: %3u:%3u:%3u:%3u:    -n3sbs8 %3u:%3u:%3u:%3u: %3u:%3u:%3u:%3u:",
        _t4a_0_(ps, 3, sbs_k), _t4a_4_(ps, 3, sbs_k), _t4a_8_(ps, 3, sbs_k), _t4a_12_(ps, 3, sbs_k)); /* 3 index */
    scene_logd("\n -n3sbs0   %3u:%3u:%3u:%3u: %3u:%3u:%3u:%3u:    -n3sbs8 %3u:%3u:%3u:%3u: %3u:%3u:%3u:%3u:%3u:",
        _t4a_10_(ps, 3, sbs_k), _t4a_14_(ps, 3, sbs_k), _t4a_18_(ps, 3, sbs_k), /* 3 index */
        _t4a_1c_(ps, 3, sbs_k), ps[3].sbs_k[0x20]); /* 3  0x20 index */

    scene_logd("\n -n3sds0   %3u:%3u:%3u:%3u: %3u:%3u:%3u:%3u:    -n3sds8 %3u:%3u:%3u:%3u: %3u:%3u:%3u:%3u:",
        _t4a_0_(ps, 3, sds_k), _t4a_4_(ps, 3, sds_k), _t4a_8_(ps, 3, sds_k), _t4a_12_(ps, 3, sds_k)); /* 3 index */
    scene_logd("\n -n3sds0   %3u:%3u:%3u:%3u: %3u:%3u:%3u:%3u:    -n3sds8 %3u:%3u:%3u:%3u: %3u:%3u:%3u:%3u:%3u:",
        _t4a_10_(ps, 3, sds_k), _t4a_14_(ps, 3, sds_k), _t4a_18_(ps, 3, sds_k), /* 3 index */
        _t4a_1c_(ps, 3, sds_k), ps[3].sds_k[0x20]); /* 3  0x20 index */
}
#endif

ot_void scene_set_isp_attr(ot_vi_pipe_handle pipe_hdl,set_isp_attr_param param, const ot_u32 *ratio_level_thresh,
    const ot_u32 *iso_level_thresh, ot_eis_vi_drc_attr *isp_drc_attr)
{
    ot_u8 index = param.index;
    ot_u32 iso_level = param.iso_level;
    ot_u32 iso_count = param.iso_count;
    ot_u32 ratio_level = param.ratio_level;
    ot_u32 ratio_count = param.ratio_count;
    ot_u32 wdr_ratio = param.wdr_ratio;
    ot_u32 iso = param.iso;
    ot_s32 i;
    ot_u16 tonemapping_value[OT_EIS_VI_DRC_TM_NODE_NUM] = {0};
    ot_u8 curve_index;
    ot_u32 exposure = 100;   //exposure time * ISO (ms * 100)
    ot_u32 exposure_adj = 100;
    ot_eis_vi_exp_info exp_info;  //get exposure info
    ot_s32 ret;

    ret = ot_eis_vi_ae_query_info(pipe_hdl, &exp_info);       //scene_calculate_exp  different defination
    if (ret != OT_SUCCESS) {
        return;
    }
    // exposure = exp_info.exposure/100;      //equal to exp_info.exp_time * iso / 10000;30000 micro-second -> 3
    exposure = exp_info.exposure;
    //scene_logd("*******************************\n");
    //scene_logd("exp_info.exposure:%d\n",exp_info.exposure);
    //scene_logd("exposure:%d\n",exposure);
    iso = exposure;     //iso inter -> exp inter

    for (i = 0; i < OT_EIS_VI_DRC_LMIX_NODE_NUM; i++) {
        scene_set_dynamic_drc_return(isp_drc_attr->local_mixing_bright[i], local_mixing_bright[i], 1);
        scene_set_dynamic_drc_return(isp_drc_attr->local_mixing_dark[i], local_mixing_dark[i], 1);
        scene_set_dynamic_drc_return(isp_drc_attr->color_correction_lut[i], color_correction_lut[i], 0);
    }

    scene_set_dynamic_drc_return(isp_drc_attr->spatial_filter_coef, spatial_filter_coef, 1);
    scene_set_dynamic_drc_return(isp_drc_attr->range_filter_coef, range_filter_coef, 1);
    scene_set_dynamic_drc_return(isp_drc_attr->detail_adjust_coef, detail_adjust_coef, 1);
    scene_set_dynamic_drc_return(isp_drc_attr->rim_reduction_strength, rim_reduction_strength, 1);
    scene_set_dynamic_drc_return(isp_drc_attr->rim_reduction_threshold, rim_reduction_threshold, 1);
    scene_set_dynamic_drc_return(isp_drc_attr->bright_gain_limit, bright_gain_limit, 0);
    scene_set_dynamic_drc_return(isp_drc_attr->bright_gain_limit_step, bright_gain_limit_step, 0);
    scene_set_dynamic_drc_return(isp_drc_attr->dark_gain_limit_luma, dark_gain_limit_luma, 1);
    scene_set_dynamic_drc_return(isp_drc_attr->global_color_ctrl, global_color_ctrl, 1);
    scene_set_dynamic_drc_return(isp_drc_attr->shoot_reduction_en, shoot_reduction_en, 1);
    scene_set_dynamic_drc_return(isp_drc_attr->dark_gain_limit_chroma, dark_gain_limit_chroma, 1);

    scene_set_dynamic_drc_return(isp_drc_attr->high_saturation_color_ctrl, high_saturation_color_ctrl, 0);

    //MG stat
    ret = ot_eis_scene_drc_get_adj(pipe_hdl, &g_scene_pipe_param[index].dynamic_drc.drc_adj, exposure, &exposure_adj);
    if (ret != OT_SUCCESS) {
        return;
    }

    curve_index = scene_get_level_ltoh_u32(exposure_adj, g_scene_pipe_param[index].dynamic_drc.curve_index_count,g_scene_pipe_param[index].dynamic_drc.curve_index_ltoh);
    for (int j = 0; j < OT_EIS_VI_DRC_TM_NODE_NUM; j++) {
        switch (curve_index)
        {
            case 0:
                    tonemapping_value[j] = g_scene_pipe_param[index].dynamic_drc.tm_value_low_adj[j];
                break;
            case 1:
                    tonemapping_value[j] = scene_interpulate(exposure_adj,g_scene_pipe_param[index].dynamic_drc.curve_index_ltoh[curve_index - 1],g_scene_pipe_param[index].dynamic_drc.tm_value_low_adj[j],g_scene_pipe_param[index].dynamic_drc.curve_index_ltoh[curve_index],g_scene_pipe_param[index].dynamic_drc.tm_value_low[j]);
                break;
            case 2:
                    tonemapping_value[j] = scene_interpulate(exposure_adj,g_scene_pipe_param[index].dynamic_drc.curve_index_ltoh[curve_index - 1],g_scene_pipe_param[index].dynamic_drc.tm_value_low[j],g_scene_pipe_param[index].dynamic_drc.curve_index_ltoh[curve_index],g_scene_pipe_param[index].dynamic_drc.tm_value_high[j]);
                break;
            case 3:
                    tonemapping_value[j] = scene_interpulate(exposure_adj,g_scene_pipe_param[index].dynamic_drc.curve_index_ltoh[curve_index - 1],g_scene_pipe_param[index].dynamic_drc.tm_value_high[j],g_scene_pipe_param[index].dynamic_drc.curve_index_ltoh[curve_index],g_scene_pipe_param[index].dynamic_drc.tm_val_higher[j]);
                break;
            case 4:
                    tonemapping_value[j] = scene_interpulate(exposure_adj,g_scene_pipe_param[index].dynamic_drc.curve_index_ltoh[curve_index - 1],g_scene_pipe_param[index].dynamic_drc.tm_val_higher[j],g_scene_pipe_param[index].dynamic_drc.curve_index_ltoh[curve_index],g_scene_pipe_param[index].dynamic_drc.tm_val_highest[j]);
                break;
            case 5:
                    tonemapping_value[j] = scene_interpulate(exposure_adj,g_scene_pipe_param[index].dynamic_drc.curve_index_ltoh[curve_index - 1],g_scene_pipe_param[index].dynamic_drc.tm_val_highest[j],g_scene_pipe_param[index].dynamic_drc.curve_index_ltoh[curve_index],g_scene_pipe_param[index].dynamic_drc.tm_val_highest_adj[j]);
                break;
            default:
                    scene_loge("scene_auto dynamic_drc curve index invalid\n");
                break;
        }
    }

    //light sensor ctrl
    if(g_scene_pipe_param[index].dynamic_back_light.img_bright_enable)
    {

        ot_u32 img_blend_ratio;
        ot_u32 img_array_cnt = get_pipe_params()[index].dynamic_back_light.img_array_cnt;

        ot_u32 * img_light_array = get_pipe_params()[index].dynamic_back_light.img_light_array;
        ot_u32 * img_bright_array = get_pipe_params()[index].dynamic_back_light.img_bright_array;

        //get light sensor value
        ot_u32 light_sensor_value = sample_comm_lightsensor_get_lux(index);

        //get array value
        ot_u32 light_level = (ot_u32)scene_get_level_ltoh_u32((ot_u32)light_sensor_value, img_array_cnt, img_light_array);        
        if (light_level >= 16) {
            return;
        }

        if ((light_level == (img_array_cnt - 1)) || (light_level == 0)) {
            img_blend_ratio = img_bright_array[light_level];
        } else {
            img_blend_ratio = scene_interpulate(light_level,img_light_array[light_level - 1],img_bright_array[light_level - 1],img_light_array[light_level],img_bright_array[light_level]);
        }

        for (int j = 0; j < OT_EIS_VI_DRC_TM_NODE_NUM; j++)
        {
            tonemapping_value[j] = ( tonemapping_value[j] * (100 - img_blend_ratio) + g_scene_pipe_param[index].dynamic_back_light.img_bright_base[j] * img_blend_ratio) / 100;
        }

        if(get_pipe_params()[index].dynamic_back_light.info_print)
        {
            printf("img_blend_ratio:%d\n",img_blend_ratio);
        }
    }

    for (int j = 0; j < OT_EIS_VI_DRC_TM_NODE_NUM; j++) {
    //p1 base,step = ((1+p4)/(p3)) * abs(p1 -p2)
    isp_drc_attr->tone_mapping_value[j] = scene_time_filter(isp_drc_attr->tone_mapping_value[j],
        tonemapping_value[j], g_scene_pipe_param[index].dynamic_drc.curve_speed_p3, g_scene_pipe_param[index].dynamic_drc.curve_speed_p4);
    }
    i = 4; //for strength interp
    if (isp_drc_attr->op_type == 0) {
        scene_set_dynamic_drc_return(isp_drc_attr->auto_attr.strength, manual_str, 1);
    } else {
        scene_set_dynamic_drc_return(isp_drc_attr->manual_attr.strength, manual_str, 1);
    }
}


ot_s32 scene_blend_tone_mapping(ot_vi_pipe_handle pipe_hdl, ot_u8 index,
    ot_eis_scene_dynamic_drc_coef *drc_coef, ot_eis_vi_drc_attr *isp_drc_attr)
{
    ot_u32 i, j;
    ot_u16 tonemapping_value[OT_EIS_VI_DRC_TM_NODE_NUM] = {0};
    ot_eis_vi_exp_info ae_exp_info;
    ot_s32 ret;
    ot_u16 blend_alpha;

    check_scene_return_if_pause(pipe_hdl);
    ret = ot_eis_vi_ae_query_info(pipe_hdl, &ae_exp_info);
    ot_scenecomm_check_return(ret, ret);

    blend_alpha = (ae_exp_info.wdr_exp_coef < 0x200) ? 0 : ae_exp_info.wdr_exp_coef - 0x200; /* clip 0x200 */

    if (drc_coef->ref_exp_ratio >= (g_scene_pipe_param[index].static_wdr_exposure.exp_ratio_max - 2048)) { /* sub2048 */
        for (i = 0; i < OT_EIS_VI_DRC_TM_NODE_NUM; i++) {
            tonemapping_value[i] =
            ((ot_u32)(0x200 - blend_alpha) * g_scene_pipe_param[index].dynamic_drc.tm_val_higher[i] +
             /* 0x200 blend_alpha, precison 9bit */
            (ot_u32)(blend_alpha) * g_scene_pipe_param[index].dynamic_drc.tm_val_highest[i]) >> 9; /* shift9 */
        }
    } else if (drc_coef->ref_exp_ratio <= g_scene_pipe_param[index].dynamic_drc.tm_ratio_threshold) {
        for (i = 0; i < OT_EIS_VI_DRC_TM_NODE_NUM; i++) {
            tonemapping_value[i] =
            ((ot_u32)drc_coef->ref_ratio_alpha * g_scene_pipe_param[index].dynamic_drc.tm_value_low[i] +
             /* 0x100 ref_exp_ratio_alpha, precison 8bit */
            (ot_u32)(0x100 - drc_coef->ref_ratio_alpha) *
            g_scene_pipe_param[index].dynamic_drc.tm_value_high[i]) >> 8; /* shift8 */
        }
    } else {
        for (i = 0; i < OT_EIS_VI_DRC_TM_NODE_NUM; i++) {
            tonemapping_value[i] =
            /* 0x100 ref_exp_ratio_alpha, precison 8bit */
            ((ot_u32)(0x100 - drc_coef->ref_ratio_alpha) * g_scene_pipe_param[index].dynamic_drc.tm_value_high[i] +
            (ot_u32)drc_coef->ref_ratio_alpha * g_scene_pipe_param[index].dynamic_drc.tm_val_higher[i]) >> 8; /* sf8 */
        }
    }

    for (j = 0; j < OT_EIS_VI_DRC_TM_NODE_NUM; j++) {
        isp_drc_attr->tone_mapping_value[j] = scene_time_filter(isp_drc_attr->tone_mapping_value[j],
            tonemapping_value[j], g_scene_pipe_param[index].dynamic_drc.interval, drc_coef->k);
    }

    return OT_SUCCESS;
}

ot_s32 scene_set_tone_mapping_value(ot_vi_pipe_handle pipe_hdl, ot_u8 index, ot_u32 k,
    const ot_eis_vi_alg_state_info *inner_state_info, ot_eis_vi_drc_attr *isp_drc_attr)
{
    static ot_u32 ref_exp_ratio[OT_EIS_VI_MAX_PIPE_NUM][OT_SCENE_REF_EXP_RATIO_FRM] = {0};
    static ot_u32 actual_exp_ratio[OT_EIS_VI_MAX_PIPE_NUM][OT_SCENE_REF_EXP_RATIO_FRM] = {0};
    ot_u32 cur_ref_exp_ratio = 0;
    ot_u32 cur_actual_exp_ratio = 0;
    ot_u32 ref_ratio_level;
    ot_u32 ref_exp_ratio_alpha;
    ot_eis_vi_exp_info isp_exp_info;
    ot_u32 i;
    ot_eis_scene_dynamic_drc_coef dynamic_drc_coef = {0};
    ot_s32 ret;
    ot_vi_pipe_id pipe_id[OT_EIS_VI_PIPE_ID_NUM_MAX] = { 0 };
    ot_vi_pipe_id master_pipe_id = 0;
    ot_s32 pipe_num = 0;

    ot_scenecomm_expr_true_return(index >= OT_SCENE_PIPETYPE_NUM, OT_FAILURE);
    isp_drc_attr->enable = g_scene_pipe_param[index].dynamic_drc.enable;

    ret = ot_eis_vi_get_pipe_id_by_pipe_hdl(pipe_hdl, pipe_id, &pipe_num);
    master_pipe_id = pipe_id[0];
    ot_scenecomm_check_return(ret, ret);

    ret = ot_eis_vi_ae_query_info(pipe_hdl, &isp_exp_info);
    ot_scenecomm_check_return(ret, ret);

    for (i = 0; i < OT_SCENE_REF_EXP_RATIO_FRM - 1; i++) {
        ref_exp_ratio[master_pipe_id][i] = ref_exp_ratio[master_pipe_id][i + 1];
        actual_exp_ratio[master_pipe_id][i] = actual_exp_ratio[master_pipe_id][i + 1];
    }
    ref_exp_ratio[master_pipe_id][i] = isp_exp_info.ref_exp_ratio;
    actual_exp_ratio[master_pipe_id][i] = inner_state_info->wdr_exp_ratio_actual[0];

    for (i = 0; i < OT_SCENE_REF_EXP_RATIO_FRM; i++) {
        cur_ref_exp_ratio += ref_exp_ratio[master_pipe_id][i];
        cur_actual_exp_ratio += actual_exp_ratio[master_pipe_id][i];
    }

    cur_ref_exp_ratio = cur_ref_exp_ratio / OT_SCENE_REF_EXP_RATIO_FRM;
    cur_actual_exp_ratio = cur_actual_exp_ratio  / OT_SCENE_REF_EXP_RATIO_FRM;

    cur_ref_exp_ratio = cur_actual_exp_ratio;

    ref_ratio_level = scene_get_level_ltoh_u32(cur_ref_exp_ratio, g_scene_pipe_param[index].dynamic_drc.ref_ratio_count,
        g_scene_pipe_param[index].dynamic_drc.ref_ratio_ltoh);

    ot_scenecomm_expr_true_return(ref_ratio_level >= OT_EIS_SCENE_DRC_REF_RATIO_MAX_COUNT, OT_FAILURE);
    if ((ref_ratio_level == 0) || (ref_ratio_level == (g_scene_pipe_param[index].dynamic_drc.ref_ratio_count - 1))) {
        ref_exp_ratio_alpha = g_scene_pipe_param[index].dynamic_drc.ref_ratio_alpha[ref_ratio_level];
    } else {
        ref_exp_ratio_alpha = scene_interpulate(cur_ref_exp_ratio,
            g_scene_pipe_param[index].dynamic_drc.ref_ratio_ltoh[ref_ratio_level - 1],
            g_scene_pipe_param[index].dynamic_drc.ref_ratio_alpha[ref_ratio_level - 1],
            g_scene_pipe_param[index].dynamic_drc.ref_ratio_ltoh[ref_ratio_level],
            g_scene_pipe_param[index].dynamic_drc.ref_ratio_alpha[ref_ratio_level]);
    }

    dynamic_drc_coef.ref_exp_ratio = cur_ref_exp_ratio;
    dynamic_drc_coef.ref_ratio_alpha = ref_exp_ratio_alpha;
    dynamic_drc_coef.k = k;
    ret = scene_blend_tone_mapping(pipe_hdl, index, &dynamic_drc_coef, isp_drc_attr);
    ot_scenecomm_check_return(ret, ret);

    return OT_SUCCESS;
}

static ot_void scene_set_wdr_cfg(ot_u8 index, ot_u32 iso_level, ot_eis_vi_nr_attr *nr_attr,
    const ot_scene_nr_para *nr_para)
{
    ot_s32 i;

    for (i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
        nr_attr->wdr_cfg.snr_sfm0_wdr_strength[i] =
            g_scene_pipe_param[index].dynamic_nr.snr_sfm0_wdr_frame_str[i][iso_level] +
            nr_para->sfm0_wdr_incr[i];
        nr_attr->wdr_cfg.snr_sfm0_fusion_strength[i] =
            g_scene_pipe_param[index].dynamic_nr.snr_sfm0_fusion_frame_str[i][iso_level] +
            nr_para->sfm0_fusion_incr[i];

        nr_attr->wdr_cfg.md_wdr_strength[i] = g_scene_pipe_param[index].dynamic_nr.md_wdr_frame_str[i][iso_level] +
            nr_para->md_wdr_incr[i];
        nr_attr->wdr_cfg.md_fusion_strength[i] =
            g_scene_pipe_param[index].dynamic_nr.md_fusion_frame_str[i][iso_level] +
            nr_para->md_fusion_incr[i];

        nr_attr->wdr_cfg.snr_fusion_sfm6_strength[i] =
            g_scene_pipe_param[index].dynamic_nr.snr_fusion_sfm6_strength[i][iso_level] +
            nr_para->snr_fusion_sfm6_sfm7_incr[i];

        nr_attr->wdr_cfg.snr_fusion_sfm7_strength[i] =
            g_scene_pipe_param[index].dynamic_nr.snr_fusion_sfm7_strength[i][iso_level] +
            nr_para->snr_fusion_sfm6_sfm7_incr[i];

        nr_attr->wdr_cfg.snr_wdr_sfm6_strength[i] =
            g_scene_pipe_param[index].dynamic_nr.snr_wdr_sfm6_strength[i][iso_level] +
            nr_para->snr_wdr_sfm6_sfm7_incr[i];

        nr_attr->wdr_cfg.snr_wdr_sfm7_strength[i] =
            g_scene_pipe_param[index].dynamic_nr.snr_wdr_sfm7_strength[i][iso_level] +
            nr_para->snr_wdr_sfm6_sfm7_incr[i];
    }
}

static ot_void scene_set_nr_part1(ot_u8 index, ot_u32 iso, ot_eis_vi_nr_attr *nr_attr, const ot_scene_nr_para *nr_para)
{
    ot_s32 i;
    ot_u32 count = g_scene_pipe_param[index].dynamic_nr.coring_ratio_count;
    ot_u32 *pthresh = (ot_u32 *)g_scene_pipe_param[index].dynamic_nr.coring_ratio_iso;
    ot_u32 iso_level = scene_get_level_ltoh_u32(iso, count, pthresh);

    ot_u32 mid_iso = (ot_u32)iso;
    ot_u32 left_iso = pthresh[iso_level - 1];
    ot_u32 right_iso = pthresh[iso_level];

    for (i = 0; i < OT_EIS_VI_BAYERNR_LUT_LENGTH; i++) {
        nr_attr->coring_ratio[i] = scene_interpulate(mid_iso, left_iso,
            g_scene_pipe_param[index].dynamic_nr.coring_ratio[i][iso_level - 1], right_iso,
            g_scene_pipe_param[index].dynamic_nr.coring_ratio[i][iso_level]);
    }

    for (i = 0; i < OT_EIS_VI_WDR_MAX_FRAME_NUM; i++) {
        nr_attr->wdr_cfg.snr_sfm0_wdr_strength[i] = scene_interpulate(mid_iso, left_iso,
            g_scene_pipe_param[index].dynamic_nr.snr_sfm0_wdr_frame_str[i][iso_level - 1], right_iso,
            g_scene_pipe_param[index].dynamic_nr.snr_sfm0_wdr_frame_str[i][iso_level]) +
            nr_para->sfm0_wdr_incr[i];
        nr_attr->wdr_cfg.snr_sfm0_fusion_strength[i] = scene_interpulate(mid_iso, left_iso,
            g_scene_pipe_param[index].dynamic_nr.snr_sfm0_fusion_frame_str[i][iso_level - 1], right_iso,
            g_scene_pipe_param[index].dynamic_nr.snr_sfm0_fusion_frame_str[i][iso_level]) +
            nr_para->sfm0_fusion_incr[i];

        nr_attr->wdr_cfg.md_wdr_strength[i] = scene_interpulate(mid_iso, left_iso,
            g_scene_pipe_param[index].dynamic_nr.md_wdr_frame_str[i][iso_level - 1], right_iso,
            g_scene_pipe_param[index].dynamic_nr.md_wdr_frame_str[i][iso_level]) +
            nr_para->md_wdr_incr[i];
        nr_attr->wdr_cfg.md_fusion_strength[i] = scene_interpulate(mid_iso, left_iso,
            g_scene_pipe_param[index].dynamic_nr.md_fusion_frame_str[i][iso_level - 1], right_iso,
            g_scene_pipe_param[index].dynamic_nr.md_fusion_frame_str[i][iso_level]) +
            nr_para->md_fusion_incr[i];

        nr_attr->wdr_cfg.snr_fusion_sfm6_strength[i] = scene_interpulate(mid_iso, left_iso,
            g_scene_pipe_param[index].dynamic_nr.snr_fusion_sfm6_strength[i][iso_level - 1], right_iso,
            g_scene_pipe_param[index].dynamic_nr.snr_fusion_sfm6_strength[i][iso_level]) +
            nr_para->snr_fusion_sfm6_sfm7_incr[i];

        nr_attr->wdr_cfg.snr_fusion_sfm7_strength[i] = scene_interpulate(mid_iso, left_iso,
            g_scene_pipe_param[index].dynamic_nr.snr_fusion_sfm7_strength[i][iso_level - 1], right_iso,
            g_scene_pipe_param[index].dynamic_nr.snr_fusion_sfm7_strength[i][iso_level]) +
            nr_para->snr_fusion_sfm6_sfm7_incr[i];

        nr_attr->wdr_cfg.snr_wdr_sfm6_strength[i] = scene_interpulate(mid_iso, left_iso,
            g_scene_pipe_param[index].dynamic_nr.snr_wdr_sfm6_strength[i][iso_level - 1], right_iso,
            g_scene_pipe_param[index].dynamic_nr.snr_wdr_sfm6_strength[i][iso_level]) +
            nr_para->snr_wdr_sfm6_sfm7_incr[i];

        nr_attr->wdr_cfg.snr_wdr_sfm7_strength[i] = scene_interpulate(mid_iso, left_iso,
            g_scene_pipe_param[index].dynamic_nr.snr_wdr_sfm7_strength[i][iso_level - 1], right_iso,
            g_scene_pipe_param[index].dynamic_nr.snr_wdr_sfm7_strength[i][iso_level]) +
            nr_para->snr_wdr_sfm6_sfm7_incr[i];
    }
}

ot_s32 scene_set_nr_attr_para(ot_u8 index, ot_u32 iso, ot_eis_vi_nr_attr *nr_attr, const ot_scene_nr_para *nr_para)
{
    ot_s32 i;
    ot_u32 count = g_scene_pipe_param[index].dynamic_nr.coring_ratio_count;
    ot_u32 *pthresh = (ot_u32 *)g_scene_pipe_param[index].dynamic_nr.coring_ratio_iso;
    ot_u32 iso_level = scene_get_level_ltoh_u32(iso, count, pthresh);
    ot_scenecomm_expr_true_return(iso_level >= OT_EIS_SCENE_ISO_STRENGTH_NUM, OT_FAILURE);

    if ((iso_level == (count - 1)) || (iso_level == 0)) {
        for (i = 0; i < OT_EIS_VI_BAYERNR_LUT_LENGTH; i++) {
            nr_attr->coring_ratio[i] = g_scene_pipe_param[index].dynamic_nr.coring_ratio[i][iso_level];
        }
        scene_set_wdr_cfg(index, iso_level, nr_attr, nr_para);
    } else {
        scene_set_nr_part1(index, iso, nr_attr, nr_para);
    }

    return OT_SUCCESS;
}

ot_s32 scene_set_nr_wdr_ratio_para(ot_vi_pipe_handle pipe_hdl, ot_u8 index, ot_u32 wdr_ratio,
    ot_u32 ratio_index, ot_eis_vi_nr_attr *nr_attr)
{
    ot_u32 i;
    ot_s32 ret;
    ot_eis_vi_public_attr pub_attr;
    ot_u32 coring_count = get_pipe_params()[index].dynamic_nr.coring_ratio_count;
    ot_u32 ratio_count = get_pipe_params()[index].dynamic_nr.ratio_count;
    ot_u32 *level = get_pipe_params()[index].dynamic_nr.ratio_level;

    ret = ot_eis_vi_public_get_attr(pipe_hdl, &pub_attr);
    ot_scenecomm_check_return(ret, ret);

    if (pub_attr.wdr_mode == OT_EIS_WDR_MODE_NONE) {
        return OT_SUCCESS;
    }

    if ((ratio_index == 0) || (ratio_index == (ratio_count - 1))) {
        for (i = 0; i < OT_EIS_VI_BAYERNR_LUT_LENGTH1; i++) {
            nr_attr->mix_gain[i] = get_pipe_params()[index].dynamic_nr.mix_gain[i][ratio_index];
        }
    } else {
        for (i = 0; i < OT_EIS_VI_BAYERNR_LUT_LENGTH1; i++) {
            nr_attr->mix_gain[i] = scene_interpulate(wdr_ratio, level[ratio_index - 1],
                get_pipe_params()[index].dynamic_nr.mix_gain[i][ratio_index - 1],
                level[ratio_index], get_pipe_params()[index].dynamic_nr.mix_gain[i][ratio_index]);
        }
    }

    if (wdr_ratio > get_pipe_params()[index].dynamic_nr.wdr_ratio_threshold) {
        for (i = 0; i < OT_EIS_VI_AUTO_ISO_NUM; i++) {
            nr_attr->snr_cfg.snr_attr.snr_auto.coring_wgt[i] = get_pipe_params()[index].dynamic_nr.coring_wgt_h[i];
            nr_attr->snr_cfg.snr_attr.snr_auto.fine_strength[i] =
                get_pipe_params()[index].dynamic_nr.fine_strength_h[i];
        }
        for (i = 0; i < OT_EIS_VI_BAYERNR_LUT_LENGTH; i++) {
            nr_attr->coring_ratio[i] = get_pipe_params()[index].dynamic_nr.coring_ratio[i][coring_count - 1];
        }
    } else {
        for (i = 0; i < OT_EIS_VI_AUTO_ISO_NUM; i++) {
            nr_attr->snr_cfg.snr_attr.snr_auto.coring_wgt[i] = get_pipe_params()[index].dynamic_nr.coring_wgt_l[i];
            nr_attr->snr_cfg.snr_attr.snr_auto.fine_strength[i] =
                get_pipe_params()[index].dynamic_nr.fine_strength_l[i];
        }
    }

    return OT_SUCCESS;
}

#ifdef SET_3DNR

static ot_void scene_set_nry_en(ot_eis_nr_cfg *nr_param, const ot_scene_3dnr *_3dnr)
{
    nr_param->nry0_en = _3dnr->nry0_en;
    nr_param->nry1_en = _3dnr->nry1_en;
    nr_param->nry2_en = _3dnr->nry2_en;
    nr_param->nry3_en = _3dnr->nry3_en;
}

static ot_void scene_set_nry_iey(ot_eis_nr_cfg *nr_param, const ot_scene_3dnr *_3dnr)
{
    /* ie_y */
    for (ot_u32 i = 1; i < scene_array_size(_3dnr->iey); i++) {
        nr_param->iey[i].ies0 = _3dnr->iey[i].ies0;
        nr_param->iey[i].ies1 = _3dnr->iey[i].ies1;
        nr_param->iey[i].ies2 = _3dnr->iey[i].ies2;
        nr_param->iey[i].ies3 = _3dnr->iey[i].ies3;
    }

    for (ot_u32 i = 3; i <= 4; i++) { /* 4  is index */
        nr_param->iey[i].o_sht = _3dnr->iey[i].o_sht;
        nr_param->iey[i].u_sht = _3dnr->iey[i].u_sht;
    }
}

static ot_void scene_set_nry_sfy_1to4(ot_eis_nr_cfg *nr_param, const ot_scene_3dnr *_3dnr)
{
    /* sf_y */
    for (ot_u32 i = 1; i < 4; i++) { /* array sfy have 4 valid element */
        /* sfs1 */
        nr_param->sfy[i].sfs1 = _3dnr->sfy[i].sfs1;
        nr_param->sfy[i].sbr1 = _3dnr->sfy[i].sbr1;

        /* sfs2 */
        nr_param->sfy[i].sfs2 = _3dnr->sfy[i].sfs2;
        nr_param->sfy[i].sft2 = _3dnr->sfy[i].sft2;
        nr_param->sfy[i].sbr2 = _3dnr->sfy[i].sbr2;

        /* sfs4 */
        nr_param->sfy[i].sfs4 = _3dnr->sfy[i].sfs4;
        nr_param->sfy[i].sft4 = _3dnr->sfy[i].sft4;
        nr_param->sfy[i].sbr4 = _3dnr->sfy[i].sbr4;
    }

    /* sfs4 */
    nr_param->sfy[4].sfs2 = _3dnr->sfy[4].sfs2; /* 4  is index */
    nr_param->sfy[4].sft2 = _3dnr->sfy[4].sft2; /* 4  is index */
    nr_param->sfy[4].sbr2 = _3dnr->sfy[4].sbr2; /* 4  is index */

    for (ot_u32 i = 0; i < OT_SCENE_3DNR_IDX_LEN; i++) { /*  3 sfy index */
        /* 3 index */
        nr_param->sfy[3].bri_str[i] = _3dnr->sfy[3].bri_str[i];
    }

    for (ot_u32 i = 0; i < 33; i++) { /*  33 sfy index */
        /* 3 index */
        nr_param->sfy[3].sbs_k[i] = _3dnr->sfy[3].sbs_k[i]; /* 3 is index */
        nr_param->sfy[3].sds_k[i] = _3dnr->sfy[3].sds_k[i]; /* 3 is index */
        /* 2 index */
        nr_param->sfy[2].sbs_k[i] = _3dnr->sfy[2].sbs_k[i]; /* 2 is index */
        nr_param->sfy[2].sds_k[i] = _3dnr->sfy[2].sds_k[i]; /* 2 is index */
    }
}

static ot_void scene_set_nry_sfy_6to8(ot_eis_nr_cfg *nr_param, const ot_scene_3dnr *_3dnr)
{
    for (ot_u32 i = 1; i < scene_array_size(_3dnr->sfy); i++) {
        /* sfs7 */
        nr_param->sfy[i].spn = _3dnr->sfy[i].spn;
        nr_param->sfy[i].sbn = _3dnr->sfy[i].sbn;
        nr_param->sfy[i].pbr = _3dnr->sfy[i].pbr;
        nr_param->sfy[i].j_mode = _3dnr->sfy[i].j_mode;

        /* sfr6 */
        for (ot_u32 j = 0; j < scene_array_size(_3dnr->sfy[i].sfr6); j++) {
            nr_param->sfy[i].sfr6[j] = _3dnr->sfy[i].sfr6[j];
        }

        /* sbr6 */
        nr_param->sfy[i].sbr6[0] = _3dnr->sfy[i].sbr6[0];
        nr_param->sfy[i].sbr6[1] = _3dnr->sfy[i].sbr6[1];

        /* sfn */
        nr_param->sfy[i].sfn0_0 = _3dnr->sfy[i].sfn0_0;
        nr_param->sfy[i].sfn1_0 = _3dnr->sfy[i].sfn1_0;
        nr_param->sfy[i].sfn2_0 = _3dnr->sfy[i].sfn2_0;
        nr_param->sfy[i].sfn3_0 = _3dnr->sfy[i].sfn3_0;

        /* sth */
        nr_param->sfy[i].sth1_0 = _3dnr->sfy[i].sth1_0;
        nr_param->sfy[i].sth2_0 = _3dnr->sfy[i].sth2_0;
        nr_param->sfy[i].sth3_0 = _3dnr->sfy[i].sth3_0;
    }

    for (ot_u32 i = 1; i <= 2; i++) { /* 2 is index */
        nr_param->sfy[i].sfn0_1 = _3dnr->sfy[i].sfn0_1;
        nr_param->sfy[i].sfn1_1 = _3dnr->sfy[i].sfn1_1;
        nr_param->sfy[i].sfn2_1 = _3dnr->sfy[i].sfn2_1;
        nr_param->sfy[i].sfn3_1 = _3dnr->sfy[i].sfn3_1;

        nr_param->sfy[i].sth1_1 = _3dnr->sfy[i].sth1_1;
        nr_param->sfy[i].sth2_1 = _3dnr->sfy[i].sth2_1;
        nr_param->sfy[i].sth3_1 = _3dnr->sfy[i].sth3_1;
    }

    nr_param->sfy[3].bri_idx0 = _3dnr->sfy[3].bri_idx0; /* 3 is index */
    nr_param->sfy[3].bri_idx1 = _3dnr->sfy[3].bri_idx1; /* 3 is index */
    nr_param->sfy[2].k_mode  = _3dnr->sfy[2].k_mode; /* 2 is index */
    nr_param->sfy[3].k_mode  = _3dnr->sfy[3].k_mode; /* 3 is index */
}


static ot_void scene_set_nry_tfy(ot_eis_nr_cfg *nr_param, const ot_scene_3dnr *_3dnr)
{
    /* ref */
    nr_param->tfy[0].ref_en = _3dnr->tfy[0].ref_en;
    nr_param->tfy[1].ref_en = _3dnr->tfy[1].ref_en;

    nr_param->tfy[0].math_mode = _3dnr->tfy[0].math_mode;
    nr_param->tfy[0].auto_math = _3dnr->tfy[0].auto_math;
    nr_param->tfy[0].tfs0      = _3dnr->tfy[0].tfs0;

    /* tf_y */
    for (ot_u32 i = 1; i < scene_array_size(_3dnr->tfy); i++) {
        /* tss */
        nr_param->tfy[i].tss0 = _3dnr->tfy[i].tss0;
        nr_param->tfy[i].tss1 = _3dnr->tfy[i].tss1;

        /* tfs */
        nr_param->tfy[i].tfs0 = _3dnr->tfy[i].tfs0;
        nr_param->tfy[i].tfs1 = _3dnr->tfy[i].tfs1;

        /* tfr0 */
        for (ot_u32 j = 0; j < scene_array_size(_3dnr->tfy[i].tfr0); j++) {
            nr_param->tfy[i].tfr0[j] = _3dnr->tfy[i].tfr0[j];
             /* tfr1 */
            nr_param->tfy[1].tfr1[j] = _3dnr->tfy[1].tfr1[j];
            nr_param->tfy[2].tfr1[j] = _3dnr->tfy[2].tfr1[j]; /* 2 is index */
        }
    }
}

static ot_void scene_set_nry_mdy(ot_eis_nr_cfg *nr_param, const ot_scene_3dnr *_3dnr)
{
    /* md_y */
    for (ot_u32 i = 0; i < scene_array_size(_3dnr->mdy); i++) {
        /* math */
        nr_param->mdy[i].math0 = _3dnr->mdy[i].math0;
        nr_param->mdy[i].math1 = _3dnr->mdy[i].math1;

        /* mate */
        nr_param->mdy[i].mate0 = _3dnr->mdy[i].mate0;
        nr_param->mdy[i].mate1 = _3dnr->mdy[i].mate1;

        /* mabw */
        nr_param->mdy[i].mabw0 = _3dnr->mdy[i].mabw0;
        nr_param->mdy[i].mabw1 = _3dnr->mdy[i].mabw1;

        /* adv_math */
        nr_param->mdy[i].adv_math = _3dnr->mdy[i].adv_math;

        /* mcth */
        nr_param->mdy[0].mcth = _3dnr->mdy[0].mcth;

        /* dci_w */
        nr_param->mdy[0].dci_w = _3dnr->mdy[0].dci_w;
    }
}

static ot_void scene_set_nry_nc0(ot_eis_nr_cfg *nr_param, const ot_scene_3dnr *_3dnr)
{
    /* nrc0_en */
    nr_param->nrc0_en = _3dnr->nrc0_en;

    /* sfc_enhance */
    nr_param->nrc0.sfc_enhance = _3dnr->nrc0.sfc_enhance;

    /* sfc_ext */
    nr_param->nrc0.sfc_ext = _3dnr->nrc0.sfc_ext;

    /* trc */
    nr_param->nrc0.trc = _3dnr->nrc0.trc;

    /* sfc */
    nr_param->nrc0.sfc = _3dnr->nrc0.sfc;

    /* tfc */
    nr_param->nrc0.tfc = _3dnr->nrc0.tfc;

    /* tpc */
    nr_param->nrc0.tpc = _3dnr->nrc0.tpc;

    /* pre_sfc */
    nr_param->nrc0.pre_sfc = _3dnr->nrc0.pre_sfc;
}


static ot_void scene_set_nry_nc1(ot_eis_nr_cfg *nr_param, const ot_scene_3dnr *_3dnr)
{
    /* nrc1_en */
    nr_param->nrc1_en = _3dnr->nrc1_en;

    /* sfs2 */
    nr_param->nrc1.sfs2 = _3dnr->nrc1.sfs2;
    nr_param->nrc1.sft2 = _3dnr->nrc1.sft2;
    nr_param->nrc1.sbr2 = _3dnr->nrc1.sbr2;
    nr_param->nrc1.pbr0 = _3dnr->nrc1.pbr0;
    nr_param->nrc1.pbr1 = _3dnr->nrc1.pbr1;
}
#endif

ot_s32 scene_set_3dnr(ot_eis_handle vi_pipe_hdl, ot_eis_handle vproc_hdl, const ot_scene_3dnr *_3dnr, ot_u8 index, ot_eis_3dnr_pos pos_3dnr)
{
#ifdef SET_3DNR
    ot_s32 ret;
    ot_eis_3dnr_attr nrx_attr, nrx_get_attr;

    ot_u32 iso[16];
    ot_eis_nr_cfg nr_param[16];

    nrx_attr.param.nr_cfg.nr_auto.iso = iso;
    nrx_attr.param.nr_cfg.nr_auto.nr_param = nr_param;

    nrx_get_attr.param.nr_cfg.nr_auto.iso = iso;
    nrx_get_attr.param.nr_cfg.nr_auto.nr_param = nr_param;

    if (pos_3dnr == OT_EIS_3DNR_POS_VI) {
        ret = ot_eis_vi_pipe_get_3dnr_attr(vi_pipe_hdl,  &nrx_attr);
    } else {
        nrx_attr.param.nr_cfg.nr_mode = OT_EIS_OP_MODE_MANUAL;
        ret = ot_eis_vproc_grp_get_3dnr(vproc_hdl,  &nrx_attr);
    }

    ot_scenecomm_check_return(ret, ret);

    //nrx_attr.enable = get_pipe_params()[index].static_threednr.enable;
    scene_set_nry_en(&nrx_attr.param.nr_cfg.nr_manual.nr_param, _3dnr);
    scene_set_nry_iey(&nrx_attr.param.nr_cfg.nr_manual.nr_param, _3dnr);
    scene_set_nry_sfy_1to4(&nrx_attr.param.nr_cfg.nr_manual.nr_param, _3dnr);
    scene_set_nry_sfy_6to8(&nrx_attr.param.nr_cfg.nr_manual.nr_param, _3dnr);

    scene_set_nry_tfy(&nrx_attr.param.nr_cfg.nr_manual.nr_param, _3dnr);
    scene_set_nry_mdy(&nrx_attr.param.nr_cfg.nr_manual.nr_param, _3dnr);

    scene_set_nry_nc0(&nrx_attr.param.nr_cfg.nr_manual.nr_param, _3dnr);
    scene_set_nry_nc1(&nrx_attr.param.nr_cfg.nr_manual.nr_param, _3dnr);

    nrx_attr.param.nr_cfg.nr_mode = OT_EIS_OP_MODE_MANUAL;
    if (pos_3dnr == OT_EIS_3DNR_POS_VI) {
        ret = ot_eis_vi_pipe_set_3dnr_attr(vi_pipe_hdl,  &nrx_attr);
    } else {
        ret = ot_eis_vproc_grp_set_3dnr(vproc_hdl,  &nrx_attr);
    }

    ot_scenecomm_check_return(ret, ret);

    nrx_get_attr.param.nr_cfg.nr_mode = OT_EIS_OP_MODE_MANUAL;
    if (pos_3dnr == OT_EIS_3DNR_POS_VI) {
        ret = ot_eis_vi_pipe_get_3dnr_attr(vi_pipe_hdl,  &nrx_get_attr);
    } else {
        ret = ot_eis_vproc_grp_get_3dnr(vproc_hdl,  &nrx_get_attr);
    }

    ot_scenecomm_check_return(ret, ret);

    static ot_u32 count[OT_EIS_VI_MAX_PIPE_NUM] = {0};
    if ((g_scene_pipe_param[index].module_state.debug == 1) && ((count[index] % 15) == 0)) { /* 15 max count */
        scene_logd("2======================nrx_get_attr ================================");
        print_nrx(_3dnr);
    }
    count[index]++;
#endif
    return OT_SUCCESS;
}

ot_void scene_set_3dnr_nrx_nry(const _3dnr_nrx_pack *pack)
{
    scene_set_nrx_attr_interpulate(pack, nry0_en);
    scene_set_nrx_attr_interpulate(pack, nry1_en);
    scene_set_nrx_attr_interpulate(pack, nry2_en);
    scene_set_nrx_attr_interpulate(pack, nry3_en);
    scene_set_nrx_attr_interpulate(pack, adv_mode);
}

ot_void scene_set_3dnr_nrx_iey(const _3dnr_nrx_pack *pack)
{
    /* ie_y */
    for (ot_u32 i = 1; i < 5; i++) { /* 5 iey array size */
        scene_set_nrx_attr_interpulate(pack, iey[i].ies0);
        scene_set_nrx_attr_interpulate(pack, iey[i].ies1);
        scene_set_nrx_attr_interpulate(pack, iey[i].ies2);
        scene_set_nrx_attr_interpulate(pack, iey[i].ies3);
    }
}

static ot_void scene_set_nrx_sfy(const _3dnr_nrx_pack *pack)
{
    /* sf_y */
    for (ot_u32 i = 1; i < 4; i++) { /* 4 sfy index */
        /* sfs1 */
        scene_set_nrx_attr_interpulate(pack, sfy[i].sfs1);
        scene_set_nrx_attr_interpulate(pack, sfy[i].sbr1);

        /* sfs2 */
        scene_set_nrx_attr_interpulate(pack, sfy[i].sfs2);
        scene_set_nrx_attr_interpulate(pack, sfy[i].sft2);
        scene_set_nrx_attr_interpulate(pack, sfy[i].sbr2);

        /* sfs4 */
        scene_set_nrx_attr_interpulate(pack, sfy[i].sfs4);
        scene_set_nrx_attr_interpulate(pack, sfy[i].sft4);
        scene_set_nrx_attr_interpulate(pack, sfy[i].sbr4);
    }
    scene_set_nrx_attr_interpulate(pack, sfy[4].sfs2); /* 4  is index */
    scene_set_nrx_attr_interpulate(pack, sfy[4].sft2); /* 4  is index */

    scene_set_nrx_attr_interpulate(pack, sfy[2].k_mode); /* 2 is index */
    scene_set_nrx_attr_interpulate(pack, sfy[3].k_mode); /* 3 is index */
}

static ot_void set_sfs7_param(const _3dnr_nrx_pack *pack)
{
    ot_scene_3dnr *nrx_attr = pack->nrx_attr;
    const ot_scene_static_3dnr *nrx_param = pack->nrx_param;
    ot_u32 iso_level = pack->iso_level;

    for (ot_u32 i = 1; i < scene_array_size(nrx_attr->sfy); i++) {
        /* sfs7 */
        nrx_attr->sfy[i].spn = nrx_param->threednr_value[iso_level - 1].sfy[i].spn;
        nrx_attr->sfy[i].sbn = nrx_param->threednr_value[iso_level - 1].sfy[i].sbn;
        nrx_attr->sfy[i].pbr = nrx_param->threednr_value[iso_level - 1].sfy[i].pbr;
        nrx_attr->sfy[i].j_mode = nrx_param->threednr_value[iso_level - 1].sfy[i].j_mode;

        /* sfr6 */
        for (ot_u32 j = 0; j < scene_array_size(nrx_attr->sfy[i].sfr6); j++) {
            scene_set_nrx_attr_interpulate(pack, sfy[i].sfr6[j]);
        }

        /* sbr6 */
        scene_set_nrx_attr_interpulate(pack, sfy[i].sbr6[0]);
        scene_set_nrx_attr_interpulate(pack, sfy[i].sbr6[1]);

        /* sfn */
        nrx_attr->sfy[i].sfn0_0 = nrx_param->threednr_value[iso_level - 1].sfy[i].sfn0_0;
        nrx_attr->sfy[i].sfn1_0 = nrx_param->threednr_value[iso_level - 1].sfy[i].sfn1_0;
        nrx_attr->sfy[i].sfn2_0 = nrx_param->threednr_value[iso_level - 1].sfy[i].sfn2_0;
        nrx_attr->sfy[i].sfn3_0 = nrx_param->threednr_value[iso_level - 1].sfy[i].sfn3_0;

        scene_set_nrx_attr_interpulate(pack, sfy[i].sth1_0);
        scene_set_nrx_attr_interpulate(pack, sfy[i].sth2_0);
        scene_set_nrx_attr_interpulate(pack, sfy[i].sth3_0);
    }

    for (ot_u32 i = 1; i <= 2; i++) { /* 2 is index */
        /* sth */
        scene_set_nrx_attr_interpulate(pack, sfy[i].sth1_1);
        scene_set_nrx_attr_interpulate(pack, sfy[i].sth2_1);
        scene_set_nrx_attr_interpulate(pack, sfy[i].sth3_1);

        nrx_attr->sfy[i].sfn0_1 = nrx_param->threednr_value[iso_level - 1].sfy[i].sfn0_1;
        nrx_attr->sfy[i].sfn1_1 = nrx_param->threednr_value[iso_level - 1].sfy[i].sfn1_1;
        nrx_attr->sfy[i].sfn2_1 = nrx_param->threednr_value[iso_level - 1].sfy[i].sfn2_1;
        nrx_attr->sfy[i].sfn3_1 = nrx_param->threednr_value[iso_level - 1].sfy[i].sfn3_1;
    }
}

static ot_void scene_set_nrx_sfy_bri(const _3dnr_nrx_pack *pack)
{
    scene_set_nrx_attr_interpulate(pack, sfy[3].bri_idx0); /* 3 is index */
    scene_set_nrx_attr_interpulate(pack, sfy[3].bri_idx1); /* 3 is index */

    for (ot_u32 i = 0; i < OT_SCENE_3DNR_IDX_LEN; i++) {
        scene_set_nrx_attr_interpulate(pack, sfy[3].bri_str[i]); /* 3 index */
    }

    for (ot_u32 i = 0; i < 33; i++) { /* 33 is index */
        scene_set_nrx_attr_interpulate(pack, sfy[2].sbs_k[i]); /* 2 index */
        scene_set_nrx_attr_interpulate(pack, sfy[2].sds_k[i]); /* 2 index */
        scene_set_nrx_attr_interpulate(pack, sfy[3].sbs_k[i]); /* 3 index */
        scene_set_nrx_attr_interpulate(pack, sfy[3].sds_k[i]); /* 3 index */
    }
}

ot_void scene_set_3dnr_nrx_sfy(const _3dnr_nrx_pack *pack)
{
    /* sf_y */
    scene_set_nrx_sfy(pack);

    set_sfs7_param(pack);
    /* bri */
    scene_set_nrx_sfy_bri(pack);
}

ot_void scene_set_3dnr_nrx_tfy(const _3dnr_nrx_pack *pack, ot_eis_3dnr_pos pos_3dnr)
{
    ot_scene_3dnr *nrx_attr = pack->nrx_attr;

    /* ref */
    scene_set_nrx_attr_interpulate(pack, tfy[0].ref_en);
    scene_set_nrx_attr_interpulate(pack, tfy[1].ref_en);

    scene_set_nrx_attr_interpulate(pack, tfy[0].math_mode);
    scene_set_nrx_attr_interpulate(pack, tfy[0].auto_math);

    if (pos_3dnr == OT_EIS_3DNR_POS_VI) {
        scene_set_nrx_attr_interpulate(pack, tfy[0].tfs0);
    } else {
        nrx_attr->tfy[0].tfs0 = 0;
    }
    /* tf_y */
    for (ot_u32 i = 1; i < scene_array_size(nrx_attr->tfy); i++) {
        /* tss */
        scene_set_nrx_attr_interpulate(pack, tfy[i].tss0);
        scene_set_nrx_attr_interpulate(pack, tfy[i].tss1);

        /* tfs */
        scene_set_nrx_attr_interpulate(pack, tfy[i].tfs0);
        scene_set_nrx_attr_interpulate(pack, tfy[i].tfs1);

        /* tfr0 */
        for (ot_u32 j = 0; j < scene_array_size(nrx_attr->tfy[i].tfr0); j++) {
            scene_set_nrx_attr_interpulate(pack, tfy[i].tfr0[j]);
        }

        /* tfr1 */
        for (ot_u32 j = 0; j < scene_array_size(nrx_attr->tfy[1].tfr1); j++) { /* tfr1 array size 6 */
            scene_set_nrx_attr_interpulate(pack, tfy[1].tfr1[j]);
            scene_set_nrx_attr_interpulate(pack, tfy[2].tfr1[j]); /* 2 is index */
        }
    }
}

ot_void scene_set_3dnr_nrx_mdy(const _3dnr_nrx_pack *pack)
{
    /* md_y */
    for (ot_u32 i = 0; i < scene_array_size(pack->nrx_attr->mdy); i++) {
        /* math */
        scene_set_nrx_attr_interpulate(pack, mdy[i].math0);
        scene_set_nrx_attr_interpulate(pack, mdy[i].math1);

        /* mate */
        scene_set_nrx_attr_interpulate(pack, mdy[i].mate0);
        scene_set_nrx_attr_interpulate(pack, mdy[i].mate1);

        /* mabw */
        scene_set_nrx_attr_interpulate(pack, mdy[i].mabw0);
        scene_set_nrx_attr_interpulate(pack, mdy[i].mabw1);

        scene_set_nrx_attr_interpulate(pack, mdy[i].adv_math);
        /* mcth */
        scene_set_nrx_attr_interpulate(pack, mdy[0].mcth);

        /* dci_w */
        scene_set_nrx_attr_interpulate(pack, mdy[0].dci_w);
    }
}

ot_void scene_set_3dnr_nrx_nrc0(const _3dnr_nrx_pack *pack)
{
    /* nrc0_en */
    scene_set_nrx_attr_interpulate(pack, nrc0_en);
    /* sfc */
    scene_set_nrx_attr_interpulate(pack, nrc0.sfc);
    /* pre_sfc */
    scene_set_nrx_attr_interpulate(pack, nrc0.pre_sfc);
    /* sfc_enhance */
    scene_set_nrx_attr_interpulate(pack, nrc0.sfc_enhance);
    /* sfc_ext */
    scene_set_nrx_attr_interpulate(pack, nrc0.sfc_ext);

    /* tfc */
    scene_set_nrx_attr_interpulate(pack, nrc0.tfc);
    /* tpc */
    scene_set_nrx_attr_interpulate(pack, nrc0.tpc);
    /* trc */
    scene_set_nrx_attr_interpulate(pack, nrc0.trc);
}

ot_void scene_set_3dnr_nrx_adv_iey(const _3dnr_nrx_pack *pack)
{
    for (ot_u32 i = 0; i < scene_array_size(pack->nrx_attr->adv_ie_y); i++) {
        scene_set_nrx_attr_interpulate(pack, adv_ie_y[i].ies0);
        scene_set_nrx_attr_interpulate(pack, adv_ie_y[i].ies1);
        scene_set_nrx_attr_interpulate(pack, adv_ie_y[i].ies2);
        scene_set_nrx_attr_interpulate(pack, adv_ie_y[i].ies3);
        scene_set_nrx_attr_interpulate(pack, adv_ie_y[i].aux_ies);
        scene_set_nrx_attr_interpulate(pack, adv_ie_y[i].o_sht);
        scene_set_nrx_attr_interpulate(pack, adv_ie_y[i].u_sht);
    }
}

ot_void scene_set_3dnr_nrx_adv_sfy(const _3dnr_nrx_pack *pack)
{
    for (ot_u32 i = 0; i < scene_array_size(pack->nrx_attr->adv_sf_y); i++) {
        scene_set_nrx_attr_interpulate(pack, adv_sf_y[i].spn6);
        scene_set_nrx_attr_interpulate(pack, adv_sf_y[i].sbn6);
        scene_set_nrx_attr_interpulate(pack, adv_sf_y[i].pbr6);

        scene_set_nrx_attr_interpulate(pack, adv_sf_y[i].spn3);
        scene_set_nrx_attr_interpulate(pack, adv_sf_y[i].sbn3);
        scene_set_nrx_attr_interpulate(pack, adv_sf_y[i].pbr3);

        scene_set_nrx_attr_interpulate(pack, adv_sf_y[i].spn4);
        scene_set_nrx_attr_interpulate(pack, adv_sf_y[i].sbn4);
        scene_set_nrx_attr_interpulate(pack, adv_sf_y[i].pbr4);

        scene_set_nrx_attr_interpulate(pack, adv_sf_y[i].sfn0);
        scene_set_nrx_attr_interpulate(pack, adv_sf_y[i].sfn1);
        scene_set_nrx_attr_interpulate(pack, adv_sf_y[i].sfn2);
        scene_set_nrx_attr_interpulate(pack, adv_sf_y[i].sfn3);

        scene_set_nrx_attr_interpulate(pack, adv_sf_y[i].sth1);
        scene_set_nrx_attr_interpulate(pack, adv_sf_y[i].sth2);
        scene_set_nrx_attr_interpulate(pack, adv_sf_y[i].sth3);

        scene_set_nrx_attr_interpulate(pack, adv_sf_y[i].sfs1);
        scene_set_nrx_attr_interpulate(pack, adv_sf_y[i].j_mode6);

        scene_set_nrx_attr_interpulate(pack, adv_sf_y[i].sbr6[0]);
        scene_set_nrx_attr_interpulate(pack, adv_sf_y[i].sbr6[1]);

        scene_set_nrx_attr_interpulate(pack, adv_sf_y[i].bri_idx);
        for (ot_u32 j = 0; j < 4; j++) { /* 4  is index */
            scene_set_nrx_attr_interpulate(pack, adv_sf_y[i].sfr6[j]);
        }
    }
    scene_set_nrx_attr_interpulate(pack, adv_sf_y[0].sfs5);
    scene_set_nrx_attr_interpulate(pack, adv_sf_y[0].sft5);
    scene_set_nrx_attr_interpulate(pack, adv_sf_y[0].sbr5);
    scene_set_nrx_attr_interpulate(pack, adv_sf_y[0].bld1);
}

ot_void scene_set_3dnr_nrx_nrc1(const _3dnr_nrx_pack *pack)
{
    /* nrc1_en */
    scene_set_nrx_attr_interpulate(pack, nrc1_en);

    /* sfn */
    scene_set_nrx_attr_interpulate(pack, nrc1.sfs2);
    scene_set_nrx_attr_interpulate(pack, nrc1.sft2);
    scene_set_nrx_attr_interpulate(pack, nrc1.sbr2);

    scene_set_nrx_attr_interpulate(pack, nrc1.pbr0);
    scene_set_nrx_attr_interpulate(pack, nrc1.pbr1);
}
#if 0
static ot_void scene_set_qp_h265_avbr_param(ot_venc_rc_param *rc_param, ot_u32 max_bitrate)
{
    if (max_bitrate <= 768) {                                   /* low than 768 */
        rc_param->h265_avbr_param.min_i_qp = 30;                /* min_i_qp is 30 */
        rc_param->h265_avbr_param.min_qp = 32;                  /* min_qp is 32 */
        rc_param->h265_avbr_param.max_i_qp = 48;                /* max_i_qp is 48 */
        rc_param->h265_avbr_param.max_qp = 48;                  /* max_qp is 48 */
    } else if ((max_bitrate > 768) && (max_bitrate <= 1536)) {  /* 768 to 1536 */
        rc_param->h265_avbr_param.min_i_qp = 29;                /* min_i_qp is 29 */
        rc_param->h265_avbr_param.min_qp = 31;                  /* min_qp is 31 */
        rc_param->h265_avbr_param.max_i_qp = 44;                /* x is 44 */
        rc_param->h265_avbr_param.max_qp = 44;                  /* max_qp is 44 */
    } else if ((max_bitrate > 1536) && (max_bitrate <= 2560)) { /* 1536 to 2560 */
        rc_param->h265_avbr_param.min_i_qp = 26;                /* min_i_qp is 26 */
        rc_param->h265_avbr_param.min_qp = 28;                  /* min_qp is 28 */
        rc_param->h265_avbr_param.max_i_qp = 42;                /* max_i_qp is 42 */
        rc_param->h265_avbr_param.max_qp = 42;                  /* max_qp is 42 */
    } else if ((max_bitrate > 2560) && (max_bitrate <= 3584)) { /* 2560 to 3584 */
        rc_param->h265_avbr_param.min_i_qp = 25;                /* min_i_qp is 25 */
        rc_param->h265_avbr_param.min_qp = 27;                  /* min_qp is 27 */
        rc_param->h265_avbr_param.max_i_qp = 42;                /* max_i_qp is 42 */
        rc_param->h265_avbr_param.max_qp = 42;                  /* max_qp is 42 */
    } else if ((max_bitrate > 3584) && (max_bitrate <= 4096)) { /* 3584 to 4096 */
        rc_param->h265_avbr_param.min_i_qp = 24;                /* min_i_qp is 24 */
        rc_param->h265_avbr_param.min_qp = 26;                  /* min_qp is 26 */
        rc_param->h265_avbr_param.max_i_qp = 42;                /* max_i_qp is 42 */
        rc_param->h265_avbr_param.max_qp = 42;                  /* max_qp is 42 */
    } else if (max_bitrate > 4096) {                            /* bigger than 4096 */
        rc_param->h265_avbr_param.min_i_qp = 23;                /* min_i_qp is 23 */
        rc_param->h265_avbr_param.min_qp = 25;                  /* min_qp is 25 */
        rc_param->h265_avbr_param.max_i_qp = 42;                /* max_i_qp is 42 */
        rc_param->h265_avbr_param.max_qp = 42;                  /* max_qp is 42 */
    }
    rc_param->h265_avbr_param.min_i_proportion = 1;
    rc_param->h265_avbr_param.max_i_proportion = 100; /* max_i_proportion is 100 */
    rc_param->h265_avbr_param.max_reencode_times = 2; /* max_reencode_times is 2 */

    rc_param->h265_avbr_param.min_still_percent = 25; /* min_still_percent is 25 */
    rc_param->h265_avbr_param.max_still_qp = 35;      /* max_still_qp is 35 */
    rc_param->h265_avbr_param.min_still_psnr = 0;
}


static ot_void scene_set_qp_h264_avbr_param(ot_venc_rc_param *rc_param, ot_u32 max_bitrate)
{
    if (max_bitrate <= 768) {                                   /* low than 768 */
        rc_param->h264_avbr_param.min_i_qp = 30;                /* min_i_qp is 30 */
        rc_param->h264_avbr_param.min_qp = 32;                  /* min_qp is 32 */
        rc_param->h264_avbr_param.max_i_qp = 48;                /* max_i_qp is 48 */
        rc_param->h264_avbr_param.max_qp = 48;                  /* max_qp is 48 */
    } else if ((max_bitrate > 768) && (max_bitrate <= 1536)) {  /* 768 to 1536 */
        rc_param->h264_avbr_param.min_i_qp = 29;                /* min_i_qp is 29 */
        rc_param->h264_avbr_param.min_qp = 31;                  /* min_qp is 31 */
        rc_param->h264_avbr_param.max_i_qp = 44;                /* max_i_qp is 44 */
        rc_param->h264_avbr_param.max_qp = 44;                  /* max_qp is 44 */
    } else if ((max_bitrate > 1536) && (max_bitrate <= 3072)) { /* 1536 to 3072 */
        rc_param->h264_avbr_param.min_i_qp = 27;                /* min_i_qp is 27 */
        rc_param->h264_avbr_param.min_qp = 29;                  /* min_qp is 29 */
        rc_param->h264_avbr_param.max_i_qp = 42;                /* max_i_qp is 42 */
        rc_param->h264_avbr_param.max_qp = 42;                  /* max_qp is 42 */
    } else if ((max_bitrate > 3072) && (max_bitrate <= 5120)) { /* 3072 to 5120 */
        rc_param->h264_avbr_param.min_i_qp = 26;                /* min_i_qp is 26 */
        rc_param->h264_avbr_param.min_qp = 28;                  /* min_qp is 28 */
        rc_param->h264_avbr_param.max_i_qp = 42;                /* max_i_qp is 42 */
        rc_param->h264_avbr_param.max_qp = 42;                  /* max_qp is 42 */
    } else if ((max_bitrate > 5120) && (max_bitrate <= 7168)) { /* 5120 to 7168 */
        rc_param->h264_avbr_param.min_i_qp = 25;                /* min_i_qp is 25 */
        rc_param->h264_avbr_param.min_qp = 27;                  /* min_qp is 27 */
        rc_param->h264_avbr_param.max_i_qp = 42;                /* max_i_qp is 42 */
        rc_param->h264_avbr_param.max_qp = 42;                  /* max_qp is 42 */
    } else if (max_bitrate > 7168) {                            /* bigger than 7168 */
        rc_param->h264_avbr_param.min_i_qp = 23;                /* min_i_qp is 23 */
        rc_param->h264_avbr_param.min_qp = 25;                  /* min_qp is 25 */
        rc_param->h264_avbr_param.max_i_qp = 42;                /* max_i_qp is 42 */
        rc_param->h264_avbr_param.max_qp = 42;                  /* max_qp is 42 */
    }
    rc_param->h264_avbr_param.min_i_proportion = 1;
    rc_param->h264_avbr_param.max_i_proportion = 100; /* max_qp is 100 */
    rc_param->h264_avbr_param.max_reencode_times = 2; /* max_qp is 2 */

    rc_param->h264_avbr_param.min_still_percent = 25; /* max_qp is 25 */
    rc_param->h264_avbr_param.max_still_qp = 35;      /* max_qp is 35 */
    rc_param->h264_avbr_param.min_still_psnr = 0;
}

ot_s32 scene_set_qp(ot_u32 pic_width, ot_u32 pic_height, ot_u32 max_bitrate, ot_payload_type type,
    ot_venc_rc_param *rc_param)
{
    ot_scenecomm_check_pointer_return(rc_param, OT_FAILURE);
    ot_u32 align_pic_height;
    ot_u32 align_pic_width;
    ot_u32 align_pic_height_1080p;
    ot_u32 align_pic_width_1080p;
    ot_u32 bitrate;

    align_pic_height = ((pic_height + MB_ALIGN_H264 - 1) / MB_ALIGN_H264);
    align_pic_width = ((pic_width + MB_ALIGN_H264 - 1) / MB_ALIGN_H264);
    align_pic_height_1080p = ((PIC_HEIGHT_1080P + MB_ALIGN_H264 - 1) / MB_ALIGN_H264);
    align_pic_width_1080p = ((PIC_WIDTH_1080P + MB_ALIGN_H264 - 1) / MB_ALIGN_H264);
    bitrate = (align_pic_height_1080p * align_pic_width_1080p * max_bitrate) / (align_pic_height * align_pic_width);

    if (type == OT_PT_H265) {
        scene_set_qp_h265_avbr_param(rc_param, bitrate);
    }
    if (type == OT_PT_H264) {
        scene_set_qp_h264_avbr_param(rc_param, bitrate);
    }
    return OT_SUCCESS;
}

ot_s32 get_iso_mean_qp_chn_attr(ot_vi_pipe_id vi_pipe, ot_u32 *iso, ot_u32 *mean_qp, ot_venc_chn_attr *venc_chn_attr)
{
    return OT_SUCCESS;
}


ot_void set_min_qp_delta_when_iso_larger(ot_payload_type type, ot_u32 sum_mean_qp, ot_s32 percent,
    ot_venc_rc_param *rc_param)
{
    ot_s32 local_percent = percent;
    if (sum_mean_qp <= 33 && sum_mean_qp >= 31) {        /* 31 to 33 */
        local_percent = 19 * percent / 20;                     /* 19 multiplier, 20 is divider */
    } else if (sum_mean_qp <= 30 && sum_mean_qp >= 29) { /* 29 to 30 */
        local_percent = 9 * percent / 10;                      /* 9 multiplier, 10 is divider */
    } else if (sum_mean_qp < 29) {                       /* low than 29 */
        local_percent = 8 * percent / 10;                      /* 8 multiplier, 10 is divider */
    }

    if (type == OT_PT_H264) {
        rc_param->h264_avbr_param.chg_pos = local_percent;
        rc_param->h264_avbr_param.min_qp_delta = 2; /* min_qp_delta is 2 */
    }
    if (type == OT_PT_H265) {
        rc_param->h265_avbr_param.chg_pos = local_percent;
        rc_param->h265_avbr_param.min_qp_delta = 2; /* min_qp_delta is 2 */
    }
}

ot_void set_min_qp_delta_when_iso_less(ot_payload_type type, ot_u32 sum_mean_qp, ot_venc_rc_param *rc_param,
    ot_venc_chn_attr *venc_chn_attr)
{
    if (venc_chn_attr->gop_attr.gop_mode == OT_VENC_GOP_MODE_SMART_P) {
        if (type == OT_PT_H264) {
            rc_param->h264_avbr_param.min_qp_delta = 3; /* min_qp_delta is 3 */
        }
        if (type == OT_PT_H265) {
            rc_param->h265_avbr_param.min_qp_delta = 3; /* min_qp_delta is 3 */
        }
    } else {
        if (type == OT_PT_H264) {
            rc_param->h264_avbr_param.min_qp_delta = 3; /* min_qp_delta is 3 */
        }
        if (type == OT_PT_H265) {
            rc_param->h265_avbr_param.min_qp_delta = 3; /* min_qp_delta is 3 */
        }
    }
}
#endif

#ifdef __cplusplus
}
#endif

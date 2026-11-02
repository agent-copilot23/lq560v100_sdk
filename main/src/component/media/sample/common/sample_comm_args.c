#include "sample_comm.h"
#include<unistd.h>

#define __SAMPLE_COMME_ARGS(format, ...)  format "\033[0m%s", __VA_ARGS__
#define sample_choice_info(...) \
    printf("\033[1;30;32m" __SAMPLE_COMME_ARGS(__VA_ARGS__, "\n")) // green

#define SAMPLE_INDEX_DEFAULT -1

extern char *optarg;
extern int potind, opterr, optopt;

typedef struct {
    sample_sns_type sns_type[SAMPLE_SNS_MAX];
    sample_sns_num sns_num;
    sample_vi_serdes vi_serdes;
    sample_vo_serdes vo_serdes;
    sample_vo_display_dev_type vo_display_dev_type;
    sample_param_comb param_comb;
} sample_args_info;

sample_args_info g_args_info = {
    {
        SONY_IMX623_MIPI_3M_60FPS_16BIT,
        SONY_IMX623_MIPI_3M_60FPS_16BIT,
        SONY_IMX623_MIPI_3M_60FPS_16BIT,
        SONY_IMX623_MIPI_3M_60FPS_16BIT,
    },
    SAMPLE_SNS_ONE,
    {
        "MAX9295,MAX9296",
        ONE_MIPT_ONE_SENSOR,
    },
    {
        "TI941,TI948",
        OT_VO_SERDES_FPDLINK_MODE_SINGLE,
    },
    TM070JDHP08_1280X768,
    {
        SAMPLE_INDEX_DEFAULT,
        ONE_MIPI_MAX9295_MAX9296_IMX623,
        TI941_TI948_TM070JDHP08_1280X768,
    },
};

sample_vo_attr *g_vo_attr = OT_NULL;
sample_vi_attr *g_vi_attr = OT_NULL;


sample_sns_type sample_comm_args_get_sns_type(ot_s32 idx)
{
    if (idx >= SAMPLE_SNS_MAX) {
        sample_print("Invalid sensor index %d\n", idx);
        return OT_FAILURE;
    }
    return g_args_info.sns_type[idx];
}

sample_sns_num sample_comm_args_get_sns_num(ot_void)
{
    return g_args_info.sns_num;
}

sample_vi_serdes *sample_comm_args_get_vi_serdes(ot_void)
{
    return &g_args_info.vi_serdes;
}

sample_vo_serdes *sample_comm_args_get_vo_serdes(ot_void)
{
    return &g_args_info.vo_serdes;
}

sample_vo_display_dev_type sample_comm_args_get_vo_display_dev_type(ot_void)
{
    return g_args_info.vo_display_dev_type;
}

sample_vo_attr *sample_comm_args_get_vo_attr()
{
    if (g_vo_attr == OT_NULL) {
        g_vo_attr = sample_comm_get_vo_attr(ONE_IN_PORT_MIPI_768);
    }
    return g_vo_attr;
}

void sample_comm_args_set_sample_index(ot_s32 sample_index)
{
    g_args_info.param_comb.sample_index = sample_index;
}

sample_param_comb *sample_comm_args_get_param_comb(ot_void)
{
    return &g_args_info.param_comb;
}

static ot_s32 sample_comm_args_set_vi_attr(sample_args_info *args_info)
{
    sample_sns_num sns_num = args_info->sns_num;
    sample_vi_attr vi_attr;
    g_vi_attr = &vi_attr;

    switch (sns_num) {
        case SAMPLE_SNS_ONE:
            memcpy(g_vi_attr->sns_type, args_info->sns_type, SAMPLE_SNS_MAX * sizeof(sample_sns_type));
            printf("using one sensor (%d)\n", (g_vi_attr->sns_type[0]));
            break;

        case SAMPLE_SNS_TWO:
            memcpy(g_vi_attr->sns_type, args_info->sns_type, SAMPLE_SNS_MAX * sizeof(sample_sns_type));
            printf("using two sensor (%d, %d)\n", (g_vi_attr->sns_type[0]), (g_vi_attr->sns_type[1]));
            break;

        default:
            sample_print("sensor number invalid!\n");
            return OT_FAILURE;
            break;
    }

    return OT_SUCCESS;
}


static ot_s32 sample_comm_args_set_vo_attr(sample_args_info *args_info)
{
    sample_sns_num sns_num = args_info->sns_num;
    sample_vo_serdes vo_serdes = args_info->vo_serdes;
    sample_vo_display_dev_type vo_display_dev_type = args_info->vo_display_dev_type;

    switch (sns_num) {
        case SAMPLE_SNS_ONE:
            if ((vo_display_dev_type == TM070JDHP08_1280X768) &&
                (vo_serdes.vo_serdes_mode == OT_VO_SERDES_FPDLINK_MODE_SINGLE)) {
                g_vo_attr = sample_comm_get_vo_attr(ONE_IN_PORT_MIPI_768);
            } else if ((vo_display_dev_type == AV069HDT_1280X720) &&
                (vo_serdes.vo_serdes_mode == OT_VO_SERDES_FPDLINK_MODE_SINGLE)) {
                g_vo_attr = sample_comm_get_vo_attr(ONE_IN_PORT_MIPI_AV069HDT_1280X720);
            } else if (vo_display_dev_type == JC07027001_800X1280) {
                g_vo_attr = sample_comm_get_vo_attr(ONE_IN_PORT_MIPI_800X1280);
            } else if(vo_display_dev_type == VO_BT1120_1280X720) {
                g_vo_attr = sample_comm_get_vo_attr(ONE_IN_PORT_BT1120_1280X720);
            } else {
                sample_print("not supported case for one sensor!\n");
                return OT_FAILURE;
            }
            break;

        case SAMPLE_SNS_TWO:
            if ((vo_display_dev_type == TM070JDHP08_1280X768) && (vo_serdes.vo_serdes_mode == OT_VO_SERDES_FPDLINK_MODE_SINGLE)) {
                g_vo_attr = sample_comm_get_vo_attr(TWO_IN_PORT_MIPI_768);
            } else if ((vo_display_dev_type == TM070JDHP08_1280X768) && (vo_serdes.vo_serdes_mode == OT_VO_SERDES_FPDLINK_MODE_SEPARATE)) {
                g_vo_attr = sample_comm_get_vo_attr(TWO_IN_PORT_MIPI_2560X768);
            } else if ((vo_display_dev_type == AV069HDT_1280X720) && (vo_serdes.vo_serdes_mode == OT_VO_SERDES_FPDLINK_MODE_SEPARATE)) {
                g_vo_attr = sample_comm_get_vo_attr(TWO_IN_PORT_MIPI_AV069HDT_2560X720);
            } else {
                sample_print("not supported case for two sensors!\n");
                return OT_FAILURE;
            }
            break;

        default:
            sample_print("sensor number invalid!\n");
            return OT_FAILURE;
            break;
    }

    return OT_SUCCESS;
}

static ot_s32 sample_comm_args_output_parser(char *output_args, sample_args_info *args_info)
{
    ot_s32 output_param = atoi(output_args);
    args_info->param_comb.output_param = output_param;

    switch (output_param) {
        // ti941_ti948 + screen TM070JDHP08 (TIANMA, resolution 1280*768)
        case TI941_TI948_TM070JDHP08_1280X768:
            args_info->vo_display_dev_type = TM070JDHP08_1280X768;
            break;
        // ti941_ti948 + screen SM067MPA02 (TIANMA, resolution 1280*720)
        // ti941_ti948 + screen AV069HDT (BOE, resolution 1280*720)
        case TI941_TI948_AV069HDT_1280X720:
            args_info->vo_display_dev_type = AV069HDT_1280X720;
            break;
        // ti941_ti948 + screen JC07027001 (resolution 800*1280)
        case TI941_TI948_JC07027001_800X1280:
            args_info->vo_display_dev_type = JC07027001_800X1280;
            break;
        // vo 2 port + ti941_ti948 + screen TM070JDHP08 (TIANMA, resolution 1280*768)
        case TI941_TI948_TM070JDHP08_1280X768_VO_2_PORT:
            args_info->vo_display_dev_type = TM070JDHP08_1280X768;
            break;
        // vo 2 port + ti941_ti948 separate mode + screen TM070JDHP08 (TIANMA, resolution 1280*768)
        case TI941_TI948_TM070JDHP08_2560X768_SEPARATE:
            args_info->vo_display_dev_type = TM070JDHP08_1280X768;
            args_info->vo_serdes.vo_serdes_mode = OT_VO_SERDES_FPDLINK_MODE_SEPARATE;
            break;
        case TI941_TI948_AV069HDT_2560X720_SEPARATE:
            args_info->vo_display_dev_type = AV069HDT_1280X720;
            args_info->vo_serdes.vo_serdes_mode = OT_VO_SERDES_FPDLINK_MODE_SEPARATE;
            break;
        case SCREEN_BT1120_INTERFACE_1280X720:
            args_info->vo_display_dev_type = VO_BT1120_1280X720;
            break;
        default:
            sample_print("ouput param[%d] invalid!\n", output_param);
            return OT_FAILURE;
            break;
    }

    return OT_SUCCESS;
}

static ot_s32 sample_comm_args_input_parser(char *input_args, sample_args_info *args_info)
{
    ot_s32 input_param = atoi(input_args);
    args_info->param_comb.input_param = input_param;
    switch (input_param) {
        case ONE_MIPI_MAX9295_MAX9296_IMX623:
            args_info->sns_type[0] = SONY_IMX623_MIPI_3M_60FPS_16BIT;
            break;
        case ONE_MIIP_MAX9295_MAX9296_GC2093:
            args_info->sns_type[0] = GC_GC2093_MIPI_2M_30FPS_10BIT_WDR2TO1;
            break;
        case ONE_MIPI_MAX9295_MAX9296_IMX290:
            args_info->sns_type[0] = SONY_IMX290_MIPI_2M_60FPS_10BIT_WDR2TO1;
            break;
        case ONE_MIPI_MAX9295_MAX9296_OX03C:
            args_info->sns_type[0] = OV_OX03C_MIPI_3M_60FPS_16BIT;
            break;
        case ONE_MIPI_MAX9295_MAX9296_OX08B40:
            args_info->sns_type[0] = OV_OX08B40_MIPI_8M_30FPS_12BIT;
            break;
        case ONE_MIPI_MAX9295_MAX9296_OX03F10:
            args_info->sns_type[0] = OV_OX03F10_MIPI_3M_60FPS_16BIT;
            break;
        case ONE_MIPI_MAX9295_MAX9296_SC220AT:
            args_info->sns_type[0] = SMS_SC220AT_MIPI_2M_30FPS_RAW12;
            break;
        case ONE_MIPI_MAX9295_MAX9296_OX05B1S:
            args_info->sns_type[0] = OV_OX05B1S_MIPI_5M_60FPS_10BIT;
            break;
        case ONE_MIPI_DIRECT_IMX582:
            args_info->sns_type[0] = SONY_IMX582_MIPI_8M_30FPS_10BIT_DIRECT;
            break;
        case TWO_MIPI_MAX9295_MAX9296_IMX623_IMX623:
            args_info->sns_type[0] = SONY_IMX623_MIPI_3M_60FPS_16BIT;
            args_info->sns_type[1] = SONY_IMX623_MIPI_3M_60FPS_16BIT;
            args_info->sns_num = SAMPLE_SNS_TWO;
            break;
        case TWO_MIPI_MAX9295_MAX9296_GC2093_IMX290:
            args_info->sns_type[0] = GC_GC2093_MIPI_2M_30FPS_10BIT_WDR2TO1;
            args_info->sns_type[1] = SONY_IMX290_MIPI_2M_60FPS_10BIT_WDR2TO1;
            args_info->sns_num = SAMPLE_SNS_TWO;
            break;
        case TWO_MIPI_MAX9295_MAX9296_IMX290_GC2093:
            args_info->sns_type[0] = SONY_IMX290_MIPI_2M_60FPS_10BIT_WDR2TO1;
            args_info->sns_type[1] = GC_GC2093_MIPI_2M_30FPS_10BIT_WDR2TO1;
            args_info->sns_num = SAMPLE_SNS_TWO;
            break;

        default:
            sample_print("input param[%d] invalid!\n", input_param);
            return OT_FAILURE;
            break;
    }

    return OT_SUCCESS;
}

ot_void sample_comm_vi_args_help(ot_void)
{
    printf("\ninput param:\n");
    printf("  for one sensor case\n");
    sample_choice_info("    (0) vi serdes max9295_max9296 + imx623");
    sample_choice_info("    (1) vi serdes max9295_max9296 + gc2093");
    sample_choice_info("    (2) vi serdes max9295_max9296 + imx290");
    sample_choice_info("    (3) vi serdes max9295_max9296 + ox03c");
    sample_choice_info("    (4) vi serdes max9295_max9296 + ox08b40");
    sample_choice_info("    (5) vi serdes max9295_max9296 + ox03f10");
    sample_choice_info("    (6) vi serdes max9295_max9296 + sc220at");
    sample_choice_info("    (7) vi serdes max9295_max9296 + ox05b1s");
    sample_choice_info("    (8) vi serdes direct imx582");
    printf("  for two sensor case\n");
    sample_choice_info("    (20) vi serdes max9295_max9296 + imx623_imx623");
    sample_choice_info("    (21) vi serdes max9295_max9296 + gc2093_imx290");
    sample_choice_info("    (22) vi serdes max9295_max9296 + imx290_gc2093");
}

ot_void sample_comm_vo_args_help(ot_void)
{
    printf("\noutput param:\n");
    printf("  for one display case\n");
    sample_choice_info("    (0) vo serdes ti941_ti948 + screen TM070JDHP08 (TIANMA, resolution 1280*768)");
    sample_choice_info("    (1) vo serdes ti941_ti948 + screen AV069HDT (BOE, resolution 1280*720)");
    sample_choice_info("    (2) mipitx + screen JC07027001 (resolution 800*1280)");
    sample_choice_info("    (3) vo serdes ti941_ti948 + vo 2 port + screen TM070JDHP08 (TIANMA, resolution 1280*768)");
    sample_choice_info("    (4) vo bt1120 screen   (resolution 1280*720)");
    printf("  for two display case\n");
    sample_choice_info("    (20) vo serdes ti941_ti948 separate mode + vo 2 port + 2 screen TM070JDHP08 (TIANMA, resolution 1280*768)");
    sample_choice_info("    (21) vo serdes ti941_ti948 separate mode + vo 2 port + 2 screen AV069HDT (BOE, resolution 1280*720)");
}

ot_void sample_comm_args_help(ot_void)
{
    sample_comm_vi_args_help();
    sample_comm_vo_args_help();
}

static ot_s32 sample_comm_args_parse_options(int argc, char *argv[], sample_args_info *args_info)
{
    int opt;
    char *optstr = "i:o:";

    while ((opt = getopt(argc, argv, optstr)) != -1) {
        switch (opt) {
            case 'i':
                if (sample_comm_args_input_parser(optarg, args_info)) {
                    return OT_FAILURE;
                }
                break;
            case 'o':
                if (sample_comm_args_output_parser(optarg, args_info)) {
                    return OT_FAILURE;
                }
                break;
            default:
                sample_print("invalid option!\n");
                return OT_FAILURE;
        }
    }

    return OT_SUCCESS;
}


// 第一个参数为可执行文件名，第二个参数为 index，后面的参数为需解析的参数
ot_s32 sample_comm_args_register(ot_s32 argc, ot_char *argv[])
{
    ot_s32 ret;
    sample_args_info args_info;

    memcpy(&args_info, &g_args_info, sizeof(sample_args_info));
    ret = sample_comm_args_parse_options(argc, argv, &args_info);
    if (ret != OT_SUCCESS) {
        sample_print("Invalid param!\n");
        return OT_FAILURE;
    }
    ret = sample_comm_args_set_vo_attr(&args_info);
    if (ret != OT_SUCCESS) {
        sample_print("No such vo attr found!\n");
        return OT_FAILURE;
    }
    ret = sample_comm_args_set_vi_attr(&args_info);
    if (ret != OT_SUCCESS) {
        sample_print("No such vi attr found!\n");
        return OT_FAILURE;
    }
    memcpy(&g_args_info, &args_info, sizeof(sample_args_info));

    return OT_SUCCESS;
}

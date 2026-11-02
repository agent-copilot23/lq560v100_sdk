/*
  Copyright (c), 2001-2022, Oritek Tech. Co., Ltd.
 */

#ifndef OX03F10_CMOS_EX_H
#define OX03F10_CMOS_EX_H

#include "ot_eis_vi_awb.h"
#include "ot_eis_vi_sns.h"

#ifdef __cplusplus
#if __cplusplus
extern "C" {
#endif
#endif

static const ot_eis_vi_sns_black_level g_cmos_blc = {
    OT_FALSE, /* user_black_level_en */
    {
        {1200, 1200, 1200, 1200},
        {1200, 1200, 1200, 1200},
        {1200, 1200, 1200, 1200},
        {1200, 1200, 1200, 1200},
    },   /* user_black_level */
    OT_EIS_VI_BLACK_LEVEL_MODE_AUTO, /* ot_eis_vi_black_level_mode */
    {
        {
            {800, 800, 800, 800},
            {800, 800, 800, 800},
            {800, 800, 800, 800},
            {800, 800, 800, 800},
        },
    }, /* manual_attr */
    {
        0,                            /* update mode 0 */
        {
            {800, 800, 800, 800}, /* black level */
            {800, 800, 800, 800}, /* black level */
            {800, 800, 800, 800}, /* black level */
            {800, 800, 800, 800}, /* black level */
        },
    },
};

static const ot_eis_vi_sns_black_level g_cmos_blc_wdr = {
    OT_FALSE, /* user_black_level_en */
    {
        {1200, 1200, 1200, 1200},
        {1200, 1200, 1200, 1200},
        {1200, 1200, 1200, 1200},
        {1200, 1200, 1200, 1200},
    },   /* user_black_level */
    OT_EIS_VI_BLACK_LEVEL_MODE_AUTO, /* ot_eis_vi_black_level_mode */
    {
        {
            {240, 240, 240, 240},
            {240, 240, 240, 240},
            {240, 240, 240, 240},
            {240, 240, 240, 240},
        },
    }, /* manual_attr */
    {
        0,                            /* update mode 0 */
        {
            {0x400, 0x400, 0x400, 0x400}, /* black level */
            {0x400, 0x400, 0x400, 0x400}, /* black level */
            {0x400, 0x400, 0x400, 0x400}, /* black level */
            {0x400, 0x400, 0x400, 0x400}, /* black level */
        },
    },
};

static const ot_eis_vi_dpc_dynamic_cor_attr g_cmos_dpc = {
    1,              /* dynamic correction enable */

    {
        {
            0,          /* twinkle suppression enable */
            6,          /* twinkle suppression threshold */
            21,         /* twinkle suppression slop */
            0,          /* 0, auto mode; 1, manual mode */
            {
                0,      /* manual mode strength */
                0,      /* manual mode blend ratio */
            },
            {
                { 0, 0, 0, 152, 220, 220, 220, 220, 220, 220, 152, 152, 152, 152, 152, 152 }, /* strength[16] */
                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 50, 50, 50, 50, 50, 50 },                     /* blend_ratio[16] */
            },
            96,         /* strength for bright area */
            63,         /* strength for dark area */
        }, /* dynamic config for linear frame */
	}
};

static const ot_eis_vi_dpc_dynamic_cor_attr g_cmos_dpc_wdr = {
    1,              /* dynamic correction enable */

    {
        {
            0,          /* twinkle suppression enable */
            6,          /* twinkle suppression threshold */
            21,         /* twinkle suppression slop */
            0,          /* 0, auto mode; 1, manual mode */
            {
                0,      /* manual mode strength */
                0,      /* manual mode blend ratio */
            },
            {
                { 230, 230, 230, 235, 235, 235, 240, 240, 240, 250, 250, 250, 250, 250, 250, 250 }, /* strength[16] */
                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },                      /* blend_ratio[16] */
            },
            96,         /* strength for bright area */
            63,         /* strength for dark area */
        }, /* dynamic config for short frame */

        {
            0,          /* twinkle suppression enable */
            6,          /* twinkle suppression threshold */
            21,         /* twinkle suppression slop */
            0,          /* 0, auto mode; 1, manual mode */
            {
                0,      /* manual mode strength */
                0,      /* manual mode blend ratio */
            },
            {
                { 230, 230, 230, 235, 235, 235, 240, 240, 240, 250, 250, 250, 250, 250, 250, 250 }, /* strength[16] */
                { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },                      /* blend_ratio[16] */
            },
            96,         /* strength for bright area */
            63,         /* strength for dark area */
        }, /* dynamic config for long frame */
    }
};

#ifdef CONFIG_OT_ISP_CR_SUPPORT
static const ot_eis_vi_cr_attr g_cmos_ge = {
    1,   /* enable */
    9,   /* slope */
    9,   /* sensi_slope */
    300, /* sensi_threshold */
    { 128, 128, 128, 128, 129, 129, 129, 129, 130, 130, 130, 130, 131, 131, 131, 131 }, /* strength */
    { 300, 300, 300, 300, 310, 310, 310, 310, 320, 320, 320, 320, 330, 330, 330, 330 }, /* threshold */
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, /* filter_mode */
    { 1024, 1024, 1024, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048 } /* np_offset */
};

static const ot_eis_vi_cr_attr g_cmos_ge_wdr = {
    0,   /* enable */
    9,   /* slope */
    9,   /* sensi_slope */
    300, /* sensi_threshold */
    { 128, 128, 128, 128, 129, 129, 129, 129, 130, 130, 130, 130, 131, 131, 131, 131 }, /* strength */
    { 300, 300, 300, 300, 310, 310, 310, 310, 320, 320, 320, 320, 330, 330, 330, 330 }, /* threshold */
    { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, /* filter_mode */
    { 1024, 1024, 1024, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048, 2048 } /* np_offset */
};
#endif

static const ot_eis_vi_demosaic_attr g_cmos_demosaic = {
    1, /* en */
    OT_EIS_OP_MODE_AUTO, /* op_type */
    128, /* ai_detail_strength */
    {
        48, /* nddm_strength */
        64, /* nddm_mf_detail_strength */
        0, /* nddm_hf_detail_strength */
        2, /* detail_smooth_range */
        0, /* color_noise_f_threshold */
        8, /* color_noise_f_strength */
        1, /* color_noise_y_threshold */
        10, /* color_noise_y_strength */
    },
    {
        /* nddm_strength */
        { 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48 },
        /* nddm_mf_detail_strength */
        { 68, 64, 60, 58, 55, 50, 48, 45, 45, 40, 40, 40, 40, 40, 40, 40 },
        /* nddm_hf_detail_strength */
        { 2,  2,  2,  2,  2,  2,  4,  6,  6,  6,  6,  6,  6,  6,  6,  6 },
        /* detail_smooth_range */
        { 2,  2,  2,  2,  2,  2,  2,  1,  1,  1,  1,  1,  1,  1,  1,  1 },
        /* color_noise_f_threshold */
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        /* color_noise_f_strength */
        { 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8 },
        /* color_noise_y_threshold */
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        /* color_noise_y_strength */
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    }
};

static const ot_eis_vi_demosaic_attr g_cmos_demosaic_built_in = {
    1, /* en */
    OT_EIS_OP_MODE_MANUAL, /* op_type */
    128, /* ai_detail_strength */
    {
        0, /* nddm_strength */
        0, /* nddm_mf_detail_strength */
        0, /* nddm_hf_detail_strength */
        7, /* detail_smooth_range */
        0, /* color_noise_f_threshold */
        0, /* color_noise_f_strength */
        0, /* color_noise_y_threshold */
        0, /* color_noise_y_strength */
    },
    {
        /* nddm_strength */
        { 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48, 48 },
        /* nddm_mf_detail_strength */
        { 68, 64, 60, 58, 55, 50, 48, 45, 45, 40, 40, 40, 40, 40, 40, 40 },
        /* nddm_hf_detail_strength */
        { 2,  2,  2,  2,  2,  2,  4,  6,  6,  6,  6,  6,  6,  6,  6,  6 },
        /* detail_smooth_range */
        { 2,  2,  2,  2,  2,  2,  2,  1,  1,  1,  1,  1,  1,  1,  1,  1 },
        /* color_noise_f_threshold */
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        /* color_noise_f_strength */
        { 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8 },
        /* color_noise_y_threshold */
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        /* color_noise_y_strength */
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
    }
};

static const ot_eis_vi_demosaic_attr g_cmos_demosaic_wdr = {
    1, /* en */
    OT_EIS_OP_MODE_AUTO, /* op_type */
    128, /* ai_detail_strength */
    {
        48, /* nddm_strength */
        64, /* nddm_mf_detail_strength */
        0, /* nddm_hf_detail_strength */
        2, /* detail_smooth_range */
        0, /* color_noise_f_threshold */
        8, /* color_noise_f_strength */
        1, /* color_noise_y_threshold */
        10, /* color_noise_y_strength */
    },
    {
        /* nddm_strength */
        { 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16 },
        /* nddm_mf_detail_strength */
        { 60, 60, 58, 55, 50, 45, 40, 30, 20, 16, 16, 16, 16, 16, 16, 16 },
        /* nddm_hf_detail_strength */
        { 0, 0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 5, 5, 5, 5, 5 },
        /* detail_smooth_range */
        { 2, 2, 2, 3, 3, 3, 4, 4, 4, 5, 6, 7, 7, 7, 7, 7 },
        /* color_noise_f_threshold */
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        /* color_noise_f_strength */
        { 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8 },
        /* color_noise_y_threshold */
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        /* color_noise_y_strength */
        { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 },
    }
};

static const ot_eis_vi_anti_false_color_attr g_cmos_anti_false_color = {
    1,                  /* en */
    OT_EIS_OP_MODE_AUTO,    /* op_type */
    { 8, 8 },           /* manual_attr */
    {
        { 28, 24, 20, 15, 10, 8, 8, 7, 7, 6, 5, 4, 3, 2, 1, 0 }, /* anti_false_color_threshold */
        { 28, 24, 20, 15, 10, 8, 8, 7, 7, 6, 5, 4, 3, 2, 1, 0 },   /* anti_false_color_strength */
    }
};

static const ot_eis_vi_anti_false_color_attr g_cmos_anti_false_color_wdr = {
    1,                  /* enable */
    OT_EIS_OP_MODE_AUTO,    /* op_type */
    { 8, 8 },           /* manual_attr */
    {
        { 28, 24, 20, 15, 10, 8, 8, 7, 7, 6, 5, 4, 3, 2, 1, 0 }, /* anti_false_color_threshold */
        { 28, 24, 20, 15, 10, 8, 8, 7, 7, 6, 5, 4, 3, 2, 1, 0 },   /* anti_false_color_strength */
    }
};

static const ot_eis_vi_cac_attr g_cmos_cac = {
    1, /* enable */
    OT_EIS_OP_MODE_AUTO, /* op_type */
    1, /* detect_mode */
    135, /* purple_upper_limit */
    (-50), /* purple_lower_limit */
    /* acac_cfg */
    {
        {
            {10, 300}, /* edge_threshold */
            255, /* edge_gain */
            10, /* cac_rb_strength */
            63, /* purple_alpha */
            63, /* edge_alpha */
            100, /* satu_low_threshold */
            16383, /* satu_high_threshold */
        },
        {
            /* edge_threshold */
            {
                {100, 100, 100, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
                {500, 500, 500, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600}
            },
            /* edge_gain */
            { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
            /* cac_rb_strength */
            { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 },
            /* purple_alpha */
            { 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63 },
            /* edge_alpha */
            { 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63 },
            /* satu_low_threshold */
            { 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
            /* satu_high_threshold */
            { 16383, 16383, 16383, 16383, 16383, 16383, 16383, 16383, 16383, 16383, 16383, 16383, 16383, 16383,
              16383, 16383 },
        }
    },
    /* lcac_cfg */
    {
        60, /* purple_detect_range */
        200, /* var_threshold */
        { 1500, 1500, 0 }, /* r_detect_threshold */
        { 1500, 1500, 0 }, /* g_detect_threshold */
        { 4095, 1500, 0 }, /* b_detect_threshold */
        {
            0, /* de_purple_cr_strength */
            3, /* de_purple_cb_strength */
        },
        {
            /* de_purple_cr_strength */
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
            /* de_purple_cb_strength */
            { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },
        }
    }
};

static const ot_eis_vi_cac_attr g_cmos_cac_wdr = {
    1, /* enable */
    OT_EIS_OP_MODE_AUTO, /* op_type */
    1, /* detect_mode */
    135, /* purple_upper_limit */
    (-50), /* purple_lower_limit */
    /* acac_cfg */
    {
        {
            {10, 300}, /* edge_threshold */
            255, /* edge_gain */
            10, /* cac_rb_strength */
            63, /* purple_alpha */
            63, /* edge_alpha */
            100, /* satu_low_threshold */
            1000, /* satu_high_threshold */
        },
        {
            /* edge_threshold */
            {
                {100, 100, 100, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
                {500, 500, 500, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600, 600}
            },
            /* edge_gain */
            { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
            /* cac_rb_strength */
            { 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10, 10 },
            /* purple_alpha */
            { 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63 },
            /* edge_alpha */
            { 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63, 63 },
            /* satu_low_threshold */
            { 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
            /* satu_high_threshold */
            { 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000 },
        }
    },
    /* lcac_cfg */
    {
        60, /* purple_detect_range */
        200, /* var_threshold */
        { 1500, 1500, 0 }, /* r_detect_threshold */
        { 1500, 1500, 0 }, /* g_detect_threshold */
        { 4095, 1500, 0 }, /* b_detect_threshold */
        {
            0, /* de_purple_cr_strength */
            7, /* de_purple_cb_strength */
        },
        {
            /* de_purple_cr_strength */
            { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
            /* de_purple_cb_strength */
            { 0, 0, 4, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 },
        }
    }
};

static const ot_eis_vi_bayer_sharpen_attr g_cmos_bayershp = {
    1, /* en */
    {180, 220}, /* dark_threshold */
    {200, 300}, /* texture_threshold */
    OT_EIS_OP_MODE_AUTO, /* op_type */
    {
        { 0, 0, 32, 32, 32, 32, 32, 32 }, /* mf_strength */
        { 0, 0, 16, 32, 32, 16, 16, 0 }, /* hf_strength */
        { 0, 0, 16, 16, 4, 2, 0, 0 }, /* dark_strength */
        3, /* mf_gain */
        2, /* hf_gain */
        1, /* dark_gain */
        128, /* overshoot */
        128, /* undershoot */
    },
    {
        /* mf_strength[OT_EIS_VI_BSHP_CURVE_NUM][OT_EIS_VI_AUTO_ISO_NUM] */
        {
            {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
            {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
            {32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32},
            {32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32},
            {32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32},
            {32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32},
            {32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32},
            {32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32}
        },
        /* hf_strength[OT_EIS_VI_BSHP_CURVE_NUM][OT_EIS_VI_AUTO_ISO_NUM] */
        {
            {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
            {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
            {16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16},
            {32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32},
            {32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32},
            {16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16},
            {16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16},
            {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}
        },
        /* dark_strength[OT_EIS_VI_BSHP_CURVE_NUM][OT_EIS_VI_AUTO_ISO_NUM] */
        {
            {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
            {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
            {16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16},
            {16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16},
            {4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4},
            {2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2},
            {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
            {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}
        },
        /* mf_gain */
        { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },
        /* hf_gain */
        { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
        /* dark_gain */
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        /* overshoot */
        { 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128 },
        /* undershoot */
        { 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128 },
    }
};

static const ot_eis_vi_bayer_sharpen_attr g_cmos_bayershp_wdr = {
    1, /* en */
    {180, 220}, /* dark_threshold */
    {200, 300}, /* texture_threshold */
    OT_EIS_OP_MODE_AUTO, /* op_type */
    {
        { 0, 0, 32, 32, 32, 32, 32, 32 }, /* mf_strength */
        { 0, 0, 16, 32, 32, 16, 16, 0 }, /* hf_strength */
        { 0, 0, 16, 16, 4, 2, 0, 0 }, /* dark_strength */
        3, /* mf_gain */
        2, /* hf_gain */
        1, /* dark_gain */
        128, /* overshoot */
        128, /* undershoot */
    },
    {
        /* mf_strength[OT_EIS_VI_BSHP_CURVE_NUM][OT_EIS_VI_AUTO_ISO_NUM] */
        {
            {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
            {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
            {32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32},
            {32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32},
            {32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32},
            {32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32},
            {32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32},
            {32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32}
        },
        /* hf_strength[OT_EIS_VI_BSHP_CURVE_NUM][OT_EIS_VI_AUTO_ISO_NUM] */
        {
            {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
            {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
            {16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16},
            {32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32},
            {32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32,  32},
            {16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16},
            {16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16},
            {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}
        },
        /* dark_strength[OT_EIS_VI_BSHP_CURVE_NUM][OT_EIS_VI_AUTO_ISO_NUM] */
        {
            {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
            {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
            {16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16},
            {16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16,  16},
            {4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4,   4},
            {2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2,   2},
            {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0},
            {0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0,   0}
        },
        /* mf_gain */
        { 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3 },
        /* hf_gain */
        { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2 },
        /* dark_gain */
        { 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 },
        /* overshoot */
        { 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128 },
        /* undershoot */
        { 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128 },
    }
};

/* BAYER NR */
static ot_eis_vi_noise_calibration g_cmos_noise_calibration = {
    {6400.0000000000 ,0.0002512419 ,0.1453450236 ,0.0000001937 ,-0.0002749600 ,0.2833128606 ,
	0.0002512419 ,0.1453450236 ,0.0000001937 ,-0.0002749600 ,0.2833128606 ,0 ,0 ,0 ,0 ,0 ,}
};

static const ot_eis_vi_nr_attr g_cmos_bayer_nr = {
    0,      /* enable */
    0,      /* op_type */
    1,      /* tnr_enable */
    0,      /* lsc_nr_enable */
    0,      /* lsc_ratio1 */
    {
        85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
        85, 85, 85, 85, 85, 85, 90, 90, 90, 95, 95, 95, 100, 100, 100, 100, 100
    },   /* coring_ratio */
    {
        128, 128, 128, 128, 128, 128, 110, 90,
        77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77
    },
    /* snr_cfg */
    {
        0, /* snr_version */
        {
            {
                /* snr auto */
                {
                    {
                        {108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108},
                        {108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108},
                        {108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108},
                        {108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108}
                    },
                    {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}, /* sfm0_detail_prot */
                    {512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512}, /* sfm1_str */
                    {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}, /* sfm1_adp_strength */
                    {64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64}, /* sfm6_strength */
                    {64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64}, /* sfm7_strength */
                    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100}, /* sth */
                    {128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128}, /* fine_str */
                    {50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50}, /* coring_wgt */
                    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} /* coring_mot_ratio */
                },

                /* snr manual */
                {
                    { 108, 108, 108, 108 }, /* snr_sfm0_coarstr */
                    16, /* sfm0_detail_prot */
                    512, /* sfm1_str */
                    2, /* sfm1_adp_strength */
                    64, /* sfm6_strength */
                    64, /* sfm7_strength */
                    100, /* sth */
                    128, /* fine_str */
                    50, /* coring_wgt */
                    0,  /* coring_mot_ratio */
                },
            },
        }
    },
    /* tnr_cfg */
    {
        {
            /* tnr auto */
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, /* md_mode */
            {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32}, /* md_size_ratio */
            {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32}, /* md_anti_flicker_str */
            {26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26},   /* md_static_ratio */
            {13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13},   /* md_motion_ratio */
            {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32},   /* md_static_finestr */
            {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}, /* tnr_tfs */
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* user_define_md */
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* user_define_slope */
            {90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90}, /* user_define_dark_thresh */
            {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}, /* user_define_color_thresh */
            {26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26}, /* sfr_r */
            {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32}, /* sfr_g */
            {26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26}, /* sfr_b */
        },
        {
            1, /* md_mode */
            32, /* md_size_ratio */
            32, /* md_anti_flicker_str */
            26, /* md_static_ratio */
            13, /* md_motion_ratio */
            32, /* md_static_finestr */
            255, /* tnr_tfs */
            0, /* user_define_md */
            0, /* user_define_bright_thresh */
            90, /* user_define_dark_thresh */
            16, /* user_define_color_thresh */
            26, /* sfr_r */
            32, /* sfr_g */
            26, /* sfr_b */
        }
    },
    /* wdr_cfg */
    {
        {16, 16, 16, 16}, /* sfm0_wdr_frame_str */
        {16, 16, 16, 16}, /* sfm0_fusion_frame_str */
        {64, 16, 16, 16}, /* snr_wdr_sfm6_strength */
        {64, 16, 16, 16}, /* snr_wdr_sfm7_strength */
        {64, 16, 16, 16}, /* snr_fusion_sfm6_strength */
        {64, 16, 16, 16}, /* snr_fusion_sfm7_strength */
        {16, 16, 16, 16}, /* md_wdr_frame_str */
        {16, 16, 16, 16}, /* md_fusion_frame_str */
    },
    /* dering_cfg */
    {
        {
            {64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64}, /* dering_strength */
            {64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64}, /* dering_thresh */
            {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}, /* dering_static_str */
            {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}, /* dering_motion_str */
        },
        {
            64, /* dering_strength */
            64, /* dering_thresh */
            16, /* dering_static_str */
            16, /* dering_motion_str */
        }
    },
};

static const ot_eis_vi_nr_attr g_cmos_bayer_nr_built_in = {
    1,      /* enable */
    0,      /* op_type */
    0,      /* tnr_enable */
    0,      /* lsc_nr_enable */
    0,      /* lsc_ratio1 */
    {
        85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
        85, 85, 85, 85, 85, 85, 90, 90, 90, 95, 95, 95, 100, 100, 100, 100, 100
    },   /* coring_ratio */
    {
        128, 128, 128, 128, 128, 128, 110, 90,
        77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77
    },
    /* snr_cfg */
    {
        0, /* snr_version */
        {
            {
                /* snr auto */
                {
                    {
                        {108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108},
                        {108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108},
                        {108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108},
                        {108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108}
                    },
                    {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}, /* sfm0_detail_prot */
                    {512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512}, /* sfm1_str */
                    {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}, /* sfm1_adp_strength */
                    {64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64}, /* sfm6_strength */
                    {64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64}, /* sfm7_strength */
                    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100}, /* sth */
                    {128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128}, /* fine_str */
                    {50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50}, /* coring_wgt */
                    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} /* coring_mot_ratio */
                },

                /* snr manual */
                {
                    { 108, 108, 108, 108 }, /* snr_sfm0_coarstr */
                    16, /* sfm0_detail_prot */
                    512, /* sfm1_str */
                    2, /* sfm1_adp_strength */
                    64, /* sfm6_strength */
                    64, /* sfm7_strength */
                    100, /* sth */
                    128, /* fine_str */
                    50, /* coring_wgt */
                    0,  /* coring_mot_ratio */
                },
            },
        }
    },
    /* tnr_cfg */
    {
        {
            /* tnr auto */
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, /* md_mode */
            {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32}, /* md_size_ratio */
            {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32}, /* md_anti_flicker_str */
            {26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26},   /* md_static_ratio */
            {13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13},   /* md_motion_ratio */
            {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32},   /* md_static_finestr */
            {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}, /* tnr_tfs */
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* user_define_md */
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* user_define_slope */
            {90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90}, /* user_define_dark_thresh */
            {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}, /* user_define_color_thresh */
            {26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26}, /* sfr_r */
            {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32}, /* sfr_g */
            {26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26}, /* sfr_b */
        },
        {
            1, /* md_mode */
            32, /* md_size_ratio */
            32, /* md_anti_flicker_str */
            26, /* md_static_ratio */
            13, /* md_motion_ratio */
            32, /* md_static_finestr */
            255, /* tnr_tfs */
            0, /* user_define_md */
            0, /* user_define_bright_thresh */
            90, /* user_define_dark_thresh */
            16, /* user_define_color_thresh */
            26, /* sfr_r */
            32, /* sfr_g */
            26, /* sfr_b */
        }
    },
    /* wdr_cfg */
    {
        {16, 16, 16, 16}, /* sfm0_wdr_frame_str */
        {16, 16, 16, 16}, /* sfm0_fusion_frame_str */
        {64, 16, 16, 16}, /* snr_wdr_sfm6_strength */
        {64, 16, 16, 16}, /* snr_wdr_sfm7_strength */
        {64, 16, 16, 16}, /* snr_fusion_sfm6_strength */
        {64, 16, 16, 16}, /* snr_fusion_sfm7_strength */
        {16, 16, 16, 16}, /* md_wdr_frame_str */
        {16, 16, 16, 16}, /* md_fusion_frame_str */
    },
    /* dering_cfg */
    {
        {
            {64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64}, /* dering_strength */
            {64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64}, /* dering_thresh */
            {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}, /* dering_static_str */
            {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}, /* dering_motion_str */
        },
        {
            64, /* dering_strength */
            64, /* dering_thresh */
            16, /* dering_static_str */
            16, /* dering_motion_str */
        }
    },
};

static const ot_eis_vi_nr_attr g_cmos_bayer_nr_wdr = {
    1,      /* enable */
    0,      /* op_type */
    1,      /* tnr_enable */
    0,      /* lsc_nr_enable */
    0,      /* lsc_ratio1 */
    {
        85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85, 85,
        85, 85, 85, 85, 85, 85, 90, 90, 90, 95, 95, 95, 100, 100, 100, 100, 100
    },   /* coring_ratio */
    {
        128, 128, 128, 128, 128, 128, 110, 90,
        77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77,
        77, 77, 77, 77, 77, 77, 77, 77
    },
    /* snr_cfg */
    {
        0, /* snr_version */
        {
            {
                /* snr auto */
                {
                    {
                        {108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108},
                        {108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108},
                        {108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108},
                        {108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108, 108}
                    },
                    {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}, /* sfm0_detail_prot */
                    {512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512, 512}, /* sfm1_str */
                    {2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2}, /* sfm1_adp_strength */
                    {64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64}, /* sfm6_strength */
                    {64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64}, /* sfm7_strength */
                    {100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100}, /* sth */
                    {128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128}, /* fine_str */
                    {50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50, 50}, /* coring_wgt */
                    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} /* coring_mot_ratio */
                },
                /* snr manual */
                {
                    { 108, 108, 108, 108 }, /* snr_sfm0_coarstr */
                    16, /* sfm0_detail_prot */
                    512, /* sfm1_str */
                    2, /* sfm1_adp_strength */
                    64, /* sfm6_strength */
                    64, /* sfm7_strength */
                    100, /* sth */
                    128, /* fine_str */
                    50, /* coring_wgt */
                    0,  /* coring_mot_ratio */
                },
            },
        }
    },
    /* tnr_cfg */
    {
        {
            /* tnr auto */
            {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1}, /* md_mode */
            {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32}, /* md_size_ratio */
            {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32}, /* md_anti_flicker_str */
            {26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26},   /* md_static_ratio */
            {13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13, 13},   /* md_motion_ratio */
            {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32},   /* md_static_finestr */
            {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255}, /* tnr_tfs */
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* user_define_md */
            {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /* user_define_slope */
            {90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90, 90}, /* user_define_dark_thresh */
            {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}, /* user_define_color_thresh */
            {26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26}, /* sfr_r */
            {32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32}, /* sfr_g */
            {26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26}, /* sfr_b */
        },
        {
            1, /* md_mode */
            32, /* md_size_ratio */
            32, /* md_anti_flicker_str */
            26, /* md_static_ratio */
            13, /* md_motion_ratio */
            32, /* md_static_finestr */
            255, /* tnr_tfs */
            0, /* user_define_md */
            0, /* user_define_bright_thresh */
            90, /* user_define_dark_thresh */
            16, /* user_define_color_thresh */
            26, /* sfr_r */
            32, /* sfr_g */
            26, /* sfr_b */
        }
    },
    /* wdr_cfg */
    {
        {16, 16, 16, 16}, /* sfm0_wdr_frame_str */
        {16, 16, 16, 16}, /* sfm0_fusion_frame_str */
        {64, 16, 16, 16}, /* snr_wdr_sfm6_strength */
        {64, 16, 16, 16}, /* snr_wdr_sfm7_strength */
        {64, 16, 16, 16}, /* snr_fusion_sfm6_strength */
        {64, 16, 16, 16}, /* snr_fusion_sfm7_strength */
        {16, 16, 16, 16}, /* md_wdr_frame_str */
        {16, 16, 16, 16}, /* md_fusion_frame_str */
    },
    /* dering_cfg */
    {
        {
            {64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64}, /* dering_strength */
            {64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64}, /* dering_thresh */
            {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}, /* dering_static_str */
            {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16}, /* dering_motion_str */
        },
        {
            64, /* dering_strength */
            64, /* dering_thresh */
            16, /* dering_static_str */
            16, /* dering_motion_str */
        }
    },
};

static const ot_eis_vi_ldci_attr g_cmos_ldci = {
    /* enable */
    1,
    /* gauss_lpf_sigma */
    36,
    /* ot_op_mode */
    0,

    /* ot_eis_vi_ldci_manual_attr */
    {
        /* ot_eis_vi_ldci_he_wgt_attr */
        {
            /* he_pos_wgt */
            {
                50, 80, 32
            },
            /* he_neg_wgt */
            {
                60, 80, 128
            }
        },

        /* blc_ctrl */
        20
    },

    /* ot_eis_vi_ldci_auto_attr */
    {
        /* he_wgt[OT_EIS_VI_AUTO_ISO_NUM] */
        {
            {{50, 80, 32}, {60, 80, 128}},
            {{45, 80, 16}, {45, 80, 128}},
            {{42, 70, 0}, {45, 80, 128}},
            {{40, 70, 0}, {45, 80, 0}},
            {{35, 70, 0}, {24, 72, 0}},
            {{32, 64, 0}, {12, 64, 0}},
            {{12, 24, 0}, {8, 54, 0}},
            {{8, 20, 0}, {6, 36, 0}},
            {{6, 12, 0}, {0, 8, 0}},
            {{0, 8, 0}, {0, 6, 0}},
            {{0, 6, 0}, {0, 2, 0}},
            {{0, 2, 0}, {0, 1, 0}},
            {{0, 1, 0}, {0, 1, 0}},
            {{0, 1, 0}, {0, 1, 0}},
            {{0, 1, 0}, {0, 1, 0}},
            {{0, 1, 0}, {0, 1, 0}}
        },

        /* blc_ctrl */
        { 20, 20, 20, 20, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 }
    },

    /* tpr_incr_coef */
    8,
    /* tpr_decr_coef */
    8
};

static const ot_eis_vi_ldci_attr g_cmos_ldci_built_in = {
    /* enable */
    1,
    /* gauss_lpf_sigma */
    32,
    /* ot_op_mode */
    0,

    /* ot_eis_vi_ldci_manual_attr */
    {
        /* ot_eis_vi_ldci_he_wgt_attr */
        {
            /* he_pos_wgt */
            {
                32, 32, 32
            },
            /* he_neg_wgt */
            {
                60, 80, 128
            }
        },

        /* blc_ctrl */
        20
    },

    /* ot_eis_vi_ldci_auto_attr */
    {
        /* he_wgt[OT_EIS_VI_AUTO_ISO_NUM] */
        {
            {{48, 80, 32}, {56, 80, 128}},
            {{45, 80, 16}, {52, 80, 128}},
            {{42, 70, 0}, {48, 80, 128}},
            {{40, 70, 0}, {42, 80, 64}},
            {{35, 70, 0}, {36, 80, 0}},
            {{32, 64, 0}, {28, 72, 0}},
            {{12, 24, 0}, {16, 64, 0}},
            {{8, 20, 0}, {8, 54, 0}},
            {{6, 12, 0}, {6, 36, 0}},
            {{0, 8, 0}, {0, 8, 0}},
            {{0, 6, 0}, {0, 6, 0}},
            {{0, 2, 0}, {0, 2, 0}},
            {{0, 1, 0}, {0, 1, 0}},
            {{0, 1, 0}, {0, 1, 0}},
            {{0, 1, 0}, {0, 1, 0}},
            {{0, 1, 0}, {0, 1, 0}}
        },

        /* blc_ctrl */
        { 20, 20, 20, 25, 25, 25, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 }
    },

    /* tpr_incr_coef */
    8,
    /* tpr_decr_coef */
    8
};
static const ot_eis_vi_ldci_attr g_cmos_ldci_wdr = {
    /* enable */
    1,
    /* gauss_lpf_sigma */
    36,
    /* ot_op_mode */
    0,

    /* ot_eis_vi_ldci_manual_attr */
    {
        /* ot_eis_vi_ldci_he_wgt_attr */
        {
            /* he_pos_wgt */
            {
                16, 80, 20
            },
            /* he_neg_wgt */
            {
                32, 80, 180
            }
        },

        /* blc_ctrl */
        20
    },

    /* ot_eis_vi_ldci_auto_attr */
    {
        /* he_wgt[OT_EIS_VI_AUTO_ISO_NUM] */
        {
            {{32, 48, 128}, {32, 80, 200}},
            {{32, 48, 128}, {32, 80, 200}},
            {{32, 48, 128}, {32, 80, 200}},
            {{32, 48, 128}, {32, 80, 200}},
            {{32, 48, 128}, {32, 80, 200}},
            {{32, 48, 128}, {32, 80, 200}},
            {{32, 48, 128}, {32, 80, 200}},
            {{32, 48, 128}, {32, 80, 200}},
            {{32, 48, 128}, {32, 80, 200}},
            {{32, 48, 128}, {32, 80, 200}},
            {{32, 48, 128}, {32, 80, 200}},
            {{32, 48, 128}, {32, 80, 200}},
            {{32, 48, 128}, {32, 80, 200}},
            {{32, 48, 128}, {32, 80, 200}},
            {{32, 48, 128}, {32, 80, 200}},
            {{32, 48, 128}, {32, 80, 200}}
        },

        /* blc_ctrl */
        { 20,  20,  20,   20, 20,  20,  20,  20,  20,  20,   20,  20,  20,  20,  20,  20 }
    },

    /* tpr_incr_coef */
    8,
    /* tpr_decr_coef */
    8
};

static const ot_eis_vi_gamma_attr g_cmos_gamma = {

    1,
    {0,6,11,17,24,30,37,43,50,57,65,72,80,88,96,104,112,121,
        130,138,147,156,166,175,185,194,204,214,224,234,245,255,266,277,287,298,
        309,320,332,343,354,366,378,389,401,413,425,437,449,461,473,486,498,510,
        523,535,548,561,573,586,599,612,625,637,650,663,676,689,702,715,728,741,
        754,767,780,793,806,819,832,845,858,871,884,897,910,923,936,948,961,974,
        987,999,1012,1024,1037,1049,1062,1074,1086,1098,1110,1122,1134,1146,1158,1170,1181,1193,
        1204,1215,1227,1238,1249,1260,1270,1281,1292,1302,1312,1322,1333,1342,1352,1362,1371,1381,
        1390,1399,1408,1417,1426,1434,1443,1452,1460,1469,1478,1486,1494,1503,1511,1520,1528,1536,
        1544,1553,1561,1569,1577,1585,1593,1601,1609,1617,1624,1632,1640,1648,1655,1663,1671,1678,
        1686,1693,1701,1708,1716,1723,1730,1738,1745,1752,1759,1767,1774,1781,1788,1795,1802,1809,
        1816,1823,1830,1836,1843,1850,1857,1863,1870,1877,1883,1890,1897,1903,1910,1916,1923,1929,
        1936,1942,1948,1955,1961,1967,1973,1980,1986,1992,1998,2004,2010,2017,2023,2029,2035,2041,
        2047,2052,2058,2064,2070,2076,2082,2088,2093,2099,2105,2111,2116,2122,2128,2133,2139,2144,
        2150,2155,2161,2167,2172,2177,2183,2188,2194,2199,2205,2210,2215,2221,2226,2231,2237,2242,
        2247,2252,2258,2263,2268,2273,2278,2283,2288,2293,2298,2303,2308,2313,2318,2323,2328,2332,
        2337,2342,2346,2351,2355,2360,2364,2369,2373,2378,2382,2386,2390,2395,2399,2403,2407,2411,
        2416,2420,2424,2428,2432,2436,2440,2444,2448,2451,2455,2459,2463,2467,2470,2474,2478,2482,
        2485,2489,2493,2496,2500,2503,2507,2510,2514,2517,2521,2524,2528,2531,2535,2538,2541,2545,
        2548,2551,2555,2558,2561,2565,2568,2571,2574,2578,2581,2584,2587,2590,2594,2597,2600,2603,
        2606,2609,2613,2616,2619,2622,2625,2628,2631,2634,2637,2641,2644,2647,2650,2653,2656,2659,
        2662,2665,2668,2671,2675,2678,2681,2684,2687,2690,2693,2696,2699,2702,2706,2709,2712,2715,
        2718,2721,2724,2727,2731,2734,2737,2740,2743,2747,2750,2753,2756,2759,2762,2765,2768,2771,
        2775,2778,2781,2784,2787,2790,2793,2796,2799,2802,2805,2808,2811,2814,2816,2819,2822,2825,
        2828,2831,2834,2837,2840,2843,2845,2848,2851,2854,2857,2860,2862,2865,2868,2871,2874,2876,
        2879,2882,2885,2887,2890,2893,2896,2898,2901,2904,2906,2909,2912,2914,2917,2920,2922,2925,
        2928,2930,2933,2936,2938,2941,2944,2946,2949,2951,2954,2957,2959,2962,2964,2967,2969,2972,
        2975,2977,2980,2982,2985,2987,2990,2992,2995,2997,3000,3002,3005,3007,3010,3012,3015,3017,
        3020,3022,3025,3027,3030,3032,3035,3037,3040,3042,3045,3047,3050,3052,3055,3057,3060,3062,
        3064,3067,3069,3072,3074,3077,3079,3082,3084,3086,3089,3091,3094,3096,3099,3101,3103,3106,
        3108,3110,3113,3115,3118,3120,3122,3125,3127,3129,3132,3134,3136,3139,3141,3143,3145,3148,
        3150,3152,3155,3157,3159,3161,3164,3166,3168,3170,3173,3175,3177,3179,3181,3184,3186,3188,
        3190,3193,3195,3197,3199,3201,3203,3206,3208,3210,3212,3214,3216,3219,3221,3223,3225,3227,
        3229,3231,3234,3236,3238,3240,3242,3244,3246,3248,3251,3253,3255,3257,3259,3261,3263,3265,
        3267,3269,3271,3274,3276,3278,3280,3282,3284,3286,3288,3290,3292,3294,3296,3298,3300,3303,
        3305,3307,3309,3311,3313,3315,3317,3319,3321,3323,3325,3327,3329,3331,3333,3335,3337,3339,
        3342,3344,3346,3348,3350,3352,3354,3356,3358,3360,3362,3364,3366,3368,3370,3372,3374,3376,
        3378,3380,3382,3384,3386,3388,3390,3392,3394,3396,3398,3400,3402,3404,3406,3408,3410,3412,
        3414,3416,3418,3420,3422,3424,3426,3427,3429,3431,3433,3435,3437,3439,3441,3443,3445,3447,
        3449,3451,3452,3454,3456,3458,3460,3462,3464,3466,3468,3470,3471,3473,3475,3477,3479,3481,
        3483,3485,3487,3488,3490,3492,3494,3496,3498,3500,3502,3503,3505,3507,3509,3511,3513,3515,
        3517,3518,3520,3522,3524,3526,3528,3530,3532,3533,3535,3537,3539,3541,3543,3545,3547,3549,
        3550,3552,3554,3556,3558,3560,3562,3564,3566,3568,3569,3571,3573,3575,3577,3579,3581,3583,
        3585,3587,3589,3591,3592,3594,3596,3598,3600,3602,3604,3606,3608,3610,3612,3614,3616,3618,
        3620,3622,3624,3626,3628,3630,3632,3634,3636,3638,3640,3641,3643,3645,3647,3649,3651,3653,
        3655,3657,3659,3661,3663,3665,3667,3669,3671,3673,3675,3677,3679,3681,3683,3685,3687,3689,
        3692,3694,3696,3698,3700,3702,3704,3706,3708,3710,3712,3714,3716,3718,3720,3722,3724,3726,
        3728,3730,3732,3734,3736,3738,3740,3742,3744,3746,3748,3750,3752,3754,3756,3758,3760,3762,
        3764,3766,3768,3770,3772,3774,3776,3778,3780,3782,3784,3786,3788,3790,3792,3794,3796,3798,
        3800,3802,3804,3806,3808,3810,3812,3814,3816,3818,3819,3821,3823,3825,3827,3829,3831,3833,
        3835,3837,3839,3841,3843,3845,3847,3849,3851,3852,3854,3856,3858,3860,3862,3864,3866,3868,
        3870,3872,3873,3875,3877,3879,3881,3883,3885,3887,3888,3890,3892,3894,3896,3898,3900,3902,
        3904,3905,3907,3909,3911,3913,3915,3917,3919,3920,3922,3924,3926,3928,3930,3932,3934,3935,
        3937,3939,3941,3943,3945,3947,3949,3950,3952,3954,3955,3957,3959,3961,3963,3965,3967,3969,
        3971,3972,3974,3976,3977,3979,3981,3983,3985,3987,3989,3991,3993,3994,3996,3998,4000,4001,
        4003,4005,4006,4008,4010,4012,4014,4016,4018,4020,4022,4023,4025,4027,4028,4030,4032,4034,
        4036,4037,4039,4041,4042,4044,4046,4048,4050,4052,4054,4056,4058,4059,4061,4063,4064,4066,
        4068,4070,4072,4073,4075,4077,4078,4080,4082,4084,4086,4087,4089,4091,4092,4094,4095
    },
    3,
};

static const ot_eis_vi_gamma_attr g_cmos_gamma_wdr = {
    1,
    {0,    5,   10,   15,   20,   25,   30,   35,   40,   44,   49,   54,   59,   64,   69,   74,   79,   84,
        89,   94,   99,  103,  108,  113,  118,  123,  128,  133,  138,  142,  147,  152,  157,  162,  167,  171,
        176,  181,  186,  191,  196,  200,  205,  210,  215,  220,  224,  229,  234,  239,  244,  249,  253,  258,
        263,  268,  273,  278,  283,  287,  292,  297,  302,  307,  312,  317,  321,  326,  331,  336,  341,  346,
        351,  356,  361,  366,  371,  376,  381,  386,  391,  396,  401,  406,  411,  416,  421,  426,  431,  436,
        441,  446,  451,  456,  461,  466,  472,  477,  482,  487,  492,  498,  503,  508,  513,  519,  524,  529,
        535,  540,  545,  551,  556,  561,  567,  572,  578,  583,  589,  594,  600,  605,  611,  616,  622,  628,
        633,  639,  645,  650,  656,  662,  667,  673,  679,  685,  691,  697,  702,  708,  714,  720,  726,  732,
        738,  744,  750,  756,  762,  768,  774,  780,  787,  793,  799,  805,  811,  817,  824,  830,  836,  842,
        849,  855,  861,  867,  874,  880,  886,  893,  899,  905,  912,  918,  925,  931,  937,  944,  950,  957,
        963,  970,  976,  982,  989,  995, 1002, 1008, 1015, 1021, 1028, 1034, 1041, 1047, 1054, 1060, 1067, 1074,
        1080, 1087, 1093, 1100, 1106, 1113, 1119, 1126, 1132, 1139, 1145, 1152, 1159, 1165, 1172, 1178, 1185, 1191,
        1198, 1204, 1211, 1217, 1224, 1230, 1237, 1244, 1250, 1257, 1263, 1270, 1276, 1283, 1289, 1295, 1302, 1308,
        1315, 1321, 1328, 1334, 1341, 1347, 1353, 1360, 1366, 1373, 1379, 1385, 1392, 1398, 1404, 1411, 1417, 1423,
        1430, 1436, 1442, 1448, 1455, 1461, 1467, 1473, 1479, 1486, 1492, 1498, 1505, 1511, 1517, 1524, 1530, 1536,
        1543, 1549, 1555, 1562, 1568, 1575, 1581, 1587, 1594, 1600, 1607, 1613, 1620, 1626, 1632, 1639, 1645, 1652,
        1658, 1665, 1671, 1678, 1684, 1690, 1697, 1703, 1710, 1716, 1723, 1729, 1736, 1742, 1749, 1755, 1761, 1768,
        1774, 1781, 1787, 1794, 1800, 1806, 1813, 1819, 1826, 1832, 1838, 1845, 1851, 1857, 1864, 1870, 1876, 1883,
        1889, 1895, 1902, 1908, 1914, 1920, 1927, 1933, 1939, 1945, 1952, 1958, 1964, 1970, 1976, 1982, 1988, 1995,
        2001, 2007, 2013, 2019, 2025, 2031, 2037, 2043, 2049, 2055, 2060, 2066, 2072, 2078, 2084, 2090, 2095, 2101,
        2107, 2113, 2118, 2124, 2130, 2135, 2141, 2147, 2152, 2158, 2163, 2169, 2174, 2180, 2185, 2190, 2196, 2201,
        2206, 2212, 2217, 2222, 2227, 2232, 2238, 2243, 2248, 2253, 2258, 2263, 2268, 2273, 2278, 2283, 2287, 2292,
        2297, 2302, 2307, 2312, 2316, 2321, 2326, 2330, 2335, 2340, 2344, 2349, 2354, 2358, 2363, 2367, 2372, 2376,
        2381, 2385, 2390, 2394, 2398, 2403, 2407, 2411, 2416, 2420, 2424, 2429, 2433, 2437, 2441, 2446, 2450, 2454,
        2458, 2462, 2467, 2471, 2475, 2479, 2483, 2487, 2491, 2495, 2499, 2503, 2507, 2511, 2515, 2519, 2523, 2527,
        2531, 2535, 2539, 2543, 2547, 2551, 2555, 2558, 2562, 2566, 2570, 2574, 2578, 2582, 2585, 2589, 2593, 2597,
        2601, 2604, 2608, 2612, 2616, 2619, 2623, 2627, 2631, 2634, 2638, 2642, 2646, 2649, 2653, 2657, 2660, 2664,
        2668, 2671, 2675, 2679, 2683, 2686, 2690, 2694, 2697, 2701, 2705, 2708, 2712, 2716, 2719, 2723, 2727, 2730,
        2734, 2738, 2741, 2745, 2749, 2752, 2756, 2760, 2764, 2768, 2771, 2775, 2779, 2782, 2786, 2789, 2793, 2796,
        2800, 2804, 2807, 2811, 2814, 2818, 2821, 2825, 2828, 2832, 2835, 2839, 2842, 2846, 2849, 2853, 2856, 2860,
        2863, 2867, 2870, 2873, 2877, 2880, 2884, 2887, 2891, 2894, 2897, 2901, 2904, 2908, 2911, 2914, 2918, 2921,
        2924, 2928, 2931, 2934, 2938, 2941, 2944, 2948, 2951, 2954, 2958, 2961, 2964, 2967, 2971, 2974, 2977, 2981,
        2984, 2987, 2990, 2994, 2997, 3000, 3003, 3006, 3010, 3013, 3016, 3019, 3023, 3026, 3029, 3032, 3035, 3038,
        3042, 3045, 3048, 3051, 3054, 3057, 3061, 3064, 3067, 3070, 3073, 3076, 3079, 3082, 3086, 3089, 3092, 3095,
        3098, 3101, 3104, 3107, 3110, 3113, 3116, 3119, 3122, 3125, 3129, 3132, 3135, 3138, 3141, 3144, 3147, 3150,
        3153, 3156, 3159, 3162, 3165, 3168, 3171, 3174, 3177, 3180, 3183, 3185, 3188, 3191, 3194, 3197, 3200, 3203,
        3206, 3209, 3212, 3215, 3218, 3221, 3224, 3227, 3229, 3232, 3235, 3238, 3241, 3244, 3247, 3250, 3252, 3255,
        3258, 3261, 3264, 3267, 3270, 3272, 3275, 3278, 3281, 3284, 3287, 3289, 3292, 3295, 3298, 3301, 3303, 3306,
        3309, 3312, 3315, 3317, 3320, 3323, 3326, 3328, 3331, 3334, 3337, 3339, 3342, 3345, 3348, 3350, 3353, 3356,
        3359, 3361, 3364, 3367, 3370, 3372, 3375, 3378, 3380, 3383, 3386, 3388, 3391, 3394, 3396, 3399, 3402, 3405,
        3407, 3410, 3413, 3415, 3418, 3420, 3423, 3426, 3428, 3431, 3434, 3436, 3439, 3442, 3444, 3447, 3449, 3452,
        3455, 3457, 3460, 3462, 3465, 3468, 3470, 3473, 3475, 3478, 3481, 3483, 3486, 3488, 3491, 3493, 3496, 3498,
        3501, 3504, 3506, 3509, 3511, 3514, 3516, 3519, 3521, 3524, 3526, 3529, 3531, 3534, 3536, 3539, 3541, 3544,
        3546, 3549, 3551, 3554, 3556, 3559, 3561, 3564, 3566, 3569, 3571, 3574, 3576, 3579, 3581, 3584, 3586, 3588,
        3591, 3593, 3596, 3598, 3601, 3603, 3606, 3608, 3610, 3613, 3615, 3618, 3620, 3622, 3625, 3627, 3630, 3632,
        3634, 3637, 3639, 3642, 3644, 3646, 3649, 3651, 3654, 3656, 3658, 3661, 3663, 3665, 3668, 3670, 3673, 3675,
        3677, 3680, 3682, 3684, 3687, 3689, 3691, 3694, 3696, 3698, 3701, 3703, 3705, 3708, 3710, 3712, 3714, 3717,
        3719, 3721, 3724, 3726, 3728, 3731, 3733, 3735, 3737, 3740, 3742, 3744, 3747, 3749, 3751, 3753, 3756, 3758,
        3760, 3762, 3765, 3767, 3769, 3771, 3774, 3776, 3778, 3780, 3783, 3785, 3787, 3789, 3792, 3794, 3796, 3798,
        3801, 3803, 3805, 3807, 3809, 3812, 3814, 3816, 3818, 3820, 3823, 3825, 3827, 3829, 3831, 3834, 3836, 3838,
        3840, 3842, 3844, 3847, 3849, 3851, 3853, 3855, 3857, 3860, 3862, 3864, 3866, 3868, 3870, 3873, 3875, 3877,
        3879, 3881, 3883, 3885, 3888, 3890, 3892, 3894, 3896, 3898, 3900, 3902, 3905, 3907, 3909, 3911, 3913, 3915,
        3917, 3919, 3921, 3924, 3926, 3928, 3930, 3932, 3934, 3936, 3938, 3940, 3942, 3944, 3946, 3949, 3951, 3953,
        3955, 3957, 3959, 3961, 3963, 3965, 3967, 3969, 3971, 3973, 3975, 3977, 3979, 3981, 3984, 3986, 3988, 3990,
        3992, 3994, 3996, 3998, 4000, 4002, 4004, 4006, 4008, 4010, 4012, 4014, 4016, 4018, 4020, 4022, 4024, 4026,
        4028, 4030, 4032, 4034, 4036, 4038, 4040, 4042, 4044, 4046, 4048, 4050, 4052, 4054, 4056, 4058, 4060, 4062,
        4064, 4066, 4068, 4070, 4072, 4074, 4075, 4077, 4079, 4081, 4083, 4085, 4087, 4089, 4091, 4093, 4095,
    },
    3,
};

static const ot_eis_vi_sharpen_attr g_cmos_yuv_sharpen = {
    /* enable */
    1,
    /* motion_en */
    0,
    /* motion_threshold0 */
    4,
    /* motion_threshold1 */
    10,
    /* motion_gain0 */
    0,
    /* motion_gain1 */
    255,
    /* skin_umin */
    100,
    /* skin_vmin */
    135,
    /* skin_umax */
    128,
    /* skin_vmax */
    150,
    /* op_type */
    OT_EIS_OP_MODE_AUTO,
    /* detail_map */
    OT_EIS_VI_SHARPEN_NORMAL,
    /* manual para */
    {
        /* luma_wgt[OT_EIS_VI_SHARPEN_LUMA_NUM] */
        {
            31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
            31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31
        },
        /* texture_strength[OT_EIS_VI_SHARPEN_GAIN_NUM] */
        {
            250, 420, 390, 390, 390, 390, 390, 370, 350, 330, 310, 290, 270, 270, 270, 270,
            270, 270, 266, 260, 244, 230, 230, 230, 230, 230, 230, 210, 190, 190, 170, 150
        },
        /* edge_strength[OT_EIS_VI_SHARPEN_GAIN_NUM] */
        {
            120, 123, 125, 128, 130, 135, 140, 148, 160, 168, 180, 190, 200, 210, 210, 210,
            210, 210, 200, 190, 185, 175, 165, 160, 146, 136, 130, 128, 125, 123, 120, 120
        },
        /* texture_freq */
        190,
        /* edge_freq */
        128,
        /* over_shoot */
        65,
        /* under_shoot */
        75,
        /* motion_texture_strength[OT_EIS_VI_SHARPEN_GAIN_NUM] */
        {
            250, 420, 390, 390, 390, 390, 390, 370, 350, 330, 310, 290, 270, 270, 270, 270,
            270, 270, 266, 260, 244, 230, 230, 230, 230, 230, 230, 210, 190, 190, 170, 150
        },
        /* motion_edge_strength[OT_EIS_VI_SHARPEN_GAIN_NUM] */
        {
            120, 123, 125, 128, 130, 135, 140, 148, 160, 168, 180, 190, 200, 210, 210, 210,
            210, 210, 200, 190, 185, 175, 165, 160, 146, 136, 130, 128, 125, 123, 120, 120
        },
        /* motion_texture_freq */
        190,
        /* motion_edge_freq */
        128,
        /* motion_over_shoot */
        65,
        /* motion_under_shoot */
        75,
        /* u8shoot_sup_str */
        7,
        /* u8shoot_sup_adj */
        9,
        /* detail_ctrl */
        128,
        /* detail_ctrl_threshold */
        180,
        /* edge_filt_strength */
        60,
        /* edge_filt_max_cap */
        18,
        /* r_gain */
        20,
        /* g_gain */
        32,
        /* b_gain */
        20,
        /* skin_gain */
        25,
        /* max_sharp_gain */
        40,
    },
    /* auto para */
    {
        /* luma_wgt[OT_EIS_VI_SHARPEN_GAIN_NUM][OT_EIS_VI_AUTO_ISO_NUM] */
        {
            { 31, 31, 20, 20, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 },
            { 31, 31, 20, 20, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 },
            { 31, 31, 20, 20, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 },
            { 31, 31, 20, 20, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 },
            { 31, 31, 20, 20, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 },
            { 31, 31, 20, 20, 17, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 },
            { 31, 31, 20, 20, 20, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18 },
            { 31, 31, 23, 23, 23, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21 },
            { 31, 31, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 },
            { 31, 31, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28 },
            { 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 28, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 }
        },
        /* texture_strength[OT_EIS_VI_SHARPEN_GAIN_NUM][OT_EIS_VI_AUTO_ISO_NUM] */
        {
            { 153, 153, 160, 145, 130, 130, 130, 105, 105, 105, 105, 85, 85, 85, 85, 85 },
            { 172, 167, 190, 170, 145, 145, 145, 120, 120, 120, 120, 100, 100, 100, 100, 100 },
            { 186, 178, 220, 195, 165, 165, 160, 140, 140, 135, 135, 120, 120, 120, 120, 120 },
            { 201, 192, 255, 215, 180, 180, 175, 160, 160, 150, 150, 135, 135, 140, 140, 140 },
            { 217, 207, 275, 230, 200, 200, 185, 175, 175, 160, 160, 150, 150, 160, 160, 160 },
            { 233, 223, 285, 245, 210, 215, 200, 190, 190, 170, 170, 165, 165, 180, 180, 180 },
            { 251, 239, 280, 260, 225, 225, 215, 205, 205, 180, 180, 185, 185, 200, 200, 200 },
            { 269, 255, 275, 265, 240, 240, 230, 220, 220, 190, 190, 200, 200, 210, 210, 210 },
            { 286, 272, 270, 270, 250, 250, 240, 225, 225, 200, 200, 220, 220, 220, 220, 220 },
            { 300, 288, 265, 270, 250, 260, 245, 235, 235, 210, 210, 235, 235, 230, 230, 230 },
            { 308, 299, 260, 265, 250, 270, 255, 245, 245, 220, 220, 245, 245, 230, 230, 230 },
            { 312, 307, 260, 260, 250, 275, 260, 250, 250, 230, 230, 255, 255, 230, 230, 230 },
            { 313, 310, 255, 255, 245, 280, 265, 255, 255, 235, 235, 265, 265, 230, 230, 230 },
            { 311, 311, 250, 250, 240, 285, 270, 255, 255, 240, 240, 275, 275, 230, 230, 230 },
            { 306, 311, 245, 245, 240, 290, 275, 250, 255, 245, 245, 280, 280, 230, 230, 230 },
            { 297, 308, 240, 240, 240, 295, 280, 240, 250, 250, 250, 285, 285, 230, 230, 230 },
            { 285, 301, 235, 235, 235, 295, 280, 235, 250, 250, 255, 290, 290, 230, 230, 230 },
            { 273, 291, 230, 230, 230, 290, 280, 230, 250, 250, 260, 295, 295, 230, 230, 230 },
            { 263, 281, 225, 225, 225, 285, 275, 225, 245, 250, 265, 300, 300, 225, 225, 225 },
            { 255, 271, 225, 220, 220, 275, 270, 225, 245, 250, 270, 300, 300, 220, 220, 220 },
            { 249, 259, 220, 215, 215, 270, 265, 225, 240, 245, 275, 300, 300, 210, 210, 210 },
            { 243, 248, 220, 210, 210, 260, 260, 220, 235, 245, 280, 305, 305, 200, 200, 200 },
            { 234, 238, 215, 205, 205, 255, 255, 220, 230, 240, 280, 305, 305, 190, 190, 190 },
            { 224, 228, 215, 200, 200, 245, 245, 220, 230, 240, 285, 310, 310, 180, 180, 180 },
            { 216, 218, 210, 195, 195, 235, 240, 215, 225, 235, 285, 310, 310, 175, 175, 175 },
            { 208, 208, 200, 190, 190, 230, 235, 215, 220, 230, 285, 305, 305, 170, 170, 170 },
            { 201, 201, 195, 180, 180, 220, 225, 210, 220, 225, 285, 305, 305, 160, 160, 160 },
            { 195, 195, 185, 175, 175, 205, 220, 205, 215, 220, 285, 300, 300, 160, 160, 160 },
            { 188, 188, 175, 170, 170, 195, 210, 195, 210, 215, 280, 300, 300, 150, 150, 150 },
            { 181, 181, 170, 160, 160, 180, 205, 195, 205, 210, 280, 300, 300, 145, 145, 145 },
            { 175, 175, 160, 155, 155, 165, 195, 195, 200, 205, 275, 300, 300, 130, 130, 130 },
            { 171, 171, 150, 150, 150, 150, 150, 150, 195, 200, 270, 300, 300, 115, 115, 115 }
        },
        /* edge_strength */
        {
            { 195, 195, 195, 195, 195, 195, 400, 440, 500, 475, 475, 475, 475, 475, 475, 475 },
            { 210, 210, 200, 200, 200, 200, 400, 440, 500, 485, 485, 485, 485, 485, 485, 485 },
            { 235, 235, 220, 220, 220, 220, 400, 440, 500, 495, 495, 495, 495, 495, 495, 495 },
            { 260, 260, 230, 230, 230, 230, 400, 440, 500, 500, 500, 500, 500, 500, 500, 500 },
            { 295, 295, 240, 240, 240, 240, 400, 440, 500, 510, 510, 510, 510, 510, 510, 510 },
            { 320, 320, 250, 250, 250, 250, 400, 440, 500, 520, 520, 520, 520, 520, 520, 520 },
            { 340, 340, 260, 260, 260, 260, 420, 460, 540, 530, 530, 530, 530, 530, 530, 530 },
            { 350, 350, 270, 270, 270, 270, 420, 460, 540, 540, 540, 540, 540, 540, 540, 540 },
            { 360, 360, 280, 280, 280, 280, 420, 460, 540, 545, 545, 545, 545, 545, 545, 545 },
            { 360, 360, 280, 280, 280, 280, 420, 460, 540, 550, 550, 550, 550, 550, 550, 550 },
            { 355, 355, 285, 285, 285, 285, 420, 460, 540, 560, 560, 560, 560, 560, 560, 560 },
            { 345, 345, 290, 290, 290, 290, 420, 460, 540, 565, 565, 565, 565, 565, 565, 565 },
            { 335, 335, 290, 290, 290, 290, 420, 460, 540, 570, 570, 570, 570, 570, 570, 570 },
            { 330, 330, 295, 295, 295, 295, 430, 470, 550, 575, 575, 575, 575, 575, 575, 575 },
            { 325, 325, 295, 295, 295, 295, 430, 470, 550, 580, 580, 580, 580, 580, 580, 580 },
            { 320, 320, 295, 295, 295, 295, 440, 470, 550, 575, 575, 575, 575, 575, 575, 575 },
            { 315, 315, 295, 295, 295, 295, 440, 470, 550, 570, 570, 570, 570, 570, 570, 570 },
            { 310, 310, 295, 295, 295, 295, 450, 480, 580, 565, 565, 565, 565, 565, 565, 565 },
            { 305, 305, 295, 295, 295, 295, 460, 500, 580, 560, 560, 560, 560, 560, 560, 560 },
            { 295, 295, 290, 290, 290, 290, 460, 500, 580, 555, 555, 555, 555, 555, 555, 555 },
            { 285, 285, 285, 285, 285, 285, 460, 500, 580, 550, 550, 550, 550, 550, 550, 550 },
            { 280, 280, 280, 280, 280, 280, 460, 500, 580, 540, 540, 540, 540, 540, 540, 540 },
            { 275, 275, 275, 275, 275, 275, 460, 500, 580, 535, 535, 535, 535, 535, 535, 535 },
            { 265, 265, 265, 265, 265, 265, 460, 500, 580, 530, 530, 530, 530, 530, 530, 530 },
            { 250, 250, 250, 250, 250, 250, 460, 500, 580, 520, 520, 520, 520, 520, 520, 520 },
            { 240, 240, 240, 240, 240, 240, 470, 500, 580, 515, 515, 515, 515, 515, 515, 515 },
            { 220, 220, 220, 220, 220, 220, 470, 500, 580, 510, 510, 510, 510, 510, 510, 510 },
            { 200, 200, 200, 200, 200, 200, 470, 500, 580, 500, 500, 500, 500, 500, 500, 500 },
            { 180, 180, 180, 180, 180, 180, 480, 500, 580, 495, 495, 495, 495, 495, 495, 495 },
            { 160, 160, 160, 160, 160, 160, 480, 500, 580, 485, 485, 485, 485, 485, 485, 485 },
            { 140, 140, 140, 140, 140, 140, 480, 500, 580, 480, 480, 480, 480, 480, 480, 480 },
            { 125, 125, 125, 125, 125, 125, 480, 500, 580, 470, 470, 470, 470, 470, 470, 470 }
        },
        /* texture_freq[OT_EIS_VI_AUTO_ISO_NUM] */
        { 180, 190, 190, 170, 170, 170, 160, 140, 128, 128, 128, 100, 100, 100, 100, 100 },
        /* edge_freq[OT_EIS_VI_AUTO_ISO_NUM] */
        { 128, 100, 100, 100, 100, 100, 100, 100, 100, 100, 96, 96, 96, 96, 96, 96 },
        /* over_shoot[OT_EIS_VI_AUTO_ISO_NUM] */
        { 60, 62, 62, 64, 64, 64, 56, 52, 50, 50, 30, 10, 10, 10, 10, 10 },
        /* under_shoot[OT_EIS_VI_AUTO_ISO_NUM] */
        { 60, 62, 62, 64, 64, 64, 54, 50, 50, 30, 20, 20, 15, 15, 15, 15 },
        /* motion_texture_strength[OT_EIS_VI_SHARPEN_GAIN_NUM][OT_EIS_VI_AUTO_ISO_NUM] */
        {
            { 153, 153, 160, 145, 130, 130, 130, 105, 105, 105, 105, 85, 85, 85, 85, 85 },
            { 172, 167, 190, 170, 145, 145, 145, 120, 120, 120, 120, 100, 100, 100, 100, 100 },
            { 186, 178, 220, 195, 165, 165, 160, 140, 140, 135, 135, 120, 120, 120, 120, 120 },
            { 201, 192, 255, 215, 180, 180, 175, 160, 160, 150, 150, 135, 135, 140, 140, 140 },
            { 217, 207, 275, 230, 200, 200, 185, 175, 175, 160, 160, 150, 150, 160, 160, 160 },
            { 233, 223, 285, 245, 210, 215, 200, 190, 190, 170, 170, 165, 165, 180, 180, 180 },
            { 251, 239, 280, 260, 225, 225, 215, 205, 205, 180, 180, 185, 185, 200, 200, 200 },
            { 269, 255, 275, 265, 240, 240, 230, 220, 220, 190, 190, 200, 200, 210, 210, 210 },
            { 286, 272, 270, 270, 250, 250, 240, 225, 225, 200, 200, 220, 220, 220, 220, 220 },
            { 300, 288, 265, 270, 250, 260, 245, 235, 235, 210, 210, 235, 235, 230, 230, 230 },
            { 308, 299, 260, 265, 250, 270, 255, 245, 245, 220, 220, 245, 245, 230, 230, 230 },
            { 312, 307, 260, 260, 250, 275, 260, 250, 250, 230, 230, 255, 255, 230, 230, 230 },
            { 313, 310, 255, 255, 245, 280, 265, 255, 255, 235, 235, 265, 265, 230, 230, 230 },
            { 311, 311, 250, 250, 240, 285, 270, 255, 255, 240, 240, 275, 275, 230, 230, 230 },
            { 306, 311, 245, 245, 240, 290, 275, 250, 255, 245, 245, 280, 280, 230, 230, 230 },
            { 297, 308, 240, 240, 240, 295, 280, 240, 250, 250, 250, 285, 285, 230, 230, 230 },
            { 285, 301, 235, 235, 235, 295, 280, 235, 250, 250, 255, 290, 290, 230, 230, 230 },
            { 273, 291, 230, 230, 230, 290, 280, 230, 250, 250, 260, 295, 295, 230, 230, 230 },
            { 263, 281, 225, 225, 225, 285, 275, 225, 245, 250, 265, 300, 300, 225, 225, 225 },
            { 255, 271, 225, 220, 220, 275, 270, 225, 245, 250, 270, 300, 300, 220, 220, 220 },
            { 249, 259, 220, 215, 215, 270, 265, 225, 240, 245, 275, 300, 300, 210, 210, 210 },
            { 243, 248, 220, 210, 210, 260, 260, 220, 235, 245, 280, 305, 305, 200, 200, 200 },
            { 234, 238, 215, 205, 205, 255, 255, 220, 230, 240, 280, 305, 305, 190, 190, 190 },
            { 224, 228, 215, 200, 200, 245, 245, 220, 230, 240, 285, 310, 310, 180, 180, 180 },
            { 216, 218, 210, 195, 195, 235, 240, 215, 225, 235, 285, 310, 310, 175, 175, 175 },
            { 208, 208, 200, 190, 190, 230, 235, 215, 220, 230, 285, 305, 305, 170, 170, 170 },
            { 201, 201, 195, 180, 180, 220, 225, 210, 220, 225, 285, 305, 305, 160, 160, 160 },
            { 195, 195, 185, 175, 175, 205, 220, 205, 215, 220, 285, 300, 300, 160, 160, 160 },
            { 188, 188, 175, 170, 170, 195, 210, 195, 210, 215, 280, 300, 300, 150, 150, 150 },
            { 181, 181, 170, 160, 160, 180, 205, 195, 205, 210, 280, 300, 300, 145, 145, 145 },
            { 175, 175, 160, 155, 155, 165, 195, 195, 200, 205, 275, 300, 300, 130, 130, 130 },
            { 171, 171, 150, 150, 150, 150, 150, 150, 195, 200, 270, 300, 300, 115, 115, 115 }
        },
        /* motion_edge_strength     */
        {
            { 195, 195, 195, 195, 195, 195, 400, 440, 500, 475, 475, 475, 475, 475, 475, 475 },
            { 210, 210, 200, 200, 200, 200, 400, 440, 500, 485, 485, 485, 485, 485, 485, 485 },
            { 235, 235, 220, 220, 220, 220, 400, 440, 500, 495, 495, 495, 495, 495, 495, 495 },
            { 260, 260, 230, 230, 230, 230, 400, 440, 500, 500, 500, 500, 500, 500, 500, 500 },
            { 295, 295, 240, 240, 240, 240, 400, 440, 500, 510, 510, 510, 510, 510, 510, 510 },
            { 320, 320, 250, 250, 250, 250, 400, 440, 500, 520, 520, 520, 520, 520, 520, 520 },
            { 340, 340, 260, 260, 260, 260, 420, 460, 540, 530, 530, 530, 530, 530, 530, 530 },
            { 350, 350, 270, 270, 270, 270, 420, 460, 540, 540, 540, 540, 540, 540, 540, 540 },
            { 360, 360, 280, 280, 280, 280, 420, 460, 540, 545, 545, 545, 545, 545, 545, 545 },
            { 360, 360, 280, 280, 280, 280, 420, 460, 540, 550, 550, 550, 550, 550, 550, 550 },
            { 355, 355, 285, 285, 285, 285, 420, 460, 540, 560, 560, 560, 560, 560, 560, 560 },
            { 345, 345, 290, 290, 290, 290, 420, 460, 540, 565, 565, 565, 565, 565, 565, 565 },
            { 335, 335, 290, 290, 290, 290, 420, 460, 540, 570, 570, 570, 570, 570, 570, 570 },
            { 330, 330, 295, 295, 295, 295, 430, 470, 550, 575, 575, 575, 575, 575, 575, 575 },
            { 325, 325, 295, 295, 295, 295, 430, 470, 550, 580, 580, 580, 580, 580, 580, 580 },
            { 320, 320, 295, 295, 295, 295, 440, 470, 550, 575, 575, 575, 575, 575, 575, 575 },
            { 315, 315, 295, 295, 295, 295, 440, 470, 550, 570, 570, 570, 570, 570, 570, 570 },
            { 310, 310, 295, 295, 295, 295, 450, 480, 580, 565, 565, 565, 565, 565, 565, 565 },
            { 305, 305, 295, 295, 295, 295, 460, 500, 580, 560, 560, 560, 560, 560, 560, 560 },
            { 295, 295, 290, 290, 290, 290, 460, 500, 580, 555, 555, 555, 555, 555, 555, 555 },
            { 285, 285, 285, 285, 285, 285, 460, 500, 580, 550, 550, 550, 550, 550, 550, 550 },
            { 280, 280, 280, 280, 280, 280, 460, 500, 580, 540, 540, 540, 540, 540, 540, 540 },
            { 275, 275, 275, 275, 275, 275, 460, 500, 580, 535, 535, 535, 535, 535, 535, 535 },
            { 265, 265, 265, 265, 265, 265, 460, 500, 580, 530, 530, 530, 530, 530, 530, 530 },
            { 250, 250, 250, 250, 250, 250, 460, 500, 580, 520, 520, 520, 520, 520, 520, 520 },
            { 240, 240, 240, 240, 240, 240, 470, 500, 580, 515, 515, 515, 515, 515, 515, 515 },
            { 220, 220, 220, 220, 220, 220, 470, 500, 580, 510, 510, 510, 510, 510, 510, 510 },
            { 200, 200, 200, 200, 200, 200, 470, 500, 580, 500, 500, 500, 500, 500, 500, 500 },
            { 180, 180, 180, 180, 180, 180, 480, 500, 580, 495, 495, 495, 495, 495, 495, 495 },
            { 160, 160, 160, 160, 160, 160, 480, 500, 580, 485, 485, 485, 485, 485, 485, 485 },
            { 140, 140, 140, 140, 140, 140, 480, 500, 580, 480, 480, 480, 480, 480, 480, 480 },
            { 125, 125, 125, 125, 125, 125, 480, 500, 580, 470, 470, 470, 470, 470, 470, 470 }
        },
        /* motion_texture_freq[OT_EIS_VI_AUTO_ISO_NUM] */
        { 180, 190, 190, 170, 170, 170, 160, 140, 128, 128, 128, 100, 100, 100, 100, 100 },
        /* motion_edge_freq[OT_EIS_VI_AUTO_ISO_NUM] */
        { 128, 100, 100, 100, 100, 100, 100, 100, 100, 100, 96, 96, 96, 96, 96, 96 },
        /* motion_over_shoot[OT_EIS_VI_AUTO_ISO_NUM] */
        { 60, 62, 62, 64, 64, 64, 56, 52, 50, 50, 30, 10, 10, 10, 10, 10 },
        /* motion_under_shoot[OT_EIS_VI_AUTO_ISO_NUM] */
        { 60, 62, 62, 64, 64, 64, 54, 50, 50, 30, 20, 20, 15, 15, 15, 15 },
        /* shoot_sup_strength[OT_EIS_VI_AUTO_ISO_NUM] */
        { 8, 8, 7, 7, 7, 6, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0 },
        /* shoot_sup_adj[OT_EIS_VI_AUTO_ISO_NUM] */
        { 9, 9, 8, 8, 7, 7, 7, 6, 0, 0, 0, 0, 0, 0, 0, 0 },
        /* detail_ctrl[OT_EIS_VI_AUTO_ISO_NUM] */
        { 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 120, 120, 120, 120, 120, 120 },
        /* detail_ctrl_threshold[OT_EIS_VI_AUTO_ISO_NUM] */
        { 180, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160 },
        /* edge_filt_strength[OT_EIS_VI_AUTO_ISO_NUM] */
        { 60, 58, 60, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62 },
        /* edge_filt_max_cap */
        { 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18 },
        /* r_gain[OT_EIS_VI_AUTO_ISO_NUM] */
        { 12, 16, 20, 24, 24, 24, 26, 28, 31, 31, 31, 31, 31, 31, 31, 31 },
        /* g_gain[OT_EIS_VI_AUTO_ISO_NUM] */
        { 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32 },
        /* b_gain[OT_EIS_VI_AUTO_ISO_NUM] */
        { 18, 20, 24, 24, 24, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22 },
        /* skin_gain[OT_EIS_VI_AUTO_ISO_NUM] */
        { 30, 30, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
        /* max_sharp_gain[OT_EIS_VI_AUTO_ISO_NUM] */
        { 30, 30, 30, 30, 30, 26, 20, 20, 18, 18, 18, 18, 18, 18, 18, 18 },
    },
};

static const ot_eis_vi_sharpen_attr g_cmos_yuv_sharpen_built_in = {
    /* enable */
    1,
    /* motion_en */
    0,
    /* motion_threshold0 */
    4,
    /* motion_threshold1 */
    10,
    /* motion_gain0 */
    0,
    /* motion_gain1 */
    255,
    /* skin_umin */
    100,
    /* skin_vmin */
    135,
    /* skin_umax */
    128,
    /* skin_vmax */
    150,
    /* op_type */
    OT_EIS_OP_MODE_AUTO,
    /* detail_map */
    OT_EIS_VI_SHARPEN_NORMAL,
    /* manual para */
    {
        /* luma_wgt[OT_EIS_VI_SHARPEN_LUMA_NUM] */
        {
            31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
            31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31
        },
        /* texture_strength[OT_EIS_VI_SHARPEN_GAIN_NUM] */
        {
            250, 420, 390, 390, 390, 390, 390, 370, 350, 330, 310, 290, 270, 270, 270, 270,
            270, 270, 266, 260, 244, 230, 230, 230, 230, 230, 230, 210, 190, 190, 170, 150
        },
        /* edge_strength[OT_EIS_VI_SHARPEN_GAIN_NUM] */
        {
            120, 123, 125, 128, 130, 135, 140, 148, 160, 168, 180, 190, 200, 210, 210, 210,
            210, 210, 200, 190, 185, 175, 165, 160, 146, 136, 130, 128, 125, 123, 120, 120
        },
        /* texture_freq */
        1,
        /* edge_freq */
        4095,
        /* over_shoot */
        80,
        /* under_shoot */
        80,
        /* motion_texture_strength[OT_EIS_VI_SHARPEN_GAIN_NUM] */
        {
            250, 420, 390, 390, 390, 390, 390, 370, 350, 330, 310, 290, 270, 270, 270, 270,
            270, 270, 266, 260, 244, 230, 230, 230, 230, 230, 230, 210, 190, 190, 170, 150
        },
        /* motion_edge_strength[OT_EIS_VI_SHARPEN_GAIN_NUM] */
        {
            120, 123, 125, 128, 130, 135, 140, 148, 160, 168, 180, 190, 200, 210, 210, 210,
            210, 210, 200, 190, 185, 175, 165, 160, 146, 136, 130, 128, 125, 123, 120, 120
        },
        /* motion_texture_freq */
        0,
        /* motion_edge_freq */
        128,
        /* motion_over_shoot */
        65,
        /* motion_under_shoot */
        75,
        /* u8shoot_sup_str */
        128,
        /* u8shoot_sup_adj */
        15,
        /* detail_ctrl */
        128,
        /* detail_ctrl_threshold */
        180,
        /* edge_filt_strength */
        60,
        /* edge_filt_max_cap */
        18,
        /* r_gain */
        20,
        /* g_gain */
        32,
        /* b_gain */
        20,
        /* skin_gain */
        25,
        /* max_sharp_gain */
        16,
    },
    /* auto para */
    {
        /* luma_wgt[OT_EIS_VI_SHARPEN_GAIN_NUM][OT_EIS_VI_AUTO_ISO_NUM] */
        {
            { 31, 31, 20, 20, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 },
            { 31, 31, 20, 20, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 },
            { 31, 31, 20, 20, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 },
            { 31, 31, 20, 20, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 },
            { 31, 31, 20, 20, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 },
            { 31, 31, 20, 20, 17, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15, 15 },
            { 31, 31, 20, 20, 20, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18 },
            { 31, 31, 23, 23, 23, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21, 21 },
            { 31, 31, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 },
            { 31, 31, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28 },
            { 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 28, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 23, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 }
        },
        /* texture_strength[OT_EIS_VI_SHARPEN_GAIN_NUM][OT_EIS_VI_AUTO_ISO_NUM] */
        {
            { 153, 153, 160, 145, 130, 130, 130, 105, 105, 105, 105, 85, 85, 85, 85, 85 },
            { 172, 167, 190, 170, 145, 145, 145, 120, 120, 120, 120, 100, 100, 100, 100, 100 },
            { 186, 178, 220, 195, 165, 165, 160, 140, 140, 135, 135, 120, 120, 120, 120, 120 },
            { 201, 192, 255, 215, 180, 180, 175, 160, 160, 150, 150, 135, 135, 140, 140, 140 },
            { 217, 207, 275, 230, 200, 200, 185, 175, 175, 160, 160, 150, 150, 160, 160, 160 },
            { 233, 223, 285, 245, 210, 215, 200, 190, 190, 170, 170, 165, 165, 180, 180, 180 },
            { 251, 239, 280, 260, 225, 225, 215, 205, 205, 180, 180, 185, 185, 200, 200, 200 },
            { 269, 255, 275, 265, 240, 240, 230, 220, 220, 190, 190, 200, 200, 210, 210, 210 },
            { 286, 272, 270, 270, 250, 250, 240, 225, 225, 200, 200, 220, 220, 220, 220, 220 },
            { 300, 288, 265, 270, 250, 260, 245, 235, 235, 210, 210, 235, 235, 230, 230, 230 },
            { 308, 299, 260, 265, 250, 270, 255, 245, 245, 220, 220, 245, 245, 230, 230, 230 },
            { 312, 307, 260, 260, 250, 275, 260, 250, 250, 230, 230, 255, 255, 230, 230, 230 },
            { 313, 310, 255, 255, 245, 280, 265, 255, 255, 235, 235, 265, 265, 230, 230, 230 },
            { 311, 311, 250, 250, 240, 285, 270, 255, 255, 240, 240, 275, 275, 230, 230, 230 },
            { 306, 311, 245, 245, 240, 290, 275, 250, 255, 245, 245, 280, 280, 230, 230, 230 },
            { 297, 308, 240, 240, 240, 295, 280, 240, 250, 250, 250, 285, 285, 230, 230, 230 },
            { 285, 301, 235, 235, 235, 295, 280, 235, 250, 250, 255, 290, 290, 230, 230, 230 },
            { 273, 291, 230, 230, 230, 290, 280, 230, 250, 250, 260, 295, 295, 230, 230, 230 },
            { 263, 281, 225, 225, 225, 285, 275, 225, 245, 250, 265, 300, 300, 225, 225, 225 },
            { 255, 271, 225, 220, 220, 275, 270, 225, 245, 250, 270, 300, 300, 220, 220, 220 },
            { 249, 259, 220, 215, 215, 270, 265, 225, 240, 245, 275, 300, 300, 210, 210, 210 },
            { 243, 248, 220, 210, 210, 260, 260, 220, 235, 245, 280, 305, 305, 200, 200, 200 },
            { 234, 238, 215, 205, 205, 255, 255, 220, 230, 240, 280, 305, 305, 190, 190, 190 },
            { 224, 228, 215, 200, 200, 245, 245, 220, 230, 240, 285, 310, 310, 180, 180, 180 },
            { 216, 218, 210, 195, 195, 235, 240, 215, 225, 235, 285, 310, 310, 175, 175, 175 },
            { 208, 208, 200, 190, 190, 230, 235, 215, 220, 230, 285, 305, 305, 170, 170, 170 },
            { 201, 201, 195, 180, 180, 220, 225, 210, 220, 225, 285, 305, 305, 160, 160, 160 },
            { 195, 195, 185, 175, 175, 205, 220, 205, 215, 220, 285, 300, 300, 160, 160, 160 },
            { 188, 188, 175, 170, 170, 195, 210, 195, 210, 215, 280, 300, 300, 150, 150, 150 },
            { 181, 181, 170, 160, 160, 180, 205, 195, 205, 210, 280, 300, 300, 145, 145, 145 },
            { 175, 175, 160, 155, 155, 165, 195, 195, 200, 205, 275, 300, 300, 130, 130, 130 },
            { 171, 171, 150, 150, 150, 150, 150, 150, 195, 200, 270, 300, 300, 115, 115, 115 }
        },
        /* edge_strength */
        {
            { 195, 195, 195, 195, 195, 195, 400, 440, 500, 475, 475, 475, 475, 475, 475, 475 },
            { 210, 210, 200, 200, 200, 200, 400, 440, 500, 485, 485, 485, 485, 485, 485, 485 },
            { 235, 235, 220, 220, 220, 220, 400, 440, 500, 495, 495, 495, 495, 495, 495, 495 },
            { 260, 260, 230, 230, 230, 230, 400, 440, 500, 500, 500, 500, 500, 500, 500, 500 },
            { 295, 295, 240, 240, 240, 240, 400, 440, 500, 510, 510, 510, 510, 510, 510, 510 },
            { 320, 320, 250, 250, 250, 250, 400, 440, 500, 520, 520, 520, 520, 520, 520, 520 },
            { 340, 340, 260, 260, 260, 260, 420, 460, 540, 530, 530, 530, 530, 530, 530, 530 },
            { 350, 350, 270, 270, 270, 270, 420, 460, 540, 540, 540, 540, 540, 540, 540, 540 },
            { 360, 360, 280, 280, 280, 280, 420, 460, 540, 545, 545, 545, 545, 545, 545, 545 },
            { 360, 360, 280, 280, 280, 280, 420, 460, 540, 550, 550, 550, 550, 550, 550, 550 },
            { 355, 355, 285, 285, 285, 285, 420, 460, 540, 560, 560, 560, 560, 560, 560, 560 },
            { 345, 345, 290, 290, 290, 290, 420, 460, 540, 565, 565, 565, 565, 565, 565, 565 },
            { 335, 335, 290, 290, 290, 290, 420, 460, 540, 570, 570, 570, 570, 570, 570, 570 },
            { 330, 330, 295, 295, 295, 295, 430, 470, 550, 575, 575, 575, 575, 575, 575, 575 },
            { 325, 325, 295, 295, 295, 295, 430, 470, 550, 580, 580, 580, 580, 580, 580, 580 },
            { 320, 320, 295, 295, 295, 295, 440, 470, 550, 575, 575, 575, 575, 575, 575, 575 },
            { 315, 315, 295, 295, 295, 295, 440, 470, 550, 570, 570, 570, 570, 570, 570, 570 },
            { 310, 310, 295, 295, 295, 295, 450, 480, 580, 565, 565, 565, 565, 565, 565, 565 },
            { 305, 305, 295, 295, 295, 295, 460, 500, 580, 560, 560, 560, 560, 560, 560, 560 },
            { 295, 295, 290, 290, 290, 290, 460, 500, 580, 555, 555, 555, 555, 555, 555, 555 },
            { 285, 285, 285, 285, 285, 285, 460, 500, 580, 550, 550, 550, 550, 550, 550, 550 },
            { 280, 280, 280, 280, 280, 280, 460, 500, 580, 540, 540, 540, 540, 540, 540, 540 },
            { 275, 275, 275, 275, 275, 275, 460, 500, 580, 535, 535, 535, 535, 535, 535, 535 },
            { 265, 265, 265, 265, 265, 265, 460, 500, 580, 530, 530, 530, 530, 530, 530, 530 },
            { 250, 250, 250, 250, 250, 250, 460, 500, 580, 520, 520, 520, 520, 520, 520, 520 },
            { 240, 240, 240, 240, 240, 240, 470, 500, 580, 515, 515, 515, 515, 515, 515, 515 },
            { 220, 220, 220, 220, 220, 220, 470, 500, 580, 510, 510, 510, 510, 510, 510, 510 },
            { 200, 200, 200, 200, 200, 200, 470, 500, 580, 500, 500, 500, 500, 500, 500, 500 },
            { 180, 180, 180, 180, 180, 180, 480, 500, 580, 495, 495, 495, 495, 495, 495, 495 },
            { 160, 160, 160, 160, 160, 160, 480, 500, 580, 485, 485, 485, 485, 485, 485, 485 },
            { 140, 140, 140, 140, 140, 140, 480, 500, 580, 480, 480, 480, 480, 480, 480, 480 },
            { 125, 125, 125, 125, 125, 125, 480, 500, 580, 470, 470, 470, 470, 470, 470, 470 }
        },
        /* texture_freq[OT_EIS_VI_AUTO_ISO_NUM] */
        { 180, 190, 190, 170, 170, 170, 160, 140, 128, 128, 128, 100, 100, 100, 100, 100 },
        /* edge_freq[OT_EIS_VI_AUTO_ISO_NUM] */
        { 128, 100, 100, 100, 100, 100, 100, 100, 100, 100, 96, 96, 96, 96, 96, 96 },
        /* over_shoot[OT_EIS_VI_AUTO_ISO_NUM] */
        { 60, 62, 62, 64, 64, 64, 56, 52, 50, 50, 30, 10, 10, 10, 10, 10 },
        /* under_shoot[OT_EIS_VI_AUTO_ISO_NUM] */
        { 60, 62, 62, 64, 64, 64, 54, 50, 50, 30, 20, 20, 15, 15, 15, 15 },
        /* motion_texture_strength[OT_EIS_VI_SHARPEN_GAIN_NUM][OT_EIS_VI_AUTO_ISO_NUM] */
        {
            { 153, 153, 160, 145, 130, 130, 130, 105, 105, 105, 105, 85, 85, 85, 85, 85 },
            { 172, 167, 190, 170, 145, 145, 145, 120, 120, 120, 120, 100, 100, 100, 100, 100 },
            { 186, 178, 220, 195, 165, 165, 160, 140, 140, 135, 135, 120, 120, 120, 120, 120 },
            { 201, 192, 255, 215, 180, 180, 175, 160, 160, 150, 150, 135, 135, 140, 140, 140 },
            { 217, 207, 275, 230, 200, 200, 185, 175, 175, 160, 160, 150, 150, 160, 160, 160 },
            { 233, 223, 285, 245, 210, 215, 200, 190, 190, 170, 170, 165, 165, 180, 180, 180 },
            { 251, 239, 280, 260, 225, 225, 215, 205, 205, 180, 180, 185, 185, 200, 200, 200 },
            { 269, 255, 275, 265, 240, 240, 230, 220, 220, 190, 190, 200, 200, 210, 210, 210 },
            { 286, 272, 270, 270, 250, 250, 240, 225, 225, 200, 200, 220, 220, 220, 220, 220 },
            { 300, 288, 265, 270, 250, 260, 245, 235, 235, 210, 210, 235, 235, 230, 230, 230 },
            { 308, 299, 260, 265, 250, 270, 255, 245, 245, 220, 220, 245, 245, 230, 230, 230 },
            { 312, 307, 260, 260, 250, 275, 260, 250, 250, 230, 230, 255, 255, 230, 230, 230 },
            { 313, 310, 255, 255, 245, 280, 265, 255, 255, 235, 235, 265, 265, 230, 230, 230 },
            { 311, 311, 250, 250, 240, 285, 270, 255, 255, 240, 240, 275, 275, 230, 230, 230 },
            { 306, 311, 245, 245, 240, 290, 275, 250, 255, 245, 245, 280, 280, 230, 230, 230 },
            { 297, 308, 240, 240, 240, 295, 280, 240, 250, 250, 250, 285, 285, 230, 230, 230 },
            { 285, 301, 235, 235, 235, 295, 280, 235, 250, 250, 255, 290, 290, 230, 230, 230 },
            { 273, 291, 230, 230, 230, 290, 280, 230, 250, 250, 260, 295, 295, 230, 230, 230 },
            { 263, 281, 225, 225, 225, 285, 275, 225, 245, 250, 265, 300, 300, 225, 225, 225 },
            { 255, 271, 225, 220, 220, 275, 270, 225, 245, 250, 270, 300, 300, 220, 220, 220 },
            { 249, 259, 220, 215, 215, 270, 265, 225, 240, 245, 275, 300, 300, 210, 210, 210 },
            { 243, 248, 220, 210, 210, 260, 260, 220, 235, 245, 280, 305, 305, 200, 200, 200 },
            { 234, 238, 215, 205, 205, 255, 255, 220, 230, 240, 280, 305, 305, 190, 190, 190 },
            { 224, 228, 215, 200, 200, 245, 245, 220, 230, 240, 285, 310, 310, 180, 180, 180 },
            { 216, 218, 210, 195, 195, 235, 240, 215, 225, 235, 285, 310, 310, 175, 175, 175 },
            { 208, 208, 200, 190, 190, 230, 235, 215, 220, 230, 285, 305, 305, 170, 170, 170 },
            { 201, 201, 195, 180, 180, 220, 225, 210, 220, 225, 285, 305, 305, 160, 160, 160 },
            { 195, 195, 185, 175, 175, 205, 220, 205, 215, 220, 285, 300, 300, 160, 160, 160 },
            { 188, 188, 175, 170, 170, 195, 210, 195, 210, 215, 280, 300, 300, 150, 150, 150 },
            { 181, 181, 170, 160, 160, 180, 205, 195, 205, 210, 280, 300, 300, 145, 145, 145 },
            { 175, 175, 160, 155, 155, 165, 195, 195, 200, 205, 275, 300, 300, 130, 130, 130 },
            { 171, 171, 150, 150, 150, 150, 150, 150, 195, 200, 270, 300, 300, 115, 115, 115 }
        },
        /* motion_edge_strength     */
        {
            { 195, 195, 195, 195, 195, 195, 400, 440, 500, 475, 475, 475, 475, 475, 475, 475 },
            { 210, 210, 200, 200, 200, 200, 400, 440, 500, 485, 485, 485, 485, 485, 485, 485 },
            { 235, 235, 220, 220, 220, 220, 400, 440, 500, 495, 495, 495, 495, 495, 495, 495 },
            { 260, 260, 230, 230, 230, 230, 400, 440, 500, 500, 500, 500, 500, 500, 500, 500 },
            { 295, 295, 240, 240, 240, 240, 400, 440, 500, 510, 510, 510, 510, 510, 510, 510 },
            { 320, 320, 250, 250, 250, 250, 400, 440, 500, 520, 520, 520, 520, 520, 520, 520 },
            { 340, 340, 260, 260, 260, 260, 420, 460, 540, 530, 530, 530, 530, 530, 530, 530 },
            { 350, 350, 270, 270, 270, 270, 420, 460, 540, 540, 540, 540, 540, 540, 540, 540 },
            { 360, 360, 280, 280, 280, 280, 420, 460, 540, 545, 545, 545, 545, 545, 545, 545 },
            { 360, 360, 280, 280, 280, 280, 420, 460, 540, 550, 550, 550, 550, 550, 550, 550 },
            { 355, 355, 285, 285, 285, 285, 420, 460, 540, 560, 560, 560, 560, 560, 560, 560 },
            { 345, 345, 290, 290, 290, 290, 420, 460, 540, 565, 565, 565, 565, 565, 565, 565 },
            { 335, 335, 290, 290, 290, 290, 420, 460, 540, 570, 570, 570, 570, 570, 570, 570 },
            { 330, 330, 295, 295, 295, 295, 430, 470, 550, 575, 575, 575, 575, 575, 575, 575 },
            { 325, 325, 295, 295, 295, 295, 430, 470, 550, 580, 580, 580, 580, 580, 580, 580 },
            { 320, 320, 295, 295, 295, 295, 440, 470, 550, 575, 575, 575, 575, 575, 575, 575 },
            { 315, 315, 295, 295, 295, 295, 440, 470, 550, 570, 570, 570, 570, 570, 570, 570 },
            { 310, 310, 295, 295, 295, 295, 450, 480, 580, 565, 565, 565, 565, 565, 565, 565 },
            { 305, 305, 295, 295, 295, 295, 460, 500, 580, 560, 560, 560, 560, 560, 560, 560 },
            { 295, 295, 290, 290, 290, 290, 460, 500, 580, 555, 555, 555, 555, 555, 555, 555 },
            { 285, 285, 285, 285, 285, 285, 460, 500, 580, 550, 550, 550, 550, 550, 550, 550 },
            { 280, 280, 280, 280, 280, 280, 460, 500, 580, 540, 540, 540, 540, 540, 540, 540 },
            { 275, 275, 275, 275, 275, 275, 460, 500, 580, 535, 535, 535, 535, 535, 535, 535 },
            { 265, 265, 265, 265, 265, 265, 460, 500, 580, 530, 530, 530, 530, 530, 530, 530 },
            { 250, 250, 250, 250, 250, 250, 460, 500, 580, 520, 520, 520, 520, 520, 520, 520 },
            { 240, 240, 240, 240, 240, 240, 470, 500, 580, 515, 515, 515, 515, 515, 515, 515 },
            { 220, 220, 220, 220, 220, 220, 470, 500, 580, 510, 510, 510, 510, 510, 510, 510 },
            { 200, 200, 200, 200, 200, 200, 470, 500, 580, 500, 500, 500, 500, 500, 500, 500 },
            { 180, 180, 180, 180, 180, 180, 480, 500, 580, 495, 495, 495, 495, 495, 495, 495 },
            { 160, 160, 160, 160, 160, 160, 480, 500, 580, 485, 485, 485, 485, 485, 485, 485 },
            { 140, 140, 140, 140, 140, 140, 480, 500, 580, 480, 480, 480, 480, 480, 480, 480 },
            { 125, 125, 125, 125, 125, 125, 480, 500, 580, 470, 470, 470, 470, 470, 470, 470 }
        },
        /* motion_texture_freq[OT_EIS_VI_AUTO_ISO_NUM] */
        { 180, 190, 190, 170, 170, 170, 160, 140, 128, 128, 128, 100, 100, 100, 100, 100 },
        /* motion_edge_freq[OT_EIS_VI_AUTO_ISO_NUM] */
        { 128, 100, 100, 100, 100, 100, 100, 100, 100, 100, 96, 96, 96, 96, 96, 96 },
        /* motion_over_shoot[OT_EIS_VI_AUTO_ISO_NUM] */
        { 60, 62, 62, 64, 64, 64, 56, 52, 50, 50, 30, 10, 10, 10, 10, 10 },
        /* motion_under_shoot[OT_EIS_VI_AUTO_ISO_NUM] */
        { 60, 62, 62, 64, 64, 64, 54, 50, 50, 30, 20, 20, 15, 15, 15, 15 },
        /* shoot_sup_strength[OT_EIS_VI_AUTO_ISO_NUM] */
        { 8, 8, 7, 7, 7, 6, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0 },
        /* shoot_sup_adj[OT_EIS_VI_AUTO_ISO_NUM] */
        { 9, 9, 8, 8, 7, 7, 7, 6, 0, 0, 0, 0, 0, 0, 0, 0 },
        /* detail_ctrl[OT_EIS_VI_AUTO_ISO_NUM] */
        { 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 120, 120, 120, 120, 120, 120 },
        /* detail_ctrl_threshold[OT_EIS_VI_AUTO_ISO_NUM] */
        { 180, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160 },
        /* edge_filt_strength[OT_EIS_VI_AUTO_ISO_NUM] */
        { 60, 58, 60, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62, 62 },
        /* edge_filt_max_cap */
        { 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18 },
        /* r_gain[OT_EIS_VI_AUTO_ISO_NUM] */
        { 12, 16, 20, 24, 24, 24, 26, 28, 31, 31, 31, 31, 31, 31, 31, 31 },
        /* g_gain[OT_EIS_VI_AUTO_ISO_NUM] */
        { 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32 },
        /* b_gain[OT_EIS_VI_AUTO_ISO_NUM] */
        { 18, 20, 24, 24, 24, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22, 22 },
        /* skin_gain[OT_EIS_VI_AUTO_ISO_NUM] */
        { 30, 30, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
        /* max_sharp_gain[OT_EIS_VI_AUTO_ISO_NUM] */
        { 30, 30, 30, 30, 30, 26, 20, 20, 18, 18, 18, 18, 18, 18, 18, 18 },
    },
};

static const ot_eis_vi_sharpen_attr g_cmos_yuv_sharpen_wdr = {
    /* en */
    1,
    /* motion_en */
    0,
    /* motion_threshold0 */
    2,
    /* motion_threshold1 */
    8,
    /* motion_gain0 */
    0,
    /* motion_gain1 */
    256,
    /* skin_umin */
    100,
    /* skin_vmin */
    135,
    /* skin_umax */
    128,
    /* skin_vmax */
    150,
    /* op_type */
    OT_EIS_OP_MODE_AUTO,
    /* detail_map */
    OT_EIS_VI_SHARPEN_NORMAL,
    /* manual para */
    {
        /* luma_wgt[OT_EIS_VI_SHARPEN_LUMA_NUM] */
        {
            31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31,
            31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31
        },
        /* texture_strength[OT_EIS_VI_SHARPEN_GAIN_NUM] */
        {
            250, 420, 390, 390, 390, 390, 390, 370, 350, 330, 310, 290, 270, 270, 270, 270,
            270, 270, 266, 260, 244, 230, 230, 230, 230, 230, 230, 210, 190, 190, 170, 150
        },
        /* edge_strength[OT_EIS_VI_SHARPEN_GAIN_NUM] */
        {
            120, 123, 125, 128, 130, 135, 140, 148, 160, 168, 180, 190, 200, 210, 210, 210,
            210, 210, 200, 190, 185, 175, 165, 160, 146, 136, 130, 128, 125, 123, 120, 120
        },
        /* texture_freq */
        200,
        /* edge_freq */
        128,
        /* over_shoot */
        65,
        /* under_shoot */
        75,
        /* motion_texture_strength[OT_EIS_VI_SHARPEN_GAIN_NUM] */
        {
            250, 420, 390, 390, 390, 390, 390, 370, 350, 330, 310, 290, 270, 270, 270, 270,
            270, 270, 266, 260, 244, 230, 230, 230, 230, 230, 230, 210, 190, 190, 170, 150
        },
        /* motion_edge_strength[OT_EIS_VI_SHARPEN_GAIN_NUM] */
        {
            120, 123, 125, 128, 130, 135, 140, 148, 160, 168, 180, 190, 200, 210, 210, 210,
            210, 210, 200, 190, 185, 175, 165, 160, 146, 136, 130, 128, 125, 123, 120, 120
        },
        /* motion_texture_freq */
        200,
        /* motion_edge_freq */
        128,
        /* motion_over_shoot */
        65,
        /* motion_under_shoot */
        75,
        /* u8shoot_sup_str */
        7,
        /* u8shoot_sup_adj */
        9,
        /* detail_ctrl */
        128,
        /* detail_ctrl_threshold */
        180,
        /* edge_filt_strength */
        50,
        /* edge_filt_max_cap */
        18,
        /* r_gain */
        20,
        /* g_gain */
        32,
        /* b_gain */
        20,
        /* skin_gain */
        25,
        /* max_sharp_gain */
        40,
    },
    /* auto para */
    {
        /* luma_wgt[OT_EIS_VI_SHARPEN_GAIN_NUM][OT_EIS_VI_AUTO_ISO_NUM] */
        {
            { 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24 },
            { 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 },
            { 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 },
            { 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 },
            { 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26 },
            { 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26 },
            { 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26, 26 },
            { 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27 },
            { 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27, 27 },
            { 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28 },
            { 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28 },
            { 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29 },
            { 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29 },
            { 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29, 29 },
            { 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 },
            { 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 },
            { 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 },
            { 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 },
            { 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30 },
            { 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 },
            { 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31, 31 }
        },

        /* texture_strength[OT_EIS_VI_SHARPEN_GAIN_NUM][OT_EIS_VI_AUTO_ISO_NUM] */
        {
            {159, 159, 159, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140, 140},
            {167, 167, 167, 144, 144, 144, 144, 143, 143, 143, 143, 143, 143, 143, 143, 143},
            {176, 176, 176, 151, 151, 151, 151, 147, 147, 147, 147, 147, 147, 147, 147, 147},
            {186, 186, 186, 158, 158, 158, 158, 152, 152, 152, 152, 152, 152, 152, 152, 152},
            {196, 196, 196, 166, 166, 166, 166, 158, 158, 158, 158, 158, 158, 158, 158, 158},
            {206, 206, 206, 176, 176, 176, 176, 164, 164, 164, 164, 164, 164, 164, 164, 164},
            {217, 217, 217, 185, 185, 185, 185, 171, 171, 171, 171, 171, 171, 171, 171, 171},
            {227, 227, 227, 195, 195, 195, 195, 177, 177, 177, 177, 177, 177, 177, 177, 177},
            {237, 237, 237, 205, 205, 205, 205, 184, 184, 184, 184, 184, 184, 184, 184, 184},
            {246, 246, 246, 214, 214, 214, 214, 190, 190, 190, 190, 190, 190, 190, 190, 190},
            {254, 254, 254, 223, 223, 223, 223, 196, 196, 196, 196, 196, 196, 196, 196, 196},
            {261, 261, 261, 231, 231, 231, 231, 201, 201, 201, 201, 201, 201, 201, 201, 201},
            {268, 268, 268, 237, 237, 237, 237, 206, 206, 206, 206, 206, 206, 206, 206, 206},
            {272, 272, 272, 242, 242, 242, 242, 209, 209, 209, 209, 209, 209, 209, 209, 209},
            {276, 276, 276, 245, 245, 245, 245, 211, 211, 211, 211, 211, 211, 211, 211, 211},
            {277, 277, 277, 247, 247, 247, 247, 212, 212, 212, 212, 212, 212, 212, 212, 212},
            {276, 276, 276, 246, 246, 246, 246, 211, 211, 211, 211, 211, 211, 211, 211, 211},
            {274, 274, 274, 243, 243, 243, 243, 209, 209, 209, 209, 209, 209, 209, 209, 209},
            {270, 270, 270, 238, 238, 238, 238, 206, 206, 206, 206, 206, 206, 206, 206, 206},
            {265, 265, 265, 232, 232, 232, 232, 202, 202, 202, 202, 202, 202, 202, 202, 202},
            {258, 258, 258, 225, 225, 225, 225, 197, 197, 197, 197, 197, 197, 197, 197, 197},
            {250, 250, 250, 217, 217, 217, 217, 192, 192, 192, 192, 192, 192, 192, 192, 192},
            {242, 242, 242, 209, 209, 209, 209, 186, 186, 186, 186, 186, 186, 186, 186, 186},
            {233, 233, 233, 199, 199, 199, 199, 180, 180, 180, 180, 180, 180, 180, 180, 180},
            {224, 224, 224, 190, 190, 190, 190, 174, 174, 174, 174, 174, 174, 174, 174, 174},
            {215, 215, 215, 181, 181, 181, 181, 167, 167, 167, 167, 167, 167, 167, 167, 167},
            {206, 206, 206, 172, 172, 172, 172, 161, 161, 161, 161, 161, 161, 161, 161, 161},
            {198, 198, 198, 163, 163, 163, 163, 156, 156, 156, 156, 156, 156, 156, 156, 156},
            {191, 191, 191, 156, 156, 156, 156, 150, 150, 150, 150, 150, 150, 150, 150, 150},
            {184, 184, 184, 149, 149, 149, 149, 146, 146, 146, 146, 146, 146, 146, 146, 146},
            {179, 179, 179, 143, 143, 143, 143, 142, 142, 142, 142, 142, 142, 142, 142, 142},
            {175, 175, 175, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139, 139},
        },
        /* edge_strength     */
        {
            {149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149},
            {155, 155, 155, 155, 155, 155, 155, 154, 154, 154, 154, 154, 154, 154, 154, 154},
            {164, 164, 164, 164, 164, 164, 164, 161, 161, 161, 161, 161, 161, 161, 161, 161},
            {175, 175, 175, 175, 175, 175, 175, 170, 170, 170, 170, 170, 170, 170, 170, 170},
            {187, 187, 187, 187, 187, 187, 187, 180, 180, 180, 180, 180, 180, 180, 180, 180},
            {201, 201, 201, 201, 201, 201, 201, 190, 190, 190, 190, 190, 190, 190, 190, 190},
            {215, 215, 215, 215, 215, 215, 215, 202, 202, 202, 202, 202, 202, 202, 202, 202},
            {229, 229, 229, 229, 229, 229, 229, 213, 213, 213, 213, 213, 213, 213, 213, 213},
            {243, 243, 243, 243, 243, 243, 243, 224, 224, 224, 224, 224, 224, 224, 224, 224},
            {257, 257, 257, 257, 257, 257, 257, 235, 235, 235, 235, 235, 235, 235, 235, 235},
            {269, 269, 269, 269, 269, 269, 269, 246, 246, 246, 246, 246, 246, 246, 246, 246},
            {281, 281, 281, 281, 281, 281, 281, 255, 255, 255, 255, 255, 255, 255, 255, 255},
            {290, 290, 290, 290, 290, 290, 290, 262, 262, 262, 262, 262, 262, 262, 262, 262},
            {298, 298, 298, 298, 298, 298, 298, 268, 268, 268, 268, 268, 268, 268, 268, 268},
            {302, 302, 302, 302, 302, 302, 302, 272, 272, 272, 272, 272, 272, 272, 272, 272},
            {304, 304, 304, 304, 304, 304, 304, 273, 273, 273, 273, 273, 273, 273, 273, 273},
            {303, 303, 303, 303, 303, 303, 303, 272, 272, 272, 272, 272, 272, 272, 272, 272},
            {298, 298, 298, 298, 298, 298, 298, 269, 269, 269, 269, 269, 269, 269, 269, 269},
            {292, 292, 292, 292, 292, 292, 292, 264, 264, 264, 264, 264, 264, 264, 264, 264},
            {284, 284, 284, 284, 284, 284, 284, 257, 257, 257, 257, 257, 257, 257, 257, 257},
            {273, 273, 273, 273, 273, 273, 273, 249, 249, 249, 249, 249, 249, 249, 249, 249},
            {262, 262, 262, 262, 262, 262, 262, 240, 240, 240, 240, 240, 240, 240, 240, 240},
            {250, 250, 250, 250, 250, 250, 250, 230, 230, 230, 230, 230, 230, 230, 230, 230},
            {237, 237, 237, 237, 237, 237, 237, 219, 219, 219, 219, 219, 219, 219, 219, 219},
            {223, 223, 223, 223, 223, 223, 223, 209, 209, 209, 209, 209, 209, 209, 209, 209},
            {210, 210, 210, 210, 210, 210, 210, 198, 198, 198, 198, 198, 198, 198, 198, 198},
            {197, 197, 197, 197, 197, 197, 197, 188, 188, 188, 188, 188, 188, 188, 188, 188},
            {185, 185, 185, 185, 185, 185, 185, 178, 178, 178, 178, 178, 178, 178, 178, 178},
            {174, 174, 174, 174, 174, 174, 174, 169, 169, 169, 169, 169, 169, 169, 169, 169},
            {164, 164, 164, 164, 164, 164, 164, 161, 161, 161, 161, 161, 161, 161, 161, 161},
            {156, 156, 156, 156, 156, 156, 156, 155, 155, 155, 155, 155, 155, 155, 155, 155},
            {150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150}
        },
        /* texture_freq[OT_EIS_VI_AUTO_ISO_NUM] */
        { 200, 180, 180, 170, 160, 160, 140, 140, 140, 128, 128, 100, 100, 100, 100, 100 },
        /* edge_freq[OT_EIS_VI_AUTO_ISO_NUM] */
        { 128, 128, 128, 128, 128, 128, 100, 100, 100, 100, 100, 100, 100, 100, 100, 100 },
        /* over_shoot[OT_EIS_VI_AUTO_ISO_NUM] */
        { 60, 	60,  60,  60,  55,  55,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50 },
        /* under_shoot[OT_EIS_VI_AUTO_ISO_NUM] */
        { 60, 	60,  60,  60,  55,  55,  50,  50,  50,  50,  50,  50,  50,  50,  50,  50 },
        /* motion_texture_strength[OT_EIS_VI_SHARPEN_GAIN_NUM][OT_EIS_VI_AUTO_ISO_NUM] */
        {
            {149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149},
            {154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154},
            {161, 161, 161, 161, 161, 161, 161, 161, 161, 161, 161, 161, 161, 161, 161, 161},
            {170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170},
            {180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180},
            {190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190},
            {202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202},
            {213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213},
            {224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224},
            {235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235},
            {246, 246, 246, 246, 246, 246, 246, 246, 246, 246, 246, 246, 246, 246, 246, 246},
            {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
            {262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262},
            {268, 268, 268, 268, 268, 268, 268, 268, 268, 268, 268, 268, 268, 268, 268, 268},
            {272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272},
            {273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273},
            {272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272},
            {269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269},
            {264, 264, 264, 264, 264, 264, 264, 264, 264, 264, 264, 264, 264, 264, 264, 264},
            {257, 257, 257, 257, 257, 257, 257, 257, 257, 257, 257, 257, 257, 257, 257, 257},
            {249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249},
            {240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240},
            {230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230},
            {219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219},
            {209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209},
            {198, 198, 198, 198, 198, 198, 198, 198, 198, 198, 198, 198, 198, 198, 198, 198},
            {188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188},
            {178, 178, 178, 178, 178, 178, 178, 178, 178, 178, 178, 178, 178, 178, 178, 178},
            {169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169},
            {161, 161, 161, 161, 161, 161, 161, 161, 161, 161, 161, 161, 161, 161, 161, 161},
            {155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155},
            {150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150},
        },
        /* motion_edge_strength     */
        {
            {149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149, 149},
            {154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154, 154},
            {161, 161, 161, 161, 161, 161, 161, 161, 161, 161, 161, 161, 161, 161, 161, 161},
            {170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170, 170},
            {180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180, 180},
            {190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190, 190},
            {202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202, 202},
            {213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213, 213},
            {224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224, 224},
            {235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235, 235},
            {246, 246, 246, 246, 246, 246, 246, 246, 246, 246, 246, 246, 246, 246, 246, 246},
            {255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},
            {262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262, 262},
            {268, 268, 268, 268, 268, 268, 268, 268, 268, 268, 268, 268, 268, 268, 268, 268},
            {272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272},
            {273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273, 273},
            {272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272, 272},
            {269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269, 269},
            {264, 264, 264, 264, 264, 264, 264, 264, 264, 264, 264, 264, 264, 264, 264, 264},
            {257, 257, 257, 257, 257, 257, 257, 257, 257, 257, 257, 257, 257, 257, 257, 257},
            {249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249, 249},
            {240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240, 240},
            {230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230, 230},
            {219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219, 219},
            {209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209, 209},
            {198, 198, 198, 198, 198, 198, 198, 198, 198, 198, 198, 198, 198, 198, 198, 198},
            {188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188, 188},
            {178, 178, 178, 178, 178, 178, 178, 178, 178, 178, 178, 178, 178, 178, 178, 178},
            {169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169, 169},
            {161, 161, 161, 161, 161, 161, 161, 161, 161, 161, 161, 161, 161, 161, 161, 161},
            {155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155},
            {150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150, 150}
        },
        /* motion_texture_freq[OT_EIS_VI_AUTO_ISO_NUM] */
        { 200, 190, 190, 170, 170, 170, 160, 140, 128, 128, 128, 100, 100, 100, 100, 100 },
        /* motion_edge_freq[OT_EIS_VI_AUTO_ISO_NUM] */
        { 128, 100, 100, 100, 100, 100, 100, 100, 100, 100, 96, 96, 96, 96, 96, 96 },
        /* motion_over_shoot[OT_EIS_VI_AUTO_ISO_NUM] */
        { 65, 65, 62, 64, 64, 64, 56, 52, 50, 50, 30, 10, 10, 10, 10, 10 },
        /* motion_under_shoot[OT_EIS_VI_AUTO_ISO_NUM] */
        { 65, 65, 62, 64, 64, 64, 54, 50, 50, 30, 20, 20, 15, 15, 15, 15 },
        /* motion_shoot_sup_strength[OT_EIS_VI_AUTO_ISO_NUM] */
        { 4, 4, 4, 4, 4, 6, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5 },
        /* shoot_sup_adj[OT_EIS_VI_AUTO_ISO_NUM] */
        { 6, 9, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8 },
        /* detail_ctrl[OT_EIS_VI_AUTO_ISO_NUM] */
        { 128, 128, 128, 128, 128, 128, 128, 110, 110, 100, 100, 100, 100, 100, 100, 100 },
        /* detail_ctrl_threshold[OT_EIS_VI_AUTO_ISO_NUM] */
        { 180, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160, 160 },
        /* edge_filt_strength[OT_EIS_VI_AUTO_ISO_NUM] */
        { 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61},
        /* edge_filt_max_cap */
        { 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18, 18 },
        /* r_gain[OT_EIS_VI_AUTO_ISO_NUM] */
        {31, 31, 28, 28, 28, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24 },
        /* g_gain[OT_EIS_VI_AUTO_ISO_NUM] */
        { 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32, 32 },
        /* b_gain[OT_EIS_VI_AUTO_ISO_NUM] */
        { 31, 31, 28, 28, 28, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24 },
        /* skin_gain[OT_EIS_VI_AUTO_ISO_NUM] */
        { 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25 },
        /* max_sharp_gain[OT_EIS_VI_AUTO_ISO_NUM] */
        { 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40, 40 },
    },
};

static ot_eis_vi_awb_ccm g_awb_ccm = {
    /* the number of CCM matrixes */
    3,
    {
        {
            7000,
            {
                0x019D,  0x80DF,  0x0042,
                0x8039,  0x0141,  0x8008,
                0x000F,  0x80C4,  0x01B5,

            },
        },
        {
            5000,
            {
                0x01AC,  0x80FC,  0x0050,
                0x8043,  0x014E,  0x800B,
                0x0025,  0x810C,  0x01E7,
            },
        },
        {
            2800,
            {
                0x01F3,  0x80DB,  0x8018,
                0x805A,  0x0177,  0x801D,
                0x0042,  0x81A0,  0x025E,
            },
        },
        {
            2525,
            { 0x0100, 0x0000, 0x0000, 0x0000, 0x0100, 0x0000, 0x0000, 0x0000, 0x0100 },
        },
        {
            2100,
            { 0x0100, 0x0000, 0x0000, 0x0000, 0x0100, 0x0000, 0x0000, 0x0000, 0x0100 },
        },
        {
            1600,
            { 0x0100, 0x0000, 0x0000, 0x0000, 0x0100, 0x0000, 0x0000, 0x0000, 0x0100 },
        },
        {
            1400,
            { 0x0100, 0x0000, 0x0000, 0x0000, 0x0100, 0x0000, 0x0000, 0x0000, 0x0100 },
        },
    },
};



static ot_eis_vi_awb_ccm g_awb_ccm_built_in = {
    /* the number of CCM matrixes */
    6,
    {
        {
            7500,
            {
                0x0183,  0x8074,  0x800f,
                0x803c,  0x016b,  0x802f,
                0x0011,  0x8074,  0x0163,
            },
        },
        {
            6500,
            {
                0x0161,  0x805c,  0x8005,
                0x8042,  0x0161,  0x801f,
                0x000c,  0x8073,  0x0167,
            },
        },
        {
            5000,
            {
                0x016c,  0x805d,  0x800f,
                0x804b,  0x016b,  0x8020,
                0x000d,  0x809a,  0x018d,
            },
        },
        {
            4000,
            {
                0x015e,  0x8044,  0x801a,
                0x8054,  0x0163,  0x800f,
                0x0005,  0x8096,  0x0191,
            },
        },
        {
            2856,
            {
                0x015b,  0x8025,  0x8036,
                0x807e,  0x0181,  0x8003,
                0x8013,  0x80d3,  0x01e6,
            },
        },
        {
            2300,
            {
                0x0161,  0x805c,  0x8005,
                0x8042,  0x0161,  0x801f,
                0x000c,  0x8073,  0x0167,
            },
        },
    },
};


static ot_eis_vi_awb_agc_table g_awb_agc_table = {
    /* bvalid */
    1,

    /* 1,  2,  4,  8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768 */
    /* saturation */
    { 128, 128, 128, 128, 124, 120, 116, 112, 108, 104, 100, 94, 90, 90, 90, 90 }
};

static const ot_eis_vi_wdr_attr g_cmos_wdr = {
    /* ot_eis_vi_wdr_merge_mode */
    OT_EIS_VI_MERGE_FUSION_MODE,

    /* ot_eis_vi_wdr_combine_attr */
    {
        /* motion_comp */
        1,

        /* short_thr */
        4032,

        /* long_thr */
        3008,

        /* force_long */
        1,

        /* force_long_low_threshold */
        500,

        /* force_long_hig_threshold */
        700,

        /* ot_eis_vi_fswdr_mdt_attr */
        {
            /* short_expo_chk */
            0,

            /* short_check_thd */
            8,

            /* md_ref_flicker */
            1,

            /* mdt_still_thd */
            20,

            /* mdt_full_threshold */
            254,

            /* mdt_long_blend */
            0,

            /* ot_op_mode motion_comp */
            OT_EIS_OP_MODE_AUTO,

            /* ot_eis_vi_fswdr_manual_attr */
            {
                /* md_thr_low_gain */
                64, 64
            },

            /* ot_eis_vi_fswdr_auto_attr */
            {
                /* md_thr_low_gain[10][16] */
                {
                    { 45, 45, 45, 64, 96, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 45, 45, 45, 64, 96, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 24, 24, 24, 45, 64,  96, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 24, 24, 24, 45, 64,  96, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 45, 45, 45, 45, 64, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 45, 45, 45, 45, 64, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 64, 64, 64, 64, 128, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 128, 128, 128, 128, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 128, 128, 128, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 }
                },

                /* md_thr_hig_gain[10][16] */
                {
                    { 64, 64, 64, 64, 96, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 64, 64, 64, 64, 96, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 45, 45, 45, 64, 96, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 45, 45, 45, 64, 96, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 64, 64, 64, 64, 96, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 64, 64, 64, 64, 96, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 128, 128, 128, 128, 128, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                    { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
                },
            },
        },
    },

    /* ot_eis_vi_fusion_attr */
    {
        /* fusion blend en */
        1,
        /* fusion_blend_wgt */
        4,
        /* fusion_threshold */
        { 3855, 3000, 3000, 3000 },
        /* fusion_force_gray_en */
        1,
        /* fusion_force_blend_threshold */
        14,
    },
};

static const ot_eis_vi_dehaze_attr g_cmos_dehaze = {
    /* enable */
    0,
    /* user_lut_enable */
    0,
    /* dehaze_lut */
    {
        0, 9, 13, 17, 21, 25, 29, 33, 37, 42, 46, 50, 54, 58, 62, 67, 71, 75, 79, 83, 87, 91,
        95, 99, 102, 106, 110, 114, 117, 121, 124, 128, 131, 134, 137, 141, 144, 147, 150,
        153, 156, 159, 162, 165, 168, 170, 173, 176, 178, 181, 184, 186, 189, 191, 193, 196,
        198, 200, 203, 205, 207, 209, 211, 213, 215, 217, 219, 221, 222, 224, 226, 228, 229,
        231, 232, 234, 235, 237, 238, 240, 241, 242, 243, 244, 246, 247, 248, 249, 249, 250,
        251, 252, 253, 253, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 253, 253, 252, 251, 251, 250, 249,
        249, 248, 247, 247, 246, 245, 244, 244, 243, 242, 241, 240, 239, 238, 237, 236, 235,
        233, 232, 231, 230, 229, 227, 226, 225, 223, 222, 221, 219, 218, 216, 215, 213, 212,
        210, 209, 208, 206, 205, 203, 201, 200, 198, 197, 195, 193, 192, 190, 188, 187, 185,
        183, 181, 180, 178, 176, 174, 172, 170, 169, 167, 165, 163, 161, 159, 157, 155, 153,
        151, 149, 147, 145, 143, 140, 138, 136, 134, 132, 130, 128, 125, 123, 121, 119, 116,
        114, 112, 110, 107, 105, 103, 100, 98, 96, 93, 91, 89, 86, 84, 81, 79, 77, 74, 72, 69,
        67, 64, 62, 59, 57, 54, 52, 49, 47, 44, 42, 39, 37, 34, 31, 29, 26, 24, 21, 18, 16, 13, 11,
        8, 5, 3, 0
    },
    /* op_type */
    0,
    /* auto_strength */
    {
        128
    },
    /* manual_strength */
    {
        128
    },
    /* tmprflt_incr_coef */
    8,
    /* tmprflt_decr_coef */
    64,
};

static const ot_eis_vi_dehaze_attr g_cmos_dehaze_wdr = {
    /* enable */
    1,
    /* user_lut_enable */
    1,
    /* dehaze_lut */
    {
        0, 9, 13, 17, 21, 25, 29, 33, 37, 42, 46, 50, 54, 58, 62, 67, 71, 75, 79, 83, 87, 91,
        95, 99, 102, 106, 110, 114, 117, 121, 124, 128, 131, 134, 137, 141, 144, 147, 150,
        153, 156, 159, 162, 165, 168, 170, 173, 176, 178, 181, 184, 186, 189, 191, 193, 196,
        198, 200, 203, 205, 207, 209, 211, 213, 215, 217, 219, 221, 222, 224, 226, 228, 229,
        231, 232, 234, 235, 237, 238, 240, 241, 242, 243, 244, 246, 247, 248, 249, 249, 250,
        251, 252, 253, 253, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,
        255, 255, 255, 255, 255, 255, 255, 255, 254, 254, 253, 253, 252, 251, 251, 250, 249,
        249, 248, 247, 247, 246, 245, 244, 244, 243, 242, 241, 240, 239, 238, 237, 236, 235,
        233, 232, 231, 230, 229, 227, 226, 225, 223, 222, 221, 219, 218, 216, 215, 213, 212,
        210, 209, 208, 206, 205, 203, 201, 200, 198, 197, 195, 193, 192, 190, 188, 187, 185,
        183, 181, 180, 178, 176, 174, 172, 170, 169, 167, 165, 163, 161, 159, 157, 155, 153,
        151, 149, 147, 145, 143, 140, 138, 136, 134, 132, 130, 128, 125, 123, 121, 119, 116,
        114, 112, 110, 107, 105, 103, 100, 98, 96, 93, 91, 89, 86, 84, 81, 79, 77, 74, 72, 69,
        67, 64, 62, 59, 57, 54, 52, 49, 47, 44, 42, 39, 37, 34, 31, 29, 26, 24, 21, 18, 16, 13, 11,
        8, 5, 3, 0
    },
    /* op_type */
    0,
    /* auto_strength */
    {
        100
    },
    /* manual_strength */
    {
        128
    },
    /* tmprflt_incr_coef */
    8,
    /* tmprflt_decr_coef */
    64,
};

static const ot_eis_vi_drc_attr g_cmos_drc = {
    /* enable */
    0,
    /* curve_select */
    OT_EIS_VI_DRC_CURVE_ASYMMETRY,
    /* purple_reduction_strength */
    35,
    /* bright_gain_limit */
    10,
    /* bright_gain_limit_step */
    8,
    /* dark_gain_limit_luma */
    0,
    /* dark_gain_limit_chroma */
    0,
    /* contrast_ctrl */
    8,
    /* rim_reduction_strength */
    32,
    /* rim_reduction_threshold */
    30,
    /* color_correction_lut[OT_EIS_VI_DRC_CC_NODE_NUM] */
    {1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
     1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024},
    /* tone_mapping_value[OT_EIS_VI_DRC_TM_NODE_NUM] */
    {8,     8,     16,    24,    31,    39,    48,    56,    65,    75,    85,    95,    106,   118,   130,   143,
     156,   170,   185,   201,   218,   235,   254,   274,   294,   316,   339,   364,   390,   417,   446,   477,
     509,   543,   579,   617,   658,   701,   746,   794,   844,   898,   955,   1015,  1078,  1145,  1216,  1291,
     1370,  1454,  1543,  1637,  1736,  1841,  1952,  2069,  2194,  2325,  2465,  2612,  2767,  2932,  3106,  3290,
     3485,  3691,  3909,  4140,  4384,  4641,  4914,  5202,  5507,  5830,  6171,  6531,  6913,  7316,  7742,  8193,
     8669,  9173,  9705,  10268, 10863, 11492, 12145, 12808, 13483, 14171, 14872, 15587, 16319, 17069, 17840, 18635,
     19458, 19881, 20313, 20754, 21204, 21661, 22122, 22586, 23053, 23525, 24000, 24480, 24965, 25455, 25950, 26451,
     26959, 27473, 27995, 28524, 29062, 29609, 30165, 30732, 31309, 31899, 32501, 33116, 33746, 34391, 35043, 35706,
     36381, 37066, 37763, 38472, 39192, 39925, 40671, 41429, 42201, 42591, 42986, 43383, 43784, 44189, 44597, 45008,
     45424, 45842, 46265, 46691, 47121, 47555, 47993, 48434, 48880, 49329, 49783, 50241, 50703, 51169, 51639, 52113,
     52592, 53075, 53564, 54056, 54552, 55054, 55560, 56071, 56586, 56846, 57107, 57369, 57632, 57896, 58162, 58429,
     58697, 58967, 59238, 59510, 59783, 60057, 60333, 60611, 60889, 61169, 61451, 61733, 62017, 62303, 62589, 62877,
     63167, 63458, 63750, 64044, 64340, 64636, 64934, 65234},
    /* spatial_filter_coef */
    1,
    /* range_filter_coef */
    2,
    /* detail_adjust_coef */
    8,
    /* local_mixing_bright[OT_EIS_VI_DRC_LMIX_NODE_NUM] */
    {15, 17, 18, 20, 22, 23, 25, 26, 28, 29, 31, 32, 34, 36, 37, 39, 40, 42,
     43, 45, 46, 48, 49, 51, 53, 54, 56, 57, 59, 61, 62, 63, 64},
    /* local_mixing_dark[OT_EIS_VI_DRC_LMIX_NODE_NUM] */
    {15, 17, 18, 20, 22, 23, 25, 26, 28, 29, 31, 32, 34, 36, 37, 39, 40, 42,
     43, 45, 46, 48, 49, 51, 53, 54, 56, 57, 59, 61, 62, 63, 64},
    /* high_saturation_color_ctrl */
    15,
    /* global_color_ctrl */
    0,
    /* shoot_reduction_en */
    1,
    /* op_type */
    OT_EIS_OP_MODE_AUTO,
    /* manual_attr */
    {
        /* manual_strength */
        1023,
    },
    /* auto_attr */
    {
        /* auto_strength */
        512,
        /* auto_strength_max */
        1023,
        /* auto_strength_min */
        512,
    },
    /* asymmetry_curve */
    {
        /* asymmetry */
        6,
        /* second_pole */
        200,
        /* stretch */
        40,
        /* compress */
        170
    }
};

static const ot_eis_vi_drc_attr g_cmos_drc_built_in = {
    /* enable */
    1,
    /* curve_select */
    OT_EIS_VI_DRC_CURVE_ASYMMETRY,
    /* purple_reduction_strength */
    35,
    /* bright_gain_limit */
    10,
    /* bright_gain_limit_step */
    8,
    /* dark_gain_limit_luma */
    0,
    /* dark_gain_limit_chroma */
    0,
    /* contrast_ctrl */
    8,
    /* rim_reduction_strength */
    32,
    /* rim_reduction_threshold */
    30,
    /* color_correction_lut[OT_EIS_VI_DRC_CC_NODE_NUM] */
    {1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
     1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024},
    /* tone_mapping_value[OT_EIS_VI_DRC_TM_NODE_NUM] */
    {8,     8,     16,    24,    31,    39,    48,    56,    65,    75,    85,    95,    106,   118,   130,   143,
     156,   170,   185,   201,   218,   235,   254,   274,   294,   316,   339,   364,   390,   417,   446,   477,
     509,   543,   579,   617,   658,   701,   746,   794,   844,   898,   955,   1015,  1078,  1145,  1216,  1291,
     1370,  1454,  1543,  1637,  1736,  1841,  1952,  2069,  2194,  2325,  2465,  2612,  2767,  2932,  3106,  3290,
     3485,  3691,  3909,  4140,  4384,  4641,  4914,  5202,  5507,  5830,  6171,  6531,  6913,  7316,  7742,  8193,
     8669,  9173,  9705,  10268, 10863, 11492, 12145, 12808, 13483, 14171, 14872, 15587, 16319, 17069, 17840, 18635,
     19458, 19881, 20313, 20754, 21204, 21661, 22122, 22586, 23053, 23525, 24000, 24480, 24965, 25455, 25950, 26451,
     26959, 27473, 27995, 28524, 29062, 29609, 30165, 30732, 31309, 31899, 32501, 33116, 33746, 34391, 35043, 35706,
     36381, 37066, 37763, 38472, 39192, 39925, 40671, 41429, 42201, 42591, 42986, 43383, 43784, 44189, 44597, 45008,
     45424, 45842, 46265, 46691, 47121, 47555, 47993, 48434, 48880, 49329, 49783, 50241, 50703, 51169, 51639, 52113,
     52592, 53075, 53564, 54056, 54552, 55054, 55560, 56071, 56586, 56846, 57107, 57369, 57632, 57896, 58162, 58429,
     58697, 58967, 59238, 59510, 59783, 60057, 60333, 60611, 60889, 61169, 61451, 61733, 62017, 62303, 62589, 62877,
     63167, 63458, 63750, 64044, 64340, 64636, 64934, 65234},
    /* spatial_filter_coef */
    1,
    /* range_filter_coef */
    2,
    /* detail_adjust_coef */
    8,
    /* local_mixing_bright[OT_EIS_VI_DRC_LMIX_NODE_NUM] */
    {15, 17, 18, 20, 22, 23, 25, 26, 28, 29, 31, 32, 34, 36, 37, 39, 40, 42,
     43, 45, 46, 48, 49, 51, 53, 54, 56, 57, 59, 61, 62, 63, 64},
    /* local_mixing_dark[OT_EIS_VI_DRC_LMIX_NODE_NUM] */
    {15, 17, 18, 20, 22, 23, 25, 26, 28, 29, 31, 32, 34, 36, 37, 39, 40, 42,
     43, 45, 46, 48, 49, 51, 53, 54, 56, 57, 59, 61, 62, 63, 64},
    /* high_saturation_color_ctrl */
    15,
    /* global_color_ctrl */
    0,
    /* shoot_reduction_en */
    1,
    /* op_type */
    OT_EIS_OP_MODE_MANUAL,
    /* manual_attr */
    {
        /* manual_strength */
        1023,
    },
    /* auto_attr */
    {
        /* auto_strength */
        512,
        /* auto_strength_max */
        1023,
        /* auto_strength_min */
        512,
    },
    /* asymmetry_curve */
    {
        /* asymmetry */
        6,
        /* second_pole */
        200,
        /* stretch */
        40,
        /* compress */
        170
    }
};

static const ot_eis_vi_drc_attr g_cmos_drc_wdr = {
    /* enable */
    1,
    /* curve_select */
    OT_EIS_VI_DRC_CURVE_ASYMMETRY,
    /* purple_reduction_strength */
    35,
    /* bright_gain_limit */
    10,
    /* bright_gain_limit_step */
    8,
    /* dark_gain_limit_luma */
    0,
    /* dark_gain_limit_chroma */
    0,
    /* contrast_ctrl */
    8,
    /* rim_reduction_strength */
    32,
    /* rim_reduction_threshold */
    30,
    /* color_correction_lut[OT_EIS_VI_DRC_CC_NODE_NUM] */
    {1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
     1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024},
    /* tone_mapping_value[OT_EIS_VI_DRC_TM_NODE_NUM] */
    {8,     8,     16,    24,    31,    39,    48,    56,    65,    75,    85,    95,    106,   118,   130,   143,
     156,   170,   185,   201,   218,   235,   254,   274,   294,   316,   339,   364,   390,   417,   446,   477,
     509,   543,   579,   617,   658,   701,   746,   794,   844,   898,   955,   1015,  1078,  1145,  1216,  1291,
     1370,  1454,  1543,  1637,  1736,  1841,  1952,  2069,  2194,  2325,  2465,  2612,  2767,  2932,  3106,  3290,
     3485,  3691,  3909,  4140,  4384,  4641,  4914,  5202,  5507,  5830,  6171,  6531,  6913,  7316,  7742,  8193,
     8669,  9173,  9705,  10268, 10863, 11492, 12145, 12808, 13483, 14171, 14872, 15587, 16319, 17069, 17840, 18635,
     19458, 19881, 20313, 20754, 21204, 21661, 22122, 22586, 23053, 23525, 24000, 24480, 24965, 25455, 25950, 26451,
     26959, 27473, 27995, 28524, 29062, 29609, 30165, 30732, 31309, 31899, 32501, 33116, 33746, 34391, 35043, 35706,
     36381, 37066, 37763, 38472, 39192, 39925, 40671, 41429, 42201, 42591, 42986, 43383, 43784, 44189, 44597, 45008,
     45424, 45842, 46265, 46691, 47121, 47555, 47993, 48434, 48880, 49329, 49783, 50241, 50703, 51169, 51639, 52113,
     52592, 53075, 53564, 54056, 54552, 55054, 55560, 56071, 56586, 56846, 57107, 57369, 57632, 57896, 58162, 58429,
     58697, 58967, 59238, 59510, 59783, 60057, 60333, 60611, 60889, 61169, 61451, 61733, 62017, 62303, 62589, 62877,
     63167, 63458, 63750, 64044, 64340, 64636, 64934, 65234},
    /* spatial_filter_coef */
    1,
    /* range_filter_coef */
    2,
    /* detail_adjust_coef */
    8,
    /* local_mixing_bright[OT_EIS_VI_DRC_LMIX_NODE_NUM] */
    {15, 17, 18, 20, 22, 23, 25, 26, 28, 29, 31, 32, 34, 36, 37, 39, 40, 42,
     43, 45, 46, 48, 49, 51, 53, 54, 56, 57, 59, 61, 62, 63, 64},
    /* local_mixing_dark[OT_EIS_VI_DRC_LMIX_NODE_NUM] */
    {15, 17, 18, 20, 22, 23, 25, 26, 28, 29, 31, 32, 34, 36, 37, 39, 40, 42,
     43, 45, 46, 48, 49, 51, 53, 54, 56, 57, 59, 61, 62, 63, 64},
    /* high_saturation_color_ctrl */
    15,
    /* global_color_ctrl */
    0,
    /* shoot_reduction_en */
    1,
    /* op_type */
    OT_EIS_OP_MODE_AUTO,
    /* manual_attr */
    {
        /* manual_strength */
        1023,
    },
    /* auto_attr */
    {
        /* auto_strength */
        512,
        /* auto_strength_max */
        1023,
        /* auto_strength_min */
        512,
    },
    /* asymmetry_curve */
    {
        /* asymmetry */
        6,
        /* second_pole */
        200,
        /* stretch */
        40,
        /* compress */
        170
    }
};

static const ot_eis_vi_sns_lsc g_cmos_lsc = {
    /* cmos_lsc_attr */
    {
        /* en */
        1,
        /* mesh_strength */
        4096,
        /* blend_ratio */
        256,
    },
    /* cmos_lsc_lut */
    {
        /* mesh_scale */
        4,
        /* x_grid_width */
        {30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30},
        /* y_grid_width */
        {24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24},
        /* lsc_gain_lut */
        {
            {
               //RGain:
               {
                 555, 514, 471, 426, 390, 338, 324, 280, 249, 244, 221, 194, 198, 179, 169, 169, 169, 163, 177, 187, 188, 204, 224, 234, 272, 299, 318, 346, 382, 437, 472, 552, 586, 
                 543, 492, 444, 395, 365, 323, 303, 271, 241, 234, 198, 182, 175, 159, 155, 156, 154, 154, 158, 164, 171, 184, 197, 222, 243, 273, 298, 330, 365, 407, 460, 500, 567, 
                 506, 470, 424, 364, 327, 301, 263, 236, 214, 203, 173, 166, 145, 134, 130, 123, 135, 117, 127, 138, 151, 157, 173, 200, 216, 238, 266, 303, 343, 378, 427, 474, 536, 
                 479, 436, 386, 348, 314, 264, 246, 211, 199, 172, 159, 147, 135, 120, 109, 103, 103, 111, 113, 116, 132, 137, 159, 181, 198, 218, 249, 281, 317, 352, 396, 447, 485, 
                 446, 409, 355, 331, 277, 254, 228, 196, 177, 146, 138, 113, 103, 94, 87, 90, 81, 82, 91, 95, 104, 113, 143, 151, 181, 191, 227, 243, 296, 323, 358, 408, 449, 
                 419, 378, 345, 305, 275, 231, 206, 182, 156, 138, 113, 102, 91, 87, 74, 66, 71, 68, 77, 86, 87, 99, 112, 130, 158, 180, 204, 226, 267, 310, 353, 399, 428, 
                 409, 372, 326, 282, 245, 219, 193, 156, 148, 114, 98, 82, 71, 71, 61, 53, 53, 51, 64, 63, 72, 81, 108, 111, 133, 155, 196, 218, 253, 287, 328, 377, 400, 
                 381, 349, 309, 265, 236, 199, 168, 150, 130, 105, 92, 82, 68, 58, 51, 39, 42, 46, 45, 51, 70, 73, 80, 101, 125, 148, 176, 200, 227, 275, 303, 356, 393, 
                 355, 334, 287, 255, 222, 179, 155, 127, 119, 90, 77, 65, 52, 48, 39, 35, 30, 34, 39, 42, 52, 62, 72, 86, 102, 132, 156, 180, 217, 254, 284, 337, 370, 
                 348, 329, 289, 243, 209, 180, 145, 119, 105, 87, 73, 54, 46, 34, 27, 26, 23, 25, 27, 33, 45, 50, 65, 80, 94, 119, 144, 175, 215, 236, 284, 333, 382, 
                 357, 310, 264, 238, 207, 161, 140, 115, 83, 71, 64, 51, 35, 26, 24, 19, 18, 14, 22, 28, 36, 39, 51, 76, 90, 111, 130, 165, 193, 243, 263, 322, 351, 
                 332, 301, 256, 229, 191, 155, 121, 106, 90, 74, 58, 40, 32, 18, 15, 12, 8, 11, 9, 17, 23, 42, 49, 65, 78, 104, 129, 148, 185, 217, 255, 316, 335, 
                 331, 293, 260, 220, 183, 150, 127, 103, 83, 66, 50, 39, 30, 20, 2, 6, 9, 9, 11, 17, 24, 33, 51, 60, 78, 95, 121, 150, 191, 221, 258, 296, 329, 
                 320, 294, 250, 209, 177, 139, 116, 99, 76, 64, 52, 34, 22, 16, 8, 4, 0, 4, 8, 10, 22, 31, 43, 55, 75, 93, 120, 147, 180, 213, 255, 287, 333, 
                 330, 287, 250, 216, 180, 150, 118, 95, 79, 62, 40, 31, 26, 20, 9, 4, 2, 6, 4, 11, 25, 32, 34, 59, 74, 93, 116, 156, 171, 219, 248, 302, 321, 
                 328, 288, 248, 219, 181, 151, 121, 96, 82, 60, 46, 33, 21, 10, 8, 0, 9, 4, 7, 10, 23, 26, 41, 61, 75, 94, 114, 145, 178, 204, 249, 300, 325, 
                 331, 282, 243, 211, 176, 151, 122, 94, 79, 66, 43, 32, 17, 15, 4, 4, 0, 1, 3, 14, 19, 29, 39, 55, 72, 87, 119, 139, 171, 208, 254, 288, 320, 
                 327, 296, 251, 207, 182, 150, 120, 100, 84, 66, 47, 34, 24, 14, 6, 2, 3, 1, 5, 11, 22, 31, 47, 57, 72, 89, 121, 147, 181, 209, 246, 291, 329, 
                 332, 294, 258, 220, 181, 150, 130, 104, 84, 59, 52, 39, 31, 19, 11, 11, 3, 2, 9, 18, 30, 34, 49, 59, 77, 105, 126, 152, 184, 215, 259, 307, 342, 
                 333, 304, 258, 228, 197, 165, 136, 115, 90, 74, 57, 43, 31, 22, 13, 6, 10, 10, 11, 22, 28, 34, 49, 62, 85, 106, 133, 159, 191, 228, 260, 317, 344, 
                 340, 313, 255, 231, 189, 170, 132, 113, 95, 77, 58, 49, 40, 29, 16, 17, 17, 13, 20, 21, 32, 43, 65, 71, 91, 107, 136, 168, 196, 228, 269, 326, 338, 
                 345, 339, 285, 241, 212, 178, 150, 127, 107, 84, 71, 60, 47, 33, 29, 25, 25, 27, 24, 33, 41, 49, 71, 84, 97, 119, 150, 184, 208, 242, 281, 326, 360, 
                 361, 340, 292, 260, 216, 186, 170, 142, 116, 98, 81, 70, 54, 55, 47, 37, 36, 40, 47, 42, 57, 61, 80, 94, 109, 142, 154, 184, 211, 248, 296, 338, 367, 
                 397, 354, 318, 270, 236, 203, 172, 161, 134, 111, 86, 79, 69, 55, 58, 45, 48, 47, 55, 62, 67, 76, 94, 105, 129, 145, 175, 203, 237, 265, 308, 353, 388, 
                 404, 374, 328, 295, 252, 221, 194, 166, 144, 119, 113, 90, 81, 76, 67, 61, 58, 58, 61, 66, 80, 86, 102, 126, 140, 154, 187, 212, 247, 284, 320, 374, 400, 
                 427, 391, 352, 307, 279, 237, 213, 188, 160, 141, 122, 115, 98, 89, 77, 77, 70, 76, 77, 80, 94, 111, 122, 138, 156, 179, 207, 227, 271, 315, 353, 398, 423, 
                 448, 418, 367, 331, 295, 257, 224, 206, 180, 170, 141, 134, 114, 107, 99, 100, 92, 89, 97, 102, 107, 126, 147, 154, 172, 193, 233, 256, 287, 324, 375, 421, 456, 
                 468, 440, 395, 346, 320, 278, 254, 224, 196, 185, 166, 151, 133, 125, 121, 115, 109, 110, 116, 122, 135, 134, 165, 169, 191, 221, 247, 270, 308, 351, 395, 436, 461, 
                 497, 453, 410, 378, 335, 305, 268, 249, 226, 208, 185, 166, 158, 151, 151, 134, 127, 133, 137, 153, 152, 167, 182, 197, 221, 251, 269, 315, 329, 378, 419, 462, 494, 
                 531, 498, 454, 402, 366, 339, 306, 277, 251, 232, 218, 195, 183, 176, 166, 167, 159, 163, 166, 172, 176, 186, 207, 224, 239, 263, 304, 325, 364, 411, 455, 501, 548, 
                 577, 545, 474, 441, 398, 367, 340, 305, 274, 250, 236, 221, 215, 204, 194, 190, 188, 185, 186, 190, 207, 221, 234, 242, 278, 304, 322, 358, 392, 438, 478, 529, 572, 
                 615, 564, 502, 474, 427, 393, 350, 323, 300, 278, 259, 244, 236, 224, 210, 209, 202, 204, 207, 210, 228, 240, 250, 269, 303, 322, 346, 374, 421, 462, 493, 563, 634, 
                 640, 612, 545, 503, 453, 420, 382, 365, 327, 326, 293, 267, 259, 255, 233, 237, 238, 236, 231, 236, 253, 271, 270, 293, 325, 363, 380, 402, 457, 491, 554, 601, 652
               },
               //GrGain:
               {
                 348, 321, 274, 248, 227, 202, 184, 168, 151, 140, 129, 115, 113, 100, 102, 96, 98, 89, 106, 104, 111, 114, 129, 136, 151, 178, 192, 200, 224, 261, 282, 328, 375, 
                 335, 298, 267, 240, 215, 195, 181, 163, 146, 132, 120, 109, 104, 95, 89, 86, 87, 93, 93, 98, 104, 111, 121, 133, 148, 165, 175, 199, 225, 248, 272, 310, 346, 
                 310, 281, 252, 221, 204, 180, 159, 144, 129, 117, 109, 101, 92, 82, 84, 77, 79, 78, 77, 88, 94, 96, 110, 119, 131, 148, 164, 192, 209, 232, 262, 286, 319, 
                 284, 260, 235, 214, 190, 170, 151, 134, 123, 111, 97, 88, 79, 74, 68, 65, 64, 65, 69, 74, 80, 87, 99, 107, 120, 134, 150, 172, 197, 221, 244, 267, 289, 
                 270, 246, 219, 198, 171, 152, 137, 124, 108, 92, 85, 70, 68, 59, 56, 55, 51, 56, 52, 60, 67, 74, 82, 96, 108, 125, 138, 155, 179, 201, 219, 251, 273, 
                 256, 232, 206, 188, 162, 139, 124, 111, 99, 87, 67, 65, 55, 50, 42, 40, 41, 46, 47, 48, 56, 65, 74, 83, 93, 112, 125, 144, 162, 190, 212, 238, 262, 
                 236, 225, 194, 174, 153, 129, 115, 102, 87, 77, 64, 53, 43, 41, 36, 32, 36, 35, 37, 42, 45, 51, 60, 69, 84, 97, 113, 133, 155, 177, 200, 231, 242, 
                 223, 211, 184, 160, 141, 125, 106, 92, 76, 64, 56, 47, 42, 34, 27, 31, 27, 32, 31, 38, 39, 45, 56, 61, 76, 96, 105, 123, 137, 163, 187, 215, 232, 
                 218, 203, 174, 158, 139, 114, 97, 82, 69, 60, 49, 38, 30, 28, 25, 20, 20, 19, 26, 28, 33, 37, 48, 54, 67, 85, 97, 114, 130, 152, 174, 205, 221, 
                 209, 193, 167, 149, 129, 109, 90, 73, 63, 51, 40, 38, 26, 25, 21, 16, 19, 14, 22, 21, 30, 31, 37, 50, 60, 72, 86, 108, 125, 148, 169, 200, 217, 
                 201, 187, 163, 143, 122, 102, 84, 68, 53, 48, 38, 34, 24, 19, 13, 13, 15, 14, 18, 17, 25, 28, 36, 42, 58, 70, 83, 97, 116, 141, 160, 191, 200, 
                 197, 184, 154, 136, 119, 96, 77, 68, 55, 42, 36, 28, 23, 13, 9, 12, 7, 9, 11, 12, 18, 27, 31, 43, 51, 63, 73, 95, 120, 135, 153, 185, 194, 
                 188, 175, 153, 133, 114, 98, 76, 65, 52, 39, 36, 26, 14, 11, 6, 6, 7, 9, 7, 11, 18, 23, 30, 38, 52, 60, 73, 94, 108, 130, 156, 176, 195, 
                 184, 176, 150, 129, 108, 87, 73, 60, 52, 38, 32, 23, 18, 13, 8, 6, 3, 8, 6, 13, 17, 23, 30, 40, 47, 58, 73, 91, 108, 127, 150, 175, 194, 
                 185, 172, 150, 134, 106, 91, 71, 58, 50, 42, 30, 22, 14, 11, 7, 2, 3, 2, 6, 9, 12, 24, 28, 39, 47, 54, 74, 93, 106, 127, 151, 175, 197, 
                 188, 173, 148, 131, 109, 88, 70, 57, 49, 38, 28, 20, 17, 7, 9, 6, 6, 8, 8, 13, 10, 24, 25, 36, 46, 63, 71, 88, 109, 127, 152, 172, 195, 
                 195, 171, 148, 130, 109, 93, 74, 62, 48, 41, 30, 25, 18, 10, 7, 1, 0, 3, 7, 10, 16, 19, 28, 40, 48, 57, 72, 89, 106, 128, 149, 170, 197, 
                 190, 178, 155, 126, 116, 97, 77, 62, 55, 40, 28, 24, 15, 9, 5, 6, 4, 4, 4, 13, 18, 21, 29, 40, 47, 61, 74, 90, 107, 126, 152, 171, 188, 
                 196, 179, 155, 137, 114, 98, 79, 68, 54, 41, 34, 29, 20, 13, 9, 8, 7, 10, 11, 17, 21, 27, 30, 41, 50, 61, 78, 96, 110, 131, 157, 186, 201, 
                 197, 182, 159, 139, 120, 101, 80, 72, 54, 42, 41, 26, 22, 20, 14, 10, 11, 12, 13, 18, 23, 23, 39, 43, 49, 64, 79, 98, 114, 139, 160, 187, 202, 
                 200, 191, 162, 143, 122, 105, 82, 73, 63, 50, 38, 29, 26, 21, 17, 14, 13, 14, 17, 18, 25, 28, 37, 46, 56, 68, 81, 105, 120, 143, 164, 190, 206, 
                 207, 205, 174, 153, 128, 109, 92, 83, 65, 61, 46, 36, 32, 29, 24, 19, 21, 24, 20, 29, 29, 39, 49, 56, 61, 80, 88, 104, 128, 147, 169, 197, 217, 
                 225, 204, 179, 161, 134, 118, 100, 85, 74, 62, 55, 46, 37, 33, 29, 24, 25, 29, 32, 33, 39, 43, 49, 59, 66, 81, 94, 114, 128, 151, 176, 204, 217, 
                 237, 218, 193, 167, 148, 132, 110, 99, 83, 72, 63, 54, 46, 41, 35, 33, 32, 34, 38, 41, 46, 49, 60, 68, 81, 93, 106, 124, 141, 162, 188, 214, 235, 
                 242, 228, 204, 183, 155, 134, 122, 106, 95, 77, 71, 60, 54, 49, 45, 40, 38, 40, 43, 47, 54, 56, 72, 72, 84, 104, 114, 130, 144, 170, 197, 222, 244, 
                 261, 247, 215, 198, 173, 147, 132, 119, 101, 94, 81, 73, 68, 63, 56, 54, 48, 50, 53, 54, 62, 69, 76, 90, 100, 113, 125, 143, 164, 196, 210, 235, 256, 
                 275, 258, 231, 207, 179, 158, 141, 130, 117, 103, 93, 86, 77, 72, 67, 61, 58, 61, 63, 71, 69, 81, 94, 96, 108, 121, 142, 154, 175, 200, 227, 253, 266, 
                 294, 277, 250, 218, 197, 178, 155, 143, 132, 118, 109, 96, 91, 83, 77, 72, 74, 72, 70, 75, 82, 90, 103, 106, 121, 134, 150, 171, 188, 211, 241, 266, 282, 
                 305, 285, 265, 237, 214, 192, 172, 158, 142, 129, 117, 108, 105, 101, 97, 87, 87, 88, 89, 94, 94, 106, 115, 120, 136, 156, 164, 183, 203, 232, 255, 283, 295, 
                 335, 312, 278, 259, 232, 212, 191, 178, 158, 148, 135, 124, 122, 111, 109, 104, 102, 100, 105, 109, 112, 116, 126, 136, 152, 161, 183, 201, 226, 243, 274, 297, 325, 
                 367, 338, 302, 275, 251, 233, 208, 193, 171, 160, 152, 143, 137, 130, 118, 118, 113, 118, 115, 121, 121, 132, 141, 154, 165, 183, 195, 220, 245, 269, 295, 326, 353, 
                 396, 359, 315, 297, 271, 243, 222, 206, 192, 175, 160, 154, 148, 137, 130, 123, 125, 128, 129, 132, 142, 145, 152, 164, 178, 197, 210, 226, 252, 284, 311, 340, 379, 
                 414, 383, 335, 319, 284, 262, 238, 225, 207, 189, 183, 171, 161, 153, 150, 141, 142, 150, 141, 144, 151, 162, 168, 178, 195, 211, 225, 246, 272, 304, 336, 369, 406
               },
               //GbGain:
               {
                 370, 323, 287, 262, 236, 208, 186, 177, 157, 148, 140, 122, 113, 110, 109, 99, 100, 98, 109, 106, 117, 121, 126, 140, 159, 173, 186, 207, 233, 257, 288, 322, 374, 
                 344, 312, 274, 248, 225, 201, 183, 169, 156, 137, 123, 115, 106, 99, 96, 91, 90, 94, 94, 103, 109, 115, 126, 134, 150, 161, 183, 206, 221, 247, 274, 309, 349, 
                 311, 291, 261, 230, 209, 185, 168, 149, 133, 121, 109, 100, 93, 87, 80, 80, 76, 75, 81, 92, 95, 97, 105, 120, 136, 145, 166, 185, 207, 230, 256, 291, 319, 
                 293, 269, 247, 218, 200, 175, 154, 140, 128, 115, 100, 93, 86, 76, 72, 71, 69, 69, 70, 74, 84, 89, 102, 116, 125, 137, 153, 171, 193, 219, 245, 272, 289, 
                 275, 251, 223, 203, 183, 156, 140, 126, 112, 97, 87, 76, 72, 66, 57, 53, 57, 57, 58, 62, 69, 76, 83, 97, 110, 121, 138, 159, 183, 199, 221, 258, 279, 
                 259, 242, 213, 193, 167, 145, 129, 115, 101, 87, 77, 67, 57, 57, 52, 45, 45, 47, 49, 53, 63, 68, 72, 85, 98, 111, 129, 143, 171, 189, 213, 242, 263, 
                 249, 228, 205, 187, 159, 136, 121, 106, 90, 73, 65, 56, 46, 49, 43, 43, 35, 41, 39, 44, 45, 56, 65, 75, 86, 101, 118, 134, 156, 176, 199, 232, 249, 
                 238, 218, 191, 168, 151, 125, 111, 94, 82, 69, 60, 52, 45, 39, 31, 31, 29, 33, 38, 39, 44, 49, 52, 62, 77, 95, 104, 124, 140, 170, 193, 213, 235, 
                 227, 208, 186, 159, 140, 119, 105, 85, 70, 66, 53, 47, 34, 33, 30, 27, 24, 24, 33, 32, 33, 39, 55, 59, 69, 80, 102, 116, 134, 156, 177, 205, 222, 
                 214, 203, 180, 155, 136, 119, 95, 83, 69, 56, 50, 39, 33, 26, 27, 17, 19, 21, 23, 27, 35, 36, 46, 54, 66, 77, 93, 112, 129, 150, 175, 202, 221, 
                 209, 192, 172, 146, 125, 106, 86, 69, 59, 52, 44, 34, 28, 19, 20, 21, 12, 16, 23, 16, 27, 35, 40, 46, 62, 72, 87, 101, 120, 144, 163, 191, 211, 
                 208, 187, 167, 148, 121, 102, 82, 71, 59, 48, 39, 29, 22, 17, 19, 16, 9, 10, 14, 20, 26, 27, 37, 45, 56, 65, 78, 100, 116, 140, 157, 193, 205, 
                 201, 187, 161, 142, 119, 105, 80, 66, 57, 45, 35, 32, 18, 20, 9, 10, 7, 8, 10, 15, 20, 24, 35, 43, 50, 63, 75, 94, 112, 133, 159, 181, 199, 
                 199, 183, 162, 136, 117, 90, 80, 64, 53, 47, 42, 26, 23, 14, 10, 10, 8, 8, 10, 15, 21, 28, 31, 45, 52, 61, 76, 94, 111, 134, 156, 180, 193, 
                 200, 183, 158, 134, 113, 101, 76, 59, 54, 44, 36, 30, 21, 12, 6, 6, 0, 6, 9, 11, 19, 27, 34, 38, 47, 61, 75, 90, 108, 128, 154, 182, 195, 
                 199, 182, 158, 138, 117, 100, 79, 64, 52, 45, 33, 25, 20, 12, 11, 8, 9, 7, 8, 14, 21, 22, 33, 41, 56, 63, 75, 87, 107, 127, 153, 179, 196, 
                 199, 181, 158, 139, 121, 98, 79, 65, 55, 46, 34, 23, 18, 10, 14, 8, 4, 5, 11, 15, 21, 23, 35, 44, 51, 59, 77, 90, 111, 132, 154, 179, 196, 
                 202, 186, 159, 136, 118, 98, 76, 72, 57, 48, 32, 30, 22, 13, 7, 7, 8, 8, 13, 16, 20, 31, 33, 38, 51, 63, 79, 96, 116, 130, 156, 175, 191, 
                 205, 186, 163, 140, 126, 97, 85, 68, 60, 48, 37, 36, 22, 18, 16, 13, 7, 12, 11, 22, 28, 27, 37, 45, 56, 68, 78, 97, 116, 132, 157, 184, 195, 
                 206, 192, 166, 144, 126, 106, 88, 74, 62, 48, 42, 35, 24, 20, 19, 12, 12, 17, 18, 21, 29, 34, 39, 47, 54, 64, 80, 98, 119, 141, 161, 187, 205, 
                 210, 199, 173, 149, 126, 112, 84, 77, 67, 52, 49, 33, 31, 25, 19, 17, 20, 21, 21, 23, 28, 34, 41, 48, 62, 72, 82, 98, 124, 140, 164, 192, 205, 
                 220, 218, 188, 162, 137, 120, 100, 83, 67, 58, 53, 44, 37, 29, 29, 26, 23, 28, 27, 35, 35, 42, 49, 54, 67, 79, 89, 111, 129, 150, 172, 207, 222, 
                 227, 215, 192, 169, 149, 125, 109, 92, 81, 67, 61, 53, 44, 38, 37, 30, 31, 29, 33, 37, 41, 48, 55, 64, 74, 83, 100, 117, 133, 155, 182, 209, 228, 
                 248, 227, 202, 176, 158, 134, 113, 101, 87, 76, 68, 64, 51, 47, 43, 41, 39, 43, 42, 42, 54, 53, 65, 73, 86, 93, 110, 126, 148, 164, 191, 215, 235, 
                 257, 236, 209, 193, 163, 139, 124, 110, 95, 82, 77, 67, 57, 53, 49, 46, 45, 45, 51, 50, 59, 65, 72, 80, 90, 102, 115, 130, 147, 173, 199, 231, 245, 
                 272, 258, 225, 206, 181, 158, 132, 124, 108, 98, 87, 78, 73, 63, 57, 59, 55, 56, 56, 61, 66, 71, 80, 91, 105, 113, 124, 147, 168, 192, 216, 242, 259, 
                 283, 264, 237, 212, 194, 167, 147, 135, 121, 108, 95, 90, 77, 72, 67, 66, 60, 60, 66, 67, 74, 84, 88, 95, 113, 124, 144, 157, 178, 204, 226, 255, 272, 
                 294, 287, 257, 232, 204, 182, 163, 149, 131, 120, 112, 100, 97, 91, 79, 76, 70, 72, 76, 84, 85, 93, 103, 112, 122, 135, 155, 169, 189, 213, 244, 266, 284, 
                 316, 300, 271, 244, 220, 204, 183, 166, 145, 134, 127, 114, 110, 102, 96, 92, 87, 88, 94, 95, 102, 110, 115, 126, 142, 153, 167, 190, 206, 234, 260, 280, 303, 
                 339, 318, 291, 262, 239, 219, 193, 177, 162, 152, 137, 131, 120, 114, 111, 105, 97, 108, 110, 111, 118, 124, 130, 139, 152, 166, 186, 203, 226, 253, 278, 304, 328, 
                 375, 344, 312, 283, 258, 236, 214, 199, 176, 162, 152, 148, 138, 133, 123, 123, 115, 117, 123, 123, 127, 136, 149, 156, 170, 182, 203, 224, 246, 268, 298, 324, 353, 
                 395, 369, 328, 302, 271, 251, 228, 209, 194, 179, 160, 154, 148, 139, 134, 134, 131, 128, 134, 131, 146, 149, 165, 171, 183, 198, 216, 233, 263, 287, 309, 340, 386, 
                 422, 393, 344, 320, 290, 267, 243, 226, 203, 203, 188, 172, 163, 156, 148, 147, 147, 148, 154, 149, 161, 166, 175, 183, 198, 219, 236, 254, 281, 296, 330, 374, 400
               },
               //BGain:
               {
                 295, 276, 239, 217, 197, 185, 165, 157, 133, 131, 109, 106, 104, 105, 98, 98, 87, 84, 97, 109, 102, 114, 108, 121, 139, 154, 165, 181, 213, 226, 259, 292, 326, 
                 294, 259, 229, 209, 185, 168, 158, 144, 130, 118, 108, 98, 101, 92, 90, 80, 79, 84, 84, 95, 94, 104, 115, 113, 130, 134, 160, 174, 192, 212, 242, 273, 318, 
                 263, 233, 207, 185, 171, 155, 137, 122, 113, 101, 93, 82, 84, 72, 77, 65, 71, 65, 66, 78, 89, 87, 90, 101, 110, 121, 139, 154, 174, 202, 226, 250, 279, 
                 246, 215, 198, 175, 166, 146, 130, 116, 102, 97, 80, 76, 73, 65, 63, 64, 60, 54, 61, 61, 69, 75, 91, 99, 108, 118, 129, 141, 166, 188, 208, 238, 263, 
                 234, 202, 177, 159, 144, 130, 108, 100, 89, 84, 69, 53, 61, 49, 52, 51, 40, 40, 46, 49, 62, 72, 71, 82, 90, 103, 111, 124, 148, 167, 183, 222, 234, 
                 215, 195, 173, 152, 133, 115, 98, 90, 81, 74, 64, 60, 53, 48, 40, 42, 37, 38, 41, 45, 47, 58, 62, 69, 85, 88, 104, 120, 130, 158, 181, 203, 226, 
                 201, 182, 155, 138, 125, 103, 99, 84, 68, 60, 59, 47, 43, 42, 40, 31, 33, 30, 31, 35, 40, 47, 57, 61, 65, 91, 96, 112, 136, 153, 169, 200, 201, 
                 192, 175, 144, 133, 111, 100, 88, 76, 63, 60, 46, 40, 39, 30, 30, 23, 24, 26, 28, 35, 35, 48, 47, 53, 68, 77, 85, 99, 116, 146, 157, 193, 201, 
                 194, 163, 141, 128, 116, 96, 82, 61, 62, 53, 49, 43, 38, 25, 28, 28, 19, 19, 31, 25, 33, 39, 46, 47, 59, 64, 79, 91, 109, 131, 150, 176, 187, 
                 178, 161, 144, 125, 105, 95, 71, 61, 64, 43, 38, 32, 30, 27, 24, 16, 21, 21, 23, 30, 28, 43, 40, 46, 63, 61, 78, 87, 110, 125, 147, 175, 203, 
                 169, 155, 135, 115, 101, 87, 69, 56, 48, 36, 37, 31, 28, 24, 21, 20, 10, 18, 14, 19, 32, 28, 36, 36, 54, 57, 62, 81, 102, 118, 144, 165, 187, 
                 164, 153, 125, 110, 90, 79, 68, 63, 46, 39, 33, 32, 30, 14, 17, 15, 17, 8, 14, 23, 28, 30, 30, 39, 47, 55, 57, 76, 96, 111, 132, 162, 181, 
                 164, 142, 124, 117, 97, 81, 69, 60, 46, 47, 40, 35, 22, 24, 13, 11, 9, 9, 12, 19, 26, 26, 35, 36, 41, 53, 63, 80, 87, 108, 136, 150, 167, 
                 164, 150, 127, 105, 91, 76, 63, 56, 47, 44, 38, 30, 24, 21, 14, 16, 14, 20, 17, 15, 28, 29, 39, 37, 45, 60, 67, 80, 96, 113, 138, 154, 175, 
                 160, 139, 122, 104, 95, 81, 64, 53, 46, 42, 35, 27, 17, 19, 14, 9, 10, 11, 16, 15, 29, 33, 32, 36, 45, 55, 64, 81, 89, 111, 123, 153, 172, 
                 162, 147, 124, 110, 90, 80, 66, 52, 46, 45, 33, 22, 22, 18, 18, 11, 18, 15, 15, 20, 24, 32, 32, 37, 50, 60, 65, 76, 94, 109, 131, 146, 177, 
                 167, 149, 125, 110, 88, 82, 60, 57, 48, 40, 40, 25, 21, 14, 17, 12, 6, 12, 14, 20, 25, 28, 34, 39, 43, 54, 62, 73, 96, 110, 130, 148, 177, 
                 153, 147, 130, 104, 88, 80, 56, 59, 50, 45, 33, 26, 24, 17, 9, 9, 0, 10, 10, 22, 27, 28, 35, 35, 49, 60, 66, 79, 98, 111, 134, 152, 176, 
                 170, 149, 122, 108, 99, 78, 70, 60, 51, 44, 33, 31, 26, 16, 11, 17, 8, 18, 18, 25, 30, 36, 34, 42, 42, 63, 70, 84, 98, 114, 136, 153, 179, 
                 174, 152, 131, 115, 101, 88, 76, 68, 58, 42, 41, 27, 25, 23, 23, 11, 17, 19, 24, 25, 29, 37, 38, 44, 54, 58, 71, 81, 102, 116, 138, 162, 172, 
                 167, 153, 134, 118, 98, 90, 72, 66, 54, 49, 47, 32, 30, 27, 19, 16, 24, 17, 22, 28, 33, 42, 42, 44, 55, 57, 67, 86, 108, 119, 139, 166, 177, 
                 178, 170, 146, 118, 107, 100, 80, 74, 57, 57, 42, 45, 34, 25, 27, 27, 20, 28, 33, 31, 32, 39, 43, 53, 60, 70, 80, 93, 113, 121, 150, 172, 200, 
                 191, 171, 154, 132, 113, 100, 90, 74, 66, 57, 50, 52, 44, 42, 35, 30, 32, 32, 38, 33, 43, 43, 47, 62, 65, 73, 85, 93, 110, 131, 147, 176, 195, 
                 208, 178, 156, 137, 121, 105, 100, 87, 74, 69, 62, 55, 47, 38, 40, 33, 37, 35, 39, 45, 49, 57, 60, 63, 73, 81, 91, 103, 123, 139, 160, 182, 208, 
                 211, 189, 174, 147, 127, 115, 102, 92, 83, 67, 70, 53, 56, 47, 39, 39, 37, 40, 45, 51, 54, 55, 74, 66, 81, 87, 95, 113, 125, 141, 169, 192, 214, 
                 226, 200, 178, 158, 145, 130, 117, 100, 92, 84, 75, 64, 64, 59, 48, 54, 48, 50, 42, 52, 61, 68, 72, 79, 93, 102, 109, 123, 145, 165, 181, 202, 228, 
                 223, 214, 191, 170, 140, 138, 122, 107, 97, 88, 82, 78, 68, 59, 54, 58, 52, 59, 58, 59, 60, 76, 85, 85, 95, 109, 121, 124, 152, 166, 190, 219, 233, 
                 245, 222, 194, 185, 155, 143, 134, 120, 104, 93, 94, 82, 88, 64, 62, 59, 58, 65, 64, 63, 76, 78, 97, 95, 103, 112, 129, 139, 162, 179, 202, 230, 240, 
                 258, 235, 213, 193, 174, 150, 140, 128, 113, 113, 98, 91, 88, 86, 83, 69, 70, 67, 80, 76, 89, 97, 100, 107, 118, 134, 135, 153, 165, 189, 218, 239, 271, 
                 269, 250, 224, 196, 186, 171, 153, 148, 127, 113, 112, 110, 101, 95, 90, 89, 84, 87, 88, 93, 94, 104, 110, 124, 127, 138, 155, 171, 188, 208, 232, 251, 289, 
                 294, 275, 246, 223, 204, 186, 170, 162, 143, 141, 121, 113, 109, 100, 97, 96, 102, 94, 95, 105, 105, 119, 126, 129, 149, 144, 165, 183, 203, 224, 247, 270, 309, 
                 322, 284, 255, 230, 217, 195, 181, 172, 155, 141, 135, 120, 116, 105, 100, 103, 99, 108, 106, 109, 115, 121, 136, 136, 153, 168, 172, 181, 206, 232, 260, 290, 333, 
                 352, 319, 292, 253, 237, 225, 199, 192, 175, 163, 146, 145, 128, 129, 118, 125, 125, 134, 126, 124, 137, 142, 147, 150, 164, 188, 191, 217, 240, 255, 288, 330, 356
               },
            },
            {
                /* r_gain: */
                {
                 1023, 1023, 936, 767, 655, 566, 494, 433, 383, 336, 293, 268, 242, 225, 209, 203, 201, 199, 202, 217, 232, 254, 279, 311, 346, 393, 443, 512, 593, 702, 846, 1023, 1023, 
                 1023, 1023, 851, 705, 598, 514, 450, 394, 343, 300, 263, 235, 213, 192, 184, 173, 168, 170, 176, 186, 203, 225, 248, 279, 314, 358, 410, 470, 546, 642, 770, 956, 1023, 
                 1023, 949, 763, 638, 541, 470, 405, 351, 305, 262, 233, 204, 179, 162, 150, 141, 140, 140, 149, 156, 170, 190, 218, 245, 279, 320, 371, 428, 498, 584, 700, 856, 1020, 
                 1023, 858, 700, 580, 496, 428, 368, 318, 272, 236, 202, 175, 151, 136, 123, 116, 111, 112, 119, 129, 144, 161, 186, 213, 248, 287, 337, 391, 461, 543, 644, 779, 922, 
                 944, 786, 646, 538, 460, 393, 341, 289, 247, 210, 176, 146, 127, 110, 97, 88, 82, 89, 95, 105, 118, 139, 161, 191, 220, 258, 309, 367, 427, 501, 595, 719, 839, 
                 864, 726, 601, 504, 430, 367, 313, 263, 222, 186, 152, 126, 102, 85, 74, 68, 63, 65, 72, 80, 94, 114, 138, 168, 200, 239, 287, 339, 398, 467, 555, 668, 772, 
                 800, 679, 565, 476, 401, 344, 287, 242, 199, 164, 133, 108, 84, 64, 53, 48, 42, 46, 52, 59, 75, 96, 120, 146, 180, 222, 264, 318, 376, 444, 524, 625, 722, 
                 759, 647, 538, 450, 385, 322, 268, 224, 181, 146, 113, 91, 65, 47, 35, 29, 26, 28, 31, 41, 55, 76, 105, 132, 166, 205, 247, 297, 353, 418, 496, 591, 688, 
                 714, 617, 513, 431, 363, 303, 252, 206, 169, 131, 100, 74, 48, 34, 21, 16, 12, 14, 17, 24, 41, 60, 86, 118, 154, 191, 232, 281, 339, 401, 474, 563, 651, 
                 698, 594, 494, 415, 349, 287, 237, 192, 152, 118, 89, 60, 38, 23, 10, 7, 4, 5, 8, 18, 27, 52, 79, 108, 143, 176, 218, 265, 324, 383, 456, 544, 626, 
                 669, 578, 483, 399, 335, 277, 229, 182, 143, 110, 76, 50, 26, 13, 6, 5, 2, 3, 4, 13, 26, 46, 69, 101, 132, 168, 211, 255, 314, 374, 442, 529, 596, 
                 660, 566, 474, 396, 329, 271, 221, 176, 136, 101, 69, 41, 21, 8, 4, 2, 1, 1, 6, 13, 21, 39, 63, 95, 125, 162, 203, 250, 305, 362, 438, 520, 593, 
                 649, 556, 461, 388, 325, 265, 215, 173, 132, 96, 65, 36, 16, 8, 2, 2, 0, 2, 9, 14, 20, 36, 60, 90, 123, 159, 199, 247, 301, 359, 431, 514, 581, 
                 644, 549, 461, 391, 321, 264, 214, 170, 129, 93, 64, 37, 15, 6, 2, 3, 3, 5, 10, 14, 20, 35, 59, 85, 119, 160, 196, 244, 299, 357, 424, 506, 574, 
                 634, 549, 460, 388, 322, 266, 216, 172, 127, 92, 65, 38, 17, 8, 2, 6, 6, 7, 12, 16, 19, 34, 61, 86, 120, 158, 198, 243, 300, 362, 431, 509, 579, 
                 648, 559, 466, 390, 325, 267, 220, 173, 130, 96, 66, 42, 21, 11, 9, 6, 6, 9, 12, 16, 23, 40, 63, 94, 124, 162, 202, 247, 304, 366, 434, 518, 591, 
                 653, 560, 468, 396, 332, 276, 224, 179, 139, 104, 75, 48, 30, 20, 13, 13, 10, 10, 12, 19, 30, 47, 72, 100, 132, 168, 210, 255, 308, 370, 443, 528, 601, 
                 677, 577, 481, 405, 338, 280, 232, 186, 146, 113, 83, 59, 42, 27, 19, 14, 14, 15, 17, 26, 35, 56, 79, 109, 143, 176, 219, 266, 320, 378, 452, 538, 613, 
                 696, 597, 496, 418, 353, 295, 242, 199, 159, 124, 96, 73, 53, 39, 26, 21, 19, 17, 21, 34, 48, 69, 92, 120, 155, 190, 230, 277, 336, 397, 468, 560, 638, 
                 720, 626, 517, 435, 368, 308, 258, 213, 173, 141, 113, 89, 66, 52, 38, 32, 29, 30, 37, 47, 60, 82, 106, 133, 169, 204, 247, 293, 352, 413, 493, 583, 667, 
                 764, 654, 544, 462, 389, 326, 276, 230, 195, 159, 131, 105, 83, 68, 55, 48, 47, 46, 53, 65, 79, 96, 123, 154, 184, 220, 264, 314, 372, 436, 514, 614, 701, 
                 811, 692, 577, 488, 417, 350, 296, 256, 215, 178, 153, 126, 103, 89, 73, 66, 62, 67, 75, 84, 99, 119, 144, 172, 208, 242, 286, 335, 395, 460, 545, 647, 745, 
                 877, 741, 616, 520, 446, 384, 326, 280, 241, 203, 172, 149, 126, 110, 96, 92, 86, 89, 97, 107, 121, 140, 167, 194, 230, 265, 315, 360, 421, 492, 583, 696, 799, 
                 960, 806, 670, 560, 490, 414, 358, 312, 266, 228, 198, 173, 152, 134, 121, 110, 109, 110, 118, 132, 148, 164, 193, 217, 252, 290, 342, 394, 454, 530, 623, 747, 874, 
                 1023, 886, 728, 610, 526, 450, 393, 340, 296, 255, 224, 200, 177, 163, 150, 141, 135, 137, 145, 157, 174, 194, 219, 244, 283, 326, 373, 428, 493, 571, 678, 810, 958, 
                 1023, 984, 798, 674, 571, 492, 429, 376, 332, 289, 258, 236, 211, 196, 183, 171, 167, 168, 176, 189, 204, 224, 250, 282, 315, 364, 410, 468, 538, 627, 737, 893, 1023, 
                 1023, 1023, 895, 743, 630, 539, 471, 420, 373, 331, 294, 276, 253, 232, 217, 208, 201, 201, 210, 220, 238, 261, 290, 320, 358, 402, 452, 514, 591, 687, 820, 1006, 1023, 
                 1023, 1023, 1005, 828, 696, 597, 524, 464, 414, 372, 344, 322, 294, 276, 256, 248, 245, 243, 250, 263, 282, 301, 333, 361, 402, 446, 502, 573, 656, 767, 923, 1023, 1023, 
                 1023, 1023, 1023, 932, 784, 669, 585, 517, 466, 423, 394, 367, 342, 319, 302, 291, 286, 281, 290, 305, 323, 350, 377, 411, 450, 503, 560, 641, 737, 867, 1023, 1023, 1023, 
                 1023, 1023, 1023, 1023, 887, 756, 656, 582, 520, 475, 442, 417, 391, 367, 352, 341, 325, 328, 343, 353, 370, 395, 430, 459, 506, 564, 633, 719, 838, 1000, 1023, 1023, 1023, 
                 1023, 1023, 1023, 1023, 1023, 862, 747, 653, 591, 536, 496, 466, 440, 417, 400, 388, 375, 380, 385, 398, 421, 448, 487, 521, 580, 636, 718, 820, 962, 1023, 1023, 1023, 1023, 
                 1023, 1023, 1023, 1023, 1023, 1010, 861, 752, 674, 614, 569, 526, 494, 475, 454, 439, 433, 430, 444, 457, 482, 514, 552, 598, 654, 728, 824, 950, 1023, 1023, 1023, 1023, 1023, 
                 1023, 1023, 1023, 1023, 1023, 1023, 973, 849, 752, 673, 632, 583, 548, 525, 500, 490, 485, 481, 488, 510, 536, 571, 612, 663, 722, 809, 929, 1023, 1023, 1023, 1023, 1023, 1023,
                },

                /* gr_gain: */
                {
                 1023, 1023, 889, 730, 621, 534, 463, 405, 358, 314, 280, 249, 226, 208, 198, 190, 181, 188, 194, 201, 214, 233, 257, 288, 324, 367, 418, 479, 561, 656, 792, 992, 1023, 
                 1023, 1015, 805, 666, 572, 489, 426, 372, 325, 285, 250, 221, 200, 182, 173, 164, 158, 160, 167, 176, 190, 207, 230, 261, 295, 336, 386, 445, 518, 606, 725, 895, 1023, 
                 1023, 899, 725, 607, 519, 446, 385, 334, 289, 249, 222, 191, 171, 154, 142, 135, 131, 133, 139, 147, 161, 181, 204, 229, 264, 304, 352, 408, 473, 558, 660, 801, 956, 
                 984, 813, 663, 559, 475, 409, 353, 303, 261, 226, 194, 167, 145, 129, 116, 110, 105, 107, 115, 123, 135, 154, 177, 202, 235, 274, 321, 376, 441, 516, 611, 732, 860, 
                 891, 744, 612, 518, 442, 377, 323, 276, 236, 200, 170, 143, 120, 105, 94, 86, 81, 85, 90, 98, 112, 130, 154, 179, 211, 250, 293, 347, 409, 480, 565, 676, 788, 
                 820, 690, 573, 484, 414, 350, 300, 252, 211, 177, 147, 121, 98, 83, 72, 65, 62, 63, 67, 77, 90, 110, 132, 159, 190, 227, 272, 321, 380, 446, 528, 628, 730, 
                 760, 645, 541, 456, 385, 328, 277, 231, 190, 156, 127, 103, 80, 62, 54, 45, 41, 43, 49, 58, 72, 91, 113, 140, 173, 211, 253, 300, 355, 419, 497, 590, 680, 
                 717, 615, 514, 433, 364, 308, 257, 214, 175, 139, 110, 86, 60, 46, 34, 28, 26, 27, 32, 40, 55, 74, 98, 125, 159, 195, 236, 282, 336, 397, 471, 559, 642, 
                 684, 588, 489, 412, 347, 291, 242, 196, 160, 125, 95, 70, 48, 31, 20, 15, 14, 14, 18, 24, 38, 59, 84, 112, 144, 179, 219, 264, 318, 378, 451, 534, 611, 
                 657, 568, 472, 396, 330, 276, 228, 184, 147, 113, 83, 56, 35, 20, 10, 8, 7, 7, 8, 17, 29, 50, 73, 104, 132, 166, 208, 251, 304, 363, 435, 517, 581, 
                 638, 549, 459, 383, 321, 266, 218, 175, 137, 103, 73, 47, 25, 13, 5, 6, 4, 5, 7, 12, 23, 42, 65, 95, 123, 158, 199, 245, 295, 351, 422, 501, 567, 
                 623, 537, 447, 375, 312, 257, 212, 167, 129, 95, 67, 40, 20, 9, 3, 3, 2, 2, 8, 12, 19, 36, 60, 89, 118, 152, 191, 236, 287, 345, 412, 489, 558, 
                 613, 528, 441, 370, 308, 256, 207, 164, 126, 91, 62, 36, 16, 7, 3, 2, 0, 4, 9, 14, 20, 34, 57, 85, 115, 149, 188, 232, 285, 340, 406, 486, 547, 
                 607, 523, 437, 370, 305, 252, 205, 161, 123, 88, 60, 35, 15, 8, 3, 1, 4, 4, 8, 14, 18, 32, 55, 82, 113, 148, 186, 229, 282, 338, 405, 481, 548, 
                 607, 524, 438, 365, 303, 251, 206, 162, 124, 89, 62, 36, 16, 9, 4, 4, 5, 6, 10, 14, 19, 33, 56, 84, 114, 149, 187, 231, 282, 339, 406, 480, 548, 
                 613, 528, 439, 369, 306, 255, 209, 165, 125, 93, 64, 41, 20, 11, 7, 7, 6, 7, 11, 14, 22, 37, 58, 86, 117, 153, 192, 235, 287, 344, 408, 487, 557, 
                 618, 533, 447, 374, 314, 261, 213, 170, 133, 99, 70, 47, 27, 18, 13, 11, 11, 10, 15, 16, 27, 42, 65, 94, 126, 159, 199, 243, 292, 349, 420, 497, 565, 
                 636, 548, 458, 384, 323, 269, 222, 178, 140, 107, 79, 56, 39, 24, 17, 13, 13, 13, 18, 22, 34, 51, 75, 104, 134, 168, 209, 252, 304, 360, 431, 509, 578, 
                 652, 566, 472, 400, 337, 279, 231, 190, 152, 117, 91, 69, 50, 36, 23, 18, 17, 17, 23, 31, 46, 63, 87, 114, 143, 180, 220, 263, 316, 374, 447, 528, 601, 
                 679, 589, 494, 418, 351, 294, 245, 203, 164, 135, 108, 83, 64, 48, 37, 29, 27, 29, 33, 44, 59, 76, 99, 127, 159, 193, 235, 279, 331, 395, 464, 551, 625, 
                 718, 618, 519, 439, 373, 312, 263, 220, 183, 152, 125, 100, 79, 64, 52, 44, 41, 45, 50, 61, 74, 93, 116, 146, 177, 209, 250, 295, 350, 414, 488, 576, 652, 
                 766, 653, 550, 466, 399, 335, 281, 241, 206, 170, 142, 119, 97, 83, 70, 63, 59, 61, 68, 79, 93, 112, 135, 164, 195, 230, 271, 315, 374, 437, 514, 609, 695, 
                 824, 701, 587, 499, 424, 365, 310, 267, 229, 192, 166, 141, 118, 103, 92, 85, 81, 83, 90, 100, 116, 135, 157, 185, 217, 250, 296, 343, 401, 464, 548, 650, 747, 
                 905, 763, 635, 537, 471, 398, 343, 295, 254, 216, 186, 162, 143, 126, 113, 105, 103, 105, 113, 125, 139, 155, 179, 209, 239, 276, 321, 372, 431, 503, 588, 696, 806, 
                 993, 832, 686, 583, 500, 432, 371, 323, 282, 244, 214, 188, 168, 154, 142, 132, 129, 131, 137, 145, 163, 183, 207, 233, 268, 305, 356, 406, 467, 540, 631, 753, 881, 
                 1023, 922, 755, 641, 544, 473, 410, 357, 312, 271, 245, 223, 201, 184, 172, 164, 160, 159, 167, 176, 191, 212, 236, 265, 300, 343, 390, 444, 508, 590, 688, 826, 990, 
                 1023, 1023, 840, 703, 599, 515, 448, 397, 350, 312, 279, 260, 238, 217, 204, 197, 193, 190, 196, 208, 223, 244, 272, 301, 338, 381, 431, 486, 558, 644, 762, 930, 1023, 
                 1023, 1023, 945, 781, 661, 571, 499, 441, 390, 351, 324, 300, 278, 257, 242, 234, 230, 226, 236, 246, 263, 284, 311, 342, 380, 423, 476, 542, 614, 716, 856, 1023, 1023, 
                 1023, 1023, 1023, 878, 735, 630, 553, 489, 440, 396, 369, 346, 321, 297, 286, 274, 267, 263, 271, 285, 302, 324, 352, 385, 425, 473, 531, 602, 685, 801, 974, 1023, 1023, 
                 1023, 1023, 1023, 1009, 828, 705, 618, 547, 493, 449, 415, 387, 364, 344, 329, 315, 304, 306, 314, 327, 346, 370, 398, 432, 479, 532, 594, 669, 774, 918, 1023, 1023, 1023, 
                 1023, 1023, 1023, 1023, 962, 804, 696, 617, 554, 504, 465, 437, 413, 391, 377, 360, 347, 352, 360, 373, 393, 421, 454, 491, 539, 596, 664, 758, 888, 1023, 1023, 1023, 1023, 
                 1023, 1023, 1023, 1023, 1023, 934, 795, 697, 624, 569, 525, 492, 463, 440, 423, 408, 402, 402, 411, 425, 450, 477, 512, 555, 606, 670, 761, 874, 1023, 1023, 1023, 1023, 1023, 
                 1023, 1023, 1023, 1023, 1023, 1023, 889, 776, 687, 624, 580, 540, 507, 483, 461, 451, 448, 442, 453, 469, 493, 522, 564, 606, 663, 739, 850, 994, 1023, 1023, 1023, 1023, 1023,
                },

                /* gb_gain: */
                {
                 1023, 1023, 889, 726, 618, 533, 466, 409, 361, 314, 276, 246, 225, 208, 194, 188, 184, 183, 191, 200, 211, 234, 257, 286, 323, 365, 419, 477, 560, 653, 791, 993, 1023, 
                 1023, 1017, 804, 666, 569, 492, 429, 374, 327, 284, 250, 221, 200, 182, 174, 163, 158, 159, 166, 176, 190, 207, 230, 259, 292, 334, 384, 444, 516, 604, 724, 894, 1023, 
                 1023, 898, 724, 607, 518, 445, 385, 335, 289, 249, 219, 194, 172, 154, 142, 135, 132, 133, 138, 147, 161, 179, 201, 230, 262, 305, 351, 405, 472, 557, 658, 797, 958, 
                 983, 813, 663, 558, 476, 410, 352, 304, 260, 225, 194, 168, 146, 130, 116, 109, 107, 108, 114, 122, 135, 153, 176, 202, 236, 273, 321, 374, 438, 513, 607, 729, 865, 
                 890, 743, 610, 517, 440, 379, 324, 277, 235, 201, 169, 140, 120, 103, 93, 85, 80, 84, 89, 99, 111, 130, 152, 179, 210, 247, 292, 344, 407, 476, 562, 672, 783, 
                 813, 685, 573, 484, 413, 350, 299, 253, 211, 178, 147, 120, 98, 82, 71, 63, 60, 62, 67, 76, 89, 109, 131, 157, 189, 225, 271, 322, 379, 445, 526, 625, 721, 
                 759, 646, 540, 457, 386, 328, 276, 231, 191, 155, 125, 101, 79, 62, 53, 45, 42, 42, 47, 57, 71, 88, 114, 137, 171, 208, 250, 298, 353, 416, 493, 586, 677, 
                 717, 614, 512, 432, 365, 308, 258, 214, 173, 139, 108, 83, 63, 44, 35, 28, 24, 26, 31, 39, 53, 73, 98, 125, 157, 193, 233, 278, 331, 395, 468, 556, 640, 
                 684, 587, 489, 412, 347, 291, 242, 195, 160, 124, 94, 68, 46, 31, 20, 14, 13, 12, 17, 24, 37, 56, 81, 112, 144, 179, 218, 262, 314, 377, 449, 529, 610, 
                 658, 565, 471, 394, 331, 276, 228, 183, 146, 112, 82, 55, 35, 20, 10, 6, 6, 5, 9, 15, 27, 49, 72, 101, 131, 166, 204, 250, 302, 360, 431, 511, 582, 
                 635, 547, 460, 381, 319, 263, 219, 173, 135, 103, 71, 47, 25, 12, 6, 3, 3, 4, 6, 10, 23, 41, 63, 92, 121, 156, 196, 240, 289, 348, 418, 498, 564, 
                 619, 533, 447, 375, 313, 258, 209, 168, 129, 97, 64, 39, 19, 8, 4, 2, 1, 2, 7, 10, 19, 36, 60, 87, 116, 150, 190, 235, 285, 341, 409, 487, 557, 
                 609, 526, 441, 369, 309, 254, 207, 164, 126, 91, 61, 36, 16, 6, 2, 1, 0, 1, 7, 12, 18, 33, 57, 84, 113, 148, 185, 232, 281, 336, 402, 479, 545, 
                 606, 523, 436, 366, 306, 250, 205, 161, 123, 86, 60, 34, 13, 6, 3, 1, 3, 3, 8, 12, 17, 32, 54, 80, 111, 146, 184, 230, 281, 334, 400, 477, 542, 
                 605, 523, 435, 366, 305, 252, 204, 161, 122, 89, 60, 34, 17, 7, 3, 4, 4, 6, 8, 13, 18, 31, 55, 82, 112, 147, 184, 229, 281, 337, 402, 477, 543, 
                 608, 526, 439, 368, 307, 254, 207, 164, 127, 90, 63, 39, 19, 9, 8, 6, 6, 6, 10, 14, 20, 36, 58, 86, 117, 151, 190, 233, 284, 340, 405, 485, 551, 
                 616, 534, 447, 372, 313, 261, 212, 170, 133, 99, 71, 45, 27, 18, 12, 11, 8, 8, 13, 16, 26, 42, 65, 92, 123, 158, 197, 241, 289, 347, 414, 496, 563, 
                 634, 546, 457, 384, 320, 267, 220, 178, 140, 106, 78, 55, 39, 25, 16, 13, 13, 11, 17, 21, 34, 50, 73, 101, 131, 167, 206, 250, 299, 357, 428, 506, 576, 
                 653, 563, 473, 396, 333, 279, 231, 189, 153, 118, 90, 67, 49, 35, 22, 18, 16, 17, 22, 32, 45, 61, 85, 112, 144, 178, 217, 263, 313, 374, 443, 525, 596, 
                 676, 588, 492, 415, 351, 293, 246, 203, 166, 134, 107, 81, 63, 47, 36, 29, 26, 27, 35, 43, 59, 76, 98, 126, 157, 191, 232, 276, 329, 391, 462, 547, 622, 
                 714, 616, 520, 439, 371, 312, 262, 220, 182, 151, 123, 99, 78, 65, 51, 44, 40, 42, 49, 60, 73, 91, 114, 143, 174, 206, 248, 294, 349, 412, 486, 574, 652, 
                 759, 652, 549, 466, 398, 335, 280, 241, 203, 169, 140, 116, 96, 81, 70, 63, 59, 61, 70, 79, 92, 110, 134, 164, 195, 227, 267, 317, 370, 435, 511, 606, 692, 
                 823, 698, 587, 496, 421, 362, 310, 266, 229, 190, 165, 140, 117, 101, 91, 83, 80, 82, 89, 100, 113, 132, 156, 182, 213, 247, 295, 339, 395, 465, 547, 645, 740, 
                 898, 760, 636, 535, 468, 395, 342, 292, 252, 214, 187, 162, 141, 123, 111, 105, 103, 103, 111, 123, 137, 155, 177, 205, 237, 274, 320, 370, 428, 500, 584, 690, 801, 
                 992, 827, 685, 582, 501, 430, 375, 321, 281, 243, 214, 187, 168, 151, 139, 132, 128, 129, 136, 146, 161, 180, 205, 232, 265, 304, 350, 404, 464, 538, 627, 751, 879, 
                 1023, 917, 753, 640, 543, 470, 407, 357, 311, 271, 244, 222, 200, 183, 171, 162, 158, 159, 166, 176, 190, 210, 236, 264, 296, 341, 385, 441, 506, 587, 686, 827, 987, 
                 1023, 1023, 836, 701, 598, 515, 451, 394, 349, 308, 278, 259, 238, 216, 203, 193, 189, 188, 196, 207, 222, 245, 270, 300, 335, 378, 428, 485, 556, 641, 759, 926, 1023, 
                 1023, 1023, 940, 779, 656, 566, 497, 437, 390, 350, 323, 300, 276, 257, 243, 232, 228, 225, 234, 245, 258, 282, 309, 339, 379, 423, 475, 538, 613, 712, 852, 1023, 1023, 
                 1023, 1023, 1023, 871, 732, 631, 552, 490, 439, 396, 368, 343, 317, 299, 282, 272, 264, 263, 269, 283, 299, 322, 349, 383, 423, 472, 528, 598, 680, 800, 973, 1023, 1023, 
                 1023, 1023, 1023, 1005, 827, 705, 614, 545, 489, 448, 414, 386, 366, 342, 327, 314, 301, 306, 315, 326, 343, 368, 397, 432, 476, 528, 592, 666, 771, 915, 1023, 1023, 1023, 
                 1023, 1023, 1023, 1023, 953, 800, 693, 613, 553, 502, 466, 438, 410, 388, 374, 357, 346, 349, 359, 371, 391, 418, 449, 488, 536, 593, 661, 754, 882, 1023, 1023, 1023, 1023, 
                 1023, 1023, 1023, 1023, 1023, 930, 791, 694, 621, 567, 523, 491, 461, 439, 424, 406, 399, 397, 408, 423, 448, 474, 510, 553, 602, 665, 756, 873, 1023, 1023, 1023, 1023, 1023, 
                 1023, 1023, 1023, 1023, 1023, 1023, 894, 774, 687, 622, 571, 534, 506, 482, 463, 450, 444, 443, 452, 470, 494, 521, 559, 605, 658, 734, 845, 993, 1023, 1023, 1023, 1023, 1023,
                },

                /* b_gain: */
                {
                 1023, 1023, 871, 712, 603, 522, 457, 400, 357, 314, 283, 251, 227, 209, 198, 192, 186, 187, 192, 203, 216, 233, 262, 283, 321, 360, 411, 473, 549, 646, 775, 980, 1023, 
                 1023, 982, 779, 640, 543, 464, 405, 358, 312, 274, 240, 213, 190, 175, 165, 158, 150, 152, 157, 168, 182, 200, 222, 249, 281, 319, 367, 425, 494, 578, 691, 851, 1023, 
                 1023, 868, 692, 573, 488, 419, 362, 318, 276, 240, 211, 182, 164, 146, 134, 128, 126, 125, 130, 139, 153, 172, 191, 217, 250, 284, 331, 385, 448, 524, 617, 751, 910, 
                 965, 783, 631, 527, 445, 383, 331, 288, 250, 215, 185, 160, 138, 122, 111, 103, 99, 102, 107, 116, 128, 145, 168, 190, 221, 258, 301, 351, 406, 471, 560, 681, 815, 
                 874, 713, 579, 487, 412, 353, 306, 262, 224, 192, 162, 136, 115, 100, 89, 83, 77, 83, 85, 93, 107, 124, 145, 169, 200, 234, 273, 314, 369, 432, 512, 625, 743, 
                 809, 665, 540, 456, 383, 329, 283, 241, 202, 171, 144, 117, 97, 82, 71, 64, 61, 61, 67, 74, 86, 103, 126, 150, 180, 213, 242, 284, 336, 402, 480, 584, 693, 
                 750, 622, 511, 429, 362, 309, 263, 221, 187, 153, 124, 102, 80, 66, 57, 48, 46, 46, 51, 58, 70, 87, 107, 134, 161, 186, 221, 264, 315, 379, 452, 547, 645, 
                 713, 589, 485, 407, 343, 291, 246, 207, 171, 139, 109, 86, 66, 52, 40, 34, 29, 32, 35, 44, 56, 74, 98, 116, 139, 170, 205, 246, 296, 355, 429, 519, 610, 
                 677, 565, 463, 389, 328, 275, 232, 191, 157, 125, 98, 74, 53, 38, 29, 23, 23, 22, 23, 31, 46, 61, 74, 96, 123, 151, 192, 233, 283, 341, 411, 494, 575, 
                 658, 546, 449, 376, 316, 264, 220, 180, 147, 115, 89, 64, 44, 31, 21, 18, 17, 16, 17, 24, 34, 45, 62, 87, 113, 144, 181, 224, 270, 327, 396, 477, 556, 
                 637, 531, 438, 364, 301, 253, 209, 173, 136, 107, 80, 56, 35, 23, 17, 17, 15, 15, 15, 17, 20, 31, 52, 77, 104, 136, 172, 214, 262, 318, 384, 465, 542, 
                 620, 518, 428, 354, 296, 248, 205, 166, 132, 101, 74, 48, 31, 18, 16, 14, 12, 12, 14, 9, 12, 27, 49, 74, 100, 132, 167, 209, 259, 309, 378, 455, 535, 
                 607, 511, 423, 351, 294, 246, 202, 165, 128, 95, 70, 47, 27, 21, 14, 14, 11, 8, 5, 7, 10, 25, 45, 71, 97, 129, 164, 205, 255, 307, 372, 451, 525, 
                 604, 509, 420, 348, 291, 241, 198, 161, 125, 93, 67, 45, 26, 19, 15, 13, 6, 1, 3, 6, 11, 24, 45, 68, 95, 129, 162, 203, 252, 307, 370, 448, 523, 
                 600, 506, 418, 347, 289, 240, 198, 162, 126, 93, 69, 46, 28, 19, 14, 6, 0, 0, 3, 6, 11, 24, 46, 70, 96, 129, 163, 204, 253, 306, 371, 449, 524, 
                 608, 511, 420, 352, 292, 244, 201, 163, 130, 95, 71, 49, 30, 21, 8, 3, 2, 1, 5, 9, 14, 30, 48, 73, 100, 133, 169, 208, 255, 309, 375, 455, 532, 
                 616, 516, 426, 357, 297, 248, 206, 168, 133, 103, 76, 55, 35, 15, 9, 5, 2, 3, 8, 12, 19, 33, 54, 78, 107, 138, 175, 213, 260, 315, 382, 464, 539, 
                 633, 527, 437, 365, 306, 257, 211, 175, 141, 111, 83, 58, 33, 17, 11, 7, 7, 7, 11, 14, 25, 41, 61, 87, 115, 146, 184, 223, 269, 325, 394, 475, 554, 
                 650, 545, 451, 381, 319, 267, 222, 187, 151, 119, 89, 59, 40, 25, 13, 11, 11, 12, 14, 25, 35, 53, 73, 96, 125, 157, 194, 235, 281, 339, 408, 490, 573, 
                 669, 570, 472, 397, 333, 280, 236, 196, 162, 125, 94, 70, 51, 37, 28, 21, 18, 20, 25, 34, 47, 65, 83, 109, 137, 169, 205, 246, 298, 358, 428, 512, 595, 
                 712, 596, 496, 418, 353, 297, 250, 214, 170, 134, 105, 82, 65, 51, 41, 36, 31, 34, 39, 49, 61, 78, 100, 127, 153, 183, 221, 265, 314, 375, 448, 536, 624, 
                 756, 631, 524, 443, 377, 317, 268, 221, 183, 149, 120, 99, 82, 68, 58, 51, 48, 50, 56, 65, 78, 96, 117, 144, 169, 203, 241, 285, 337, 396, 473, 565, 659, 
                 808, 675, 561, 472, 404, 344, 280, 237, 200, 166, 143, 121, 101, 87, 75, 69, 67, 70, 75, 85, 97, 115, 137, 161, 190, 222, 262, 306, 360, 423, 500, 604, 714, 
                 885, 733, 608, 507, 441, 361, 305, 259, 222, 188, 164, 141, 120, 107, 96, 89, 84, 87, 95, 105, 117, 135, 156, 183, 209, 243, 286, 332, 390, 456, 539, 649, 763, 
                 979, 806, 659, 551, 456, 387, 332, 288, 250, 214, 187, 165, 145, 130, 119, 113, 109, 110, 116, 126, 139, 160, 181, 208, 238, 272, 317, 364, 423, 494, 580, 702, 834, 
                 1023, 888, 719, 593, 497, 422, 367, 320, 280, 243, 216, 194, 173, 158, 148, 140, 136, 139, 144, 154, 169, 185, 211, 236, 267, 307, 351, 402, 461, 538, 637, 773, 935, 
                 1023, 989, 785, 649, 545, 471, 408, 357, 315, 277, 250, 231, 207, 192, 177, 172, 166, 166, 173, 183, 198, 218, 241, 273, 304, 345, 390, 442, 510, 593, 706, 869, 1023, 
                 1023, 1023, 880, 721, 606, 521, 456, 402, 356, 315, 292, 269, 248, 229, 216, 207, 203, 202, 210, 219, 237, 256, 282, 310, 348, 387, 438, 493, 565, 662, 794, 997, 1023, 
                 1023, 1023, 1016, 812, 679, 583, 506, 452, 403, 364, 335, 312, 287, 268, 254, 246, 238, 237, 244, 255, 274, 295, 322, 354, 393, 435, 491, 552, 632, 744, 910, 1023, 1023, 
                 1023, 1023, 1023, 941, 773, 658, 573, 508, 456, 417, 381, 355, 333, 313, 300, 286, 275, 279, 288, 301, 317, 341, 369, 402, 444, 492, 549, 626, 722, 853, 1023, 1023, 1023, 
                 1023, 1023, 1023, 1023, 894, 755, 653, 575, 517, 472, 435, 405, 381, 360, 347, 331, 320, 324, 333, 348, 368, 392, 425, 460, 504, 556, 622, 708, 826, 1007, 1023, 1023, 1023, 
                 1023, 1023, 1023, 1023, 1023, 878, 750, 659, 590, 536, 498, 465, 434, 415, 401, 385, 377, 374, 385, 402, 427, 453, 483, 528, 573, 637, 716, 825, 984, 1023, 1023, 1023, 1023, 
                 1023, 1023, 1023, 1023, 1023, 1015, 850, 741, 662, 600, 555, 515, 486, 467, 442, 430, 434, 429, 436, 451, 475, 506, 545, 584, 639, 713, 814, 951, 1023, 1023, 1023, 1023, 1023,
                },
            },
        },
    }
};

static const ot_eis_vi_cmos_acs g_cmos_acs = {
    /* acs_attr */
    {
        /* en */
        1,
        /* y_strength */
        256,
        /* run_interval */
        2,
        /* lock_en */
        0,
    },
    /* acs_calib_param */
    {
        /* light_index */
        {
            -1, 23, 10, 5, 4, 1, 5, -1, 6, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, 22,
            -1, 0, 10, 14, 3, 14, 5, -1, 2, 13, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, 23,
        },
        /* model_ar_min */
        -0.08f,
        /* model_ar_step */
        0.098f,
        /* model_ab_min */
        0,
        /* model_ab_step */
        0.28f,
        /* light_type_g_high */
        1,
        /* light_type_g_low */
        2
    },

    /* acs_y_shading_lut */
    {
        /* g_param_high_ct */
        {
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024
        },
        /* g_param_low_ct */
        {
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024,
            1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024, 1024
        },
    },

    /* acs_color_shading_lut */
    {
        /* avg_rg_map */
        {
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000
        },

        /* avg_bg_map */
        {
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000
        },

        /* prof_rg_map */
        {
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000
        },

        /* prof_bg_map */
        {
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000, 1.000000000,
            1.000000000
        }
    }
};

static const ot_eis_vi_sns_clut g_cmos_clut = {
    {
        1,
        128,
        128,
        128,
    },
    {
        {
            0,    3149813,    1046533,    3149814,    6299621,    9450452,    6299622,    9450451,   12602304,
            16802729,   12602303,   16803752,   22052752,   26255221,   21005198,   25206643,   30457689,   35708733,
            29409112,   32563005,   39910177,   45161223,   35714854,   38866705,   49362669,   54612692,   42017535,
            45168367,   60912311,   68261528,   48319196,   52519622,   75610743,    3149814,   57769646,    4198390,
            2095110,    9451475,    3143687,    9451474,    6299621,   15755175,    6300644,   14706598,   12603326,
            23110515,   11554749,   21012342,   19956621,   29416257,   17859470,   25219913,   26263385,   32572189,
            23116638,   26276652,   31519533,   34675465,   26273592,   25232160,   33624849,   37828337,   26279717,
            24187669,   35727104,    5246965,   24184604,    6295538,   39930592,    9451473,   25238283,    9451472,
            4192261,   13658024,    6290435,   11559850,    7349219,   17865595,    7349218,   14716804,   11554749,
            19973973,   10506174,   14727012,   15761298,   18932542,   13663127,   11587410,   18920295,   15788854,
            14722931,    7395148,   19978055,   12645170,   13681497,    2153291,   17884985,    8393711,    9491278,
            9443307,   14741301,    9451472,    4249419,    8402896,   11597614,    9461678, 1072749387,    7363507,
            7341055,   10520463,   10487801,    6323098,    8398816,    8430453,    9447390,    2134918,    9456575,
            4241254,    8408001, 1070637947,   10515358, 1071693666,    7367590, 1063299959,    9475969, 1065403234,
            4229007, 1054911353,    6336364,   11541479,      39808,   12591075,    1097571,    8402896, 1067493240,
            8402896, 1068548962,    5264313, 1059105655,    3166143, 1061208932,    2125734, 1050717050, 1071670194,
            12586995, 1070629783,   14685164, 1064333224,    9448411, 1063292815,   10496985, 1054899107,    7358404,
            1053856653,    6308808, 1045462946,    4219823, 1044420494,    1071032, 1034977187, 1072723870,   12592094,
            1068525484,   12593114, 1066436498,    7354321, 1060140965,    6304722, 1059099533,    2115526, 1050704802,
            16333, 1049663373, 1067471805, 1040220066, 1063274441, 1040226191, 1058037688, 1029734309, 1052790727,
            15735781, 1047553974,   15737822, 1040208840,   10498007, 1037068214,    9449430, 1027625931,    4210635,
            1025534905,    3161039, 1015043022, 1071665089,   11545558, 1068516298,    9449426, 1063279546,    5256147,
            1058032584,    4207572, 1052795830, 1071658963, 1046499271, 1069560794, 1042311094, 1059077076, 1033917385,
            1055929311, 1030777783, 1046495190, 1021334477, 1041248229, 1019243450, 1032863706, 1008751568, 1025519596,
            14690264, 1019232223,   11546577, 1010838515,    8400852, 1005600740,    6303699,  996157434,    2111443,
            6304718,    1061847,    3160010, 1065368531,    2109398, 1062220764,    1060823, 1052786645, 1067461600,
            1048588258, 1065362407, 1040203736, 1051731946, 1033908201, 1047534581, 1026572253, 1034952692, 1018178543,
            1029705730, 1012940770, 1019223038, 1003498487, 1011877903,  998260711, 1002443783,  987768830,  994050075,
            8402891,  974131202,    4209605,  946862085,    4207569,      15302,    2110416, 1071641611,    1060827,
            11225,      11232,    2102257, 1059071973, 1063264237, 1056972782,    8402898, 1043342319, 1043336192,
            1038096380,   16801708, 1026564089, 1023410193, 1020267529,   26254200, 1010832386, 1005581344, 1002439701,
            36754246,  985665539,  986704943,  985660448,   45154078,  961547264,  966778940,  931132416,   55650040,
            17343, 1070593036, 1068492823, 1073739785,      13263,    2102257,       2046,    3151855,       9188,
            8401877,    5252066,    8403918, 1053825015,   15751092,   12602304,   15756196, 1033896968,   27300735,
            21003156,   22060919, 1015019545,   40949569,   31504222,   26263385,  996143144,   54595340,   40953649,
            28366660,  977266741,   67192538,   50402060,   32567088,  899670016, 1072690186,   59850468,    2096132,
            1067443224,    3151856, 1071638548,    3151853, 1072694271,    8402896,    1050621,    7355341,    5252067,
            16803749,    5253088,   10511275,   11551685,   25207666,   11555770,   12617612,   19951519,   31509326,
            18908045,   12624758,   34649951,   36760369,   24162150,   12628840,   47248165,   43059989,   27316045,
            11583326,   60893939,    1046535,   30466874,    5244924,   73491137,    3151854,   33618724,    4202471,
            1069541398,    8403917,    1044494,    4209616,          0,   12609447,    2100217,    4216760,    5253089,
            17863553,    5254110,    3174308,   12603324,   19968871,    8409020,      33686,   21006220,   21022550,
            11564955, 1069584269,   28358495,   21026632,   12621696, 1065392006,   34659134,    3145728,   12626799,
            8392690,   39910180,    4201450,   11582307,    5252067,   47258373,    6306766,   10536792,    2111445,
            1072688146,    7364529,    6289414, 1071663047,    1050619,    7371672,    4199410, 1066424253,    5253087,
            6329222,    4205532, 1060135861,   10507194,    4235131,    4212676, 1052797872,   15761299,    1092466,
            3171246, 1045458861,   18916210,    6294519,    1079197,    9443304,   19970910,    5251045, 1071679377,
            4203489,   21024591,    3160018, 1067488137, 1072705499,   22077248,    1068992, 1062247300, 1064318935,
            3142667, 1071670193,    9438202, 1055932372,    3149814, 1066431398,    6297579, 1046496211,    5254109,
            1061190559,    3156956, 1037060051,    6310847, 1054901146,      16334, 1026575315,    7368612,    9442285,
            1068519362,    7347165,    7374734,    5252066, 1063279544,    3154911,    5282688,    1061848, 1056991154,
            1069557731,    2140022, 1068515279, 1049653166, 1058023398, 1072738159, 1061178312, 1042314156, 1045440491,
            8387584, 1053840324,   11538412, 1032857583,    6296558, 1045452737,    7347172, 1020274676,    4205532,
            1036017600,    2108382, 1007690746,    2114504,    8395747, 1068512217,    3154899, 1072716728,    4203488,
            1060125653,    1057757, 1069575083, 1071655903, 1051739091, 1067458538, 1064335266, 1061171167, 1042302930,
            1051727861, 1058045852, 1050686431, 1031817171, 1035996161, 1052805016, 1040200673, 1022381012, 1020266508,
            11537395, 1028667364,    9444317, 1004535830,    7347175, 1017133030,    5251038,  963636228,    3156957,
            5251032,    1058785, 1064295459, 1071659987,    2106334, 1064314852, 1070592014, 1064323019, 1068508134,
            1051731944,    2100215, 1056985030, 1054875630, 1039149037,    9448410, 1049647042, 1041243126, 1026566130,
            20996013, 1041259457, 1026562045, 1013982199,   33592188, 1031823296, 1012928517, 1001399292,   46187343,
            11539429,  988809218,    4203471,   58781478,    6299617,      10190,    2106328, 1067442205,    1058783,
            9181,       9188, 1071641610, 1066414047, 1065360366, 1059068912,    2102258, 1055929311, 1047531517,
            1043338235,    9451474, 1045443552, 1030751244, 1027607558,   16801708, 1033910242, 1015019546, 1011876881,
            25200521, 1023424485,  997191719,  998243355,   32549738, 1011890152,  980412467,  950002688,   39896909,
            7348182, 1063245862, 1061146670, 1070589974,    4203483, 1069543440, 1067443224, 1073740805,       9186,
            1050619, 1073740803,    2103278, 1061168106,    9447392,    5250026,    6305745, 1047535602,   22043568,
            14697413,   10509236, 1033903098,   37787511,   27293589,   14710683, 1019220993,   53530435,   39889765,
            16813958,  999296002,   69272339,   52484923,   19964786,  977273856, 1065344032,   65078035,    1044493,
            10184, 1071641612, 1064293415,    1050621,       9173,    2101236, 1069542420,    1055723,       9190,
            9450453,       1023,    2110422, 1056970741,   18898860,    5252067,    3165122, 1039140868,   29396866,
            13650879,    3170225, 1023409171,   39892828,   21001114,    2126754, 1005581344,   49339194,   28350329,
            1082261,  988802093, 1068491802,   35699547,    5240834,  920635392, 1072691207,   43046719,    2101238,
            1059048498,    2103280, 1069539358,       8169, 1066394651,    8402896, 1071640589, 1071656925, 1072692229,
            14704559,       3066, 1068514257,    5249007,   19955601,    4204512, 1064324039,   14695371,   25205624,
            8408002, 1061181375,   30439316,   29407072,   12609447, 1056989111,   46183260, 1072688145,   15762320,
            6293494,   60877610,          1,   18913148,    3150831,   76618492,    2104300,   22065001, 1072701418,
            1063244842,    5257171,    1042452, 1066412005, 1068492822,    7361467,    1046533, 1059074016, 1072693249,
            8416166,    1052661, 1052783581,    5251046,    9469844,    2107360, 1045445594,   14698433,   10523524,
            3162060, 1038106584,   24147862,    3142664,    3168185,    6294505,   34644847,    1051642,    2124713,
            2103273,   44092235,    1055722,    2128796, 1070603243,   53538601,      13273,    1083279, 1061167085,
            1066392610, 1072710601,    6288392, 1050681328, 1070590993, 1070618556,    3146748, 1040195571,          0,
            1068524464,    1054704, 1030758390,    5253089, 1066430374, 1072703459, 1020272633,   11553728,    5243900,
            1069561815,    3150812,   16805792,    2101235, 1066419148,    1055716,   22056836, 1072701417, 1063276483,
            1068506093,   27305836, 1068510177, 1059085243, 1054873590,   32555860, 1064318937, 1054894004, 1042288639,
            1071637529, 1059078098,    9437177, 1028655112, 1073738762, 1052788685,    4197363, 1015021585,    1051640,
            1047547848,    1054701,  981459971,    3155936,    6294511, 1069556711, 1054849090,    6309830,    3150828,
            1063267298, 1063243820,    8413104, 1071652843, 1055929311, 1070589975,    9467805, 1063265257, 1049638876,
            5244924,   10521484, 1054877672, 1042300889,   17837015,   11574140, 1046490088, 1034961879,   33577899,
            3140623, 1038102504,    7342057,   48270206,    3144705, 1029713897,    4199401,   64010067,    1053682,
            4198370,    1055722, 1059046454,      10209,    2103271, 1065361388, 1065343012,      15313, 1069554668,
            1055924207, 1072689168, 1071665090, 1058020338, 1045438449,    5245942, 1069570998, 1046484984, 1036001268,
            14692311, 1067477931, 1034949630, 1025515512,   25187252, 1064335266, 1022365700, 1015029755,   35683216,
            8386561, 1003488257,    3149785,   46178158,    4196343,       6101,    2103263, 1064292394,    1054702,
            7137,       7145, 1068491802, 1070605285, 1067457518, 1062213618, 1072690186, 1066414045, 1052775419,
            1048581115,    3150835, 1061174229, 1037042695, 1034947588,    9449432, 1055933391, 1023409171, 1022362637,
            15748030, 1050692554, 1008727071, 1008730134,   22046628, 1044403142,  994044971,  969923584,   29393802,
            8389617, 1052750919, 1050652748, 1068489759,    4198382, 1061145649, 1059046455, 1070590994,    1055723,
            1070587931, 1067441185, 1072692227, 1067458538,    5243903,    2093066,    1053681, 1059070952,   19934168,
            11540459,    3156958, 1050683368,   36724647,   25183170,    6307786, 1042295784,   54563701,   40924053,
            8411064, 1033908200,   72402757,   56664936,   11561894, 1024472041, 1056948284,   71356221, 1073735699,
            6294497, 1064293416, 1055898688, 1073739784,    3150820, 1071639571, 1062195245, 1071646717,       7146,
            5245945, 1068491802, 1071650801, 1063263215,   16789463,    2096131, 1070605285, 1051727861,   29382575,
            9445351, 1069560793, 1040193531,   41976711,   19940293, 1068516301, 1028657153,   55618400,   30435234,
            1067470786, 1011877890, 1061145648,   40930175,    4191236,  994050048, 1067441183,   51425117,    1049598,
            5073, 1072689165, 1061144627, 1071648760,       6107,    4198388, 1066392609, 1067456498,       8168,
            12595159, 1070590995, 1063264237, 1060116468,   20991928,    1049599, 1059071975, 1045433345,   29389722,
            6299622, 1054879714, 1029701645,   37786492,   12599243, 1049637854, 1016068121, 1066391589,   18897841,
            5242869, 1001386021, 1069541398,   25196439,    2100212,  941603840, 1072691206,   32543613, 1070601204,
            1047504978,    2102258, 1067439142, 1063261173, 1056948284,    6302683, 1069540377, 1055922165, 1065343014,
            11552708, 1071641611, 1047533558,    1043470,   15753134,       3067, 1040194552,   11539437,   19953560,
            2105319, 1032854521,   28328896, 1071637529,    5257172,    2099173,   46167950, 1072689165,    7359425,
            1052650,   64007005, 1071644672,   10510255, 1069553649,   81846061, 1072699377,   12612509, 1059066871,
            1052750918,      10209,    1041432, 1048580094, 1061144626,    1063889, 1073737742, 1037043717, 1068490782,
            2116546, 1073740803, 1025508365,    2095111,    2120628, 1071648759,  999285763,   10492905,    3141644,
            1070604267, 1043305572,   22037443,    1047555, 1069558751, 1054846030,   35680155, 1071647738, 1069562835,
            1065337910,   48273267, 1069553649, 1067468744,    4186139,   61914957, 1066410985, 1066424253,   17826810,
            1059045433, 1064315872,    5238791,   34614226, 1064293416, 1061173208,    3144705,   52451240, 1069541399,
            1059079120,       2043,   70289276,    1048577,    5241852, 1069552629, 1049601108,    8396774,    1050617,
            1065360367, 1059043393,   16793544, 1070600182, 1061168106, 1067438124,   25191337, 1065359347, 1056975845,
            4188181,   33588107, 1059068913, 1051735008,   13633528,   41984877, 1053827055, 1047541723,   27274199,
            1064292397, 1047536621,    7338997,   39866291, 1068490781, 1041246187,    3147764,   54555535, 1071640591,
            4195309,       4084, 1055897668,          0,    1052655, 1066406900, 1062193204,    4202470, 1070601203,
            1059067893, 1069537315,    9452495, 1061164022, 1051727862,    3141647,   13652921, 1051726842, 1044388855,
            9440247,   17853347, 1042289662, 1036000248,   18884572,   22053773, 1032850434, 1028660218,   28328896,
            1070586911, 1018168321,    4194275,   37774244, 1071639571,       2013,    1051623, 1062193205, 1072691207,
            5093,       6125, 1066390567, 1072696313, 1069553647, 1064309748, 1070588953,       8169, 1056969721,
            1053823995,    1045513,    1061849, 1044383747, 1042286594,    5245942,    1065930, 1031798797, 1031799817,
            9446370,    2118587, 1020262423, 1020264465,   15743949,    3171245, 1007677474,  988796928,   20991929,
            3139600, 1040157805, 1038059632, 1068488740,    2094087, 1052746836, 1048551513, 1069540378, 1072694271,
            1065335867, 1060091970, 1070592015, 1070600182,    4185118, 1071632425, 1072692228, 1068506093,   19923963,
            11528202, 1072697335, 1065363428,   38809552,   26220519,    1056745, 1063269340,   58743714,   43008957,
            2109404, 1060125652,   78679922,   61894546,    4209614, 1056982988, 1046453340,   79731558,    1042451,
            6289406, 1056945223, 1045404766, 1072689164,    3146747, 1066387505, 1053798475, 1071643654,       3064,
            4187160, 1063240759, 1068500991, 1068504053,   15730681, 1072683041, 1066405881, 1062213618,   30419925,
            9433094, 1065359346, 1055923184,   46158765,   19929064, 1063264235, 1050682350,   61897605,   33569733,
            1061170148, 1044391916, 1052748876,   47211425,    4191232, 1038101483, 1060094010,   60852093,    2097150,
            6291436, 1068487720, 1052748877, 1070598141,    2099181,    3140625, 1059045436, 1065356284,       5105,
            11536375, 1065340972, 1060114427, 1065358324,   23078873, 1072686105, 1054872570, 1056969720,   34622394,
            6289411, 1049629690, 1047532540,   46164889,   14686186, 1044387833, 1038093312, 1059045436,   23082958,
            3144685, 1025509377, 1064291373,   32527282,    1050608, 1011874816, 1069539358,   41972629, 1070601205,
            986,    2093068, 1060094011, 1062211577,       4064,    7343094, 1064291374, 1053822974,       7146,
            14689247, 1068489760, 1045432323, 1063262196,   22036423, 1072688145, 1037042697, 1050677246,   29383598,
            2097152, 1016062978, 1038091272, 1065340973,    7346156, 1031761034, 1026554898, 1067441185,   12595160,
            1045398641, 1013969949, 1070589972,   17843139, 1061134422,  963622912, 1073739782,   24140718,    3128377,
            1033863288,    2101239, 1067438121,   19913753, 1046452321,    5251046, 1068489759,   37749748, 1059041352,
            9450453, 1070589973,   57682892, 1071631405,   12601283, 1071641610,   78665634,   12575756, 1071636508,
            1071646718, 1039107189,   29366246, 1071639571,       6128, 1050646624,   48251833, 1071641611,    1058787,
            1063234633,   68187018, 1069547521,    3158997,    2081841,   88123226, 1069551608,    5260231,   16769045,
            1041208423, 1069554669,    2090006,   30409717, 1051699282, 1069557731, 1073736720,   46147539, 1061141565,
            1069560793, 1072690185,   62933935, 1071633445,    3141641, 1069547523, 1046452320,   10480648,    1046534,
            1067453436, 1055893582,   23074791, 1070596098, 1065358325, 1065335868,   38813633, 1067452414, 1064311790,
            1036327,   54552474, 1063260154, 1062216679,   12576784,   70292337, 1060115446, 1060122592,   22022134,
            1048552533, 1055923186,    5237761,   34612186, 1056946243, 1052778479,    3144703,   46153662, 1064290353,
            4193270,       2046, 1053797452, 1072685085,    1050615, 1068502013, 1061141565,    7336964, 1070599161,
            1063259131, 1067437102,   17831913, 1064308730, 1058017275, 1073732638,   28325834, 1056968700, 1052775418,
            7335948,   39869353, 1050677247, 1047532538,   14682104,   51411849, 1043336193, 1042290681,   22028258,
            1055897668, 1032848385,    3142636,   30421964, 1061144629,    1047523,    1049582, 1062192183, 1067439142,
            3049,       4082, 1065340972, 1072687125, 1071650800, 1066405879, 1068489761,    5241857, 1061164023,
            1058017275, 1072687125,   10491883, 1050676223, 1049626624,    3143688,   17839059, 1040188424, 1041236998,
            6293497,   25185210, 1030749200, 1031799820,   10491882,   32532386, 1020262425, 1008720896,   14690267,
            1063241778, 1027563668, 1024417942, 1069538337, 1066390567, 1043299449, 1038055549, 1070588954, 1069540379,
            1060083805, 1052741732, 1070590995, 1071640589,    3126334, 1068477512, 1070593036,          0,   22009883,
            11521066, 1071643652,    3151854,   41944052,   29352965, 1070599163,    7351261,   63974344,   48240608,
            1071649779,   10501068,   86006683,   68173751, 1072700395,   14700475, 1034909824,   88107916,    2092043,
            1071635488, 1048547432, 1032812672,    1045513, 1071637527, 1062185040, 1045400682, 1071642630, 1071640591,
            3129397, 1056941141, 1068498947, 1070595078,   17817623, 1069529149, 1065354241, 1069549565,   34604021,
            9425955, 1062210558, 1069552627,   52438991,   23063556, 1059065852, 1069555688,   70274985,   37754852,
            1056968698, 1069558750, 1043303531,   54540225,    2094068, 1069561812, 1053794391,   70279069,    1049590,
            4189195, 1064285250, 1042255977, 1072696313,    2094087,    2083884, 1051697239, 1066404859, 1073740804,
            14672914, 1060090949, 1060113406, 1068499968,   26216438, 1070581810, 1053820929, 1065356284,   39855063,
            6282268, 1047528453, 1062212600,   54544310,   16773123, 1033890817, 1058019316, 1050649686,   28316649,
            1018119345, 1054874608, 1057993798,   40907724, 1037000852, 1050682349, 1066386485,   52449199, 1055881335,
            4191222,    1038371, 1050650707,    2070616,    3145719,    9432078, 1057993797,   22001719,       3064,
            18876407, 1064289334,   42981395, 1067453434,   28319710, 1070584870,   65012717, 1061163003,   38812613,
            4187157,   88090564, 1053822973, 1057994817,   11532289, 1027563671, 1046481920, 1063240757,   17830893,
            1043297407, 1038091265, 1068487720,   26225623, 1059032166, 1027604480, 1073734681,   34619329,    1025100,
            1046497,    5239818, 1060094012,   17809456,        998,   10487800, 1063241777,   35643409,       5101,
            15735782, 1067439143,   52429808, 1066406900,   23080916, 1070587931,   71312334, 1055920123, 1065340972,
            1044494, 1037006973, 1045432323, 1067440163,    4193280, 1049594985, 1034944524, 1069540378,    8392690,
            1061134421, 1025506325, 1071639568,   12591075,    1029183,  986691584, 1073739782,   16789460,   13618216,
            1019172002,    1051642, 1069537317,   28304398, 1034907783,    4200431, 1069539358,   40895475, 1051691115,
            6300643, 1070589975,   55582679, 1068475470, 1073734678, 1070592016, 1046451300,   12568621, 1072688146,
            1071642632, 1054843988,   32498694, 1071641613, 1070596096, 1064285251,   52434910, 1069545480, 1071648759,
            1073726514,   74466226, 1067450370, 1071650799,    9426975,   97547140, 1066404861, 1072702439,   19917835,
            1028615307, 1065357304,    3139597,   29361142, 1042252916, 1064310770,    2093066,   39853024, 1054840924,
            3141632, 1073739783, 1054847050, 1069528131,    2097151, 1070595077, 1061141566,   10473510, 1071645695,
            1066401794, 1067436082,   26208261, 1067452415, 1063258111, 1072682021,   42997730, 1062209535, 1061161981,
            5235735,   60832700, 1058015232, 1058017275,   12578824,   79717269, 1053820929, 1054872569,   17826809,
            1038058612, 1047528448,    3141620,   25170921, 1048549473,    1046505,    2096117, 1064291376, 1059040333,
            2029,       2039, 1067439144, 1069531192, 1072698354, 1069550586, 1069538336,    8378400, 1065357303,
            1063259132, 1072686104,   19918851, 1056968701, 1056966656,    1044495,   33559526, 1049625603, 1050674179,
            4192261,   47200199, 1041235978, 1044382727,    6293500,   61888422, 1033894929, 1027597312,    9441266,
            1046453342, 1013921981, 1009727679, 1073734678, 1054846030, 1033852063, 1027559586, 1072688146, 1062190142,
            1054830720, 1046441094, 1071640591, 1070582828,    2068574, 1065322600, 1071641611,    5234713,   24096827,
            11511880, 1070594055,   13628418,   47174676,   32491558, 1069547523,   23073771,   71303147,   54520832,
            1068500991,   33566674,   95431615,   76551128, 1068502012,   44058552, 1023365284,   98581424,    2093052,
            1055896647, 1040148617, 1020220579,    1047548, 1060094011, 1056932975, 1035954315,       2045, 1065339950,
            1024082, 1050640499, 1069549566, 1070586913,   19905587, 1066375257, 1065355263,    2092050,   39836690,
            9417790, 1062208512,    7339009,   58721262,   26202144, 1058014211,   13636591,   79701961,   45083648,
            1050670081,   19933149, 1032809610,   61871071, 1005526231,   26228683, 1046446196,   80753596, 1028602040,
            1065339952, 1060083805, 1030713479, 1050629272, 1066390568,    1027141, 1043301491, 1073706103, 1068489759,
            15714348, 1055889503,   24088661, 1070589973,   31449104, 1067428938,   48215089, 1072690187,   47186929,
            7323700,   73390091,          0,   63971282,   20961307,   98566116,    3149813, 1041205360,   34598912,
            1016020153,    5250025, 1052743775,   48238565, 1034900638,    8399837, 1063234636,   62925769, 1054829699,
            1073734681, 1073724473, 1041206380, 1073710183, 1072687124,   11522084, 1050647644,   19898442, 1071640591,
            24111117, 1060088908,   39828523, 1070593034,   35652597, 1069530171,   60808203, 1069546501,   48241628,
            5229609,   82837482, 1067451392, 1050648663,   14671894, 1027562650, 1066404858, 1057992777,   25162752,
            1042247812, 1065358325, 1065335867,   34607083, 1057981550, 1064311792, 1073728556,   46146517, 1073715286,
            3140608,    7330843, 1052747856,   15708222,    2095103,   15724554, 1057993796,   32491557,       1023,
            24118263, 1064288312,   49274891, 1069549567,   32511973, 1070583852,   66060272, 1065355263, 1060092989,
            2087966, 1038056572, 1060111360, 1064290355,    9432080, 1049594986, 1055918080, 1068486697,   15726592,
            1061133400, 1049625600, 1072684062,   20974577, 1073720390, 1044381696,    3139603,   28318689,   12566578,
            1043432,    8385543, 1063241780,   25154590,    1048555,   12583930, 1065340972,   37742601,       4079,
            17830894, 1068488740,   51380212, 1069552628, 1068488739, 1070587932, 1048550494, 1061163002, 1069539357,
            1073735699, 1056942160, 1052772352, 1070588951,    3142666, 1065333827, 1045431303, 1071639569,    5241856,
            1073725493, 1038089230, 1072690187,    8391671,    8376358, 1008714752,    1047556,   11539438,   17817623,
            1004480716,    1050621, 1073734680,   27257863, 1024410798,    2100215, 1072687124,   36700151, 1044340879,
            2092041, 1072688145, 1059044416, 1065320559,    1045511, 1071641613, 1063240758,   12558413, 1073739782,
            1071642633, 1068485677,   35636264, 1071643652, 1070595077, 1073731619,   59762688, 1067450371, 1069547521,
            5234713,   82843605, 1065353218, 1068500989,   10480655,  108019625, 1063257089, 1068502010,   16774149,
            1014973617, 1061159936,    2092028,   20972539, 1031756951,    1044463,    2095100, 1068489761, 1048541308,
            1048560,       1020, 1070587932, 1066373217,       4084, 1071646717, 1072686103,   10464323, 1069551607,
            1067452414, 1073736722,   29346851, 1063260155, 1063257088,    2093069,   49277952, 1058016255, 1060111361,
            4192263,   69211100, 1051722756, 1055917060,    6290434,   90191798, 1046478858, 1046473728,    7341054,
            1025466517, 1000279271,  995036391,    2092035, 1039104127, 1024403653, 1017063624,    1045506, 1052740713,
            1048529059, 1039090856,    1047553, 1067425874, 1073703039, 1062167688, 1072693248,    8370233,   26184795,
            11501670, 1071644672,   24106014,   52408372,   35628099, 1069547520,   39841792,   79680523,   60802078,
            1068498944,   55579618,  106954721,   85978104, 1066400768,   72363971, 1010773192,  111154128,  992932094,
            1035960441, 1031750827, 1007628486, 1019154652, 1046450280, 1052729486, 1025460396, 1045377209, 1057988693,
            1073708143, 1045389457, 1072647318, 1068479555,   21993552, 1064269941,   25128051,    6277166,   44021806,
            9409625,   53448783,   17816600,   67098635,   30388283,   80719914,   30405632,   90177511,   50318363,
            109040643,   41947112, 1022315690,   72347643, 1005525211,   54536143, 1039098001,   92278746, 1027551422,
            1047501918, 1055881336, 1019170982, 1049578656, 1054845008, 1073713247, 1034904719, 1072653442, 1062189122,
            17803332, 1050638457,   21987428, 1069532211,   36684840, 1066373218,   45063237,    4183076,   55566347,
            8365131,   69187622,   11527187,   74448877,   24099890,   92263430,   19920896, 1032809611,   40883224,
            1017068728,   28315630, 1046445175,   57667581, 1035948191,   36708315, 1060081763,   74451938, 1054828679,
            1057994818, 1073718350, 1031763077, 1073709166, 1062192184,   14661688, 1043301491,   18846805, 1066388526,
            29346850, 1055888481,   37727291, 1070585892,   44033034, 1067426895,   56607778,    1041433,   58720242,
            6273084,   76535816,    6287373, 1043303533,   18860072, 1029660821,   10484736, 1052743773,   31448084,
            1044345985,   14683124, 1063233613,   45085695, 1059030125,   19929064, 1073723453,   56625129, 1073715289,
            1068487718,   10471468, 1044354148,   14657605, 1069538336,   22009883, 1052745815,   30391345, 1070588954,
            32500744, 1061137482,   46124060, 1071638548,   44040182, 1069530172,   60809223, 1072689166, 1053797455,
            4180013, 1041204338, 1073739783, 1060090947,   13621278, 1052742755,          0, 1067434040,   22012943,
            1063231571,    2100218, 1073728556,   31455231,    1028164,    3149812,    7329824,   39847920,   12565557,
            2091018,   14672915, 1055897668,   23055397,    1044488,   22016006, 1061142587,   34592790,    1046535,
            29360121, 1066387506,   47179782, 1072692229, 1064291377, 1071632424, 1052748879, 1069546499, 1067438121,
            3136542, 1060090948, 1066401794, 1070585890,    8381460, 1067434042, 1064305665, 1073733659,   13627402,
            2082863, 1062208512,    4188179,   18872320,    9425956, 1060111360,    7335947,   24119286,   16768025,
            1042414,   11532292, 1068488740,   25158670,    1046511,   13632508, 1069538335,   33550339,       2034,
            1073736722, 1071637530, 1064292396, 1071648757, 1073737743, 1072687124, 1068487718, 1066405881, 1073737740,
            1043471, 1072684064, 1061161981, 1073738761,    3142666,    3137561, 1054868482, 1073739782,    5240837,
            7333907, 1049624583,    1047556,    7339008,   11529228, 1031788544,          1,    8390652,   15725574,
            988740855,          0,    2091012,   19920896, 1012865238,    1044468,    1044482,    2094090, 1036990644,
            1047540,    1046529,    3143688, 1061116049,       1013,          1,    3143685,   12548205, 1072697336,
            1072693248,    4193283,   38771784, 1069550585, 1070596096,    4193281,   66043936, 1066404860, 1069547520,
            4195327,   93318134, 1063258111, 1067450369,    5243902,  120591308, 1060110339, 1065352192,    6292478,
            1001331927,  986636560,  979296528,          0, 1021262010, 1014956267, 1006567661,          0, 1042240668,
            1043275974, 1032790219,          0, 1063219326, 1072644257, 1059012776,          0,   10456160,   27223163,
            12541061,          0,   32483391,   56592468,   38763617,          0,   55560221,   87010347,   67084348,
            0,   78638073,  118476802,   95404054,          0,  101715925,  999228653,  123725808,          0,
            1012875446, 1023353037,  993986794,          0, 1030706333, 1047477421, 1016014028,          0, 1047489669,
            1072650380, 1039088815,          0, 1065321580,   24082540, 1061116049,          0,    9411666,   49255498,
            10450035,          0,   27243574,   74429480,   33525845,          0,   46125082,  100652037,   56601654,
            0,   65007612, 1010772169,   80726038,          0,   82840542, 1031749806,  104851446,          0,
            1025466517, 1052727443, 1007628484,          0, 1039103105, 1072656504, 1026508971,          0, 1052739693,
            19893340, 1045388435,          0, 1066375257,   40870976, 1064268922,          0,    7318595,   62897188,
            9406561,          0,   22004781,   83875847,   28287048,          0,   36689942, 1023365286,   47167535,
            0,   51377150, 1040147600,   67096597,          0,   65014758, 1056929914,   85977083,          0,
            1038058613, 1073712228, 1022318750,          0, 1048547429,   16752717, 1037002891,          0, 1057988693,
            33535030, 1051688055,          0, 1068478533,   51365919, 1066372195,          0,    5226549,   68148232,
            7315535,          0,   15716387, 1034908803,   23048251,          0,   27254801, 1048544370,   37733414,
            0,   37746687, 1061131360,   53467154,          0,   48236524, 1073718351,   69200893,          0,
            1050649684,   13612093, 1035960441,          0, 1056944201,   27247659, 1046449258,          0, 1063238718,
            39834649, 1057987675,          0, 1070581810,   53470215, 1068476492,          0,    3133478, 1047500896,
            6272061,          0,   10477593, 1055892563,   17810477,          0,   17820685, 1065332807,   29348893,
            0,   25163776,    1031226,   40886286,          0,   31459315,   10471468,   52425726,          0,
            1062193204,   19911711, 1049601108,          0, 1065340973,   30400530, 1056944202,          0, 1068487718,
            39840772, 1064286271,          0, 1072684062, 1059044414, 1071629364,          0,    2090007, 1064289333,
            5229609,          0,    5237775, 1070582829,   13621279,          0,    9434119,    2085924,   20963348,
            0,   12581888,    8379419,   29355017,          0,   15730681,   14672915,   36698110,          0,
            1073735700,   19917834, 1063242800,          0, 1073736721,   26211330, 1066390569,          0, 1073737742,
            1070587931, 1070585891,          0, 1073738763, 1072686103,    1040412,          0, 1073739784,    1043474,
            5235734,          0, 1073740805,    3141646,    9432079,          0,    1047554,    5239818,   13627401,
            0,          0,    8386566,   17822723,          0,       1023,   10483714,   22020093,          0,
            1042420,   13631487,    2093067,          0,    1045492,    1044473,    3142665,          0,    1048565,
            1045496,    3143687,          0,       3062,    1047543,    3144708,          0, 1071647736,    1048567,
            4193282,          0, 1068502011,       1015,    4194304,          0, 1064307710, 1072694264,    4195327,
            0, 1061159937, 1072694265,    5243902,          0, 1054863360, 1072694268,    6292477,          0,
            972999971,          0,          0,          0, 1001320701,          0,          0,          0, 1029640408,
            0,          0,          0, 1057960115,          0,          0,          0,   12539022,          0,
            0,          0,   41907303,          0,          0,          0,   72325184,          0,          0,
            0,  102744086,          0,          0,          0,  133162989,          0,          0,          0,
            986641661,          0,          0,          0, 1010766045,          0,          0,          0, 1035940029,
            0,          0,          0, 1060064413,          0,          0,          0,   11495548,          0,
            0,          0,   36669531,          0,          0,          0,   61842489,          0,          0,
            0,   88065046,          0,          0,          0,  114288627,          0,          0,          0,
            1001331927,          0,          0,          0, 1021260988,          0,          0,          0, 1042238625,
            0,          0,          0, 1062167686,          0,          0,          0,    9404522,          0,
            0,          0,   30382159,          0,          0,          0,   52408370,          0,          0,
            0,   73387030,          0,          0,          0,   95414265,          0,          0,          0,
            1014973617,          0,          0,          0, 1031755931,          0,          0,          0, 1048538245,
            0,          0,          0, 1065320559,          0,          0,          0,    8361048,          0,
            0,          0,   25143362,          0,          0,          0,   42974251,          0,          0,
            0,   59757587,          0,          0,          0,   77589500,          0,          0,          0,
            1028615308,          0,          0,          0, 1042250874,          0,          0,          0, 1054837865,
            0,          0,          0, 1067424856,          0,          0,          0,    7318598,          0,
            0,          0,   19905588,          0,          0,          0,   33541154,          0,          0,
            0,   47176720,          0,          0,          0,   60813310,          0,          0,          0,
            1042256999,          0,          0,          0, 1051697242,          0,          0,          0, 1061136461,
            0,          0,          0, 1070576704,          0,          0,          0,    6275123,          0,
            0,          0,   15715366,          0,          0,          0,   25155609,          0,          0,
            0,   34595851,          0,          0,          0,   45085694,          0,          0,          0,
            1055897666,          0,          0,          0, 1062191161,          0,          0,          0, 1067436081,
            0,          0,          0, 1072681000,          0,          0,          0,    5232672,          0,
            0,          0,   11526167,          0,          0,          0,   16771086,          0,          0,
            0,   23064582,          0,          0,          0,   29359102,          0,          0,          0,
            1069539358,          0,          0,          0, 1071637529,          0,          0,          0, 1073735701,
            0,          0,          0,    2092048,          0,          0,          0,    4190220,          0,
            0,          0,    7336968,          0,          0,          0,    9435140,          0,          0,
            0,   12581889,          0,          0,          0,   14680061,          0,          0,          0,
            1043449,          0,          0,          0,    1044472,          0,          0,          0,    1046519,
            0,          0,          0,    1047543,          0,          0,          0,       1015,          0,
            0,          0,       1015,          0,          0,          0, 1072694264,          0,          0,
            0, 1072694266,          0,          0,          0,          0,          0,          0,          0,
            1071640590, 1072693248, 1071640590,          0,    2102258,    5252067,    3150834,    5252066,    9451474,
            12602303,    9451473,   12603325,   16802731,   21003158,   16803752,   21004176,   25204605,   31504223,
            25206646,   29408090,   36755264,   43054884,   33609537,   37810987,   48305929,   54604528,   42010392,
            45161222,   60903125,   67202746,   49360628,   53561056,   74549918,          0,   58810058,    1049599,
            1072689166,    6301665, 1073737741,    6301665,    3150834,   12603323,    3150833,   11555770,    8403919,
            19957644,    8403919,   17860492,   15756197,   27311960,   14707620,   24165215,   24159089,   32565044,
            21012341,   26270529,   30463812,   35717917,   25217870,   27324210,   34666278,   40967940,   27322168,
            28376866,   37818131,    2098174,   27325228,    4196347,   43070195,    6301664,   29428504,    6301662,
            2094091,   10507195,    4191240,    9457596,    4200431,   15762321,    5249005,   12614552,    8403918,
            18919275,    7355342,   13672312,   13658022,   19974994,   11560874,   12629860,   17864573,   17881926,
            13668231,    9487194,   19971933,   16835389,   13676397,    6343508,   18928459,    5245944,   11583326,
            7344116,   17882946,    6302685,    8439639,    6302684,   16837431,    7360446,    4247378,    6310850,
            6289412,    8418208,    8388607,    5269417,    6298603,    8425351,    7347176,    2129813,    7355342,
            5284726,    6306767, 1071682441,    8413103,    1093486,    5266357, 1066441602,    8422290, 1069594474,
            4224926, 1060152191,    7379837,    8393711,    1084302,   10491882,    3189617,    6302682, 1069586309,
            6302682, 1072739180,    4212678, 1063296896,    3163082, 1067497321,    1073075, 1057006463, 1071666109,
            10487802, 1070625700,   11537395, 1065378738,    7348197, 1064337306,    8397794, 1058041772,    5257168,
            1058047894,    4208594, 1049654185,    3167164, 1049660308,    1067971, 1040218024, 1072719787,   10492902,
            1068522423,   10493921, 1067482014,    6302681, 1061185454,    5254105, 1061192599,    1063887, 1053848490,
            14292, 1053854613, 1067469766, 1045460904, 1064320976, 1046515604, 1060131776, 1036024744, 1053837262,
            13636588, 1050696637,   13637605, 1043352525,    8397791, 1041260476,    8398813, 1032867791,    3158996,
            1030775741,    3158998, 1021333457, 1071662026,   10494940, 1068514258,    8398807, 1064325059,    4205529,
            1059079118,    3156954, 1054889918, 1071657945, 1048594381, 1069558750, 1045454780, 1060124633, 1038109646,
            1056975843, 1036018620, 1048590299, 1026576336, 1043344360, 1025533885, 1037056990, 1015042002, 1029712878,
            12591070, 1024474081,   10494935, 1016080372,    7350234, 1011891173,    6301656, 1001400314,    2109401,
            6302675,    1059804,    3156942, 1066415065,    2108378, 1063267296,    1058778, 1054881754, 1068509155,
            1049635813, 1066410984, 1042299868, 1052779500, 1036004331, 1049631733, 1030765535, 1038098421, 1022371825,
            1032851457, 1018182627, 1022368766, 1008740343, 1016072205, 1005599719, 1007686662,  995108861,  999292952,
            8399825,  981470210,    4206538,  955248644,    4205526,      12234,    2108372, 1068492821,      10207,
            10203,      10210, 1072694271, 1060120552, 1064311790, 1058021359,    5251046, 1045438448, 1045434366,
            1041242107,   12600261, 1030757369, 1027604494, 1024461831,   23100308, 1015026690, 1009776669, 1007682579,
            34648930,  993004547,  991947819,  991951902,   46194487,  969934848,  974119992,  940567552,   56691472,
            13252, 1067444248, 1065344034, 1070590995,      11218, 1072693248, 1071641611,          0,       9189,
            4201449,    2102259,    5253089, 1054873590,   11549645,    8400855,   11554748, 1035995142,   24146842,
            17850286,   18907026, 1019214870,   39892829,   29398906,   25207665, 1001386020,   53538599,   40946508,
            30457686,  982509617,   67183351,   51443491,   35707710,  910152704, 1069542420,   61939452, 1073738764,
            1064294436,       2046, 1068490781,    1051641, 1070593036,    5252067, 1072691208,    4205534,    2101237,
            12602303,    2103280,    8409022,    8399836,   22052753,    8402896,   11562913,   16798647,   30452584,
            15755175,   13667209,   31496058,   37801799,   22057857,   14720887,   47240001,   46199591,   28357475,
            15774568,   60885775, 1071640592,   33607498,    3143684,   74529504,    1050619,   38857521,    2102259,
            1066392607,    5253087, 1072687125,    3156958, 1071641609,   10506172,    1047555,    3163079,    2102257,
            15759256,    3151853,    3170227,    8401875,   19961724,    6306766,    1077155,   17851304,   23113575,
            10510255, 1072725911,   26253179,   25216851,   12615572, 1070631822,   34651991,    1045513,   14718847,
            6293498,   42001207,    2101238,   14723951,    3151853,   50397975,    4205534,   15776608,    1059807,
            1069540377,    6310850,    3141645, 1071659986, 1073739782,    7366570,    2099196, 1067469767,    3151854,
            7372694,    3153897, 1062229950,    7355342,    7376775,    3160018, 1056989111,   13657002,    6332283,
            3167165, 1051748274,   17861513,    5241856,    2123691,    7344111,   21014385,    3150831,      30621,
            3152872,   24165213,    2108382, 1071679378, 1072703459,   27317065,      16332, 1068536713, 1065365470,
            1043473, 1071666109,    8386561, 1058027482,    1048576, 1068524465,    5246963, 1049639896,    3152875,
            1065381799,    2105318, 1041252310,    5258192, 1061190560, 1072706521, 1032864726,    7363509,    7343093,
            1069564876,    7345123,    7369631,    4201450, 1065373634,    3152868,    7374734,      10209, 1060133818,
            1070605287,    6330241, 1068512216, 1053844405, 1059070954,    5285749, 1062223825, 1048603568, 1048585197,
            6288391, 1055934411,    9439219, 1037050865,    4197368, 1049644999,    5247979, 1025516533,    2105319,
            1042307012,    1056741, 1013982201,    1062869,    7345129, 1069558752,    3152855, 1072712644,    3152870,
            1061172188,    1056737, 1070619575, 1071653861, 1053834201, 1067458540, 1066429356, 1062217700, 1045446615,
            1052776438, 1063286692, 1052781540, 1037059030, 1039141888, 1058045853, 1043345380, 1028671446, 1024460810,
            9438202, 1033909222,    8392675, 1009778708,    5247983, 1023423464,    5248995,  973072388,    2105317,
            5248989,    1056742, 1060097074, 1071656924,    2105314, 1064313832, 1066393628, 1065368532, 1068507113,
            1053828075, 1073739783, 1059079118, 1055924208, 1042293743,    7346156, 1052789705, 1043340279, 1031807987,
            18891716, 1045451717, 1030756350, 1020273655,   33585045, 1038113731, 1018171396, 1008739324,   47228775,
            9440235,  996148225,    4200404,   60871485,    5248999,       8146,    2104284, 1063244841,    1056741,
            8159,       8166, 1068492823, 1067460580, 1066408942, 1060117489,          1, 1058024420, 1050678268,
            1046483963,    7348197, 1048588260, 1033896969, 1031801861,   15747011, 1038102501, 1019214870, 1017119759,
            24144799, 1028666343, 1003484195, 1002438680,   33592189, 1018180585,  986703919,  959438848,   43037533,
            6296540, 1057998902, 1055899708, 1067441184,    3152863, 1065344032, 1063245863, 1071640591,       8166,
            1072690187, 1070590994,          0, 1062215660,    7345136,    3147771,    5252066, 1049631731,   20988869,
            12594137,   10503111, 1037047802,   37780368,   26237868,   14704557, 1024462849,   54570845,   39882622,
            19954581, 1005586434,   70312749,   54574930,   24155006,  985661440, 1061146670,   68217640, 1071638550,
            7117, 1067443225, 1060096050, 1073739784,       8152,    1047556, 1066392608,       4087,       8167,
            7347176, 1071641612,    1057764, 1058019317,   17844163,    3149812,    3161040, 1042286595,   29389722,
            11547604,    4214718, 1026555920,   40934258,   19946417,    5268397, 1010825245,   52478797,   29392782,
            6322078,  995093545, 1065343012,   38839149,    3141642,  931119104, 1069542419,   47236941,    1049599,
            1053801537,       1023, 1065341992, 1072698355, 1062196267,    6300643, 1069541399, 1071653863, 1069541398,
            12601284, 1072691207, 1069559771,    2098175,   19949478,    2103280, 1067466704,   13641692,   27297673,
            7353301, 1064324038,   28336043,   33596269,   12604346, 1062229949,   46176118, 1069540379,   17854369,
            6291453,   61918020, 1072690188,   22054793,    2100215,   78707477,       3066,   27304818, 1071650801,
            1057997879,    3154915, 1071636509, 1066409964, 1064294436,    6307787, 1072689167, 1061169127, 1071640591,
            9459637, 1072693248, 1055928290,    3148791,   12611489,    1054702, 1049637854,   11546582,   15762318,
            2109402, 1044397019,   23092142,    1043472,    4212679,    5244911,   34637702,    1047555,    5266357,
            2101231,   46182239,       4085,    6319013, 1070602224,   57726779,       9189,    6324118, 1062214641,
            1063243821,      14293,    3140624, 1052777459, 1067442204, 1072711623,    2095109, 1044388853, 1072690186,
            1071667129,       3065, 1034951671,    3150834, 1070622637, 1071651821, 1026564089,    9450452,    4192260,
            1070607329,    2100192,   16799669,    1050619, 1068513237,    1053671,   23099287, 1072698354, 1066420171,
            1069553647,   30447483, 1068507113, 1063277505, 1056969719,   36746080, 1065364449, 1061183416, 1045434367,
            1068488739, 1061173209,    7336960, 1032849415, 1070590995, 1056980946,    3146746, 1020265487, 1073740803,
            1052789708,       4084,  989848579,    2103279,    5243894, 1069554670, 1049602131,    5256151,    2101235,
            1064313833, 1059044413,    8408000, 1070602224, 1059072996, 1068487718,   11559851, 1064311790, 1052782560,
            5237771,   13663127, 1056972781, 1047541725,   17831913,   16813956, 1049633771, 1041252313,   33571776,
            1041431, 1042294763,    7340015,   50360212,    1045514, 1034955754,    3148783,   67149672,       2044,
            4196327,       6127, 1054848069,       7149,    2102251, 1066407920, 1062193202,      11229, 1069553647,
            1058020338, 1069539358, 1072709582, 1059067892, 1048583156,    4192261, 1072713664, 1049630713, 1040194550,
            13638632, 1071669171, 1038095358, 1030758392,   25182150, 1070624679, 1027608579, 1021321210,   37774242,
            5238792, 1010828289,    3147742,   50367359,    3145727,       4057,    2101219, 1060095031,       4087,
            6115,       7147, 1065341991, 1070603246, 1068506095, 1063262195, 1070589974, 1067460581, 1054872570,
            1050678267,    3144705, 1063268317, 1041238021, 1039141891,    9444328, 1059077077, 1027604496, 1026556939,
            16791501, 1054884815, 1013970971, 1015021587,   25188274, 1050693577, 1000337446,  979360768,   33584023,
            8388600, 1046454361, 1044356189, 1065341994,    4196340, 1056946242, 1053799496, 1068490780,       5106,
            1067438123, 1063241778, 1071640590, 1067457519,    5236750, 1073733658,          0, 1061167085,   18880490,
            10486780,    4201450, 1053828076,   37766077,   25177045,    8401878, 1046489067,   56653708,   41966506,
            11552707, 1039150058,   75540316,   58754942,   15752111, 1031811050, 1051701324,   75544402, 1071636507,
            5243879, 1060095031, 1050651726, 1071639569,    3149801, 1069537314, 1057996859, 1071642630,       6125,
            5238792, 1066390568, 1071647739, 1064310770,   15734761, 1073736723, 1071651822, 1053825014,   29376451,
            8391672, 1071654881, 1044386811,   44067739,   19934167, 1072707540, 1032851456,   58758003,   31477684,
            1072711624, 1018169345, 1056947262,   44070800,    3141643, 1003486208, 1064292396,   56662893,    1046533,
            3029, 1070588954, 1056947263, 1070598143,       5085,    4192259, 1063242799, 1067454457,       7145,
            11541480, 1068489759, 1065359347, 1061165044,   20986826, 1073737740, 1062215660, 1047531519,   31481770,
            6295541, 1059071974, 1033896970,   41975691,   13642715, 1055928289, 1021312022, 1063242800,   20989888,
            4193274, 1007678497, 1067441186,   29385636,    1050617,  953137152, 1071639570,   37782409, 1070599161,
            1041208421,    1049599, 1064291376, 1064308728, 1051700302,    7347177, 1067440163, 1058018296, 1062192183,
            12597202, 1070589973, 1051726840, 1072684062,   18894778, 1073739782, 1045436409,   11535358,   25192355,
            2101236, 1038096377,   28322772, 1068488739,    6301664,    3145705,   47210405, 1070589975,   10501069,
            1051629,   66097012, 1071641610,   13652921, 1070601203,   84984644, 1072695292,   17852325, 1061164024,
            1047503958,    1055724, 1071635488, 1050677246, 1055897666,    3157980, 1071638550, 1041238020, 1065340973,
            5259212, 1071641612, 1031799819, 1073735702,    7361468, 1070596097, 1007674370,    9439225,    1042452,
            1071649780, 1037009015,   23079894, 1072690187, 1071652840, 1050646623,   36722607, 1071643651, 1071656923,
            1063235654,   51412871, 1069550586, 1072709582,    3132458,   66103135, 1068505072, 1072713666,   18868232,
            1053799495, 1067459559,    3139598,   35657700, 1061144629, 1065365469,    2094088,   54542266, 1067440164,
            1064319956, 1073740802,   74476431, 1073737743,    4191235, 1069550588, 1044354149,    7343094,    1049599,
            1066406902, 1054845008,   16788441, 1070599164, 1063263216, 1065335867,   26233786, 1066405880, 1060119529,
            3134499,   36728730, 1061164021, 1056975844,   14674949,   47222651, 1056971762, 1053832158,   28317671,
            1060095032, 1051729904,    6289403,   43006916, 1065341993, 1046488045,    3145722,   57696160, 1069540378,
            4194289,       3065, 1051699282, 1073738761,    1051635, 1067454456, 1059043393,    4198388, 1070600182,
            1061164024, 1067437103,    9447389, 1062211576, 1054872568,    2088987,   15745990, 1054871547, 1048582136,
            10482691,   22043567, 1046482942, 1041242105,   19929066,   27292567, 1038093314, 1034951673,   31470541,
            1067439145, 1025507329,    3144679,   41964465, 1069539357,        992,    1050603, 1057995840, 1070590993,
            4071,       5104, 1063241778, 1072692227, 1070602224, 1065358325, 1068488740,       5108, 1059066872,
            1055920123,    1042452,    2106340, 1047529473, 1046480897,    6290433,    4208596, 1035994122, 1035994119,
            11539437,    6309828, 1025506324, 1026555918,   18885592,    8412084, 1013969950,  998234112,   26231746,
            1041432, 1033861248, 1030714499, 1065339949, 1073737743, 1047498855, 1043303531, 1067440163, 1072691207,
            1062186060, 1056941139, 1069540376, 1070598142,    4180014, 1070579769, 1072690188, 1068504053,   20964361,
            11525146,          0, 1067458539,   39852002,   27263991,    3150833, 1066413026,   60834742,   45099983,
            6300643, 1064317913,   81818503,   65034149,    9450453, 1063272400, 1041206382,   83919738, 1072686106,
            5238789, 1052745816, 1039109231, 1071639571,    3143681, 1064286272, 1049600090, 1070593037, 1072695294,
            3133478, 1060090950, 1070595078, 1068502010,   16772103, 1070581807, 1068500990, 1064309751,   32511973,
            9430037, 1067454455, 1059067892,   49299391,   20973559, 1067457519, 1053826033,   66086807,   35661781,
            1066410983, 1049633774, 1047501915,   50351026,    3141638, 1044391916, 1056944201,   66089869,    1046532,
            5240817, 1066386485, 1047502939, 1070596098,    2098162,    2087967, 1054847050, 1066402816,       4085,
            12578820, 1063239736, 1063259134, 1066405879,   24123368, 1071633446, 1059065852, 1059065849,   36714440,
            6286352, 1054871547, 1050677245,   50354088,   15730679, 1050678266, 1042287616, 1054847049,   26223580,
            2095089, 1031799809, 1061142586,   36717503,    1050611, 1020263424, 1068486698,   47210402, 1071648759,
            1047518,    1041431, 1055896647, 1064308730,       2019,    8386562, 1061142585, 1056967678,       6123,
            16783339, 1066389547, 1049626626, 1064309748,   25178067, 1071636508, 1042285575, 1053822972,   33573817,
            3142667, 1024452610, 1042285574, 1062192183,    8391671, 1025464477, 1030750223, 1065340971,   15736803,
            1041199234, 1019213849, 1069539358,   22034381, 1057983591,  975157248, 1073736720,   29380536,    2074697,
            1026518157,    3145728, 1064290354,   20957224, 1041204340,    7345135, 1066390568,   40888322, 1054841946,
            12593117, 1068490782,   60823517, 1069529149,   17841100, 1071639570,   82853811,   12572701, 1068488740,
            1073739782, 1033859206,   30409719, 1069539355,    2100217, 1047496815,   50343884, 1070590994,    5250026,
            1061133400,   71326623, 1071642633,    7351260,    2077758,   93358960, 1071646719,   10501069,   16764962,
            1034911865, 1072698356, 1072685085,   32500738, 1046451299,       8169, 1071638551,   49288162, 1057991756,
            1059806, 1071640592,   67123135, 1070580788,    2092048, 1070594057, 1042253935,   10477592, 1073738764,
            1068498946, 1052744796,   24118263, 1071642631, 1068502010, 1063235656,   40905682, 1068498947, 1067456499,
            1033267,   57693099, 1065355262, 1067458539,   12572700,   74480515, 1063260153, 1066412003,   25161730,
            1043305573, 1061165045,    4189191,   37753831, 1052747858, 1059068912,    2094084,   51392458, 1061141567,
            3143675, 1073740803, 1050648664, 1071632426,    1049595, 1068498945, 1057992777,    8382483, 1071646716,
            1065355263, 1065336889,   18876407, 1065356285, 1061161981, 1073729576,   30418904, 1060113406, 1056968700,
            8381462,   44058552, 1053821951, 1052775418,   16774145,   56650648, 1048578049, 1048581113,   26218476,
            1051699281, 1040188416,    3141616,   35661782, 1057994817,    1046502,    2097138, 1059043392, 1064290354,
            2027,       3061, 1063240757, 1071635489, 1071650801, 1068503032, 1068486697,    5238797, 1063261175,
            1061161980, 1072685085,   12584951, 1053822974, 1052772352,    4189199,   20980703, 1044382725, 1045431301,
            9436161,   29375430, 1035993101, 1038091274,   13635570,   38819757, 1027602453, 1018159104,   19931106,
            1060092989, 1020218537, 1017072810, 1067439145, 1063242801, 1039100044, 1032807568, 1068489761, 1067440164,
            1056932974, 1049591925, 1070588954, 1071637527,    2073678, 1067424856, 1071639570,    2094088,   23053355,
            11515961, 1073738761,    5245944,   45081603,   31447062,          0,   10493926,   67114970,   50332657,
            2100215,   14692308,   90194862,   72362952,    4200431,   19940291, 1029661842,   93344670,    1043473,
            1068487720, 1044348025, 1026516113, 1073737742, 1069538336, 1060082783, 1040153723, 1071641610, 1069541399,
            2076740, 1053790307, 1070594055, 1070592014,   18861093, 1068476491, 1067450372, 1071643652,   36694018,
            9421873, 1065354240, 1071647737,   55579615,   25157651, 1064307710, 1072699374,   74464185,   40895475,
            1062211579,    1057763, 1038056570,   57681872,    2094072,    2110424, 1049596005,   75515821,    2097145,
            2091026, 1062184016, 1037009016, 1072696315,    1044494,    1031225, 1047499877, 1067453436, 1072690185,
            14668831, 1057989714, 1063259134, 1070595077,   29355010, 1069529150, 1058014209, 1066402816,   44045284,
            7327784, 1052771332, 1064307708,   58733509,   18868240, 1042280449, 1062212599, 1046451299,   31458293,
            1011822788, 1060116467, 1054843986,   44048345, 1032801446, 1058021358, 1064286273,   57687996, 1053780104,
            4190203,    1034286, 1046452319,    1016936,    2096123,   10477593, 1053796432,   23045190,       2044,
            20968449, 1062189121,   46123042, 1068502012,   31462378, 1069533233,   69201916, 1063259133,   43002832,
            4184095,   92279764, 1056967678, 1054846028,   13626380, 1022315688, 1050675200, 1061141567,   20973559,
            1039099022, 1044382720, 1067436081,   30415841, 1056930932, 1035993088, 1073731619,   39859147,    1021017,
            1044453,    6285331, 1055896646,   18853949,    1048552,   13629441, 1061142587,   37736478,       4078,
            19927023, 1065339951,   57667582, 1067455476,   27272156, 1070585891,   76550108, 1059065850, 1063241781,
            2090006, 1031761036, 1049626625, 1066389547,    6288392, 1045396599, 1040188425, 1069538338,   11536377,
            1060082785, 1031797777, 1072686103,   16783338, 1073719371,  998227968,    2093068,   23078875,   14662707,
            1011826871,    5241856, 1066389548,   30398490, 1029658779,    8392693, 1067440165,   46134271, 1047491709,
            11541480, 1069539358,   60821475, 1066374238, 1071636509, 1070589974, 1042253936,   12563517, 1071637528,
            1072689166, 1052743775,   33544216, 1070590994, 1073740805, 1063233614,   55575535, 1070593036,    1050620,
            1073723452,   78654404, 1070594054,    3150835,   11521065,  102782871, 1069547520,    5250026,   23060501,
            1021270174, 1069550586,    2091026,   34599936, 1037004933, 1070601205,    1043471,   45091818, 1051691116,
            2092037, 1072690188, 1051698260, 1067425874,    1046531, 1071642633, 1059041351,   10468405, 1072692227,
            1069546501, 1066384442,   28302357, 1068498946, 1066401794, 1073728557,   46138354, 1065354241, 1065355263,
            7329823,   65021900, 1062208513, 1063259132,   14673936,   84955046, 1058014209, 1062211578,   23066624,
            1031763077, 1053819904,    2092024,   30411760, 1043302512,    1045484,    2095096, 1061143608, 1055890523,
            1007,       2042, 1065339951, 1068478533,       5107, 1070599163, 1069536295,    8374316, 1067454455,
            1065356285, 1072684062,   22012945, 1060114428, 1060111360,    3139604,   35652596, 1053820929, 1054868482,
            7335946,   51389396, 1046478855, 1049625606,   11533312,   67126196, 1040186381, 1037035520,   15730679,
            1041206380, 1007624402, 1002381523, 1071636508, 1050647643, 1029651634, 1022311605, 1071637527, 1060089930,
            1051679889, 1043290263, 1072688147, 1069531191,    1014895, 1064268920, 1072689166,    5231652,   25140299,
            11506775, 1073738762,   15722510,   49267748,   34583605, 1073740805,   26215414,   75492347,   57661455,
            0,   37756893,  100668368,   81788904,    1050621,   49297347, 1017068726,  104867776,    2093055,
            1051699283, 1035950234, 1013924021,    1047551, 1057993797, 1054830718, 1030707355,       1023, 1064288312,
            1018977, 1047490690, 1071646719, 1070583850,   20950081, 1065322599, 1068500991,    3137563,   41928736,
            9413707, 1065354240,   10481674,   63959037,   28295214, 1063257089,   15730680,   84939736,   47176718,
            1059059712,   24124390, 1027562650,   67108845,  999228651,   31468499, 1042247811,   87039947, 1023354058,
            1061142585, 1057982571, 1025466518, 1048527017, 1064291376, 1073717330, 1039103105, 1072652423, 1067439143,
            16758840, 1052739692,   25133156, 1070587933,   34591772, 1066376278,   50307136, 1073736722,   51377150,
            7320639,   77579290,    3143687,   68161504,   23054375,  103803891,    6293499, 1037006974,   37741580,
            1010772170,   10490862, 1049594987,   53477361, 1031749806,   13639650, 1061133400,   68164565, 1052728465,
            1071635488, 1073721411, 1037009016, 1073706101, 1071637530,   12567598, 1047498855,   20942935, 1071638549,
            26205207, 1057988695,   42970168, 1070592015,   39842815, 1068478533,   64998425, 1070594057,   52431847,
            5227571,   87026680, 1070595075, 1046452322,   16765983, 1022315689, 1069549565, 1055892563,   28305418,
            1039098002, 1070600184, 1064285252,   38798325, 1055880314, 1070602226, 1073726516,   51386335, 1073712226,
            3139589,    9425956, 1048551514,   16753738,    2094084,   18867218, 1054846030,   34584624,    1047555,
            28308480, 1062189121,   53465110, 1070596098,   37751789, 1069532212,   71299068, 1066401793, 1056945222,
            3134502, 1033858184, 1063257089, 1062191163,   11526167, 1046446198, 1060111361, 1067436080,   18870280,
            1060081763, 1055917056, 1073730597,   26215416, 1073717327, 1051722752,    5234713,   34608104,   13612092,
            1043435,   11529229, 1060094012,   27248679,    1047533,   17823744, 1063241780,   41933843,       3056,
            23071731, 1067438123,   55570430, 1070601205, 1066389546, 1071634466, 1044353128, 1064308729, 1068488739,
            1041433, 1054841945, 1056967678, 1070587933,    5237775, 1064283211, 1049625604, 1072687126,    9435141,
            1073723452, 1043333130,    1044495,   13632507,   10471469, 1020251136,    4191240,   17829874,   20960286,
            996085986,    6290432, 1070586910,   31450126, 1018114242,    8390650, 1071637530,   41940991, 1040141474,
            1043470, 1071638549, 1055896647, 1063218304,    1045515, 1072688145, 1062190141,   12553309, 1073738761,
            1072690188, 1068483635,   36679736, 1072691207, 1073739783,    1036329,   62903312, 1071643652,    1047555,
            7329823,   88080358, 1069547523,       1023,   13624340,  114305979, 1068498945,    1050619,   20966409,
            1007628484, 1067451392,    2091008,   27261951, 1026509992,    1044465,    2094079, 1066390567, 1045390476,
            1047538,    1048575, 1069538337, 1064271983,       3061, 1072694271, 1072685083,   10460241, 1071648759,
            1069549567,    1042453,   31438897, 1066404858, 1067451392,    4189200,   52419599, 1062210558, 1064305665,
            7335946,   74448875, 1058014210, 1062208514,   10483716,   95429574, 1052770310, 1055912960,   13631487,
            1019170981,  993981691,  987691259,    1043463, 1034905742, 1020204248, 1011815642,    1044485, 1050639479,
            1046426804, 1035941049,    1046531, 1066374239, 1072649360, 1061114008,    1047553,    8367173,   26179691,
            11497590,    1047552,   25150506,   54500420,   37720146,          0,   42983437,   82821147,   63942701,
            1023,   60817391,  112192497,   90166279,          0,   77602768, 1005525210,  117439456,          0,
            1030713479, 1027552444, 1000283352,          0, 1043300468, 1050628253, 1021260988,          0, 1054839905,
            1073704062, 1042239648,          0, 1067427918,   23038046, 1063217283,          0,    6274105,   47163452,
            10454118,          0,   19910691,   71288858,   31432776,          0,   33548299,   95415286,   53460009,
            0,   47185907, 1017067706,   76535816,          0,   59774938, 1034899616,   98565096,          0,
            1042255977, 1053780102, 1013924021,          0, 1051696219, 1073709163, 1030706333,          0, 1060088908,
            18847824, 1047489670,          0, 1069530172,   38777908, 1065320558,          0,    4181036,   58706967,
            8362070,          0,   13622299,   79686650,   26192957,          0,   24112137, 1027562649,   44024867,
            0,   32506871, 1043296388,   61856777,          0,   42996708, 1057981550,   80739310,          0,
            1053798475, 1073715289, 1026516114,          0, 1059043393,   15707203, 1040152703,          0, 1065337910,
            31440940, 1053788268,          0, 1070583851,   47174677, 1067423833,          0,    2087967,   63959037,
            7318598,          0,    8382483, 1039106168,   20954162,          0,   14676999, 1050644584,   34590749,
            0,   19923962, 1062181975,   49275912,          0,   26218477, 1073720390,   62913523,          0,
            1065339949,   12565557, 1040156783,          0, 1067439143,   24103971, 1049598049,          0, 1069538336,
            36691985, 1059038290,          0, 1071637529,   48231422, 1069527108,          0, 1073736722, 1050648663,
            5226549,          0,    3141643, 1057991755,   15715366,          0,    5240836, 1066383423,   26205206,
            0,    6292477,    1033267,   36695046,          0,    9440246,    9424934,   47184887,          0,
            1042447,   17816601, 1052748876,          0,    1044493,   26208268, 1059043394,          0,    1045514,
            34600959, 1065336888,          0, 1072691208, 1061143607, 1071631406,          0, 1071643653, 1065339951,
            4183076,          0, 1070595075, 1070584871,   10477593,          0, 1068498946,    1039391,   17819663,
            0, 1067450368,    6284311,   24114180,          0, 1067452415,   10480655,   31457274,          0,
            1042417,   15725575, 1065340970,          0,    1046514,   20970495, 1068488740,          0,       1011,
            1072686103, 1070586910,          0,       4086, 1073735699, 1073734680,          0, 1068503033,    1043471,
            3139603,          0, 1064307708,    2093068,    6287373,          0, 1060111360,    3142664,    9434119,
            0, 1055917060,    4192261,   12580866,          0, 1043325952,    6290434,   14681084,          0,
            980346125,    6292479,    1043464,          0, 1006568682,    1044470,    1044486,          0, 1032791238,
            1046518,    1045508,          0, 1060062370,    1048566,    1046530,          0,   12544126,       2039,
            1047553,          0,   40863832, 1072696313,          0,          0,   69184560, 1070599162,       1023,
            0,   97506310, 1068500989,       1023,          0,  126877660, 1066402816,          0,          0,
            993986794,          0,          0,          0, 1016014027,          0,          0,          0, 1039089837,
            0,          0,          0, 1062165645,          0,          0,          0,   11499630,          0,
            0,          0,   34576461,          0,          0,          0,   58701867,          0,          0,
            0,   83875848,          0,          0,          0,  108002276,          0,          0,          0,
            1007628486,          0,          0,          0, 1025459373,          0,          0,          0, 1044339859,
            0,          0,          0, 1064268921,          0,          0,          0,    9407582,          0,
            0,          0,   29337666,          0,          0,          0,   49266726,          0,          0,
            0,   69196809,          0,          0,          0,   89127915,          0,          0,          0,
            1020220579,          0,          0,          0, 1035954318,          0,          0,          0, 1050638457,
            0,          0,          0, 1066372196,          0,          0,          0,    7315534,          0,
            0,          0,   23049271,          0,          0,          0,   39832608,          0,          0,
            0,   55566345,          0,          0,          0,   71302129,          0,          0,          0,
            1033861248,          0,          0,          0, 1045399664,          0,          0,          0, 1056937055,
            0,          0,          0, 1068475470,          0,          0,          0,    6272061,          0,
            0,          0,   17810476,          0,          0,          0,   30398490,          0,          0,
            0,   42985479,          0,          0,          0,   54524917,          0,          0,          0,
            1046453341,          0,          0,          0, 1054845009,          0,          0,          0, 1062188101,
            0,          0,          0, 1070578745,          0,          0,          0,    5228588,          0,
            0,          0,   13620256,          0,          0,          0,   22011923,          0,          0,
            0,   30404613,          0,          0,          0,   38797304,          0,          0,          0,
            1059045435,          0,          0,          0, 1063241779,          0,          0,          0, 1068486699,
            0,          0,          0, 1072683043,          0,          0,          0,    3137563,          0,
            0,          0,    8382483,          0,          0,          0,   13627403,          0,          0,
            0,   17822723,          0,          0,          0,   23068667,          0,          0,          0,
            1071637529,          0,          0,          0, 1072687125,          0,          0,          0, 1073736721,
            0,          0,          0,    1044493,          0,          0,          0,    2094090,          0,
            0,          0,    3143686,          0,          0,          0,    5241859,          0,          0,
            0,    6290433,          0,          0,          0,    7341054,          0,          0,          0,
            1042423,          0,          0,          0,    1045494,          0,          0,          0,    1047542,
            0,          0,          0,       1015,          0,          0,          0,       3064,          0,
            0,          0, 1071647738,          0,          0,          0, 1069550588,          0,          0,
            0, 1067452414,          0,          0,          0, 1065352192,          0,          0,          0,
        }
    },
};



static const ot_eis_vi_expander_attr g_cmos_expander = {
    1,
    16,
    20,
    33,
    {
//for 16bit
#if 1
        {2,32},
        {4,64},
        {8,128},
        {12,192},
        {16,256},
        {22,384},
        {27,512},
        {33,768},
        {39,1024},
        {44,1536},
        {48,2048},
        {56,3072},
        {63,4096},
        {67,6144},
        {71,8192},
        {78,12288},
        {85,16384},
        {97,24576},
        {107,32768},
        {124,49152},
        {137,65536},
        {145,81920},
        {151,98304},
        {162,131072},
        {169,163840},
        {175,196608},
        {185,262144},
        {193,327680},
        {201,393216},
        {216,524288},
        {227,655360},
        {238,786432},
        {256,1048576},
#endif

//for 12bit
#if 0
        {20,32},
        {27,64},
        {39,128},
        {44,192},
        {48,256},
        {54,384},
        {60,512},
        {67,768},
        {71,1024},
        {78,1536},
        {84,2048},
        {94,3072},
        {99,4096},
        {107,6144},
        {114,8192},
        {126,12288},
        {132,16384},
        {141,24576},
        {149,32768},
        {163,49152},
        {169,65536},
        {175,81920},
        {180,98304},
        {189,131072},
        {197,163840},
        {205,196608},
        {217,262144},
        {223,327680},
        {228,393216},
        {236,524288},
        {241,655360},
        {247,786432},
        {256,1048576},
#endif
    },
};


#ifdef __cplusplus
#if __cplusplus
}
#endif
#endif /* End of #ifdef __cplusplus */

#endif /* OX03F10_CMOS_EX_H */

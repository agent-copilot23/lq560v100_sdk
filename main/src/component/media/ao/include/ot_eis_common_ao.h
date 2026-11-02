/*
  Copyright (c), 2021-2022, Oritek Tech. Co., Ltd.
 */

#ifndef OT_EIS_COMMON_AO_H
#define OT_EIS_COMMON_AO_H

#include "ot_eis_common.h"
#include "ot_eis_errno.h"
#include "ot_eis_mod.h"

#ifdef __cplusplus
extern "C" {
#endif /* End of #ifdef __cplusplus */


#define OT_EIS_MAX_AUDIO_FRAME_NUM    300      /* max count of audio frame in Buffer */

#define OT_EIS_MAX_VOICE_POINT_NUM    480      /* max sample per frame for voice encode */

#define OT_EIS_MAX_AUDIO_POINT_NUM    2048     /* max sample per frame for all encoder(aacplus:2048) */
#define OT_EIS_MIN_AUDIO_POINT_NUM    80       /* min sample per frame */
#define OT_EIS_MAX_AI_POINT_NUM       2048     /* max sample per frame for all encoder(aacplus:2048) */

#define OT_EIS_AUDIO_FRAME_CHN_NUM    2        /* channel number in frame */

#define OT_EIS_MAX_AUDIO_FILE_PATH_LEN 256
#define OT_EIS_MAX_AUDIO_FILE_NAME_LEN 256

#define EIS_ACODEC_FILE "/dev/tlv320aic3101"
#define EIS_INNER_ACODEC_FILE "/dev/acodec"


typedef enum {
    OT_EIS_AUDIO_SAMPLE_RATE_8000   = 8000,    /* 8kHz sample rate */
    OT_EIS_AUDIO_SAMPLE_RATE_12000  = 12000,   /* 12kHz sample rate */
    OT_EIS_AUDIO_SAMPLE_RATE_11025  = 11025,   /* 11.025kHz sample rate */
    OT_EIS_AUDIO_SAMPLE_RATE_16000  = 16000,   /* 16kHz sample rate */
    OT_EIS_AUDIO_SAMPLE_RATE_22050  = 22050,   /* 22.05kHz sample rate */
    OT_EIS_AUDIO_SAMPLE_RATE_24000  = 24000,   /* 24kHz sample rate */
    OT_EIS_AUDIO_SAMPLE_RATE_32000  = 32000,   /* 32kHz sample rate */
    OT_EIS_AUDIO_SAMPLE_RATE_44100  = 44100,   /* 44.1kHz sample rate */
    OT_EIS_AUDIO_SAMPLE_RATE_48000  = 48000,   /* 48kHz sample rate */
    OT_EIS_AUDIO_SAMPLE_RATE_64000  = 64000,   /* 64kHz sample rate */
    OT_EIS_AUDIO_SAMPLE_RATE_96000  = 96000,   /* 96kHz sample rate */
    OT_EIS_AUDIO_SAMPLE_RATE_BUTT,
} ot_eis_audio_sample_rate;

typedef enum {
    OT_EIS_AUDIO_BIT_WIDTH_8   = 0,   /* 8bit width */
    OT_EIS_AUDIO_BIT_WIDTH_16  = 1,   /* 16bit width */
    OT_EIS_AUDIO_BIT_WIDTH_24  = 2,   /* 24bit width */
    OT_EIS_AUDIO_BIT_WIDTH_BUTT,
} ot_eis_audio_bit_width;

typedef enum {
    OT_EIS_AO_MODE_I2S_MASTER  = 0,    /* AO I2S master mode */
    OT_EIS_AO_MODE_I2S_SLAVE,          /* AO I2S slave mode */
    OT_EIS_AO_MODE_PCM_SLAVE_STD,      /* AO PCM slave standard mode */
    OT_EIS_AO_MODE_PCM_SLAVE_NON_STD,  /* AO PCM slave non-standard mode */
    OT_EIS_AO_MODE_PCM_MASTER_STD,     /* AO PCM master standard mode */
    OT_EIS_AO_MODE_PCM_MASTER_NON_STD, /* AO PCM master non-standard mode */
    OT_EIS_AO_MODE_BUTT
} ot_eis_ao_mode;

typedef enum {
    OT_EIS_AO_I2STYPE_INNERCODEC = 0, /* AO I2S connect inner audio CODEC */
    OT_EIS_AO_I2STYPE_INNERHDMI,      /* AO I2S connect Inner HDMI */
    OT_EIS_AO_I2STYPE_EXTERN,         /* AO I2S connect extern hardware */
    OT_EIS_AO_I2STYPE_DMIC,           /* AO I2S connect DMIC */
} ot_eis_ao_i2s_type;

typedef enum {
    OT_EIS_AUDIO_SOUND_MODE_MONO   = 0, /* mono */
    OT_EIS_AUDIO_SOUND_MODE_STEREO = 1, /* stereo */
    OT_EIS_AUDIO_SOUND_MODE_BUTT
} ot_eis_audio_snd_mode;

typedef struct {
    ot_eis_audio_sample_rate sample_rate;    /* sample rate */
    ot_eis_audio_bit_width   bit_width;      /* bit_width */
    ot_eis_ao_mode          work_mode;      /* master or slave mode */
    ot_eis_audio_snd_mode    snd_mode;       /* mono or stereo */
    ot_u32               frame_num;      /* frame num in buf[2,OT_EIS_MAX_AUDIO_FRAME_NUM] */
    ot_u32               point_num_per_frame;   /* point num per frame (80/160/240/320/480/1024/2048)
                                                   (ADPCM IMA should add 1 point, AMR only support 160) */
    ot_u32               chn_cnt;        /* channel number on FS, valid value:1/2/4/8 */
    ot_eis_ao_i2s_type      i2s_type;       /* i2s type */
} ot_eis_ao_attr;

typedef struct {
    ot_eis_audio_bit_width bit_width; /* audio frame bit_width */
    ot_eis_audio_snd_mode snd_mode;   /* audio frame mono or stereo mode */
    ot_u8 ALIGN_ATTR *virt_addr[OT_EIS_AUDIO_FRAME_CHN_NUM];
    ot_phys_addr ALIGN_ATTR phys_addr[OT_EIS_AUDIO_FRAME_CHN_NUM];
    ot_u64 time_stamp; /* audio frame time stamp */
    ot_u32 seq;        /* audio frame seq */
    ot_u32 len;        /* data length per channel in frame */
    ot_eis_handle pool_hdl[OT_EIS_AUDIO_FRAME_CHN_NUM];
} ot_eis_audio_frame;

typedef struct {
    ot_eis_audio_frame *frame; /* frame pointer */
    ot_u32 id;             /* frame id */
} ot_eis_audio_frame_info;

typedef struct {
    ot_u32 chn_total_num; /* total number of channel buffer */
    ot_u32 chn_free_num;  /* free number of channel buffer */
    ot_u32 chn_busy_num;  /* busy number of channel buffer */
} ot_eis_ao_chn_state;

typedef enum {
    OT_EIS_AUDIO_FADE_RATE_1   = 0,
    OT_EIS_AUDIO_FADE_RATE_2   = 1,
    OT_EIS_AUDIO_FADE_RATE_4   = 2,
    OT_EIS_AUDIO_FADE_RATE_8   = 3,
    OT_EIS_AUDIO_FADE_RATE_16  = 4,
    OT_EIS_AUDIO_FADE_RATE_32  = 5,
    OT_EIS_AUDIO_FADE_RATE_64  = 6,
    OT_EIS_AUDIO_FADE_RATE_128 = 7,

    OT_EIS_AUDIO_FADE_RATE_BUTT
} ot_eis_audio_fade_rate;

typedef struct {
    ot_bool fade;
    ot_eis_audio_fade_rate fade_in_rate;
    ot_eis_audio_fade_rate fade_out_rate;
} ot_eis_audio_fade;

typedef struct {
    ot_bool cfg;
    ot_char file_path[OT_EIS_MAX_AUDIO_FILE_PATH_LEN];
    ot_char file_name[OT_EIS_MAX_AUDIO_FILE_NAME_LEN];
    ot_u32  file_size;  /* in KB */
} ot_eis_audio_save_file_info;

typedef struct {
    ot_bool saving;
} ot_eis_audio_file_status;

typedef enum {
    OT_ERR_AO_INVALID_DEV_ID = 50,
    OT_ERR_AO_INALID_CHN_ID,
    OT_ERR_AO_NOT_CFG,
    OT_ERR_EIS_AO_NOT_PERM,
    OT_ERR_EIS_AO_NOT_READY,
    OT_ERR_EIS_AO_BUTT
} ot_err_eis_ao_code;

/* invalid device ID */
#define OT_EIS_ERR_AO_INVALID_DEV_ID    OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_AO, OT_ERR_AO_INVALID_DEV_ID)
/* invalid channel ID */
#define OT_EIS_ERR_AO_INVALID_CHN_ID    OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_AO, OT_ERR_AO_INALID_CHN_ID)
/* at lease one parameter is illegal, eg, an illegal enumeration value */
#define OT_EIS_ERR_AO_ILLEGAL_PARAM     OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_AO, OT_EIS_ERRNO_ILLEGAL_PARAM)
/* using a null pointer */
#define OT_EIS_ERR_AO_NULL_PTR          OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_AO, OT_EIS_ERRNO_NULL_PTR)
/* try to enable or initialize system, device or channel, before configing attribute */
#define OT_EIS_ERR_AO_NOT_CFG           OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_AO, OT_ERR_AO_NOT_CFG)
/* operation is not supported by now */
#define OT_EIS_ERR_AO_NOT_SUPPORT       OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_AO, OT_EIS_ERRNO_NO_SUPPORT)
/* operation is not permitted, eg, try to change static attribute */
#define OT_EIS_ERR_AO_NOT_PERM          OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_AO, OT_ERR_EIS_AO_NOT_PERM)
/* the device is not enabled */
#define OT_EIS_ERR_AO_NOT_ENABLED       OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_AO, OT_EIS_ERRNO_NO_ENABLE)
/* failure caused by malloc memory */
#define OT_EIS_ERR_AO_NO_MEM            OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_AO, OT_EIS_ERRNO_NO_MEM)
/* failure caused by malloc buffer */
#define OT_EIS_ERR_AO_NO_BUF            OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_AO, OT_EIS_ERRNO_BUF_FULL)
/* no data in buffer */
#define OT_EIS_ERR_AO_BUF_EMPTY         OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_AO, OT_EIS_ERRNO_BUF_EMPTY)
/* no buffer for new data */
#define OT_EIS_ERR_AO_BUF_FULL          OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_AO, OT_EIS_ERRNO_BUF_FULL)
/* system is not ready,had not initialed or loaded */
#define OT_EIS_ERR_AO_NOT_READY         OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_AO, OT_ERR_EIS_AO_NOT_READY)

#define OT_EIS_ERR_AO_BUSY              OT_EIS_ERRNO_DEFINE(OT_EIS_MODULE_AO, OT_EIS_ERRNO_BUSY)

#ifdef __cplusplus
}
#endif /* End of #ifdef __cplusplus */

#endif /* End of #ifndef OT_EIS_COMMON_AO_H */

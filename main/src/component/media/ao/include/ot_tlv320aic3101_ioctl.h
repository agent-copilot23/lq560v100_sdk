/*
  Copyright (c), 2021-2023, Oritek Tech. Co., Ltd.
 */
#ifndef OT_TLV320AIC3101_IOCTL_H
#define OT_TLV320AIC3101_IOCTL_H

#include "ot_eis_common_ao.h"
#include "ot_eis_type.h"

#define IOC_TYPE_ACODEC 'A'

 typedef enum {
	IOC_NR_SOFT_RESET_CTRL = 0x0,
	IOC_NR_SET_I2S1_FS,
	IOC_NR_SET_MIXER_MIC,

	/* input/output volume */
	IOC_NR_SET_INPUT_VOL,
	IOC_NR_SET_OUTPUT_VOL,
	IOC_NR_GET_INPUT_VOL,
	IOC_NR_GET_OUTPUT_VOL,

	/* analog part input gain */
	IOC_NR_BOOSTL_ENABLE,
	IOC_NR_BOOSTR_ENABLE,
	IOC_NR_SET_GAIN_MICL,
	IOC_NR_SET_GAIN_MICR,
	IOC_NR_GET_GAIN_MICL,
	IOC_NR_GET_GAIN_MICR,

	/* ADC/DAC volume */
	IOC_NR_SET_DACL_VOL,
	IOC_NR_SET_DACR_VOL,
	IOC_NR_SET_ADCL_VOL,
	IOC_NR_SET_ADCR_VOL,
	IOC_NR_GET_DACL_VOL,
	IOC_NR_GET_DACR_VOL,
	IOC_NR_GET_ADCL_VOL,
	IOC_NR_GET_ADCR_VOL,

	/* input/output mute */
	IOC_NR_SET_MICL_MUTE,
	IOC_NR_SET_MICR_MUTE,
	IOC_NR_SET_DACL_MUTE,
	IOC_NR_SET_DACR_MUTE,

	/* ADC/DAC power down */
	IOC_NR_SET_PD_DACL,
	IOC_NR_SET_PD_DACR,
	IOC_NR_SET_PD_ADCL,
	IOC_NR_SET_PD_ADCR,
	IOC_NR_SET_PD_LINEINL,
	IOC_NR_SET_PD_LINEINR,

	/* ADC/DAC clk */
	IOC_NR_SEL_DAC_CLK,
	IOC_NR_SEL_ADC_CLK,
	IOC_NR_SEL_ANA_MCLK,

	/* ADC/DAC track */
	IOC_NR_DACL_SEL_TRACK,
	IOC_NR_DACR_SEL_TRACK,
	IOC_NR_ADCL_SEL_TRACK,
	IOC_NR_ADCR_SEL_TRACK,

	IOC_NR_SET_DAC_DE_EMPHASIS,
	IOC_NR_SET_ADC_HP_FILTER,
	IOC_NR_SET_I2S1_DATAWIDTH,

	/* soft mute */
	IOC_NR_DAC_SOFT_MUTE,
	IOC_NR_DAC_SOFT_UNMUTE,
	IOC_NR_DAC_SOFT_MUTE_RATE,
	IOC_CODEC_AO_CONFIG,
	IOC_CODEC_REG_DUMP,
	IOC_ACODEC_VOLUME_SET,
	IOC_ACODEC_VOLUME_GET
} ot_acodec_ioc;

typedef struct {
	ot_u32 volume;              /* volume control, 0x00~0x7e, 0x7F:mute */
	ot_u32 volume_ctrl_mute;    /* dac mute control, 1:mute, 0:unmute */
} ot_acodec_volume_ctrl;

typedef struct {
	ot_eis_ao_attr ao_attr;
	ot_acodec_volume_ctrl volume_ctrl;
} ot_ao_codec_attr;

/* reset the audio code to the default config */
#define OT_ACODEC_SOFT_RESET_CTRL \
	_IO(IOC_TYPE_ACODEC, IOC_NR_SOFT_RESET_CTRL)

/* set I2S1 sample rate */
#define OT_ACODEC_SET_I2S1_FS \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_I2S1_FS, ot_u32)

/* select the micpga's input, micin linein, or differential input(ot_acodec_mixer) */
#define OT_ACODEC_SET_MIXER_MIC \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_MIXER_MIC, ot_u32)

/* input/output volume control(both channel) */
#define OT_ACODEC_SET_INPUT_VOLUME \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_INPUT_VOL, ot_u32)
#define OT_ACODEC_SET_OUTPUT_VOLUME \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_OUTPUT_VOL, ot_u32)
#define OT_ACODEC_GET_INPUT_VOLUME \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_GET_INPUT_VOL, ot_u32)
#define OT_ACODEC_GET_OUTPUT_VOLUME \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_GET_OUTPUT_VOL, ot_u32)

/* analog part input boost control(left channel), 1:on, 0:off */
#define OT_ACODEC_ENABLE_BOOSTL \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_BOOSTL_ENABLE, ot_u32)
/* analog part input boost control(right channel), 1:on, 0:off */
#define OT_ACODEC_ENABLE_BOOSTR \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_BOOSTR_ENABLE, ot_u32)
/* analog part input gain control(left channel 0~0x1f) */
#define OT_ACODEC_SET_GAIN_MICL \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_GAIN_MICL, ot_u32)
/* analog part input gain control(right channel 0~0x1f) */
#define OT_ACODEC_SET_GAIN_MICR \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_GAIN_MICR, ot_u32)
/* get analog part input gain(left channel) */
#define OT_ACODEC_GET_GAIN_MICL \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_GET_GAIN_MICL, ot_u32)
/* get analog part input gain(right channel) */
#define OT_ACODEC_GET_GAIN_MICR \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_GET_GAIN_MICR, ot_u32)

/* DAC volume control(left channel) ot_acodec_volume_ctrl */
#define OT_ACODEC_SET_DACL_VOLUME \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_DACL_VOL, ot_acodec_volume_ctrl)
/* DAC volume control(right channel) ot_acodec_volume_ctrl */
#define OT_ACODEC_SET_DACR_VOLUME \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_DACR_VOL, ot_acodec_volume_ctrl)
/* ADC volume control(left channel) ot_acodec_volume_ctrl */
#define OT_ACODEC_SET_ADCL_VOLUME \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_ADCL_VOL, ot_acodec_volume_ctrl)
/* ADC volume control(right channel) ot_acodec_volume_ctrl */
#define OT_ACODEC_SET_ADCR_VOLUME \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_ADCR_VOL, ot_acodec_volume_ctrl)

/* get DAC volume(left channel) */
#define OT_ACODEC_GET_DACL_VOLUME \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_GET_DACL_VOL, ot_acodec_volume_ctrl)
/* get DAC volume(right channel) */
#define OT_ACODEC_GET_DACR_VOLUME \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_GET_DACR_VOL, ot_acodec_volume_ctrl)
/* get ADC volume(left channel) */
#define OT_ACODEC_GET_ADCL_VOLUME \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_GET_ADCL_VOL, ot_acodec_volume_ctrl)
/* get ADC volume(right channel) */
#define OT_ACODEC_GET_ADCR_VOLUME \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_GET_ADCR_VOL, ot_acodec_volume_ctrl)

/* Input mute control(left channel), 1:mute, 0:unmute */
#define OT_ACODEC_SET_MICL_MUTE \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_MICL_MUTE, ot_u32)
/* Input mute control(right channel), 1:mute, 0:unmute */
#define OT_ACODEC_SET_MICR_MUTE \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_MICR_MUTE, ot_u32)
/* Output mute control(left channel), 1:mute, 0:unmute */
#define OT_ACODEC_SET_DACL_MUTE \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_DACL_MUTE, ot_u32)
/* Output mute control(right channel), 1:mute, 0:unmute */
#define OT_ACODEC_SET_DACR_MUTE \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_DACR_MUTE, ot_u32)

/* set dacl power, 0: power up, 1: power down */
#define OT_ACODEC_SET_PD_DACL \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_PD_DACL, ot_u32)
/* set dacr power, 0: power up, 1: power down */
#define OT_ACODEC_SET_PD_DACR \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_PD_DACR, ot_u32)
/* set adcl power, 0: power up, 1: power down */
#define OT_ACODEC_SET_PD_ADCL \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_PD_ADCL, ot_u32)
/* set adcr power, 0: power up, 1: power down */
#define OT_ACODEC_SET_PD_ADCR \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_PD_ADCR, ot_u32)
/* set analog part input power, 0: power up, 1: power down */
#define OT_ACODEC_SET_PD_LINEINL \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_PD_LINEINL, ot_u32)
/* set analog part input power, 0: power up, 1: power down */
#define OT_ACODEC_SET_PD_LINEINR \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_PD_LINEINR, ot_u32)

/* clock of dac and adc is reverse or obverse */
#define OT_ACODEC_SELECT_DAC_CLK \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_SEL_DAC_CLK, ot_u32)
#define OT_ACODEC_SELECT_ADC_CLK \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_SEL_ADC_CLK, ot_u32)

/* clock of analog part and digital part is reverse or obverse */
#define OT_ACODEC_SELECT_ANA_MCLK \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_SEL_ANA_MCLK, ot_u32)

/* ADC/DAC track control */
#define OT_ACODEC_DACL_SELECT_TRACK \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_DACL_SEL_TRACK, ot_u32)
#define OT_ACODEC_DACR_SELECT_TRACK \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_DACR_SEL_TRACK, ot_u32)
#define OT_ACODEC_ADCL_SELECT_TRACK \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_ADCL_SEL_TRACK, ot_u32)
#define OT_ACODEC_ADCR_SELECT_TRACK \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_ADCR_SEL_TRACK, ot_u32)

#define OT_ACODEC_SET_DAC_DE_EMPHASIS \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_DAC_DE_EMPHASIS, ot_u32)
#define OT_ACODEC_SET_ADC_HP_FILTER \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_ADC_HP_FILTER, ot_u32)
#define OT_ACODEC_SET_I2S1_DATAWIDTH \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_SET_I2S1_DATAWIDTH, ot_u32)

/* DAC soft mute control, 1:enable, 0:disable */
#define OT_ACODEC_DAC_SOFT_MUTE \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_DAC_SOFT_MUTE, ot_u32)
/* DAC soft unmute control, 1:enable, 0:disable */
#define OT_ACODEC_DAC_SOFT_UNMUTE \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_DAC_SOFT_UNMUTE, ot_u32)
/* set DAC soft mute rate, 0:fs/2, 1:fs/8, 2:fs/32, 3:fs/64 */
#define OT_ACODEC_DAC_SOFT_MUTE_RATE \
	_IOWR(IOC_TYPE_ACODEC, IOC_NR_DAC_SOFT_MUTE_RATE, ot_u32)

#define OT_ACODEC_AO_CONFIG \
	_IOWR(IOC_TYPE_ACODEC, IOC_CODEC_AO_CONFIG, ot_u32)

#define OT_ACODEC_REG_DUMP \
	_IOWR(IOC_TYPE_ACODEC, IOC_CODEC_REG_DUMP, ot_u32)

#define OT_ACODEC_VOLUME_SET \
	_IOW(IOC_TYPE_ACODEC, IOC_ACODEC_VOLUME_SET, ot_acodec_volume_ctrl)

#define OT_ACODEC_VOLUME_GET \
	_IOWR(IOC_TYPE_ACODEC, IOC_ACODEC_VOLUME_GET, ot_acodec_volume_ctrl)
#endif
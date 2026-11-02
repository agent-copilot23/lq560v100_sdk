ifeq ($(MAIN_CFG), )
    MAIN_CFG := $(MAIN_DIR)/build/main_cfg.mak
    include $(MAIN_CFG)
endif

################# select sensor type for your sample ###############################
####    OV_OS08A20_MIPI_8M_30FPS_12BIT             #################
####    OV_OS08A20_MIPI_8M_30FPS_10BIT_WDR2TO1     #################
####    OV_OX08B40_MIPI_8M_30FPS_12BIT             #################
####    OV_OX08B40_MIPI_8M_30FPS_12BIT_DIRECT      #################
####    OV_OS04A10_MIPI_4M_30FPS_12BIT             #################
####    OV_OS04A10_MIPI_4M_30FPS_12BIT_WDR2TO1     #################
####    OV_OX03F10_MIPI_3M_60FPS_12BIT             #################
####    OV_OX03F10_MIPI_3M_60FPS_16BIT             #################
####    OV_OX05B1S_MIPI_5M_60FPS_10BIT             #################
####    OV_OX03C_MIPI_3M_60FPS_16BIT               #################
####    SONY_IMX623_MIPI_3M_60FPS_16BIT            #################
####    SONY_IMX290_MIPI_2M_60FPS_10BIT_WDR2TO1    #################
####    SONY_IMX290_MIPI_2M_30FPS_12BIT_WDR2TO1    #################
####    SONY_IMX347_SLAVE_MIPI_4M_30FPS_12BIT      #################
####    GC_GC2093_MIPI_2M_30FPS_10BIT              #################
####    GC_GC2093_MIPI_2M_30FPS_10BIT_WDR2TO1      #################
####    SMS_SC220AT_MIPI_2M_30FPS_YUV422           #################
####    SMS_SC220AT_MIPI_2M_30FPS_RAW12            #################

# SENSOR0_TYPE ?= OV_OS08A20_MIPI_8M_30FPS_12BIT
# SENSOR1_TYPE ?= OV_OS08A20_MIPI_8M_30FPS_12BIT
# SENSOR2_TYPE ?= OV_OS08A20_MIPI_8M_30FPS_12BIT
# SENSOR3_TYPE ?= OV_OS08A20_MIPI_8M_30FPS_12BIT
SENSOR0_TYPE ?= SONY_IMX623_MIPI_3M_60FPS_16BIT
SENSOR1_TYPE ?= SONY_IMX623_MIPI_3M_60FPS_16BIT
SENSOR2_TYPE ?= SONY_IMX623_MIPI_3M_60FPS_16BIT
SENSOR3_TYPE ?= SONY_IMX623_MIPI_3M_60FPS_16BIT

SYS_LIBS := $(REL_LIB)/libot_media_common.a
SYS_LIBS += $(REL_LIB)/libot_media_sys.a
SYS_LIBS += $(REL_LIB)/libot_media_bind.a
SYS_LIBS += $(REL_LIB)/libot_smr.a
SYS_LIBS += $(REL_LIB)/libot_buffer_pool.a
SYS_LIBS += $(REL_LIB)/libot_vrb.a
SYS_LIBS += $(REL_LIB)/libot_avp_dpu_impl.a
SYS_LIBS += $(REL_LIB)/libot_pipline.a
SYS_LIBS += $(REL_LIB)/libot_utils.a
SYS_LIBS += $(REL_LIB)/libcjson.a
SYS_LIBS += $(REL_LIB)/libot_otp.a
SYS_LIBS += $(REL_LIB)/libot_board_info.a

VI_LIBS += $(REL_LIB)/libot_ae.a
VI_LIBS += $(REL_LIB)/libot_awb.a
VI_LIBS += $(REL_LIB)/libot_isp.a
VI_LIBS += $(REL_LIB)/libot_isp_adapt.a
VI_LIBS += $(REL_LIB)/libot_dehaze.a
VI_LIBS += $(REL_LIB)/libot_stats_estimate.a
VI_LIBS += $(REL_LIB)/libot_drc.a
VI_LIBS += $(REL_LIB)/libot_ldci.a
VI_LIBS += $(REL_LIB)/libot_bnr.a
VI_LIBS += $(REL_LIB)/libot_acs.a

VI_LIBS += $(REL_LIB)/libsns_ox08b40.a
VI_LIBS += $(REL_LIB)/libsns_ox03f10.a
VI_LIBS += $(REL_LIB)/libsns_ox03c.a
VI_LIBS += $(REL_LIB)/libsns_ox05b1s.a
VI_LIBS += $(REL_LIB)/libsns_imx623.a
VI_LIBS += $(REL_LIB)/libsns_imx290.a
VI_LIBS += $(REL_LIB)/libsns_gc2093.a
VI_LIBS += $(REL_LIB)/libsns_sc220at.a
VI_LIBS += $(REL_LIB)/libsns_imx582.a

DEP_LIBS += $(REL_LIB)/libot_vi_serdes.a
DEP_LIBS += $(REL_LIB)/libot_vi_serdes_drv.a
DEP_LIBS += $(REL_LIB)/libot_vo_serdes.a
DEP_LIBS += $(REL_LIB)/libot_vo_serdes_drv.a
DEP_LIBS += $(REL_LIB)/libot_serdes_common.a

VPROC_LIBS := $(REL_LIB)/libot_vproc.a

AO_LIBS := $(REL_LIB)/libot_ao.a
AO_LIBS += $(REL_LIB)/libot_ao_post.a
AO_LIBS += $(REL_LIB)/libot_ao_pre.a
AO_LIBS += $(REL_LIB)/libot_ao_base.a
AO_LIBS += $(REL_LIB)/libot_ao_engine.a

VENC_LIBS := $(REL_LIB)/libot_venc.a
VDEC_LIBS := $(REL_LIB)/libot_vdec.a

OGE_LIBS += $(REL_LIB)/libot_oge.a
OGE_LIBS += $(REL_LIB)/libot_gfb.a

COMMON_DIR ?= $(PWD)/../common

SAMPLE_COMMON_LIB := $(COMMON_DIR)/libsample_common.a
MIPI_RX_LIBS += $(REL_LIB)/libot_vi_intf.a
MIPI_TX_LIBS += $(REL_LIB)/libot_vo_intf.a

INC_FLAGS := -I$(REL_INC)
INC_FLAGS += -I$(SDK_PATH)/$(EXTDRV)/lt8618sx
INC_FLAGS += -I$(MAIN_COMPONENT_DIR)/driver/mipi-rx/src/include
INC_FLAGS += -I$(MAIN_COMPONENT_DIR)/media/common/src/include
INC_FLAGS += $(COMMON_INCLUDE)

ISP_VERSION := ISP_V2

CFLAGS += -Wall $(INC_FLAGS) -D$(OT_ARCH) -D$(OT_FPGA) -D$(ISP_VERSION)
CFLAGS += -lstdc++
CFLAGS += -lpthread -lm -ldl
CFLAGS += $(LIBS_CFLAGS)
CFLAGS += $(LIBS_EXEC_CFLAGS)
CFLAGS += $(OMP_CFLAGS)
CFLAGS += -DSENSOR0_TYPE=$(SENSOR0_TYPE)
CFLAGS += -DSENSOR1_TYPE=$(SENSOR1_TYPE)
CFLAGS += -DSENSOR2_TYPE=$(SENSOR2_TYPE)
CFLAGS += -DSENSOR3_TYPE=$(SENSOR3_TYPE)

ifeq ($(MEM_SHARE), y)
	CFLAGS += -DSAMPLE_MEM_SHARE_ENABLE
endif

COMM_INC := -I$(MAIN_DIR)/src/component/media/sample/common/

################ select audio codec type for your sample ################
#external acodec
#ACODEC_TYPE ?= ACODEC_TYPE_ES8388
ACODEC_TYPE ?= ACODEC_TYPE_INNER

ifeq ($(ACODEC_TYPE), ACODEC_TYPE_ES8388)
	CFLAGS += -DOT_ACODEC_TYPE_ES8388
endif


ifeq ($(ACODEC_TYPE), ACODEC_TYPE_INNER)
	CFLAGS += -DOT_ACODEC_TYPE_INNER
endif

################ select audio module library type for your sample ################
AUDIO_MODULE_LIB_TYPE ?= AUDIO_LIB_TYPE_STATIC
#AUDIO_MODULE_LIB_TYPE ?= AUDIO_LIB_TYPE_SHARE

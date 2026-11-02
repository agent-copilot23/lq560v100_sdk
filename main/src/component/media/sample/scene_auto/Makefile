CUR_DIR=$(shell pwd)
MAIN_DIR = $(CUR_DIR)/../../../../../
include $(CUR_DIR)/../sample_cfg.mak


INC_FLAGS +=-I./include
INC_FLAGS +=-I./../scene
INC_FLAGS +=-I./tools/configaccess/include
INC_FLAGS +=-I./../common
INC_FLAGS +=-I$(REL_DIR)/include
INC_FLAGS +=-I./src/core

INC_FLAGS +=-I../../vi/src/isp/include
INC_FLAGS +=-I../../vi/src/isp/include/arch/lq560v100plus/include

INC_FLAGS +=-I./include
INC_FLAGS +=-I$(REL_DIR)/include

TARGET_PATH ?= $(PWD)

CFLAGS += $(INC_FLAGS)
CFLAGS += -fPIC -no-pie

SYS_LIBS += $(REL_LIB)/libprotobuf-c.a
SYS_LIBS += $(REL_LIB)/libot_smr.a
SYS_LIBS += $(REL_LIB)/libot_km.a

NPU_LIBS := $(REL_LIB)/libot_avp_npu_impl.a
NPU_LIBS += $(REL_LIB)/libot_avp_npu_intf.a
NPU_LIBS += $(REL_LIB)/libot_avp_npu.a
DEP_LIBS += $(NPU_LIBS)
SYS_LIBS += $(REL_LIB)/libot_otp.a
SYS_LIBS += $(REL_LIB)/libot_cipher.a
#SYS_LIBS += $(REL_LIB)/libot_cipher_common.a
DEP_LIBS += $(SYS_LIBS)
DEP_LIBS += $(VI_LIBS)
DEP_LIBS += $(VENC_LIBS)
DEP_LIBS += $(VPROC_LIBS)
DEP_LIBS += $(REL_LIB)/libot_vo.a
DEP_LIBS += $(SAMPLE_COMMON_LIB)
DEP_LIBS += $(MIPI_RX_LIBS)
DEP_LIBS += $(MIPI_TX_LIBS)
DEP_LIBS += $(OGE_LIBS)
DEP_LIBS += $(REL_LIB)/libot_proc.a
DEP_LIBS += $(REL_LIB)/libot_scene_auto.a
DEP_LIBS += $(REL_LIB)/libot_mce_det.a
DEP_LIBS += $(REL_LIB)/libot_haze_level.a
DEP_LIBS += $(REL_LIB)/libot_abnr.a
DEP_LIBS += $(REL_LIB)/libot_iqt.a
DEP_LIBS += $(REL_LIB)/libot_deflicker.a

# CFLAGS   += -pg

ifeq ($(CONFIG_AIISP_SUPPORT), y)
    DEP_LIBS += $(AIISP_LIBS)
    DEP_LIBS += $(REL_LIB)/libot_avp_npu_impl.a
    DEP_LIBS += $(REL_LIB)/libot_km.a
    DEP_LIBS += $(REL_LIB)/libprotobuf-c.a
    CFLAGS += -D CONFIG_AIISP_SUPPORT
endif

LIB_DIR :=$(CUR_DIR)/tools/configaccess/src\
		  $(CUR_DIR)/src/core
LIB_C_SRCS := $(foreach file,$(subst -S, ,$(LIB_DIR)), $(notdir $(wildcard $(file)/*.c)))

SMP_DIR := $(CUR_DIR)/src/sample
SMP_C_SRCS := $(foreach file,$(subst -S, ,$(SMP_DIR)), $(notdir $(wildcard $(file)/*.c)))

OBJ_PATH = ./obj
LIB_OBJ_PATH = ./$(OBJ_PATH)/lib
SMP_OBJ_PATH = ./$(OBJ_PATH)/smp
LIB_OBJS  := $(LIB_C_SRCS:%.c=$(LIB_OBJ_PATH)/%.o)
SMP_OBJS  := $(SMP_C_SRCS:%.c=$(SMP_OBJ_PATH)/%.o)

LIB_PATH = ./$(OBJ_PATH)/libs
LIB_NAME := libot_scene_core
LIB_A := $(LIB_PATH)/$(LIB_NAME).a
LIB_S := $(LIB_PATH)/$(LIB_NAME).so
LIB_ABS_PATH := $(TARGET_PATH)/$(LIB_PATH)

# BUILD_CMD := -lot_scene_auto -L$(LIB_ABS_PATH) # use shared lib

BUILD_CMD :=  # use static lib
DEP_LIBS += $(TARGET_PATH)/$(LIB_A)

VPATH = $(subst -S, ,$(LIB_DIR) $(SMP_DIR))
COMPILE = $(CC) $(CFLAGS)

CROSS_COMPILE_PREFIX = aarch64-otv02-linux-gnu-
CROSS_CC = $(CROSS_COMPILE_PREFIX)gcc

.PHONY : clean all

$(LIB_OBJ_PATH)/%.o : %.c
	@(echo "compiling $< ...")
	@[ -e $(LIB_OBJ_PATH) ] || mkdir -p $(LIB_OBJ_PATH)
	@($(COMPILE) -o $@ -c $<)

$(SMP_OBJ_PATH)/%.o : %.c
	@(echo "compiling $< ...")
	@[ -e $(SMP_OBJ_PATH) ] || mkdir -p $(SMP_OBJ_PATH)
	@($(COMPILE) -o $@ -c $<)


TARGET_SMP := sample_scene
all:$(TARGET_SMP)

$(TARGET_SMP): $(SMP_OBJS) $(LIB_OBJS)
	@[ -e $(TARGET_PATH)/$(LIB_PATH) ] || mkdir -p $(TARGET_PATH)/$(LIB_PATH)
	@($(AR) -rcv $(TARGET_PATH)/$(LIB_A) $(LIB_OBJS)) 
	@($(CC) $(LIBS_LD_CFLAGS) -shared -fPIC -o $(TARGET_PATH)/$(LIB_S) $(LIB_OBJS))
	$(CROSS_CC) $(CFLAGS) -lpthread -lm ${BUILD_CMD} -o $(TARGET_PATH)/$@ $(SMP_OBJS)  -Wl,--start-group $(DEP_LIBS)  $(REL_LIB)/libsecurec.a  -Wl,--end-group

clean:
	@rm -f $(TARGET_PATH)/$(TARGET_SMP)
	@rm -rf $(OBJ_PATH)

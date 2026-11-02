CUR_DIR=$(shell pwd)
MAIN_DIR = $(CUR_DIR)/../../../../../
include $(MAIN_DIR)/build/main_cfg.mak



########### SRCS & INC ############
CUR_SRCS := $(wildcard $(PWD)/*.c)

CFLAGS += -I$(REL_INC)
CFLAGS += -I$(PWD)/../../../include
CFLAGS += -I$(PWD)/..
DEP_LIBS += $(REL_LIB)/libot_semu.a
DEP_LIBS += $(MAIN_DIR)/out/lib/libot_vo.a
DEP_LIBS += $(MAIN_DIR)/out/lib/libot_vo_intf.a
DEP_LIBS += $(MAIN_DIR)/out/lib/libot_vo_intf.a
DEP_LIBS += $(MAIN_DIR)/out/lib/libot_media_common.a
DEP_LIBS += $(MAIN_DIR)/out/lib/libot_utils.a
DEP_LIBS += $(MAIN_DIR)/out/lib/libot_smr.a

TARGET := sample_vo_crc

TARGET_PATH := $(PWD)

CFLAGS += -fstack-protector-strong
CFLAGS += $(LIBS_CFLAGS)
LD_FLAGS += $(LIBS_LD_CFLAGS)

# compile linux
include $(MAIN_DIR)/build/sample_template.mak

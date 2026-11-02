CUR_DIR=$(shell pwd)
MAIN_DIR = $(CUR_DIR)/../../../..
include $(MAIN_DIR)/build/main_cfg.mak



########### SRCS & INC ############
CUR_SRCS := $(wildcard $(PWD)/*.c)

CFLAGS += -I$(REL_INC)
CFLAGS += -I$(PWD)/../../include
DEP_LIBS += $(REL_LIB)/libot_semu.a

TARGET := sample_semu

TARGET_PATH := $(PWD)

CFLAGS += -fstack-protector-strong
CFLAGS += $(LIBS_CFLAGS)
LD_FLAGS += $(LIBS_LD_CFLAGS)

# compile linux
include $(MAIN_DIR)/build/sample_template.mak

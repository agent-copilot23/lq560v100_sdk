CUR_DIR=$(shell pwd)
MAIN_DIR = $(CUR_DIR)/../../../../
include $(MAIN_DIR)/build/main_cfg.mak



########### SRCS & INC ############
CUR_SRCS := $(wildcard $(PWD)/*.c)

CFLAGS += -I$(REL_INC)
DEP_LIBS += $(REL_LIB)/libot_otp.a
DEP_LIBS += $(REL_LIB)/libot_cipher.a
DEP_LIBS += $(REL_LIB)/libot_km.a
DEP_LIBS += $(REL_LIB)/libot_smr.a

TARGET := sample_cipher

TARGET_PATH := $(PWD)

CFLAGS += -fstack-protector-strong
CFLAGS += $(LIBS_CFLAGS)
LD_FLAGS += $(LIBS_LD_CFLAGS)

# compile linux
include $(MAIN_DIR)/build/sample_template.mak

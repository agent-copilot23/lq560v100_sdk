CUR_DIR=$(shell pwd)
MAIN_DIR = $(CUR_DIR)/../../../../
include $(MAIN_DIR)/build/main_cfg.mak

########### SRCS & INC ############
CUR_SRCS := $(wildcard $(PWD)/*.c)

DEP_LIBS += $(REL_LIB)/libot_board_info.a
DEP_LIBS += $(REL_LIB)/libot_otp.a

CFLAGS += -I$(REL_INC)
CFLAGS += -I$(CUR_DIR)/../include
CFLAGS += -I$(CUR_DIR)/../src/common/include
CFLAGS += -I$(CUR_DIR)/../security/include

TARGET := sample_board_info

TARGET_PATH := $(PWD)

CFLAGS += -fstack-protector-strong
CFLAGS += $(LIBS_CFLAGS)
LD_FLAGS += $(LIBS_LD_CFLAGS)

include $(MAIN_DIR)/build/sample_template.mak

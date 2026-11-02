CUR_DIR = $(shell pwd)

MAIN_DIR = $(CUR_DIR)/../../../../../
ifeq ($(MAIN_CFG), )
    MAIN_CFG := $(MAIN_DIR)/build/main_cfg.mak
    include $(MAIN_CFG)
endif

CUR_SRCS := $(wildcard $(PWD)/*.c)

CFLAGS := -I$(CUR_DIR)
CFLAGS += -I$(CUR_DIR)/../include/
CFLAGS += -I$(CUR_DIR)/../../../../kal/include/
CFLAGS += -fstack-protector-all    # SP
CFLAGS += -pie -fPIE               # PIE
CFLAGS += -s                       # STRIP
LIB_CFLAGS += -Wl,-z,relro,-z,now  # BIND NOW

TARGET := sample_wtdg

TARGET_PATH := $(PWD)


include $(MAIN_DIR)/build/sample_template.mak
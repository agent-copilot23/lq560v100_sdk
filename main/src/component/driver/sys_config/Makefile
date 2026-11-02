CUR_DIR = $(shell pwd)

MAIN_DIR = $(CUR_DIR)/../../../../
ifeq ($(MAIN_CFG), )
    MAIN_CFG := $(MAIN_DIR)/build/main_cfg.mak
    include $(MAIN_CFG)
endif


TARGET := sys_config
#*************************************************************************
# all source file in this module
SRCS += src/sys_cfg.c
SRCS += src/clk_cfg.c
SRCS += src/sys_ctrl.c
SRCS += src/pin_mux.c
#*************************************************************************
src = $(PWD)
EXTRA_CFLAGS += -I$(src)
EXTRA_CFLAGS += -D$(OT_FPGA)

include $(MAKE_DRV_FILE)

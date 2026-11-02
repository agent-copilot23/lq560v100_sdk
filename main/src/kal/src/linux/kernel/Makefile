CUR_DIR = $(shell pwd)

ifeq ($(MAIN_CFG), )
    MAIN_DIR = $(CUR_DIR)/../../../../../
    MAIN_CFG := $(MAIN_DIR)/build/main_cfg.mak
    include $(MAIN_CFG)
endif

src = $(PWD)
EXTRA_CFLAGS := -I$(src)
EXTRA_CFLAGS += -I$(src)/media
EXTRA_CFLAGS += -I$(src)/../../../include
EXTRA_CFLAGS += -I$(src)/../../../include/module
EXTRA_CFLAGS += -I$(SECUREC_ROOT)/include
ifeq ($(CONFIG_RELEASE_TYPE_DEBUG), y)
EXTRA_CFLAGS += -Wno-error=format-nonliteral
EXTRA_CFLAGS += -D$(OT_RLS_MODE)
endif

# osal_xxx
SRCS = $(notdir $(wildcard $(src)/*.c))
# media
SRCS += ./media/base.c
SRCS += ./media/ot_media.c

ifneq ($(CONFIG_VENDOR_SEMU), y)
SRCS := $(filter-out osal_interrupt_report.c, $(SRCS))
endif

INC_FILE := $(wildcard $(src)/../../../include/*)
TARGET   := $(KO_PREFIX)_osal

include $(MAKE_DRV_FILE)

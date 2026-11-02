CUR_DIR = $(shell pwd)
MAIN_DIR = $(CUR_DIR)/../../../
ifeq ($(MAIN_CFG), )
    MAIN_CFG := $(MAIN_DIR)/build/main_cfg.mak
    include $(MAIN_CFG)
endif

RELEASE_HEADERS =
RELEASE_KOS =
RELEASE_LIBS =

# impl
RELEASE_LIBS = $(wildcard $(CUR_DIR)/lib/*.a)

include $(MAIN_DIR)/build/release_template.mk

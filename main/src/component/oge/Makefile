CUR_DIR = $(shell pwd)
MAIN_DIR = $(CUR_DIR)/../../../
ifeq ($(MAIN_CFG), )
    MAIN_CFG := $(MAIN_DIR)/build/main_cfg.mak
    include $(MAIN_CFG)
endif


# impl
RELEASE_HEADERS = $(wildcard $(CUR_DIR)/include/*.h)
RELEASE_KOS = $(wildcard $(CUR_DIR)/ko/*.ko)
RELEASE_LIBS = $(wildcard $(CUR_DIR)/libs/*.a)
RELEASE_LIBS += $(wildcard $(CUR_DIR)/libs/*.so)

include $(MAIN_DIR)/build/release_template.mk

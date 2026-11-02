CUR_DIR = $(shell pwd)

ifeq ($(MAIN_CFG), )
    MAIN_DIR = $(CUR_DIR)/../../
    MAIN_CFG := $(MAIN_DIR)/build/main_cfg.mak
    include $(MAIN_CFG)
endif

SRC_PATH=$(CUR_DIR)/src/linux/kernel

MAIN_COMMON_FILES = $(notdir $(wildcard $(CUR_DIR)/include/*))

.PHONY:kal kal_clean

all: kal

clean: kal_clean

kal:
	@cd $(SRC_PATH);  $(MAKE)

kal_clean:
	@cd $(SRC_PATH); $(MAKE) clean


CUR_DIR = $(shell pwd)

ifeq ($(MAIN_CFG), )
    MAIN_DIR = $(CUR_DIR)/../../
    MAIN_CFG := $(MAIN_DIR)/build/main_cfg.mak
    include $(MAIN_CFG)
endif

TARGET = $(filter-out smr driver media security upgrade board_info, $(shell ls -d */ | sed "s;/;;g"))
TARGET_CLEAN = $(addsuffix _clean,$(TARGET))

DRV_CFLAGS += -I$(MAIN_DIR)/src/component/smr/src/include

.PHONY: $(TARGET) $(TARGET_CLEAN) smr smr_clean driver driver_clean media media_clean security security_clean upgrade upgrade_clean board_info board_info_clean

all: smr driver $(TARGET) media security upgrade board_info
	@echo "~~~~~~~~~~~~~~Build All Component SUCCESS in $(CUR_DIR)~~~~~~~~~~~~~~"

clean: media_clean $(TARGET_CLEAN) smr_clean driver_clean security_clean upgrade_clean board_info_clean
	@echo "~~~~~~~~~~~~~~Clean Component SUCCESS in $(CUR_DIR)~~~~~~~~~~~~~~"

smr:
	@echo "~~~~~~~~~~Start build $@~~~~~~~~~~"
	@cd $@ && $(MAKE)

security: smr
	@echo "~~~~~~~~~~start build $@~~~~~~~~~~"
	@cd $@ && $(MAKE)

upgrade: security
	@echo "~~~~~~~~~~start build $@~~~~~~~~~~"
	@cd $@ && $(MAKE)

media: smr
	@echo "~~~~~~~~~~start build $@~~~~~~~~~~"
	@cd $@ && $(MAKE)

driver $(TARGET):
	@echo "~~~~~~~~~~Start build $@~~~~~~~~~~"
	@cd $@ && $(MAKE)

board_info: security
	@echo "~~~~~~~~~~Start build $@~~~~~~~~~~"
	@cd $@ && $(MAKE)

smr_clean driver_clean $(TARGET_CLEAN) media_clean security_clean upgrade_clean board_info_clean:
	@echo "~~~~~~~~~~Start clean $(subst _clean,,$@)~~~~~~~~~~"
	@cd $(subst _clean,,$@) && $(MAKE) clean

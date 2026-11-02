CUR_DIR = $(shell pwd)

MAIN_DIR = $(CUR_DIR)/../../../
ifeq ($(MAIN_CFG), )
    MAIN_CFG := $(MAIN_DIR)/build/main_cfg.mak
    include $(MAIN_CFG)
endif

TARGET = $(filter-out common, $(shell ls -d */ | sed "s;/;;g"))
TARGET_CLEAN = $(addsuffix _clean,$(TARGET))

.PHONY: $(TARGET) $(TARGET_CLEAN) common

all: $(TARGET)
	@echo "~~~~~~~~~~~~~~Build All SUCCESS in $(CUR_DIR)~~~~~~~~~~~~~~"

clean: $(TARGET_CLEAN) common_clean
	@echo "~~~~~~~~~~~~~~Clean SUCCESS in $(CUR_DIR)~~~~~~~~~~~~~~"

$(TARGET): common
	@echo "~~~~~~~~~~Start build $@~~~~~~~~~~"
	@cd $@ && $(MAKE)

common:
	@echo "~~~~~~~~~~Start build $@~~~~~~~~~~"
	@cd $@ && $(MAKE)

$(TARGET_CLEAN):
	@echo "~~~~~~~~~~Start clean $(subst _clean,,$@)~~~~~~~~~~"
	@cd $(subst _clean,,$@) && $(MAKE) clean

common_clean:
	@echo "~~~~~~~~~~Start clean $(subst _clean,,$@)~~~~~~~~~~"
	@cd $(subst _clean,,$@) && $(MAKE) clean

CUR_DIR = $(shell pwd)

ifeq ($(MAIN_CFG), )
    MAIN_DIR = $(CUR_DIR)/../../../
    MAIN_CFG := $(MAIN_DIR)/build/main_cfg.mak
    include $(MAIN_CFG)
endif

TARGET = $(filter-out common sample sys, $(shell ls -d */ | sed "s;/;;g"))
$(warning echo "target: $(TARGET)")

TARGET_CLEAN = $(addsuffix _clean,$(TARGET))

.PHONY:clean all common common_clean sys sys_clean sample sample_clean $(TARGET) $(TARGET_CLEAN)

all: common sys $(TARGET) sample
	@echo "~~~~~~~~~~~~~~Build All Media Mods SUCCESS~~~~~~~~~~~~~~"

clean: sample_clean $(TARGET_CLEAN) sys_clean common_clean
	@echo "~~~~~~~~~~~~~~Clean Media Mods SUCCESS~~~~~~~~~~~~~~"

common sys:
	@echo "~~~~~~~~~~Start build $@~~~~~~~~~~"
	@cd $@ && $(MAKE)

$(TARGET): common sys
	@echo "~~~~~~~~~~Start build $@~~~~~~~~~~"
	@cd $@ && $(MAKE)

sample: $(TARGET)
	@echo "~~~~~~~~~~Start build $@~~~~~~~~~~"
	@cd $@ && $(MAKE)

sample_clean $(TARGET_CLEAN) sys_clean common_clean:
	@echo "~~~~~~~~~~Start clean $(subst _clean,,$@)~~~~~~~~~~"
	@cd $(subst _clean,,$@) && $(MAKE) clean

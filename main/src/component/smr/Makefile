CUR_DIR = $(shell pwd)

ifeq ($(MAIN_CFG), )
    MAIN_DIR = $(CUR_DIR)/../../..
    MAIN_CFG := $(MAIN_DIR)/build/main_cfg.mak
    include $(MAIN_CFG)
endif

TARGET = $(filter-out include obj libs ko, $(shell ls -d */ | sed "s;/;;g"))
TARGET_CLEAN = $(addsuffix _clean,$(TARGET))

.PHONY: $(TARGET) $(TARGET_CLEAN)

all: $(TARGET)
	@echo "~~~~~~~~~~~~~~Build All Component SUCCESS~~~~~~~~~~~~~~"

clean: $(TARGET_CLEAN) 
	@echo "~~~~~~~~~~~~~~Clean Component SUCCESS~~~~~~~~~~~~~~"

$(TARGET):
	@echo "~~~~~~~~~~Start build $@~~~~~~~~~~"
	@cd $@ && $(MAKE)

$(TARGET_CLEAN):
	@echo "~~~~~~~~~~Start clean $(subst _clean,,$@)~~~~~~~~~~"
	@cd $(subst _clean,,$@) && $(MAKE) clean

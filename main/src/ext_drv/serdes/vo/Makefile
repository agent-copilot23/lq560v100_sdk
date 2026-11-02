CUR_DIR = $(shell pwd)

MAIN_DIR = $(CUR_DIR)/../../../../
ifeq ($(MAIN_CFG), )
    MAIN_CFG := $(MAIN_DIR)/build/main_cfg.mak
    include $(MAIN_CFG)
endif

TARGET = $(filter-out include libs, $(shell ls -d */ | sed "s;/;;g"))
TARGET_CLEAN = $(addsuffix _clean,$(TARGET))

.PHONY: $(TARGET) $(TARGET_CLEAN)

all: $(TARGET)
	@echo "~~~~~~~~~~~~~~Build All SUCCESS in $(CUR_DIR)~~~~~~~~~~~~~~"

clean: $(TARGET_CLEAN)
	@echo "~~~~~~~~~~~~~~Clean SUCCESS $(CUR_DIR)~~~~~~~~~~~~~~"

$(TARGET):
	@echo "~~~~~~~~~~Start build $@~~~~~~~~~~"
	@cd $@ && $(MAKE)

$(TARGET_CLEAN):
	@echo "~~~~~~~~~~Start clean $(subst _clean,,$@)~~~~~~~~~~"
	@cd $(subst _clean,,$@) && $(MAKE) clean

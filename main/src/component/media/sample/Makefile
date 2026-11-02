CUR_DIR = $(shell pwd)

ifeq ($(MAIN_CFG), )
    MAIN_DIR = $(CUR_DIR)/../../../../
    MAIN_CFG := $(MAIN_DIR)/build/main_cfg.mak
    include $(MAIN_CFG)
endif
JSON_DIR = $(CUR_DIR)/../../../../../opensource/json

TARGET = $(filter-out common, $(shell ls -d */ | sed "s;/;;g"))
$(warning echo "TARGET: $(TARGET)")
TARGET_CLEAN = $(addsuffix _clean,$(TARGET))

.PHONY:clean all common common_clean $(TARGET) $(TARGET_CLEAN) cjson cjson_clean

all: common $(TARGET)
	@echo "~~~~~~~~~~~~~~Build All Sample SUCCESS~~~~~~~~~~~~~~"

clean:$(TARGET_CLEAN) common_clean cjson_clean
	@echo "~~~~~~~~~~~~~~Clean Media Sample SUCCESS~~~~~~~~~~~~~~"

$(TARGET): common
	@echo "~~~~~~~~~~Start build $@~~~~~~~~~~"
	@mkdir -p ${REL_SAMPLE} && cd $@ && $(MAKE)

common:cjson
	@echo "~~~~~~~~~~Start build $@~~~~~~~~~~"
	@mkdir -p ${REL_SAMPLE} && cd $@ && $(MAKE)

$(TARGET_CLEAN) common_clean:
	@echo "~~~~~~~~~~Start clean $(subst _clean,,$@)~~~~~~~~~~"
	@cd $(subst _clean,,$@) && $(MAKE) clean

cjson:
	@echo "~~~~~~~~~~Start build $@~~~~~~~~~~"
	@$(MAKE) -C $(JSON_DIR)

cjson_clean:
	@$(MAKE) -C  $(JSON_DIR)  clean

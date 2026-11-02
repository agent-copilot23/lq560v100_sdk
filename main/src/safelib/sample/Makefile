CUR_DIR = $(shell pwd)

ifeq ($(MAIN_CFG), )
    MAIN_DIR = $(CUR_DIR)/../../../
    MAIN_CFG := $(MAIN_DIR)/build/main_cfg.mak
    include $(MAIN_CFG)
endif

TARGET = $(filter-out include obj libs ko test, $(shell ls -d */ | sed "s;/;;g"))
TARGET_CLEAN = $(addsuffix _clean,$(TARGET))

TARGET_VO = $(filter-out include obj libs ko test, user/vo_crc)
TARGET_VO_CLEAN = $(addsuffix _clean,$(TARGET_VO))

.PHONY: $(TARGET) $(TARGET_CLEAN) $(TARGET_VO) $(TARGET_VO_CLEAN)

all: $(TARGET) $(TARGET_VO)
	@echo "~~~~~~~~~~~~~~Build All SUCCESS in $(CUR_DIR)~~~~~~~~~~~~~~"

clean: $(TARGET_CLEAN) $(TARGET_VO_CLEAN)
	@echo "~~~~~~~~~~~~~~Clean All SUCCESS in $(CUR_DIR)~~~~~~~~~~~~~~"

$(TARGET):
	@echo "~~~~~~~~~~Start build $@~~~~~~~~~~"
	@cd $@ && $(MAKE)

$(TARGET_VO):
	@echo "~~~~~~~~~~Start build $@~~~~~~~~~~"
	@cd $@ && $(MAKE)

$(TARGET_CLEAN):
	@echo "~~~~~~~~~~Start clean $(subst _clean,,$@)~~~~~~~~~~"
	@cd $(subst _clean,,$@) && $(MAKE) clean

$(TARGET_VO_CLEAN):
	@echo "~~~~~~~~~~Start clean $(subst _clean,,$@)~~~~~~~~~~"
	@cd $(subst _clean,,$@) && $(MAKE) clean

CUR_DIR = $(shell pwd)
MAIN_DIR = $(CUR_DIR)/../../../../
ifeq ($(MAIN_CFG), )
    MAIN_CFG := $(MAIN_DIR)/build/main_cfg.mak
    include $(MAIN_CFG)
endif

RELEASE_HEADERS =
RELEASE_KOS =
RELEASE_LIBS =

# impl
RELEASE_HEADERS = $(wildcard $(CUR_DIR)/include/*.h)
RELEASE_KOS = $(wildcard $(CUR_DIR)/ko/*.ko)

include $(MAIN_DIR)/build/release_template.mk


TARGET = $(filter-out include ko, $(shell ls -d */ | sed "s;/;;g"))
TARGET_CLEAN = $(addsuffix _clean,$(TARGET))

.PHONY: $(TARGET) $(TARGET_CLEAN)

all: $(TARGET)
	@echo "~~~~~~~~~~~~~~Build All Component SUCCESS~~~~~~~~~~~~~~"

clean: $(TARGET_CLEAN)
	@echo "~~~~~~~~~~~~~~Clean Component SUCCESS~~~~~~~~~~~~~~"

$(TARGET):
	@echo "~~~~~~~~~~Start build $@~~~~~~~~~~"
	@cd $@ && make

$(TARGET_CLEAN):
	@echo "~~~~~~~~~~Start clean $(subst _clean,,$@)~~~~~~~~~~"
	@cd $(subst _clean,,$@) && make clean

CUR_DIR = $(shell pwd)
include $(MAKE_MACRO_FILE)

MAIN_DIR = $(CUR_DIR)/../../../..


ifeq ($(MAIN_CFG), )
    MAIN_CFG := $(MAIN_DIR)/build/main_cfg.mak
    include $(MAIN_CFG)
endif

TARGET=goodix
obj-m += $(TARGET).o
BUILD_DIR = $(CUR_DIR)/obj


all:
	@mkdir -p $(BUILD_DIR)
	@$(MAKE) -C $(KERNEL_ROOT) M=$(BUILD_DIR) src=$(CUR_DIR) modules
	@mkdir -p $(REL_KO) && cp -rf $(BUILD_DIR)/$(TARGET).ko $(REL_KO)

clean:
	@rm -rf $(BUILD_DIR)
	@rm $(REL_KO)/$(TARGET).ko -f

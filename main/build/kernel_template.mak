#*******************************************************************************
#The rule of make Linux kernel module
#*******************************************************************************
include $(MAKE_MACRO_FILE)

$(TARGET)-objs = $(SRCS:%.c=%.o)
ifneq ($(MOD_NAME),)
$(MOD_NAME)-objs = $(SRCS_O:%.c=%.o)
endif

ifneq ($(KERNELRELEASE),)
	obj-m += $(TARGET).o
ifneq ($(MOD_NAME),)
	obj-m += $(MOD_NAME).o
endif
endif

EXTRA_CFLAGS += $(DRV_CFLAGS)
BUILD_DIR = $(CUR_DIR)/obj
BUILD_DIR_MAKEFILE := $(BUILD_DIR)/../Makefile

ifeq ($(origin DEPEND_SYMBOL), undefined)
	EXTRA_SYMBOL := 
else
	EXTRA_SYMBOL := KBUILD_EXTRA_SYMBOLS=$(DEPEND_SYMBOL)
endif

#*******************************************************************************
.PHONY: default clean drv drv_clean $(EXT_TARGET) $(EXT_CLEAN)

default: drv $(EXT_TARGET)
clean: drv_clean $(EXT_CLEAN)

drv:
	@echo -e "\e[0;32;1m--Compiling '$(TARGET)'... Configs as follow:\e[0;36;1m"
	@echo ---- USER_CROSS=$(CROSS) KERNEL_CROSS=$(CROSS_COMPILE)
	@echo ---- OT_ARCH=$(OT_ARCH), ARCH=$(ARCH), OT_CHIP=$(OT_CHIP), CVER=$(CVER), DBG=$(OT_DBG), OT_FPGA=$(OT_FPGA)
	@echo ---- CPU_TYPE=$(CPU_TYPE)
	@echo ---- EXTRA_CFLAGS=$(EXTRA_CFLAGS)
	@echo ---- KERNEL_ROOT=$(KERNEL_ROOT)
	@echo ---- OT_GDB=$(OT_GDB)
	@echo ---- SRCS=$(SRCS)
	@echo -e "\e[0m"
	@mkdir -p $(BUILD_DIR)
	@touch $(BUILD_DIR_MAKEFILE)
	@$(call maco_cp, $(MODULE_OBJ), $(BUILD_DIR))
	@$(MAKE) -C $(KERNEL_ROOT) M=$(BUILD_DIR) src=$(CURDIR) O=$(KERNEL_ROOT) $(CLANG_CROSS) $(EXTRA_SYMBOL) V=10 modules
ifeq ($(OT_GDB), n)
	@$(STRIP) --strip-unneeded $(BUILD_DIR)/$(TARGET).ko
ifneq ($(MOD_NAME),)
	@$(STRIP) --strip-unneeded $(BUILD_DIR)/$(MOD_NAME).o
endif
endif
	@mkdir -p $(REL_KO) && cp -rf $(BUILD_DIR)/$(TARGET).ko $(REL_KO)
ifneq ($(EXTDRV_KO),)
	@mkdir -p $(REL_KO)/extdrv && mv $(REL_KO)/$(TARGET).ko $(REL_KO)/extdrv
endif
ifneq ($(MOD_NAME),)
	@mkdir -p $(REL_OBJ) && cp -rf $(BUILD_DIR)/$(MOD_NAME).o $(REL_OBJ)
	@mkdir -p $(REL_INI)/src && cp -rf $(INIT_FILE) $(REL_INI)/src
	@mkdir -p $(REL_INI)/include && cp -rf $(INIT_INC) $(REL_INI)/include
endif
	@$(call maco_cp, $(INC_FILE), $(REL_INC))

drv_clean:
ifneq ($(EXTDRV_KO),)
	@rm -f $(REL_KO)/extdrv/$(TARGET).ko
else
	@rm -f $(REL_KO)/$(TARGET).ko
endif
ifneq ($(MOD_NAME),)
	@rm -f $(REL_OBJ)/$(MOD_NAME).o
	@rm -f $(REL_INI)/src/$(shell basename $(INIT_FILE))
	@rm -f $(REL_INI)/include/$(shell basename $(INIT_INC))
endif
	@$(call maco_rm, $(INC_FILE), $(REL_INC))
	@rm -rf $(BUILD_DIR)

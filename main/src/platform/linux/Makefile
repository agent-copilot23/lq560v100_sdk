BUILD_DIR := $(shell pwd)

CHIP ?= lq560plusv100
CHIP_FAMILY ?= lq560v100

CPU_TYPE ?=
ifeq ($(CHIP), lq560plusv100)
LINUX_SUFFIX := _$(CPU_TYPE)
endif
KERNEL_ORIGIN_VER:=linux-5.10
KERNEL_VER:=linux-5.10.y
KERNEL_TAR:=$(KERNEL_VER).tgz
KERNEL_PATCH:=$(KERNEL_ORIGIN_VER).patch

MODULE_PATH:=$(BUILD_DIR)/$(KERNEL_VER)/modules
MODULE_OUT:=$(MODULE_PATH)/kermod
MODULE_VER:=$(shell echo $(KERNEL_VER) | sed 's/linux-//' | sed 's/y/0/')

LIB_TYPE ?=glibc
ARCH_TYPE ?= arm64

ifeq ($(BOOT_MEDIA), spi)
MEDIUM_FLAG = _
ifeq ($(QUICKBOOT_BUILD), y)
MEDIUM_FLAG = _mini_
endif
endif

ifeq ($(BOOT_MEDIA), emmc)
MEDIUM_FLAG = _emmc_
ifeq ($(QUICKBOOT_BUILD), y)
MEDIUM_FLAG = _emmc_mini_
endif
endif

KERNEL_CFG ?= $(if $(filter lq560miniv100 lq560litev100, $(CHIP)), lq560plusv100$(MEDIUM_FLAG), $(CHIP)$(MEDIUM_FLAG))defconfig

OSDRV_CROSS ?= aarch64-otv02-linux-gnu
PUB_IMAGE ?=$(CHIP)_$(EMMC_FLAG)image_$(LIB_TYPE)

CROSS := CROSS_COMPILE=$(OSDRV_CROSS)-
ifneq ($(LLVM), )
CROSS =
endif

GREEN = "\e[32;1m"
DONE  = "\033[0m"

all:
	@echo -e $(GREEN)"-------- kernel : $(KERNEL_ORIGIN_VER) "  $(DONE)
ifneq ($(BUILD_DIR)/$(KERNEL_VER), $(wildcard $(BUILD_DIR)/$(KERNEL_VER)))
ifneq ($(BUILD_DIR)/$(KERNEL_TAR), $(wildcard $(BUILD_DIR)/$(KERNEL_TAR)))
	@pushd $(BUILD_DIR); tar xf $(KERNEL_ORIGIN_VER).tar.gz; popd
	@echo -e $(GREEN)"patching kernel..."  $(DONE)
	@pushd $(BUILD_DIR); mv $(KERNEL_ORIGIN_VER) $(KERNEL_VER); popd
	@pushd $(BUILD_DIR)/$(KERNEL_VER); patch -p1 < ../$(KERNEL_PATCH); popd
	@pushd $(BUILD_DIR); tar -czf $(KERNEL_TAR) $(KERNEL_VER); popd
	@echo -e $(GREEN)"OK:generate patched kernel:$(KERNEL_TAR)!"  $(DONE)
else
	@pushd $(BUILD_DIR); tar xzf $(KERNEL_TAR) -C .; popd
endif
endif
	$(MAKE) -C $(BUILD_DIR)/$(KERNEL_VER) ARCH=$(ARCH_TYPE) LLVM=$(LLVM) LLVM_IAS=$(LLVM) $(CROSS) $(KERNEL_CFG)
	$(MAKE) -C $(BUILD_DIR)/$(KERNEL_VER) ARCH=$(ARCH_TYPE) LLVM=$(LLVM) LLVM_IAS=$(LLVM) $(CROSS) uImage;
ifeq ($(MODULE_PATH), $(wildcard $(MODULE_PATH)))
	@rm $(MODULE_PATH) -rf
endif
	$(MAKE) -C $(BUILD_DIR)/$(KERNEL_VER) ARCH=$(ARCH_TYPE) LLVM=$(LLVM) LLVM_IAS=$(LLVM) $(CROSS) modules
	$(MAKE) -C $(BUILD_DIR)/$(KERNEL_VER) ARCH=$(ARCH_TYPE) LLVM=$(LLVM) LLVM_IAS=$(LLVM) $(CROSS) modules_install INSTALL_MOD_PATH=$(MODULE_PATH);
	@$(BUILD_DIR)/$(KERNEL_VER)/scripts/get_modules.sh $(MODULE_PATH) $(MODULE_VER) $(MODULE_OUT)

menuconfig:
	$(MAKE) -C $(BUILD_DIR)/$(KERNEL_VER) ARCH=$(ARCH_TYPE) LLVM=$(LLVM) LLVM_IAS=$(LLVM) $(CROSS) menuconfig

savedefconfig:
	@pushd $(BUILD_DIR)/$(KERNEL_VER); cp .config arch/$(ARCH_TYPE)/configs/${KERNEL_CFG}; popd

clean:
	@rm $(MODULE_PATH) -rf
	$(MAKE) -C $(BUILD_DIR)/$(KERNEL_VER) ARCH=$(ARCH_TYPE) LLVM=$(LLVM) LLVM_IAS=$(LLVM) CROSS_COMPILE=$(OSDRV_CROSS)- clean

distclean:
	@rm $(BUILD_DIR)/$(KERNEL_ORIGIN_VER) -rf
	@rm $(BUILD_DIR)/$(KERNEL_VER) -rf
	@rm $(BUILD_DIR)/$(KERNEL_TAR) -rf
	@rm $(MODULE_PATH) -rf

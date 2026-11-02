BUILD_DIR := $(shell pwd)
PC_TOOLS_DIR := $(shell pwd)/../../../tools/pc
#OSDRV_DIR ?= ../../bsp
CHIP ?= lq560plusv100
CHIP_FAMILY ?= lq560v100

UBOOT_VER ?= u-boot-2022.07

UBOOT_TAR := $(UBOOT_VER).tar.bz2
LIB_TYPE ?= glibc
ARCH_TYPE ?= arm64
ARM_ARCH_VERSION ?= armv8
UBOOT_REG_BIN := $(CHIP)_reg_info.bin
UBOOT_PATCH := $(UBOOT_VER).patch

OPENSOURCE_DIR ?= $(BUILD_DIR)/../../../../opensource
GZIP_TOOL ?= gzip
GZIP_DIR ?= $(OPENSOURCE_DIR)/gzip
GZIP_VER ?= gzip-1.10
OSDRV_CROSS ?= aarch64-otv02-linux-gnu
PARTITION_ENV_DIR := $(BUILD_DIR)/../../../pub/uboot_env/$(CHIP)/
A55_STL_LIB_DIR := $(BUILD_DIR)/../../safelib/a55_stl/lib/

IMAGES_DIR := $(BUILD_DIR)/../../../pub

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

UBOOT_CONFIG ?= $(if $(filter $(CHIP), lq560miniv100 lq560litev100),lq560plusv100,$(CHIP))$(MEDIUM_FLAG)defconfig

PUB_IMAGE ?= $(CHIP)$(MEDIUM_FLAG)image_$(LIB_TYPE)
BUILD ?= part

GREEN = "\e[32;1m"
DONE    = "\033[0m"

ifneq ($(LLVM), )
CROSS =
else
CROSS = CROSS_COMPILE=$(OSDRV_CROSS)-
endif

define make_uboot
	@echo "$(0) $(1)"
	@cp $(PARTITION_ENV_DIR)/$(1) $(BUILD_DIR)/$(UBOOT_VER)/board/vendor/$(CHIP_FAMILY)/$(CHIP_FAMILY).env -rf
	+pushd $(BUILD_DIR)/$(UBOOT_VER); $(MAKE) LLVM=$(LLVM) $(CROSS) $(UBOOT_CONFIG)
	+pushd $(BUILD_DIR)/$(UBOOT_VER); $(MAKE) LLVM=$(LLVM) $(CROSS) 1>/dev/null; popd
	+pushd $(BUILD_DIR)/$(UBOOT_VER); $(MAKE) LLVM=$(LLVM) $(CROSS) u-boot-z.bin 1>/dev/null; popd
	+pushd $(BUILD_DIR)/$(UBOOT_VER); $(MAKE) LLVM=$(LLVM) $(CROSS) u-boot-z.clean 1>/dev/null; popd
endef

all:
	@echo -e $(GREEN)"-------- uboot : $(UBOOT_VER) "  $(DONE)

ifneq ($(BUILD_DIR)/$(UBOOT_VER), $(wildcard $(BUILD_DIR)/$(UBOOT_VER)))
	@pushd $(BUILD_DIR); tar xjf $(UBOOT_TAR) -C .; popd
	@pushd $(BUILD_DIR)/$(UBOOT_VER); patch -p1 < ../$(UBOOT_PATCH); popd
endif
	@find $(BUILD_DIR)/$(UBOOT_VER) | xargs touch
	@cd $(BUILD_DIR)/$(UBOOT_VER) && \
		$(MAKE) $(CROSS) distclean >/dev/null;

ifeq ($(GZIP_TOOL),gzip)
	@cp $(IMAGES_DIR)/bin/pc/gzip $(BUILD_DIR)/$(UBOOT_VER)/arch/arm/cpu/$(ARM_ARCH_VERSION)/$(CHIP_FAMILY)/hw_compressed -rf
	@chmod +x $(BUILD_DIR)/$(UBOOT_VER)/arch/arm/cpu/$(ARM_ARCH_VERSION)/$(CHIP_FAMILY)/hw_compressed/gzip
endif

ifeq ($(CONFIG_STL_TEST), y)
	@cp $(A55_STL_LIB_DIR)/*.a $(BUILD_DIR)/$(UBOOT_VER)/product/a55_stl -f
endif

ifeq ($(BOOT_MEDIA), spi)
	@echo "build spinand"
ifeq ($(QUICKBOOT_BUILD), y)
ifeq ($(SECUREBOOT_BUILD), y)
	$(call make_uboot,nand_squickboot.env)
else
	$(call make_uboot,nand_quickboot.env)
endif
else
ifeq ($(SECUREBOOT_BUILD), y)
	$(call make_uboot,nand_secure_boot.env)
else
	$(call make_uboot,nand.env)
endif
endif
	@pushd $(BUILD_DIR)/$(UBOOT_VER); \
	if [ "$(CHIP)" = "lq560miniv100" ] || [ "$(CHIP)" = "lq560litev100" ]; then \
		cp u-boot-lq560plusv100.bin u-boot-$(CHIP).bin; \
	fi; \
	cp u-boot-$(CHIP).bin u-boot-$(CHIP)_spinand.bin; \
	popd

	@echo "build spinor"
ifeq ($(QUICKBOOT_BUILD), y)
ifeq ($(SECUREBOOT_BUILD), y)
	$(call make_uboot,nor_squickboot.env)
else
	$(call make_uboot,nor_quickboot.env)
endif
else
ifeq ($(SECUREBOOT_BUILD), y)
	$(call make_uboot,nor_secure_boot.env)
else
	$(call make_uboot,nor.env)
endif
endif
	@pushd $(BUILD_DIR)/$(UBOOT_VER); \
	if [ "$(CHIP)" = "lq560miniv100" ] || [ "$(CHIP)" = "lq560litev100" ]; then \
		cp u-boot-lq560plusv100.bin u-boot-$(CHIP).bin; \
	fi; \
	cp u-boot-$(CHIP).bin u-boot-$(CHIP)_spinor.bin; \
	popd

else

	@echo "build emmc"
ifeq ($(QUICKBOOT_BUILD), y)
ifeq ($(SECUREBOOT_BUILD), y)
	$(call make_uboot,emmc_squickboot.env)
else
	$(call make_uboot,emmc_quickboot.env)
endif
else
ifeq ($(SECUREBOOT_BUILD), y)
	$(call make_uboot,emmc_secure_boot.env)
else
	$(call make_uboot,emmc.env)
endif
endif
	@pushd $(BUILD_DIR)/$(UBOOT_VER); \
	if [ "$(CHIP)" = "lq560miniv100" ] || [ "$(CHIP)" = "lq560litev100" ]; then \
		cp u-boot-lq560plusv100.bin u-boot-$(CHIP).bin; \
	fi; \
	popd

endif

menuconfig:
	$(MAKE) -C $(BUILD_DIR)/$(UBOOT_VER) LLVM=$(LLVM) $(CROSS) menuconfig

savedefconfig:
	@pushd $(BUILD_DIR)/$(UBOOT_VER); cp .config configs/${UBOOT_CONFIG}; popd

clean:
ifeq ($(BUILD_DIR)/$(UBOOT_VER), $(wildcard $(BUILD_DIR)/$(UBOOT_VER)))
	$(MAKE) -C $(BUILD_DIR)/$(UBOOT_VER) LLVM=$(LLVM) $(CROSS) clean
endif
	$(MAKE) -C $(GZIP_DIR) clean

distclean:
	@rm $(BUILD_DIR)/$(UBOOT_VER) -rf
	@rm $(GZIP_DIR)/$(GZIP_VER) -rf
	@rm $(GZIP_DIR)/bin -rf

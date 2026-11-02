##########################################################################################
#	prepare param
##########################################################################################
export OSDRV_DIR=$(shell pwd)

IMAGES_DIR=$(OSDRV_DIR)/../../pub
OPEN_SOURCE=$(OSDRV_DIR)/../../../opensource
export OPENSOURCE_DIR = ${OPEN_SOURCE}
TOOLS_DIR=$(OSDRV_DIR)/../../tools/
PC_TOOLS_DIR=$(TOOLS_DIR)/pc/
BOARD_TOOLS_DIR=$(TOOLS_DIR)/board/
BOOT_TOOLS_DIR=$(PC_TOOLS_DIR)/boot_tools/
MAIN_DIR=${OSDRV_DIR}/../..
MAIN_BUILD_DIR=$(MAIN_DIR)/build
MAIN_OUT_DIR=$(MAIN_DIR)/out
SAMPLE_DIR=$(MAIN_DIR)/src/component/media/sample
KO_DIR=$(MAIN_OUT_DIR)/ko
QUICKBOOT_SAMPLE=quickboot/sample_quickboot
MEDIA_KO_DIR_NAME=ompmod
OPEN_SOURCE_BOARD_TOOLS_DIR=$(IMAGES_DIR)/bin/board_glibc_arm64/
LIB_TYPE ?= glibc
export LLVM ?=
ifeq ($(LIB_TYPE), glibc)
export OSDRV_CROSS ?= aarch64-otv02-linux-gnu
else
$(warning "---------------------------------------------------------------------")
$(warning "   LIB_TYPE Input Is ERR!!       ")
$(warning "---------------------------------------------------------------------")
$(error )
endif

ifneq ($(LLVM), )
export CROSS_COMPILE=
else
export CROSS_COMPILE=$(OSDRV_CROSS)-
endif
export CPU_TYPE :=
export BUILD :=all
ARCH_TYPE ?= arm64
export ARCH :=arm64
export ARM_ARCH_VERSION := armv8
AMP_TYPE ?= linux
GDB_COMPILE ?= y


BUILD_VERSION ?= new
#TODO: new version for new board, we should also rm regbin file from param7 in "quick_build_config.json"

BOOT_MEDIA ?= spi
# BOOT_MEDIA ?= emmc

BOOT_MODE ?= normal
# BOOT_MODE ?= ab

SECUREBOOT_TYPE ?= sm
#SECUREBOOT_TYPE ?= ecc
SECUREBOOT_ENC ?= n
#SECUREBOOT_ENC ?= y

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

ifeq ($(CHIP),lq560tinyv100)
REGBIN_XLSM ?= LQ560TINYV100-DMEB_6L_T_LPDDR4_2400M-2GB_32bit-A55_850M.xlsm
else ifneq (,$(filter $(CHIP),lq560plusv100 lq560miniv100 lq560litev100))
REGBIN_XLSM ?= LQ560PLUSV100-DMEB_6L_T_LPDDR4_2933M-2GB_32bit-A55_1G.xlsm
else
$(error you must set valid CHIP: lq560plusv100 | lq560miniv100 | lq560litev100 | lq560tinyv100)
endif

KERNEL_CFG:=$(CHIP)$(MEDIUM_FLAG)defconfig

TOOLCHAINI_VERSION := ver100.140

TOOLCHAIN_FILE:= $(shell which $(OSDRV_CROSS)-gcc )
TOOLCHAIN_DIR:=$(shell dirname $(shell dirname $(TOOLCHAIN_FILE)))
RUNTIMELIB_DIR=$(shell dirname $(TOOLCHAIN_DIR))/$(RUNTIME_LIB)

CROSS_SPECIFIED:=y
TOOLCHAIN_RUNTIME_LIB:=runtime_lib

ifneq ($(LLVM),)
BUSYBOX_CFG ?= config_aarch64_a55_softfp_neon_llvm
else
BUSYBOX_CFG ?= config_aarch64_a55_softfp_neon
endif

UBOOT_CONFIG ?=$(CHIP)$(MEDIUM_FLAG)defconfig

BUSYBOX_VER:=busybox-1.34.1
BUSYBOX_DIR := $(OPEN_SOURCE)/busybox
TOOLCHAIN_RUNTIME_LIB_C:=lib.tgz

ATF_VER:=arm-trusted-firmware-2.7
ATF_TAR:=$(ATF_VER).tar.gz
ATF_DIR:=$(OSDRV_DIR)/arm-trusted-firmware

KERNEL_VER:=linux-5.10.y
KERNEL_DIR:=$(OSDRV_DIR)/linux
KERNEL_TAR:=$(KERNEL_VER).tgz

UBOOT_VER:=u-boot-2022.07
UBOOT_DIR:=$(OSDRV_DIR)/u-boot
UBOOT_TAR:=$(UBOOT_VER).tar.bz2

SECURE_BOOT:=secureboot_release
SECURE_BOOT_TAR:=$(SECURE_BOOT).tgz
CROSS_CC :=
ifneq ($(LLVM),)
	LLVM_TOOLCHAIN_PATH:=$(shell llvm-config --bindir 2>/dev/null)
	LLVM_INSTALL_TOOL:=$(LLVM_TOOLCHAIN_PATH)/../tools/install_dynamic_basic_library.sh
	LLVM_DEFAULT_TARGET:=$(shell clang -print-target-triple 2>/dev/null)
	CROSS_CC = clang
else
	CROSS_CC = $(OSDRV_CROSS)-gcc
        GCC_INSTALL_TOOL:=$(TOOLCHAIN_DIR)/tools/install_dynamic_basic_library.sh
endif

CRAMFS_VER:= util-linux-2.37.2
CRAMFS_DIR:=$(OPEN_SOURCE)/util-linux
CRAMFS_TAR:=$(CRAMFS_VER).tar.xz

XZ_DIR:= $(OPEN_SOURCE)/xz

LZMA_DIR:=$(OPEN_SOURCE)/lzma

EUDEV_VER:= eudev-3.2.10
EUDEV_DIR:=$(OPEN_SOURCE)/eudev
EUDEV_TAR:=$(EUDEV_VER).tar.gz

MTD_DIR:=$(OPEN_SOURCE)/mtd-utils/
MTD_VER:= mtd-utils-2.1.4
MTD_TAR:=$(MTD_VER).tar.bz2
VENDOR_TOOLS:=$(BOARD_TOOLS_DIR)/reg-tools-1.0.0

ZLIB_VER:= zlib-1.2.12
ZLIB_DIR:= $(OPEN_SOURCE)/zlib
ZLIB_TAR:=$(ZLIB_VER).tar.gz

SQUASHFS_VER:= squashfs-4.5
SQUASHFS_DIR:=$(OPEN_SOURCE)/squashfs
SQUASHFS_TAR:=$(SQUASHFS_VER).tar.gz

ETHTOOL_VER:= ethtool-5.15
ETHTOOL_DIR:= $(OPEN_SOURCE)/ethtool
SQUASHFS_TAR:=$(ETHTOOL_VER).tar.xz

GZIP_DIR ?= $(OPEN_SOURCE)/gzip
GZIP_VER ?= gzip-1.10

PARTITION_ENV_DIR := $(PC_TOOLS_DIR)/partition_env/
PARTITION_XML_DIR := $(PARTITION_ENV_DIR)/config/$(CHIP)/${BOOT_MODE}/
BOOT_REG_BIN:=$(CHIP)_reg_info.bin
JFFS2_IMAGE_BIN_64K:=userfs_$(CHIP)_64k.jffs2
JFFS2_IMAGE_BIN_128K:=userfs_$(CHIP)_128k.jffs2
JFFS2_IMAGE_BIN_256K:=userfs_$(CHIP)_256k.jffs2
DATAFS_JFFS2_IMAGE_BIN_64K:=datafs_$(CHIP)_64k.jffs2
DATAFS_JFFS2_IMAGE_BIN_128K:=datafs_$(CHIP)_128k.jffs2
DATAFS_JFFS2_IMAGE_BIN_256K:=datafs_$(CHIP)_256k.jffs2

ROMFS_IMAGE_BIN:=rootfs_$(CHIP).ramdisk

EMMC_USERFS_SIZE ?= 96
EMMC_DATAFS_SIZE ?= 7963
E2FSPROGS_DIR:=$(OPEN_SOURCE)/e2fsprogs/
EXT4_TOOL:=mkfs.ext4
EXT4_IMAGE_BIN:=userfs_$(CHIP)_$(EMMC_USERFS_SIZE)M.ext4
COUNT_SIZE := $(shell echo $$(( $(EMMC_USERFS_SIZE) * 1024 * 2 )))

DATAFS_EXT4_IMAGE_BIN:=datafs_$(CHIP)_$(EMMC_DATAFS_SIZE)M.ext4
DATAFS_COUNT_SIZE := $(shell echo $$(( $(EMMC_DATAFS_SIZE) * 1024 * 2 )))

XML2_DIR:=$(OPEN_SOURCE)/xml2/

REGBING_PACKAGE_VER:=RegBin-v1.0.0_linux-x86_64
REGBING_PACKAGE_FILE:=RegBin
REGBING_HEAD_MAGIC=2b8c6e1a1a6e8c2b
BOOT_IMAGE_TOOL_DIR=$(PC_TOOLS_DIR)/image_tool
MISC_TOOL_DIR=$(PC_TOOLS_DIR)/misc_tool
MISC_TOOL_SCRIPT=$(MISC_TOOL_DIR)/misc2bin.py

BOOT_IMAGE_TOOL_INPUT_DIR=$(BOOT_IMAGE_TOOL_DIR)/input
OTFL_IMAGE_TOOL_OUTPUT_FILE=$(BOOT_IMAGE_TOOL_DIR)/image/oem/otfl.img
UBOOT_IMAGE_TOOL_OUTPUT_FILE=$(BOOT_IMAGE_TOOL_DIR)/image/oem/uboot.img
KERNEL_IMAGE_TOOL_OUTPUT_FILE=$(BOOT_IMAGE_TOOL_DIR)/image/oem/uImage-fdt.gz.img
ROOTFS_IMAGE_TOOL_OUTPUT_FILE=$(BOOT_IMAGE_TOOL_DIR)/image/oem/rootfs.ramdisk.gz.img
ATF_IMAGE_TOOL_OUTPUT_FILE=$(BOOT_IMAGE_TOOL_DIR)/image/oem/bl31.bin.img

ifeq ($(SECUREBOOT_BUILD), y)
IMAGE_TOOL_SCRIPT=$(BOOT_IMAGE_TOOL_DIR)/oem/oem_quick_build_secure_boot.py
SYSTEM_IMAGE_TOOL_SCRIPT=$(BOOT_IMAGE_TOOL_DIR)/oem/oem_quick_build_system_secure_boot.py
SECUREBOOT=_secure_boot
UPGRADE_IMAGE_TOOL_SCRIPT=$(BOOT_IMAGE_TOOL_DIR)/oem/oem_quick_build_secure_boot_upgrade.py

ROMFS_IMAGE_BIN:=rootfs_$(CHIP).ramdisk.img.sig
UBOOT_EMMC_IMAGE_BIN:=uboot_emmc.img.sig
UBOOT_NAND_IMAGE_BIN:=uboot_spinand.img.sig
UBOOT_NOR_IMAGE_BIN:=uboot_spinor.img.sig
KERNEL_IMAGE_BIN:=uImage-fdt.img.sig
ATF_IMAGE_BIN:=bl31.img.sig
OTFL_EMMC_IMAGE_BIN:=otfl_emmc.img.sig
OTFL_NOR_IMAGE_BIN:=otfl_spinor.img.sig
OTFL_NAND_IMAGE_BIN:=otfl_spinand.img.sig

ifeq ($(SECUREBOOT_TYPE), ecc)
ifeq ($(SECUREBOOT_ENC), y)
IMAGE_TOOL_CONFIG=$(BOOT_IMAGE_TOOL_DIR)/oem/quick_build_secure_boot_config_nm_enc.json
else
IMAGE_TOOL_CONFIG=$(BOOT_IMAGE_TOOL_DIR)/oem/quick_build_secure_boot_config_nm.json
endif
else
ifeq ($(SECUREBOOT_ENC), y)
IMAGE_TOOL_CONFIG=$(BOOT_IMAGE_TOOL_DIR)/oem/quick_build_secure_boot_config_gm_enc.json
else
IMAGE_TOOL_CONFIG=$(BOOT_IMAGE_TOOL_DIR)/oem/quick_build_secure_boot_config_gm.json
endif
endif

else
IMAGE_TOOL_SCRIPT=$(BOOT_IMAGE_TOOL_DIR)/oem/oem_quick_build.py
SYSTEM_IMAGE_TOOL_SCRIPT=$(BOOT_IMAGE_TOOL_DIR)/oem/oem_quick_build_system.py
SECUREBOOT=
UPGRADE_IMAGE_TOOL_SCRIPT=$(BOOT_IMAGE_TOOL_DIR)/oem/oem_quick_build_upgrade.py

ROMFS_IMAGE_BIN:=rootfs_$(CHIP).ramdisk.img
UBOOT_EMMC_IMAGE_BIN:=uboot_emmc.img
UBOOT_NAND_IMAGE_BIN:=uboot_spinand.img
UBOOT_NOR_IMAGE_BIN:=uboot_spinor.img
KERNEL_IMAGE_BIN:=uImage-fdt.img
ATF_IMAGE_BIN:=bl31.img
OTFL_EMMC_IMAGE_BIN:=otfl_emmc.img
OTFL_NOR_IMAGE_BIN:=otfl_spinor.img
OTFL_NAND_IMAGE_BIN:=otfl_spinand.img
endif

PARTITION_ENV_INFIX :=

UPGRADE_IMAGE="upgrade.zip"
##########################################################################################
#	prepare regbin tools
##########################################################################################
REGBING_PACKAGE = $(shell find $(BOOT_TOOLS_DIR)/ \
					-name "$(REGBING_PACKAGE_VER).tgz")
ifeq ($(REGBING_PACKAGE), )
$(warning "---------------------------------------------------------------------")
$(warning "   Cannot found the $(REGBING_PACKAGE_VER).tgz package !!       ")
$(warning "---------------------------------------------------------------------")
$(error )
endif
TARGET_XLSM := $(REGBIN_XLSM)

export PUB_BOARD:=board_$(LIB_TYPE)_$(ARCH_TYPE)
export PUB_IMAGE:=$(CHIP)$(MEDIUM_FLAG)image_$(LIB_TYPE)${SECUREBOOT}
export PUB_ROOTFS:=rootfs_$(LIB_TYPE)_$(ARCH_TYPE)
export PUB_USERFS:=userfs_$(LIB_TYPE)_$(ARCH_TYPE)
export PUB_DATAFS:=datafs_$(LIB_TYPE)_$(ARCH_TYPE)

define copy_if_not_empty
	@if [ -z "$$(ls $(1))" ]; then \
		echo "dir $(1) is empty, nothing to copy."; \
	else \
		echo "cp from $(1) to $(2)..."; \
		cp -rf $(1)/* $(2); \
	fi
endef

BLACK = "\e[30;1m"
RED  =  "\e[31;1m"
GREEN = "\e[32;1m"
YELLOW = "\e[33;1m"
BLUE  = "\e[34;1m"
PURPLE = "\e[35;1m"
CYAN  = "\e[36;1m"
WHITE = "\e[37;1m"
DONE    = "\033[0m"
##########################################################################################
#	set task
##########################################################################################
all: otflboot_build platform rootfs_build userfs_build datafs_build
platform: atf kernel boardtools mtd_utils_pc
clean: boot_clean kernel_clean atf_clean \
       busybox_clean rootfs_clean otflboot_clean \
       secure_libs_clean userfs_clean datafs_clean mtd_utils_clean\
       e2fsprogs_clean zlib_clean xz_lib_clean\
       ethtool_clean vendor_tools_clean eudev_clean\
       mk_partition_clean gzip_clean xml2_clean

distclean:clean pub_clean
a:=$(shell $(OSDRV_CROSS)-gcc --version)
b:=$(findstring $(TOOLCHAINI_VERSION),$(a))
c:=$(word 4, $(a))
##########################################################################################
#task [0.0]	toolchain version check
##########################################################################################
toolchain_version_check:
	@echo "---------task [0.0]    toolchain_version_check"
ifneq ($(b),$(TOOLCHAINI_VERSION))
	$(warning "---------------------------------------------------------------------")
	$(warning "       < !!! Attention:please check toolchain version !!! >          ")
	$(warning "         The current toolchain version is $(c)  ")
	$(warning "         But requested toolchain version is ($(TOOLCHAINI_VERSION))")
	$(warning "         Please make sure the toolchain version is the best match    ")
	$(warning "---------------------------------------------------------------------")
	$(shell read -p "Press Enter to continue compile; Ctrl + C to stop it !!" )
endif

##########################################################################################
#task [0.1]	prepare
##########################################################################################
prepare:toolchain_version_check
	@echo "---------task [0.1]    prepare"
	@mkdir $(IMAGES_DIR)/$(PUB_IMAGE) -p
	@mkdir $(IMAGES_DIR)/bin/$(PUB_BOARD) -p
	@mkdir $(IMAGES_DIR)/bin/pc -p

##########################################################################################
#task [0.2]	regbin_prepare
##########################################################################################
regbin_prepare:
	@echo "---------task [0.2]    regbin_prepare"
	@echo "---------TARGET_XLSM: $(TARGET_XLSM)"
	@rm $(BOOT_TOOLS_DIR)/$(REGBING_PACKAGE_FILE) -rf
	@tar xzf $(REGBING_PACKAGE) -C $(BOOT_TOOLS_DIR)
	@chmod 777 $(BOOT_TOOLS_DIR)/$(REGBING_PACKAGE_FILE)/regbin
	@cp $(BOOT_TOOLS_DIR)/$(TARGET_XLSM) $(BOOT_TOOLS_DIR)/$(REGBING_PACKAGE_FILE)
	@cd $(BOOT_TOOLS_DIR)/$(REGBING_PACKAGE_FILE) && ./regbin $(TARGET_XLSM) $(BOOT_REG_BIN) -magic $(REGBING_HEAD_MAGIC); cd -
	@mv $(BOOT_TOOLS_DIR)/$(REGBING_PACKAGE_FILE)/$(BOOT_REG_BIN) $(BOOT_TOOLS_DIR)
	@rm $(BOOT_TOOLS_DIR)/$(REGBING_PACKAGE_FILE) -rf

notools_prepare:
	@pushd $(IMAGES_DIR); tar xzf $(PUB_ROOTFS).tgz; popd
##########################################################################################
#task [1]	build uboot64
##########################################################################################
boot: prepare regbin_prepare partition_uboot_env gzip
	@echo -e $(GREEN)"---------task [1]	build boot64  $(UBOOT_DIR)" $(DONE)
	$(MAKE) -C $(UBOOT_DIR) CHIP=$(CHIP) CHIP_FAMILY=$(CHIP_FAMILY) BOOT_MEDIA=$(BOOT_MEDIA) LLVM=$(LLVM) LIB_TYPE=$(LIB_TYPE) \
			ARCH_TYPE=$(ARCH_TYPE) all

boot_savedefconfig:
	$(MAKE) -C $(UBOOT_DIR) savedefconfig

boot_clean:
	@rm $(UBOOT_DIR)/$(UBOOT_VER) -rf
	@rm $(BOOT_TOOLS_DIR)/$(BOOT_REG_BIN) -rf
	$(MAKE) -C $(UBOOT_DIR) LLVM=$(LLVM) clean
##########################################################################################
#task [2]	build atf
##########################################################################################
ifeq ($(SECUREBOOT_BUILD), y)
atf: prepare securecfg_prepare
else
atf: prepare
endif
	@echo -e $(GREEN)"---------task [2] build atf"  $(DONE)
	$(MAKE) -C $(ATF_DIR) CHIP=$(CHIP) CHIP_FAMILY=$(CHIP_FAMILY) BOOT_MEDIA=$(BOOT_MEDIA) LLVM=$(LLVM) LIB_TYPE=$(LIB_TYPE) \
				ARCH_TYPE=$(ARCH_TYPE) all
	@cp $(ATF_DIR)/$(ATF_VER)/build/$(CHIP_FAMILY)/release/bl31.bin $(BOOT_IMAGE_TOOL_INPUT_DIR)
	@python $(SYSTEM_IMAGE_TOOL_SCRIPT) $(BOOT_IMAGE_TOOL_INPUT_DIR)/bl31.bin $(IMAGES_DIR)/$(PUB_IMAGE)/$(ATF_IMAGE_BIN)

atf_clean:
	@rm -rf $(ATF_DIR)/$(ATF_VER)
	@rm $(BOOT_IMAGE_TOOL_INPUT_DIR)/bl31.bin -f

gzip: prepare
	@echo -e $(GREEN)"---------task [3.1] build gzip"  $(DONE)
	$(MAKE) -C $(GZIP_DIR)
	@cp $(GZIP_DIR)/bin/gzip $(IMAGES_DIR)/bin/pc

gzip_clean:
	$(MAKE) -C $(GZIP_DIR) clean

##########################################################################################
#task [3]	build kernel
##########################################################################################
kernel: prepare gzip
	@echo -e $(GREEN)"---------task [3] build kernel"  $(DONE)
	$(MAKE) -C $(KERNEL_DIR) CHIP=$(CHIP) BOOT_MEDIA=$(BOOT_MEDIA) LLVM=$(LLVM) LIB_TYPE=$(LIB_TYPE) \
				ARCH_TYPE=$(ARCH_TYPE) all
	@cp $(KERNEL_DIR)/$(KERNEL_VER)/arch/$(ARCH_TYPE)/boot/uImage-fdt $(BOOT_IMAGE_TOOL_INPUT_DIR)
	@chmod +x $(PC_TOOLS_DIR)/compress/kernel_compress.sh
	@pushd $(IMAGES_DIR)/$(PUB_IMAGE);$(PC_TOOLS_DIR)/compress/kernel_compress.sh $(BOOT_IMAGE_TOOL_INPUT_DIR)/uImage-fdt; popd
	@python $(SYSTEM_IMAGE_TOOL_SCRIPT) $(BOOT_IMAGE_TOOL_INPUT_DIR)/uImage-fdt.gz $(IMAGES_DIR)/$(PUB_IMAGE)/$(KERNEL_IMAGE_BIN)

kernel_savedefconfig:
	$(MAKE) -C $(KERNEL_DIR) savedefconfig

kernel_clean:
	@rm $(KERNEL_DIR)/$(KERNEL_VER) -rf
	@rm $(KERNEL_DIR)/$(KERNEL_TAR) -rf
	@rm $(BOOT_IMAGE_TOOL_INPUT_DIR)/uImage-fdt.gz -f

##########################################################################################
#task [4] build secure libs
##########################################################################################
secure_libs_pc: prepare
	@echo -e $(GREEN)"---------task [4.1] build secure libs on pc"  $(DONE)
ifneq ($(OSDRV_DIR)/secure_c/secure_c_pc, $(wildcard $(OSDRV_DIR)/secure_c/secure_c_pc))
	@mkdir $(OSDRV_DIR)/secure_c/secure_c_pc
	@pushd $(OSDRV_DIR)/secure_c; tar xzf secure_c.tgz -C secure_c_pc; popd
endif
	@pushd $(OSDRV_DIR)/secure_c/secure_c_pc/src; $(MAKE) clean; CC=gcc $(MAKE) lib CHECK_OPTION=check; popd
	@mkdir $(OSDRV_DIR)/secure_c/libs -p
	@cp $(OSDRV_DIR)/secure_c/secure_c_pc/lib/libsecurec.a $(OSDRV_DIR)/secure_c/libs/libsecurec-pc.a
	@pushd $(OSDRV_DIR)/secure_c/secure_c_pc/src; $(MAKE) clean; popd

secure_libs_board: prepare
	@echo -e $(GREEN)"---------task [4.2] build secure libs on board"  $(DONE)
ifneq ($(OSDRV_DIR)/secure_c/secure_c_board, $(wildcard $(OSDRV_DIR)/secure_c/secure_c_board))
	@mkdir $(OSDRV_DIR)/secure_c/secure_c_board
	@pushd $(OSDRV_DIR)/secure_c; tar xzf secure_c.tgz -C secure_c_board; popd
endif
	@pushd $(OSDRV_DIR)/secure_c/secure_c_board/src; $(MAKE) clean; CC=$(CROSS_CC) $(MAKE) lib CHECK_OPTION=check; popd
	@mkdir $(OSDRV_DIR)/secure_c/libs -p
	@cp $(OSDRV_DIR)/secure_c/secure_c_board/lib/libsecurec.a $(OSDRV_DIR)/secure_c/libs/libsecurec-board.a
	@pushd $(OSDRV_DIR)/secure_c/secure_c_board/src; $(MAKE) clean; popd

secure_libs_clean:
	@rm -rf $(OSDRV_DIR)/secure_c/secure_c_board
	@rm -rf $(OSDRV_DIR)/secure_c/secure_c_pc
	@rm -rf $(OSDRV_DIR)/secure_c/libs
##########################################################################################
#task [5]	prepare rootfs
##########################################################################################
rootfs_prepare: prepare
	@echo -e $(GREEN)"---------task [5] prepare rootfs "  $(DONE)
ifeq ($(IMAGES_DIR)/$(PUB_ROOTFS), $(wildcard $(IMAGES_DIR)/$(PUB_ROOTFS)))
	@pushd $(IMAGES_DIR)/$(PUB_ROOTFS); chmod +w usr/bin; chmod +w usr/sbin; chmod +w sbin; popd
	@rm $(IMAGES_DIR)/$(PUB_ROOTFS)* -rf
endif
	$(MAKE) -C $(OSDRV_DIR)/fs/rootfs/ all
	@cp -r $(OSDRV_DIR)/fs/rootfs/rootfs/ $(IMAGES_DIR)/$(PUB_ROOTFS)
ifneq ($(LLVM),)
	$(LLVM_INSTALL_TOOL) TOOLCHAIN_PATH=$(LLVM_TOOLCHAIN_PATH) INSTALL_PATH=$(IMAGES_DIR)/$(PUB_ROOTFS) TARGET=$(LLVM_DEFAULT_TARGET)
else
	@echo $(TOOLCHAIN_DIR)/$(TOOLCHAIN_RUNTIME_LIB)   $(TOOLCHAIN_RUNTIME_LIB_C)
	$(GCC_INSTALL_TOOL) $(TOOLCHAIN_DIR) $(IMAGES_DIR)/$(PUB_ROOTFS)
endif

##########################################################################################
#task [6]	build busybox
##########################################################################################
busybox: prepare rootfs_prepare
	@echo -e $(GREEN)"---------task [6] build busybox "  $(DONE)
	$(MAKE) -C $(BUSYBOX_DIR) CHIP=$(CHIP) BOOT_MEDIA=$(BOOT_MEDIA) LLVM=$(LLVM)  LIB_TYPE=$(LIB_TYPE) \
				ARCH_TYPE=$(ARCH_TYPE) BUSYBOX_CFG=$(BUSYBOX_CFG) IMAGES_DIR=$(IMAGES_DIR) all

busybox_clean:
	$(MAKE)  LLVM=$(LLVM) -C $(BUSYBOX_DIR) clean

zlib_pc:
	@echo -e $(GREEN)"---------task [7.1] build zlibs_pc"  $(DONE)
	$(MAKE) -C $(ZLIB_DIR) zlib_pc

zlib_clean:
	$(MAKE) -C $(ZLIB_DIR) distclean

xz_lib_pc:
	@echo -e $(GREEN)"---------task [7.2] build xz_lib_pc"  $(DONE)
	$(MAKE) -C $(XZ_DIR)

xz_lib_clean:
	$(MAKE) -C $(XZ_DIR) distclean

mtd_utils_pc: prepare zlib_pc xz_lib_pc
	@echo -e $(GREEN)"---------task [7.3] build mtd_utils_pc"  $(DONE)
	$(MAKE) -C $(MTD_DIR) mtd_pc

	@cp $(MTD_DIR)/out/pc/mkfs.jffs2 $(IMAGES_DIR)/bin/pc
	@cp $(MTD_DIR)/out/pc/mkfs.ubifs $(IMAGES_DIR)/bin/pc
	@cp $(MTD_DIR)/out/pc/ubinize $(IMAGES_DIR)/bin/pc

mtd_utils_clean:
	$(MAKE) -C $(MTD_DIR) distclean

e2fsprogs_pc: prepare
	@echo -e $(GREEN)"---------task [7.4] build e2fsprogs_pc"  $(DONE)
	$(MAKE) -C $(E2FSPROGS_DIR) e2fsprogs_pc
	@cp $(E2FSPROGS_DIR)/out/pc/sbin/$(EXT4_TOOL) $(IMAGES_DIR)/bin/pc

e2fsprogs_clean:
	$(MAKE) -C $(E2FSPROGS_DIR)  distclean

mk_partition_pc: prepare
	@echo -e $(GREEN)"---------task [7.6] build mk_partition_pc"  $(DONE)
	$(MAKE) -C $(PARTITION_ENV_DIR)/tool all
	@cp $(PARTITION_ENV_DIR)/tool/mk_partition_env $(IMAGES_DIR)/bin/pc

mk_partition_clean:
	$(MAKE) -C $(PARTITION_ENV_DIR)/tool/ clean

nand_production: secure_libs_pc
	@echo -e $(GREEN)"---------task [7.7] build nand_product"  $(DONE)
	@cp $(OSDRV_DIR)/secure_c/libs/libsecurec-pc.a $(PC_TOOLS_DIR)/nand_production/fmc_nand_ecc_product_v100
	@cp $(OSDRV_DIR)/secure_c/secure_c_pc/include/*.h $(PC_TOOLS_DIR)/nand_production/fmc_nand_ecc_product_v100
	$(MAKE) -C $(PC_TOOLS_DIR)/nand_production/fmc_nand_ecc_product_v100
	@cp $(PC_TOOLS_DIR)/nand_production/fmc_nand_ecc_product_v100/nand_product $(IMAGES_DIR)/bin/pc

nand_product_clean:
	$(MAKE) -C $(PC_TOOLS_DIR)/nand_production/fmc_nand_ecc_product_v100 clean

zlib_board:
	@echo -e $(GREEN)"---------task [8.1] build zlib_board"  $(DONE)
	$(MAKE) -C $(ZLIB_DIR) LLVM=$(LLVM) OSDRV_CROSS=$(OSDRV_CROSS) zlib_board

mtd_utils_board: prepare zlib_board
	@echo -e $(GREEN)"---------task [8.2] build mtd_utils_board"  $(DONE)
	$(MAKE) -C $(MTD_DIR) LLVM=$(LLVM) OSDRV_CROSS=$(OSDRV_CROSS) mtd_board
	@cp $(MTD_DIR)/out/board/bin/* $(IMAGES_DIR)/bin/$(PUB_BOARD)

ethtool_board: prepare
	@echo -e $(GREEN)"---------task [8.3] build ethtool_board"  $(DONE)
	$(MAKE) -C $(ETHTOOL_DIR) LLVM=$(LLVM) OSDRV_CROSS=$(OSDRV_CROSS) all
	@cp $(ETHTOOL_DIR)/bin/sbin/ethtool $(IMAGES_DIR)/bin/$(PUB_BOARD) -rf

ethtool_clean:
	$(MAKE) -C $(ETHTOOL_DIR) clean

e2fsprogs_board: prepare
	@echo -e $(GREEN)"---------task [8.3] build e2fsprogs_board"  $(DONE)
	$(MAKE) -C $(E2FSPROGS_DIR) LLVM=$(LLVM) OSDRV_CROSS=$(OSDRV_CROSS) e2fsprogs_board
	@cp $(E2FSPROGS_DIR)/out/board/sbin/mkfs.ext2 $(IMAGES_DIR)/bin/$(PUB_BOARD) -rf
	@cp $(E2FSPROGS_DIR)/out/board/sbin/mkfs.ext3 $(IMAGES_DIR)/bin/$(PUB_BOARD) -rf
	@cp $(E2FSPROGS_DIR)/out/board/sbin/mkfs.ext4 $(IMAGES_DIR)/bin/$(PUB_BOARD) -rf
	@cp $(E2FSPROGS_DIR)/out/board/sbin/mke2fs $(IMAGES_DIR)/bin/$(PUB_BOARD) -rf

vendor_tools: prepare
	@echo -e $(GREEN)"---------task [8.4] build vendor_tools"  $(DONE)
	$(MAKE) -C $(VENDOR_TOOLS) LLVM=$(LLVM) OSDRV_CROSS=$(OSDRV_CROSS) all

vendor_tools_clean:
	$(MAKE) -C $(VENDOR_TOOLS) clean

eudev_board: prepare
	@echo -e $(GREEN)"---------task [8.5] build eudev_tools"  $(DONE)
	$(MAKE) -C $(EUDEV_DIR) LLVM=$(LLVM) OSDRV_CROSS=$(OSDRV_CROSS)
	@cp -af $(EUDEV_DIR)/rootfs/. $(IMAGES_DIR)/$(PUB_ROOTFS)/

eudev_clean:
	$(MAKE) -C $(EUDEV_DIR) clean

xml2_board: prepare
	@echo -e $(GREEN)"---------task [8.6] build xml2_board"  $(DONE)
	$(MAKE) -C $(XML2_DIR) LLVM=$(LLVM) OSDRV_CROSS=$(OSDRV_CROSS) xml2_board

xml2_clean:
	$(MAKE) -C $(XML2_DIR) LLVM=$(LLVM) OSDRV_CROSS=$(OSDRV_CROSS) clean

##########################################################################################
#task [8]	build board tools
##########################################################################################
ifneq ($(QUICKBOOT_BUILD), y)
boardtools: prepare secure_libs_board mtd_utils_board ethtool_board e2fsprogs_board vendor_tools eudev_board xml2_board
else
boardtools: prepare secure_libs_board mtd_utils_board ethtool_board e2fsprogs_board vendor_tools xml2_board
endif
	@echo -e $(GREEN)"---------task [8] build tools which run on board "  $(DONE)

##########################################################################################
#task [9]	build rootfs
##########################################################################################
rootfs_build: busybox mtd_utils_board gzip
	@echo -e $(GREEN)"---------task [9] build rootfs"  $(DONE)
	@rm $(IMAGES_DIR)/$(PUB_ROOTFS)/dev/* -rf

	@pushd $(IMAGES_DIR)/$(PUB_ROOTFS); ln -s sbin/init init; popd
	@pushd $(IMAGES_DIR)/$(PUB_ROOTFS); chmod go-x * -R; chmod go-w * -R;chmod go-r * -R; popd
	@pushd $(IMAGES_DIR)/$(PUB_ROOTFS); chmod 4755 bin/su; popd
	@pushd $(IMAGES_DIR)/$(PUB_ROOTFS); chmod 4555 bin/login usr/bin/passwd; popd
	@pushd $(IMAGES_DIR)/$(PUB_ROOTFS); chmod 2555 usr/bin/wall; popd
	@pushd $(IMAGES_DIR)/$(PUB_ROOTFS); chmod 1777 tmp; popd
	@pushd $(IMAGES_DIR)/$(PUB_ROOTFS); chmod 755 bin etc lib opt proc usr usr/bin usr/bin/crontab sbin sbin/arp usr/lib var; popd
	@pushd $(IMAGES_DIR)/$(PUB_ROOTFS); chmod 750 boot etc/init.d etc/init.d/*; popd
	@pushd $(IMAGES_DIR)/$(PUB_ROOTFS); chmod 644 etc/group etc/inittab etc/mtab etc/passwd etc/profile etc/protocols etc/services etc/shadow root/.profile; popd
	@pushd $(IMAGES_DIR)/$(PUB_ROOTFS); chmod 640 var/run/utmp; popd
	@pushd $(IMAGES_DIR)/$(PUB_ROOTFS); chmod 550 bin/mount bin/netstat sbin/route; popd

ifeq ($(QUICKBOOT_BUILD), y)
	@pushd $(IMAGES_DIR)/$(PUB_ROOTFS);cp $(SAMPLE_DIR)/$(QUICKBOOT_SAMPLE) usr/bin/ -rf; popd
	@pushd $(IMAGES_DIR)/$(PUB_ROOTFS);$(MAIN_BUILD_DIR)/get_ompmod.sh $(KO_DIR)/load_lq560v100_quickboot_stage_1 $(MEDIA_KO_DIR_NAME); popd
endif
	@chmod +x $(PC_TOOLS_DIR)/compress/kernel_compress.sh
ifeq ($(BOOT_MEDIA), spi)
	@cp ${OPEN_SOURCE}/mtd-utils/out/board/bin/ubiattach $(IMAGES_DIR)/$(PUB_ROOTFS)/usr/bin/ -vp
endif
	@cp $(PC_TOOLS_DIR)/romfs/genromfs $(IMAGES_DIR)/bin/pc
	@pushd $(IMAGES_DIR)/bin/pc; ./genromfs -d $(IMAGES_DIR)/$(PUB_ROOTFS) -f $(BOOT_IMAGE_TOOL_INPUT_DIR)/rootfs.ramdisk; popd
	@pushd $(IMAGES_DIR)/$(PUB_IMAGE); $(PC_TOOLS_DIR)/compress/kernel_compress.sh $(BOOT_IMAGE_TOOL_INPUT_DIR)/rootfs.ramdisk; popd
	@python $(SYSTEM_IMAGE_TOOL_SCRIPT) $(BOOT_IMAGE_TOOL_INPUT_DIR)/rootfs.ramdisk.gz $(IMAGES_DIR)/$(PUB_IMAGE)/$(ROMFS_IMAGE_BIN)

rootfs_clean:
	${MAKE} -C $(OSDRV_DIR)/fs/rootfs/ clean
ifeq ($(IMAGES_DIR)/$(PUB_ROOTFS), $(wildcard $(IMAGES_DIR)/$(PUB_ROOTFS)))
	@pushd $(IMAGES_DIR)/$(PUB_ROOTFS); chmod +w usr/bin; chmod +w usr/sbin; chmod +w sbin; popd
endif
	@rm $(IMAGES_DIR)/$(PUB_ROOTFS)/ -rf
	@rm $(BOOT_IMAGE_TOOL_INPUT_DIR)/rootfs.ramdisk.gz -f

##########################################################################################
#task [11]	build otfl.img
##########################################################################################
securecfg_prepare:
	cp $(IMAGE_TOOL_CONFIG) $(BOOT_IMAGE_TOOL_DIR)/oem/quick_build_secure_boot_config.json -f

ifeq ($(SECUREBOOT_BUILD), y)
otflboot_build: boot securecfg_prepare
else
otflboot_build: boot
endif
	@echo -e $(GREEN)"---------task [11] build otfl.img"  $(DONE)
	@pushd $(OSDRV_DIR)/../boot/otfl; $(MAKE) clean; $(MAKE) CHIP=$(CHIP) LLVM=$(LLVM); popd
	@cp $(BOOT_TOOLS_DIR)/$(BOOT_REG_BIN) $(BOOT_IMAGE_TOOL_INPUT_DIR)/reg_info.bin
	@pushd $(MISC_TOOL_DIR); python $(MISC_TOOL_SCRIPT) misc.bin; popd
	@cp $(MISC_TOOL_DIR)/misc.bin $(IMAGES_DIR)/$(PUB_IMAGE)/misc.bin

ifeq ($(BOOT_MEDIA), spi)
	@cp $(OSDRV_DIR)/../boot/otfl/pub/otfl.bin $(BOOT_IMAGE_TOOL_INPUT_DIR)
	@cp $(BOOT_IMAGE_TOOL_INPUT_DIR)/nand$(PARTITION_ENV_INFIX)_partition.bin  $(BOOT_IMAGE_TOOL_INPUT_DIR)/partition.bin
	@cp $(UBOOT_DIR)/$(UBOOT_VER)/u-boot-$(CHIP)_spinand.bin $(BOOT_IMAGE_TOOL_INPUT_DIR)/u-boot-original.bin
	@pushd $(BOOT_IMAGE_TOOL_DIR); python $(IMAGE_TOOL_SCRIPT); popd

	@cp $(OTFL_IMAGE_TOOL_OUTPUT_FILE) $(IMAGES_DIR)/$(PUB_IMAGE)/$(OTFL_NAND_IMAGE_BIN)
	@cp $(UBOOT_IMAGE_TOOL_OUTPUT_FILE) $(IMAGES_DIR)/$(PUB_IMAGE)/$(UBOOT_NAND_IMAGE_BIN)

	@cp $(OSDRV_DIR)/../boot/otfl/pub/otfl.bin $(BOOT_IMAGE_TOOL_INPUT_DIR)
	@cp $(BOOT_IMAGE_TOOL_INPUT_DIR)/nor$(PARTITION_ENV_INFIX)_partition.bin  $(BOOT_IMAGE_TOOL_INPUT_DIR)/partition.bin
	@cp $(UBOOT_DIR)/$(UBOOT_VER)/u-boot-$(CHIP)_spinor.bin $(BOOT_IMAGE_TOOL_INPUT_DIR)/u-boot-original.bin
	@pushd $(BOOT_IMAGE_TOOL_DIR); python $(IMAGE_TOOL_SCRIPT); popd

	@cp $(OTFL_IMAGE_TOOL_OUTPUT_FILE) $(IMAGES_DIR)/$(PUB_IMAGE)/$(OTFL_NOR_IMAGE_BIN)
	@cp $(UBOOT_IMAGE_TOOL_OUTPUT_FILE) $(IMAGES_DIR)/$(PUB_IMAGE)/$(UBOOT_NOR_IMAGE_BIN)

else
	@cp $(UBOOT_DIR)/$(UBOOT_VER)/u-boot-$(CHIP).bin $(BOOT_IMAGE_TOOL_INPUT_DIR)/u-boot-original.bin
	@cp $(OSDRV_DIR)/../boot/otfl/pub/otfl.bin $(BOOT_IMAGE_TOOL_INPUT_DIR)
	@cp $(BOOT_IMAGE_TOOL_INPUT_DIR)/emmc$(PARTITION_ENV_INFIX)_partition.bin  $(BOOT_IMAGE_TOOL_INPUT_DIR)/partition.bin
	@cp $(UBOOT_DIR)/$(UBOOT_VER)/u-boot-$(CHIP).bin $(BOOT_IMAGE_TOOL_INPUT_DIR)/u-boot-original.bin
	@pushd $(BOOT_IMAGE_TOOL_DIR); python $(IMAGE_TOOL_SCRIPT); popd

	@cp $(OTFL_IMAGE_TOOL_OUTPUT_FILE) $(IMAGES_DIR)/$(PUB_IMAGE)/$(OTFL_EMMC_IMAGE_BIN)
	@cp $(UBOOT_IMAGE_TOOL_OUTPUT_FILE) $(IMAGES_DIR)/$(PUB_IMAGE)/$(UBOOT_EMMC_IMAGE_BIN)
endif

ifeq ($(SECUREBOOT_BUILD), y)
	@mkimage -T script -C none -n 'OTP Merge Write' -d \
        $(BOOT_IMAGE_TOOL_DIR)/oem/tmp/otp_cmd/secure_boot_all_merge_otp_wr_cmd.txt \
        $(IMAGES_DIR)/$(PUB_IMAGE)/secure_boot_all_merge_otp_wr_cmd.script
	@mkimage -T script -C none -n 'OTP Pub Hash Write' -d \
        $(BOOT_IMAGE_TOOL_DIR)/oem/tmp/otp_cmd/ree_root_public_key_sha256_to_otp_wr_cmd.txt \
        $(IMAGES_DIR)/$(PUB_IMAGE)/ree_root_public_key_sha256_to_otp_wr_cmd.script
	@mkimage -T script -C none -n 'OTP obfu Write' -d \
        $(BOOT_IMAGE_TOOL_DIR)/oem/tmp/otp_cmd/obfu_mark1_otp_wr_cmd.txt \
        $(IMAGES_DIR)/$(PUB_IMAGE)/obfu_mark1_otp_wr_cmd.script
	@mkimage -T script -C none -n 'OTP SB Enable Write' -d \
        $(BOOT_IMAGE_TOOL_DIR)/oem/tmp/otp_cmd/secure_boot_verify_enable_otp_wr_cmd.txt \
        $(IMAGES_DIR)/$(PUB_IMAGE)/secure_boot_verify_enable_otp_wr_cmd.script
	@mkimage -T script -C none -n 'OTP system version Write' -d \
        $(BOOT_IMAGE_TOOL_DIR)/oem/tmp/otp_cmd/system_rollback_otp_wr_cmd.txt \
        $(IMAGES_DIR)/$(PUB_IMAGE)/system_rollback_otp_wr_cmd.script
	@mkimage -T script -C none -n 'OTP system version Write' -d \
        $(BOOT_IMAGE_TOOL_DIR)/oem/tmp/otp_cmd/otfl_rollback_otp_wr_cmd.txt \
        $(IMAGES_DIR)/$(PUB_IMAGE)/otfl_rollback_otp_wr_cmd.script
	@if [ -e $(BOOT_IMAGE_TOOL_DIR)/oem/tmp/otp_cmd/sm2_sm3_sm4_argorithm_and_rpk_DEOB_sm4_write_cmd.txt ]; then \
		mkimage -T script -C none -n 'OTP system version Write' -d \
        $(BOOT_IMAGE_TOOL_DIR)/oem/tmp/otp_cmd/sm2_sm3_sm4_argorithm_and_rpk_DEOB_sm4_write_cmd.txt \
        $(IMAGES_DIR)/$(PUB_IMAGE)/sm2_sm3_sm4_argorithm_and_rpk_DEOB_sm4_write_cmd.script;\
	fi

endif
	@echo -e $(GREEN)"---------finish bootimage work"  $(DONE)

otflboot_clean:
	@pushd $(OSDRV_DIR)/../boot/otfl; ${MAKE} clean; popd
	@rm $(BOOT_IMAGE_TOOL_DIR)/image -rf
	@rm $(BOOT_IMAGE_TOOL_DIR)/oem/tmp -rf
	@rm $(BOOT_IMAGE_TOOL_INPUT_DIR)/reg_info.bin -f
	@rm $(BOOT_IMAGE_TOOL_INPUT_DIR)/u-boot-original.bin -f
	@rm $(BOOT_IMAGE_TOOL_INPUT_DIR)/otfl.bin -f
	@rm $(BOOT_IMAGE_TOOL_INPUT_DIR)/*partition.bin -f
	@find $(BOOT_IMAGE_TOOL_DIR) -name '*.pyc' | xargs rm -rf

##########################################################################################
#task [12]	build partition.bin & u-boot env
##########################################################################################
partition_uboot_env: prepare mk_partition_pc
	@echo -e $(GREEN)"---------task [12] build partition.bin & u-boot env"  $(DONE)
	@mkdir -p $(IMAGES_DIR)/uboot_env/$(CHIP)/
ifeq ($(BOOT_MEDIA),emmc)
ifeq ($(QUICKBOOT_BUILD), y)
ifeq ($(SECUREBOOT_BUILD), y)
	$(IMAGES_DIR)/bin/pc/mk_partition_env ${PARTITION_XML_DIR}/emmc$(PARTITION_ENV_INFIX)_squickboot.xml \
											$(BOOT_IMAGE_TOOL_INPUT_DIR)/emmc$(PARTITION_ENV_INFIX)_partition.bin \
											$(IMAGES_DIR)/uboot_env/$(CHIP)/emmc$(PARTITION_ENV_INFIX)_squickboot.env
	@cp ${PARTITION_XML_DIR}/emmc$(PARTITION_ENV_INFIX)_squickboot.xml $(IMAGES_DIR)/$(PUB_IMAGE)/emmc$(PARTITION_ENV_INFIX)_burn_table.xml
else
	$(IMAGES_DIR)/bin/pc/mk_partition_env ${PARTITION_XML_DIR}/emmc$(PARTITION_ENV_INFIX)_quickboot.xml \
											$(BOOT_IMAGE_TOOL_INPUT_DIR)/emmc$(PARTITION_ENV_INFIX)_partition.bin \
											$(IMAGES_DIR)/uboot_env/$(CHIP)/emmc$(PARTITION_ENV_INFIX)_quickboot.env
	@cp ${PARTITION_XML_DIR}/emmc$(PARTITION_ENV_INFIX)_quickboot.xml $(IMAGES_DIR)/$(PUB_IMAGE)/emmc$(PARTITION_ENV_INFIX)_burn_table.xml
endif
else
ifeq ($(SECUREBOOT_BUILD), y)
	$(IMAGES_DIR)/bin/pc/mk_partition_env ${PARTITION_XML_DIR}/emmc$(PARTITION_ENV_INFIX)_secure_boot.xml \
											$(BOOT_IMAGE_TOOL_INPUT_DIR)/emmc$(PARTITION_ENV_INFIX)_partition.bin \
											$(IMAGES_DIR)/uboot_env/$(CHIP)/emmc$(PARTITION_ENV_INFIX)_secure_boot.env
	@cp ${PARTITION_XML_DIR}/emmc$(PARTITION_ENV_INFIX)_secure_boot.xml $(IMAGES_DIR)/$(PUB_IMAGE)/emmc$(PARTITION_ENV_INFIX)_burn_table.xml
else
	$(IMAGES_DIR)/bin/pc/mk_partition_env ${PARTITION_XML_DIR}/emmc$(PARTITION_ENV_INFIX).xml \
											$(BOOT_IMAGE_TOOL_INPUT_DIR)/emmc$(PARTITION_ENV_INFIX)_partition.bin \
											$(IMAGES_DIR)/uboot_env/$(CHIP)/emmc$(PARTITION_ENV_INFIX).env
	${MAKE} -C $(PARTITION_ENV_DIR) ENVTXT=$(IMAGES_DIR)/uboot_env/$(CHIP)/emmc$(PARTITION_ENV_INFIX).env
	@cp ${PARTITION_XML_DIR}/emmc$(PARTITION_ENV_INFIX).xml $(IMAGES_DIR)/$(PUB_IMAGE)/emmc$(PARTITION_ENV_INFIX)_burn_table.xml
endif
endif
else
ifeq ($(QUICKBOOT_BUILD), y)
ifeq ($(SECUREBOOT_BUILD), y)
	$(IMAGES_DIR)/bin/pc/mk_partition_env ${PARTITION_XML_DIR}/nand$(PARTITION_ENV_INFIX)_squickboot.xml \
											$(BOOT_IMAGE_TOOL_INPUT_DIR)/nand$(PARTITION_ENV_INFIX)_partition.bin \
											$(IMAGES_DIR)/uboot_env/$(CHIP)/nand$(PARTITION_ENV_INFIX)_squickboot.env
	@cp ${PARTITION_XML_DIR}/nand$(PARTITION_ENV_INFIX)_squickboot.xml $(IMAGES_DIR)/$(PUB_IMAGE)/nand$(PARTITION_ENV_INFIX)_burn_table.xml

	$(IMAGES_DIR)/bin/pc/mk_partition_env ${PARTITION_XML_DIR}/nor$(PARTITION_ENV_INFIX)_squickboot.xml \
											$(BOOT_IMAGE_TOOL_INPUT_DIR)/nor$(PARTITION_ENV_INFIX)_partition.bin \
											$(IMAGES_DIR)/uboot_env/$(CHIP)/nor$(PARTITION_ENV_INFIX)_squickboot.env
	@cp ${PARTITION_XML_DIR}/nor$(PARTITION_ENV_INFIX)_squickboot.xml $(IMAGES_DIR)/$(PUB_IMAGE)/nor$(PARTITION_ENV_INFIX)_burn_table.xml
else
	$(IMAGES_DIR)/bin/pc/mk_partition_env ${PARTITION_XML_DIR}/nand$(PARTITION_ENV_INFIX)_quickboot.xml \
											$(BOOT_IMAGE_TOOL_INPUT_DIR)/nand$(PARTITION_ENV_INFIX)_partition.bin \
											$(IMAGES_DIR)/uboot_env/$(CHIP)/nand$(PARTITION_ENV_INFIX)_quickboot.env
	@cp ${PARTITION_XML_DIR}/nand$(PARTITION_ENV_INFIX)_quickboot.xml $(IMAGES_DIR)/$(PUB_IMAGE)/nand$(PARTITION_ENV_INFIX)_burn_table.xml

	$(IMAGES_DIR)/bin/pc/mk_partition_env ${PARTITION_XML_DIR}/nor$(PARTITION_ENV_INFIX)_quickboot.xml \
											$(BOOT_IMAGE_TOOL_INPUT_DIR)/nor$(PARTITION_ENV_INFIX)_partition.bin \
											$(IMAGES_DIR)/uboot_env/$(CHIP)/nor$(PARTITION_ENV_INFIX)_quickboot.env
	@cp ${PARTITION_XML_DIR}/nor$(PARTITION_ENV_INFIX)_quickboot.xml $(IMAGES_DIR)/$(PUB_IMAGE)/nor$(PARTITION_ENV_INFIX)_burn_table.xml
endif
else
ifeq ($(SECUREBOOT_BUILD), y)
	$(IMAGES_DIR)/bin/pc/mk_partition_env ${PARTITION_XML_DIR}/nand$(PARTITION_ENV_INFIX)_secure_boot.xml \
											$(BOOT_IMAGE_TOOL_INPUT_DIR)/nand$(PARTITION_ENV_INFIX)_partition.bin \
											$(IMAGES_DIR)/uboot_env/$(CHIP)/nand$(PARTITION_ENV_INFIX)_secure_boot.env
	@cp ${PARTITION_XML_DIR}/nand$(PARTITION_ENV_INFIX)_secure_boot.xml $(IMAGES_DIR)/$(PUB_IMAGE)/nand$(PARTITION_ENV_INFIX)_burn_table.xml

	$(IMAGES_DIR)/bin/pc/mk_partition_env ${PARTITION_XML_DIR}/nor$(PARTITION_ENV_INFIX)_secure_boot.xml \
											$(BOOT_IMAGE_TOOL_INPUT_DIR)/nor$(PARTITION_ENV_INFIX)_partition.bin \
											$(IMAGES_DIR)/uboot_env/$(CHIP)/nor$(PARTITION_ENV_INFIX)_secure_boot.env
	@cp ${PARTITION_XML_DIR}/nor$(PARTITION_ENV_INFIX)_secure_boot.xml $(IMAGES_DIR)/$(PUB_IMAGE)/nor$(PARTITION_ENV_INFIX)_burn_table.xml

else
	$(IMAGES_DIR)/bin/pc/mk_partition_env ${PARTITION_XML_DIR}/nand$(PARTITION_ENV_INFIX).xml \
											$(BOOT_IMAGE_TOOL_INPUT_DIR)/nand$(PARTITION_ENV_INFIX)_partition.bin \
											$(IMAGES_DIR)/uboot_env/$(CHIP)/nand$(PARTITION_ENV_INFIX).env
	${MAKE} -C $(PARTITION_ENV_DIR) ENVTXT=$(IMAGES_DIR)/uboot_env/$(CHIP)/nand$(PARTITION_ENV_INFIX).env
	@cp ${PARTITION_XML_DIR}/nand$(PARTITION_ENV_INFIX).xml $(IMAGES_DIR)/$(PUB_IMAGE)/nand$(PARTITION_ENV_INFIX)_burn_table.xml

	$(IMAGES_DIR)/bin/pc/mk_partition_env ${PARTITION_XML_DIR}/nor$(PARTITION_ENV_INFIX).xml \
											$(BOOT_IMAGE_TOOL_INPUT_DIR)/nor$(PARTITION_ENV_INFIX)_partition.bin \
											$(IMAGES_DIR)/uboot_env/$(CHIP)/nor$(PARTITION_ENV_INFIX).env
	${MAKE} -C $(PARTITION_ENV_DIR) ENVTXT=$(IMAGES_DIR)/uboot_env/$(CHIP)/nor$(PARTITION_ENV_INFIX).env
	@cp ${PARTITION_XML_DIR}/nor$(PARTITION_ENV_INFIX).xml $(IMAGES_DIR)/$(PUB_IMAGE)/nor$(PARTITION_ENV_INFIX)_burn_table.xml
endif
endif
endif

partition_uboot_env_clean:
	${MAKE} -C $(PC_TOOLS_DIR)/partition_uboot_env/ clean
##########################################################################################

#task [13]	clean pub
##########################################################################################
pub_clean:
	@rm $(IMAGES_DIR)/* -rf

##########################################################################################
#task [14]	userfs build
##########################################################################################
userfs_build: mtd_utils_board xml2_board e2fsprogs_board e2fsprogs_pc mtd_utils_pc
	@echo -e $(GREEN)"---------task [14] build userfs build"  $(DONE)
	$(MAKE) -C $(OSDRV_DIR)/fs/userfs all
	##install kernel modules
	@cp -rPf ${KERNEL_DIR}/${KERNEL_VER}/modules/kermod $(OSDRV_DIR)/fs/userfs/userfs
	@mkdir -p $(IMAGES_DIR)/$(PUB_USERFS)
	##clean PUB_USERFS
	@rm -rf $(IMAGES_DIR)/$(PUB_USERFS)/*
	@mkdir -p $(IMAGES_DIR)/$(PUB_USERFS)/bin/
	@mkdir -p $(IMAGES_DIR)/$(PUB_USERFS)/lib/
ifeq ($(BOOT_MEDIA),spi)
	@cp $(PC_TOOLS_DIR)/ubi_sh/mkubiimg.sh $(IMAGES_DIR)/$(PUB_IMAGE)
	@chmod +x $(IMAGES_DIR)/$(PUB_IMAGE)/mkubiimg.sh
	@cp $(OPEN_SOURCE_BOARD_TOOLS_DIR)/ubiformat $(IMAGES_DIR)/$(PUB_USERFS)/bin/ -rf
	@cp $(OPEN_SOURCE_BOARD_TOOLS_DIR)/ubimkvol $(IMAGES_DIR)/$(PUB_USERFS)/bin/ -rf
	@cp $(OPEN_SOURCE_BOARD_TOOLS_DIR)/ubidetach $(IMAGES_DIR)/$(PUB_USERFS)/bin/ -rf
	@cp $(OPEN_SOURCE_BOARD_TOOLS_DIR)/flash_erase $(IMAGES_DIR)/$(PUB_USERFS)/bin/ -rf
	@cp $(OPEN_SOURCE_BOARD_TOOLS_DIR)/flash_eraseall $(IMAGES_DIR)/$(PUB_USERFS)/bin/ -rf
	@chmod u+x $(IMAGES_DIR)/$(PUB_USERFS)/bin/*
endif
ifeq ($(BOOT_MODE),ab)
	@cp $(MAIN_DIR)/src/component/upgrade/sample/sample_upgrade $(IMAGES_DIR)/$(PUB_USERFS)/bin/ -rf
endif
	@cp $(MAIN_DIR)/../opensource/zlib/out/board/lib/lib*.so* $(IMAGES_DIR)/$(PUB_USERFS)/lib/ -rf
	@cp $(MAIN_DIR)/../opensource/xml2/out/board/lib/lib*.so* $(IMAGES_DIR)/$(PUB_USERFS)/lib/ -rf
	@cp $(OPEN_SOURCE_BOARD_TOOLS_DIR)/mkfs.ext4 $(IMAGES_DIR)/$(PUB_USERFS)/bin/ -rf

	@cp $(MAIN_OUT_DIR)/lib/lib*.so  $(IMAGES_DIR)/$(PUB_USERFS)/lib/ -rf
	@cp ${OSDRV_DIR}/fs/userfs/userfs/* $(IMAGES_DIR)/$(PUB_USERFS) -rfP

ifeq ($(QUICKBOOT_BUILD), y)
	@pushd $(IMAGES_DIR)/$(PUB_USERFS);$(MAIN_BUILD_DIR)/get_ompmod.sh $(KO_DIR)/load_lq560v100_quickboot_stage_2 $(MEDIA_KO_DIR_NAME); popd
else
	@pushd $(IMAGES_DIR)/$(PUB_USERFS);$(MAIN_BUILD_DIR)/get_ompmod.sh $(KO_DIR)/load_lq560v100 $(MEDIA_KO_DIR_NAME); popd
endif
ifeq ($(BOOT_MEDIA),emmc)
	@dd if=/dev/zero of=$(IMAGES_DIR)/$(PUB_IMAGE)/$(EXT4_IMAGE_BIN) bs=512 count=$(COUNT_SIZE)
	$(IMAGES_DIR)/bin/pc/$(EXT4_TOOL) $(IMAGES_DIR)/$(PUB_IMAGE)/$(EXT4_IMAGE_BIN)
	@pushd $(E2FSPROGS_DIR)/out/pc/contrib; ./populate-extfs.sh $(IMAGES_DIR)/$(PUB_USERFS) $(IMAGES_DIR)/$(PUB_IMAGE)/$(EXT4_IMAGE_BIN); popd
else
	##@pushd $(IMAGES_DIR)/$(PUB_USERFS);cp $(SAMPLE_DIR) ./ -rf; popd
	# build the pagesize = 2k, blocksize = 128k, part_size = 32M #
	@pushd $(IMAGES_DIR)/$(PUB_IMAGE); ./mkubiimg.sh $(CHIP) 2k 128k $(IMAGES_DIR)/$(PUB_USERFS) 32M $(IMAGES_DIR)/bin/pc 0 userfs; popd
	# build the pagesize = 4k, blocksize = 256k, part_size = 32M #
	@pushd $(IMAGES_DIR)/$(PUB_IMAGE); ./mkubiimg.sh $(CHIP) 4k 256k $(IMAGES_DIR)/$(PUB_USERFS) 32M $(IMAGES_DIR)/bin/pc 0 userfs; popd

	@pushd $(IMAGES_DIR)/bin/pc; ./mkfs.jffs2 -d $(IMAGES_DIR)/$(PUB_USERFS) -l -e 0x40000 -o $(IMAGES_DIR)/$(PUB_IMAGE)/$(JFFS2_IMAGE_BIN_256K); popd
	@pushd $(IMAGES_DIR)/bin/pc; ./mkfs.jffs2 -d $(IMAGES_DIR)/$(PUB_USERFS) -l -e 0x20000 -o $(IMAGES_DIR)/$(PUB_IMAGE)/$(JFFS2_IMAGE_BIN_128K); popd
	@pushd $(IMAGES_DIR)/bin/pc; ./mkfs.jffs2 -d $(IMAGES_DIR)/$(PUB_USERFS) -l -e 0x10000 -o $(IMAGES_DIR)/$(PUB_IMAGE)/$(JFFS2_IMAGE_BIN_64K); popd
	@rm $(IMAGES_DIR)/$(PUB_IMAGE)/mkubiimg.sh -rf
endif

userfs_clean:
	@rm $(IMAGES_DIR)/$(PUB_USERFS)/ -rf
	$(MAKE) -C $(OSDRV_DIR)/fs/userfs clean

##########################################################################################
#task [15]	datafs build
##########################################################################################
datafs_build: prepare mtd_utils_pc
	@echo -e $(GREEN)"---------task [15] build datafs"  $(DONE)
	$(MAKE) -C $(OSDRV_DIR)/fs/datafs all
	@mkdir -p $(IMAGES_DIR)/$(PUB_DATAFS)
	@mkdir -p $(MAIN_OUT_DIR)/data
	##clean PUB_DATAFS
	@rm -rf $(IMAGES_DIR)/$(PUB_DATAFS)/*
	@$(call copy_if_not_empty, $(MAIN_OUT_DIR)/data, $(IMAGES_DIR)/$(PUB_DATAFS))
	@$(call copy_if_not_empty, ${OSDRV_DIR}/fs/datafs/datafs, $(IMAGES_DIR)/$(PUB_DATAFS))

ifeq ($(BOOT_MEDIA),emmc)
	@dd if=/dev/zero of=$(IMAGES_DIR)/$(PUB_IMAGE)/$(DATAFS_EXT4_IMAGE_BIN) bs=512 count=$(DATAFS_COUNT_SIZE)
	$(IMAGES_DIR)/bin/pc/$(EXT4_TOOL) $(IMAGES_DIR)/$(PUB_IMAGE)/$(DATAFS_EXT4_IMAGE_BIN)
	@pushd $(E2FSPROGS_DIR)/out/pc/contrib; ./populate-extfs.sh $(IMAGES_DIR)/$(PUB_DATAFS) $(IMAGES_DIR)/$(PUB_IMAGE)/$(DATAFS_EXT4_IMAGE_BIN); popd
else
	@cp $(PC_TOOLS_DIR)/ubi_sh/mkubiimg.sh $(IMAGES_DIR)/$(PUB_IMAGE)
	@chmod +x $(IMAGES_DIR)/$(PUB_IMAGE)/mkubiimg.sh
	# build the pagesize = 2k, blocksize = 128k, part_size = 28M #
	@pushd $(IMAGES_DIR)/$(PUB_IMAGE); ./mkubiimg.sh $(CHIP) 2k 128k $(IMAGES_DIR)/$(PUB_DATAFS) 27M $(IMAGES_DIR)/bin/pc 0 datafs; popd
	# build the pagesize = 4k, blocksize = 256k, part_size = 28M #
	@pushd $(IMAGES_DIR)/$(PUB_IMAGE); ./mkubiimg.sh $(CHIP) 4k 256k $(IMAGES_DIR)/$(PUB_DATAFS) 27M $(IMAGES_DIR)/bin/pc 0 datafs; popd

	@pushd $(IMAGES_DIR)/bin/pc; ./mkfs.jffs2 -d $(IMAGES_DIR)/$(PUB_DATAFS) -l -e 0x40000 -o $(IMAGES_DIR)/$(PUB_IMAGE)/$(DATAFS_JFFS2_IMAGE_BIN_256K); popd
	@pushd $(IMAGES_DIR)/bin/pc; ./mkfs.jffs2 -d $(IMAGES_DIR)/$(PUB_DATAFS) -l -e 0x20000 -o $(IMAGES_DIR)/$(PUB_IMAGE)/$(DATAFS_JFFS2_IMAGE_BIN_128K); popd
	@pushd $(IMAGES_DIR)/bin/pc; ./mkfs.jffs2 -d $(IMAGES_DIR)/$(PUB_DATAFS) -l -e 0x10000 -o $(IMAGES_DIR)/$(PUB_IMAGE)/$(DATAFS_JFFS2_IMAGE_BIN_64K); popd
	@rm $(IMAGES_DIR)/$(PUB_IMAGE)/mkubiimg.sh -rf
endif

datafs_clean:
	@rm $(IMAGES_DIR)/$(PUB_DATAFS)/ -rf
	$(MAKE) -C $(OSDRV_DIR)/fs/datafs clean

##########################################################################################
#task [16]	build upgrade image
##########################################################################################
upgrade: prepare
	@echo -e $(GREEN)"---------task [16] build upgrade"  $(DONE)
	$(eval common_imgs="$(KERNEL_IMAGE_BIN)" "$(ROMFS_IMAGE_BIN)" "$(ATF_IMAGE_BIN)" "upgrade.xml")
ifeq ($(BOOT_MEDIA),emmc)
	$(eval upgrade_imgs=$(common_imgs)  "userfs_$(CHIP)_96M.ext4" "$(UBOOT_EMMC_IMAGE_BIN)")
	@pushd $(IMAGES_DIR)/$(PUB_IMAGE)/;\
	rm -f $(UPGRADE_IMAGE);cp emmc$(PARTITION_ENV_INFIX)_burn_table.xml upgrade.xml;zip -r emmc_$(UPGRADE_IMAGE) $(upgrade_imgs);\
	popd;
	@python $(UPGRADE_IMAGE_TOOL_SCRIPT) $(IMAGES_DIR)/$(PUB_IMAGE)/emmc_$(UPGRADE_IMAGE);
ifeq ($(SECUREBOOT_BUILD), y)
	@mv $(IMAGES_DIR)/$(PUB_IMAGE)/emmc_$(UPGRADE_IMAGE).img $(IMAGES_DIR)/$(PUB_IMAGE)/emmc_$(UPGRADE_IMAGE).img.sig;
endif
else
	$(eval upgrade_imgs=$(common_imgs)  "userfs_$(CHIP)_2k_128k_32M.ubifs" "$(UBOOT_NAND_IMAGE_BIN)")
	@pushd $(IMAGES_DIR)/$(PUB_IMAGE)/;\
	rm -f $(UPGRADE_IMAGE);cp nand$(PARTITION_ENV_INFIX)_burn_table.xml upgrade.xml;zip -r nand_$(UPGRADE_IMAGE) $(upgrade_imgs);\
	popd;
	@python $(UPGRADE_IMAGE_TOOL_SCRIPT) $(IMAGES_DIR)/$(PUB_IMAGE)/nand_$(UPGRADE_IMAGE);
ifeq ($(SECUREBOOT_BUILD), y)
	@mv $(IMAGES_DIR)/$(PUB_IMAGE)/nand_$(UPGRADE_IMAGE).img $(IMAGES_DIR)/$(PUB_IMAGE)/nand_$(UPGRADE_IMAGE).img.sig;
endif
	$(eval upgrade_imgs=$(common_imgs)  "userfs_$(CHIP)_128k.jffs2" "$(UBOOT_NOR_IMAGE_BIN)")
	@pushd $(IMAGES_DIR)/$(PUB_IMAGE)/;\
	rm -f $(UPGRADE_IMAGE);cp nor$(PARTITION_ENV_INFIX)_burn_table.xml upgrade.xml;zip -r nor_$(UPGRADE_IMAGE) $(upgrade_imgs);\
	popd;
	@python $(UPGRADE_IMAGE_TOOL_SCRIPT) $(IMAGES_DIR)/$(PUB_IMAGE)/nor_$(UPGRADE_IMAGE);
ifeq ($(SECUREBOOT_BUILD), y)
	@mv $(IMAGES_DIR)/$(PUB_IMAGE)/nor_$(UPGRADE_IMAGE).img $(IMAGES_DIR)/$(PUB_IMAGE)/nor_$(UPGRADE_IMAGE).img.sig;
endif
endif
	@pushd $(IMAGES_DIR)/$(PUB_IMAGE)/;rm upgrade.xml;popd

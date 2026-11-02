TOOLS_TOP_DIR := $(shell pwd)
TOOL_NAME := busybox-1.34.1
TOOL_TAR_BALL := $(TOOL_NAME).tar.bz2
TOOL_BIN := bin
ARCH ?= arm64

OSDRV_CROSS ?= aarch64-otv02-linux-gnu
OSDRV_DIR ?= ../../bsp
BUSYBOX_CFG ?= config_aarch64_a55_softfp_neon
CROSS = CROSS_COMPILE=$(OSDRV_CROSS)-
ifneq ($(LLVM),)
CROSS = CC=clang LD=ld.lld AS=clang AR=llvm-ar NM=llvm-nm STRIP=llvm-strip OBJCOPY=llvm-objcopy \
			              OBJDUMP=llvm-objdump READELF=llvm-readelf RANLIB=llvm-ranlib
BUSYBOX_CFG = config_aarch64_a55_softfp_neon_llvm
endif

all:
	@echo "CROSS=$(CROSS)"
	@echo "BUSYBOX_CFG=$(BUSYBOX_CFG)"
ifneq ($(TOOLS_TOP_DIR)/$(TOOL_NAME), $(wildcard $(TOOLS_TOP_DIR)/$(TOOL_NAME)))
	-rm $(TOOLS_TOP_DIR)/$(TOOL_NAME) -rf;
	tar xjf $(TOOLS_TOP_DIR)/$(TOOL_TAR_BALL) -C $(TOOLS_TOP_DIR)
	pushd $(TOOLS_TOP_DIR)/$(TOOL_NAME)/;patch -p1 < ../busybox-1.34.1.patch;popd
	cp $(TOOLS_TOP_DIR)/$(TOOL_NAME)/$(BUSYBOX_CFG) $(TOOLS_TOP_DIR)/$(TOOL_NAME)/.config
	$(MAKE) ARCH=$(ARCH) $(CROSS) -C $(TOOLS_TOP_DIR)/$(TOOL_NAME)  1>/dev/null
	$(MAKE) ARCH=$(ARCH) $(CROSS) -C $(TOOLS_TOP_DIR)/$(TOOL_NAME) install 1>/dev/null
endif
	cp -af $(TOOLS_TOP_DIR)/$(TOOL_NAME)/_install/* $(IMAGES_DIR)/$(PUB_ROOTFS)

clean:
	-rm $(TOOLS_TOP_DIR)/$(TOOL_NAME) -rf;

distclean: clean

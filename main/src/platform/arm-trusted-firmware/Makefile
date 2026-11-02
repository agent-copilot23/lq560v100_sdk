BUILD_DIR := $(shell pwd)

SPD=none

CHIP_FAMILY ?= lq560v100
ifeq ($(CHIP_FAMILY), lq560v100)
BUILD_CHIP = lq560v100
endif

ATF_VER:=arm-trusted-firmware-2.7
ATF_TAR:=arm-trusted-firmware-2.7.tar.gz
ATF_PATCH:=$(ATF_VER).patch

OSDRV_CROSS ?= aarch64-otv02-linux-gnu
OSDRV_CROSS_CFLAGS := -fstack-protector-strong -fPIC
OSDRV_CROSS_LDFLAGS := -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack

GREEN = "\e[32;1m"
DONE    = "\033[0m"

ATF_CROSS ?= CROSS_COMPILE=$(OSDRV_CROSS)-
ifneq ($(LLVM),)
ATF_CROSS = CC=clang LD=ld.lld AS=clang AR=llvm-ar NM=llvm-nm STRIP=llvm-strip OC=llvm-objcopy \
	      OD=llvm-objdump READELF=llvm-readelf RANLIB=llvm-ranlib
endif

all:
	@echo "CROSS=$(ATF_CROSS)"
ifneq ($(BUILD_DIR)/$(ATF_VER), $(wildcard $(BUILD_DIR)/$(ATF_VER)))
	@pushd $(BUILD_DIR); tar xzf $(ATF_TAR) -C .; popd
	@pushd $(BUILD_DIR)/$(ATF_VER); patch -p1 < ../$(ATF_PATCH); popd

endif
	$(MAKE) -C $(BUILD_DIR)/$(ATF_VER) $(ATF_CROSS) PLAT=$(BUILD_CHIP) SPD=$(SPD) CCI_UP=0 BL33_SEC=0 bl31

clean:
	@rm $(BUILD_DIR)/$(ATF_VER)/build -rf;

distclean: 
	@rm $(BUILD_DIR)/$(ATF_VER) -rf;

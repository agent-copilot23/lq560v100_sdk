BUILD_DIR      := $(shell pwd)
UDEV           := eudev-3.2.10
ROOTFS         := rootfs
OSDRV_CROSS ?= aarch64-otv02-linux-gnu
HOST           := $(patsubst %-,%,$(OSDRV_CROSS))
OSDRV_CROSS_CFLAGS := -fstack-protector-strong -fPIE
OSDRV_CROSS_LDFLAGS := -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack -pie

CLANG_CROSS :=
ifneq ($(LLVM),)
	CLANG_CROSS = CC=clang LD=ld.lld AS=clang AR=llvm-ar NM=llvm-nm STRIP=llvm-strip OBJCOPY=llvm-objcopy \
		                  OBJDUMP=llvm-objdump READELF=llvm-readelf RANLIB=llvm-ranlib
endif


all:
	@rm $(BUILD_DIR)/$(ROOTFS) -rf
	@rm $(UDEV) -rf
	@tar -xzf $(UDEV).tar.gz -C $(BUILD_DIR)
	pushd $(BUILD_DIR)/$(UDEV);patch -p1 < ../$(UDEV).patch;popd;
	cd $(BUILD_DIR)/$(UDEV) && \
		./autogen.sh  && ./configure --prefix=/ --host=$(HOST) $(CLANG_CROSS) CFLAGS="$(OSDRV_CROSS_CFLAGS)" \
	 	LDFLAGS="$(OSDRV_CROSS_LDFLAGS)" --disable-introspection --disable-blkid --disable-selinux 1>/dev/null;
	$(MAKE) -C $(BUILD_DIR)/$(UDEV) USE_STATIC=false OSDRV_CROSS=${OSDRV_CROSS} 1>/dev/null
	@cp -rf $(BUILD_DIR)/$(UDEV)/$(ROOTFS)/ $(BUILD_DIR)
	@cp $(BUILD_DIR)/$(UDEV)/rules/60-block.rules $(BUILD_DIR)/$(ROOTFS)/etc/udev/rules.d/
	@mkdir $(BUILD_DIR)/$(ROOTFS)/bin -p
	@cp $(BUILD_DIR)/$(UDEV)/src/udev/udevd $(BUILD_DIR)/$(ROOTFS)/bin
	@cp $(BUILD_DIR)/$(UDEV)/src/udev/udevadm $(BUILD_DIR)/$(ROOTFS)/bin
	@$(OSDRV_CROSS)-strip $(BUILD_DIR)/$(ROOTFS)/bin/*
	@rm -rf $(BUILD_DIR)/$(UDEV)


.PHONY clean:
clean:
	@rm -rf $(BUILD_DIR)/$(ROOTFS)
	@rm -rf $(BUILD_DIR)/$(UDEV)

distclean: clean
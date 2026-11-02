###############################################################################
BUILD_DIR      := $(shell pwd)
ZLIB           := $(shell pwd)/../zlib
ZLIB_INSTALL   := $(BUILD_DIR)/../zlib
STRIP          := strip
CRAMFS         := util-linux-2.37.2

LOCAL_CFLAGS := -fPIC
LOCAL_LDFLAGS := -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack
GCC_VERSION_GE49 := $(shell echo `gcc -dumpversion | cut -f1-2 -d.` \>= 4.9 | sed -e 's/\./*100+/g' | bc)
ifeq ($(GCC_VERSION_GE49),1)
LOCAL_CFLAGS += -fstack-protector-strong
else
LOCAL_CFLAGS += -fstack-protector-all
endif
###############################################################################

all: $(BUILD_DIR)/$(CRAMFS)/sbin/mkfs.cramfs
$(BUILD_DIR)/$(CRAMFS)/sbin/mkfs.cramfs:
ifneq ($(wildcard $(BUILD_DIR)/$(CRAMFS)), $(BUILD_DIR)/$(CRAMFS))
	rm $(BUILD_DIR)/out -rf
	mkdir $(BUILD_DIR)/out
	mkdir $(BUILD_DIR)/out/pc

	@test -d $(@D) || mkdir -p $(@D)
	tar -xf $(CRAMFS).tar.xz -C $(BUILD_DIR)
ifneq ($(wildcard $(CRAMFS).patch), )
	pushd $(BUILD_DIR)/$(CRAMFS); patch -p1 < ../$(CRAMFS).patch; popd
endif
	@touch $@

	cd $(BUILD_DIR)/$(CRAMFS) && \
		./autogen.sh && \
		./configure --prefix="$(BUILD_DIR)/out/pc" \
		 --disable-libuuid --disable-libblkid --disable-libmount \
		 --disable-mount --disable-losetup --disable-fsck \
		 --disable-partx --disable-uuidd --disable-mountpoint \
		 --disable-fallocate --disable-unshare --disable-eject \
		 --disable-agetty --disable-switch_root --disable-pivot_root \
		 --disable-kill --disable-utmpdump --disable-rename \
		 --disable-login --disable-sulogin --disable-su \
		 --disable-schedutils --disable-wall \
		 --disable-bash-completion \
		 CFLAGS="$(LOCAL_CFLAGS)" \
		 LDFLAGS="$(LOCAL_LDFLAGS)" > /dev/null && \
		 $(MAKE) >/dev/null && \
		 $(MAKE) install >/dev/null

	strip $(BUILD_DIR)/out/pc/bin/*;
	strip $(BUILD_DIR)/out/pc/sbin/*;

	#@test -d $(@D) || mkdir -p $(@D)
	#install -vs -t $(BUILD_DIR)/out/pc $(BUILD_DIR)/$(CRAMFS)/sbin/mkfs.cramfs
endif
###############################################################################

clean:
	rm $(BUILD_DIR)/out -rf
	rm $(BUILD_DIR)/$(CRAMFS) -rf

distclean: clean

###############################################################################
.PHONY: clean distclean all
###############################################################################

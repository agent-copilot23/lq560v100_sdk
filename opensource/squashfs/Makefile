################################################################################
#    x86/src/squashfs
#    2011.01.16
################################################################################
# parameter
###############################################################################
BUILD_DIR      := $(shell pwd)
SQUASHFS       := squashfs-4.5
XZ_DIR         := $(BUILD_DIR)/../xz/out/pc
ZLIB_DIR       := $(BUILD_DIR)/../zlib/out/pc

LOCAL_CFLAGS := -fPIC
LOCAL_LDFLAGS := -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack
GCC_VERSION_GE49 := $(shell echo `gcc -dumpversion | cut -f1-2 -d.` \>= 4.9 | sed -e 's/\./*100+/g' | bc)
ifeq ($(GCC_VERSION_GE49),1)
LOCAL_CFLAGS += -fstack-protector-strong
else
LOCAL_CFLAGS += -fstack-protector-all
endif

################################################################################
all: $(BUILD_DIR)/out/pc/mksquashfs
$(BUILD_DIR)/out/pc/mksquashfs:

	rm $(BUILD_DIR)/out -rf
	mkdir $(BUILD_DIR)/out
	mkdir $(BUILD_DIR)/out/pc

	@test -d $(@D) || mkdir -p $(@D)
	tar -xf $(SQUASHFS).tar.gz -C $(BUILD_DIR)
ifneq ($(wildcard $(SQUASHFS).patch), )
	pushd $(BUILD_DIR)/$(SQUASHFS); patch -p1 < ../$(SQUASHFS).patch; popd
endif
	cd $(BUILD_DIR)/$(SQUASHFS)/squashfs-tools && \
		make EXTRA_CFLAGS="$(LOCAL_CFLAGS) -I$(XZ_DIR)/include -I$(ZLIB_DIR)/include" \
		LDFLAGS="-L$(XZ_DIR)/lib -L$(ZLIB_DIR)/lib \
		$(LOCAL_LDFLAGS)" -j 16 1>/dev/null && \
		make INSTALL_DIR=$(BUILD_DIR)/out/pc install 1>/dev/null

	strip $(BUILD_DIR)/out/pc/*;
	rm $(BUILD_DIR)/$(SQUASHFS) -rf

################################################################################
clean:
	@test ! -e $(BUILD_DIR)/mksquashfs || rm -rfv $(BUILD_DIR)/mksquashfs
	rm $(BUILD_DIR)/$(SQUASHFS) -rf
	rm $(BUILD_DIR)/out -rf

distclean: clean

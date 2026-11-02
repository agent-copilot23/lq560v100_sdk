###############################################################################
BUILD_DIR      := $(shell pwd)
XZ             := xz-5.2.6

LOCAL_CFLAGS := -fPIC
LOCAL_LDFLAGS := -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack
GCC_VERSION_GE49 := $(shell echo `gcc -dumpversion | cut -f1-2 -d.` \>= 4.9 | sed -e 's/\./*100+/g' | bc)
ifeq ($(GCC_VERSION_GE49),1)
LOCAL_CFLAGS += -fstack-protector-strong
else
LOCAL_CFLAGS += -fstack-protector-all
endif
################################################################################
all: $(BUILD_DIR)/out
$(BUILD_DIR)/out:
	rm $(BUILD_DIR)/out -rf
	mkdir $(BUILD_DIR)/out
	mkdir $(BUILD_DIR)/out/pc
	@test -d $(@D) || mkdir -p $(@D)
	tar -xf $(XZ).tar.gz -C $(BUILD_DIR)
ifneq ($(wildcard $(XZ).patch), )
	pushd $(BUILD_DIR)/$(XZ); patch -p1 < ../$(XZ).patch; popd
endif
	@touch $@
	cd $(BUILD_DIR)/$(XZ) && \
		./autogen.sh && \
		./configure --prefix="$(BUILD_DIR)/out/pc" \
		CFLAGS="$(LOCAL_CFLAGS)" \
		LDFLAGS="$(LOCAL_LDFLAGS)" > /dev/null && \
		$(MAKE) >/dev/null && \
		$(MAKE) install >/dev/null
	rm -rf $(BUILD_DIR)/$(XZ)
	strip $(BUILD_DIR)/out/pc/bin/xz
	strip $(BUILD_DIR)/out/pc/bin/lzmainfo
	strip $(BUILD_DIR)/out/pc/bin/lzmadec
	strip $(BUILD_DIR)/out/pc/bin/xzdec

################################################################################
clean:
	rm -rfv $(BUILD_DIR)/$(XZ)
	rm $(BUILD_DIR)/out -rf
distclean: clean


################################################################################
.PHONY: clean distclean
################################################################################

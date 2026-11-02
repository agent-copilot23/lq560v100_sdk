TOOLS_TOP_DIR ?= $(shell pwd)
TOOL_TAR_BALL := ethtool-5.15.tar.xz
TOOL_NAME := ethtool-5.15
TOOL_BIN := bin
OSDRV_CROSS ?= aarch64-otv02-linux-gnu
OSDRV_CROSS_CFLAGS := -fstack-protector-strong -fPIE
OSDRV_CROSS_LDFLAGS := -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack -pie
CLANG_CROSS :=
ifneq ($(LLVM),)
	CLANG_CROSS = CC=clang LD=ld.lld AS=clang AR=llvm-ar NM=llvm-nm STRIP=llvm-strip OBJCOPY=llvm-objcopy \
		      OBJDUMP=llvm-objdump READELF=llvm-readelf RANLIB=llvm-ranlib
endif

all:
ifneq ($(wildcard $(TOOLS_TOP_DIR)/$(TOOL_NAME)), $(TOOLS_TOP_DIR)/$(TOOL_NAME))
	-rm $(TOOLS_TOP_DIR)/$(TOOL_NAME) -rf;
	tar xf $(TOOL_TAR_BALL);
ifneq ($(wildcard $(TOOL_NAME).patch), )
	pushd $(TOOLS_TOP_DIR)/$(TOOL_NAME); patch -p1 < ../$(TOOL_NAME).patch; popd
endif
	mkdir -p $(TOOLS_TOP_DIR)/$(TOOL_BIN)/;
	mkdir -p $(TOOLS_TOP_DIR)/$(TOOL_NAME)/$(TOOL_BIN)/;

	cd $(TOOLS_TOP_DIR)/$(TOOL_NAME)/ && \
		./configure --host=$(OSDRV_CROSS) $(CLANG_CROSS) CFLAGS="$(OSDRV_CROSS_CFLAGS)" \
		LDFLAGS="$(OSDRV_CROSS_LDFLAGS)" --enable-netlink=no \
		--prefix=$(TOOLS_TOP_DIR)/$(TOOL_NAME)/$(TOOL_BIN) > /dev/null && \
	$(MAKE)  > /dev/null && \
		$(MAKE) install > /dev/null && \
		cp $(TOOL_BIN)/* $(TOOLS_TOP_DIR)/$(TOOL_BIN) -rf;
		rm -rf $(TOOLS_TOP_DIR)/$(TOOL_NAME);
		$(OSDRV_CROSS)-strip $(TOOLS_TOP_DIR)/$(TOOL_BIN)/sbin/*;
endif

.PHONY: clean
clean:
	@rm $(TOOLS_TOP_DIR)/$(TOOL_NAME) -rf;
	@rm $(TOOLS_TOP_DIR)/$(TOOL_BIN) -rf;

.PHONY: distclean
distclean:
	-rm $(TOOLS_TOP_DIR)/$(TOOL_NAME) -rf;
	-rm $(TOOLS_TOP_DIR)/$(TOOL_BIN) -rf;

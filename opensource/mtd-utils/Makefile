TOOL_BIN := bin
OSDRV_CROSS ?= aarch64-otv02-linux-gnu
TOOL_NAME :=mtd-utils-2.1.4
TOOL_TAR_BALL := $(TOOL_NAME).tar.bz2
TOOL_DIR := $(shell pwd)

ZLIB_NAME:=zlib-1.2.11
ZLIB_TAR_BALL:=$(ZLIB_NAME).tar.gz
BOARD_ZLIB_LIB_PATH :=$(TOOL_DIR)/../zlib/out/board/lib/libz.a

PC_DIR := $(shell pwd)/$(TOOL_NAME)_pc
BOARD_DIR := $(shell pwd)/$(TOOL_NAME)_board
ZLIB_PC_DIR := $(TOOL_DIR)/../zlib/out/pc

OSDRV_CROSS_CFLAGS := -fstack-protector-strong -fPIC
OSDRV_CROSS_LDFLAGS := -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack

INCLUDE += -I$(TOOL_DIR)/../zlib/out/board/include

LOCAL_CFLAGS := -fPIC
LOCAL_LDFLAGS := -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack
GCC_VERSION_GE49 := $(shell echo `gcc -dumpversion | cut -f1-2 -d.` \>= 4.9 | sed -e 's/\./*100+/g' | bc)
ifeq ($(GCC_VERSION_GE49),1)
LOCAL_CFLAGS += -fstack-protector-strong
else
LOCAL_CFLAGS += -fstack-protector-all
endif

CLANG_CROSS :=
ifneq ($(LLVM),)
CLANG_CROSS = CC=clang LD=ld.lld AS=clang AR=llvm-ar NM=llvm-nm STRIP=llvm-strip OBJCOPY=llvm-objcopy \
		      OBJDUMP=llvm-objdump READELF=llvm-readelf RANLIB=llvm-ranlib
endif
all:  mtd_pc mtd_board

prepare:
ifneq ($(TOOL_DIR)/out, $(wildcard $(TOOL_DIR)/out))
	mkdir $(TOOL_DIR)/out -p
	mkdir $(TOOL_DIR)/out/pc -p
	mkdir $(TOOL_DIR)/out/board -p
endif

mtd_board : prepare
ifneq ($(BOARD_ZLIB_LIB_PATH), $(wildcard $(BOARD_ZLIB_LIB_PATH)))
	$(warning "---------------------------------------------------------------------")
	$(warning "     Cannot found the $(BOARD_ZLIB_LIB_PATH) file                    ")
	$(warning "   	Please to make $(ZLIB_NAME) page                                ")
	$(warning "---------------------------------------------------------------------")
	$(error )
endif
ifneq ($(BOARD_DIR)/$(TOOL_NAME), $(wildcard $(BOARD_DIR)/$(TOOL_NAME)))
	rm $(BOARD_DIR) -rf;
	mkdir $(BOARD_DIR)
	tar -xf $(TOOL_TAR_BALL) -C $(BOARD_DIR)
ifneq ($(wildcard $(TOOL_NAME).patch), )
	pushd $(BOARD_DIR)/$(TOOL_NAME); patch -p1 < ../../$(TOOL_NAME).patch; popd
endif
	mkdir -p $(BOARD_DIR)/$(TOOL_NAME)/$(TOOL_BIN)/;
	mkdir -p $(BOARD_DIR)/$(TOOL_BIN)/;
	cd $(BOARD_DIR)/$(TOOL_NAME)/ && \
		./configure --without-ubifs --without-xattr --without-lzo --without-zstd --without-crypto \
		--host=$(OSDRV_CROSS) $(CLANG_CROSS) CFLAGS="$(OSDRV_CROSS_CFLAGS) $(INCLUDE)" LDFLAGS="$(OSDRV_CROSS_LDFLAGS)" \
		--prefix=$(BOARD_DIR)/$(TOOL_NAME)/$(TOOL_BIN) \
		ZLIB_LIBS=$(BOARD_ZLIB_LIB_PATH) --disable-tests > /dev/null && \
		$(MAKE) CFLAGS=--static INCLUDES=$(INCLUDE) >/dev/null && \
		$(MAKE) install >/dev/null;

	rm $(TOOL_DIR)/out/board/* -rf
	cp -rf $(BOARD_DIR)/$(TOOL_NAME)/$(TOOL_BIN)/sbin/* $(BOARD_DIR)/$(TOOL_BIN)/
	cp $(BOARD_DIR)/$(TOOL_BIN)/  $(TOOL_DIR)/out/board -r
	file $(TOOL_DIR)/out/board/bin/*|grep ELF|cut -d ':' -f 1|xargs $(OSDRV_CROSS)-strip
endif

mtd_pc : prepare
ifneq ($(PC_DIR)/$(TOOL_NAME), $(wildcard $(PC_DIR)/$(TOOL_NAME)))
	rm $(PC_DIR) -rf
	mkdir $(PC_DIR)
	tar -xf $(TOOL_TAR_BALL) -C $(PC_DIR)
ifneq ($(wildcard $(TOOL_NAME).patch), )
	pushd $(PC_DIR)/$(TOOL_NAME); patch -p1 < ../../$(TOOL_NAME).patch; popd
endif
	cd $(PC_DIR)/$(TOOL_NAME) && \
		./configure --prefix=$(PC_DIR) CC=gcc \
		--without-xattr --without-crypto --without-lzo --without-zstd \
		    CFLAGS="$(LOCAL_CFLAGS) -I$(ZLIB_PC_DIR)/include"  \
			LDFLAGS="$(LOCAL_LDFLAGS) -L$(ZLIB_PC_DIR)/lib" \
			ZLIBCPPFLAGS="-I$(ZLIB_PC_DIR)/include" \
			ZLIBLDFLAGS="-L$(ZLIB_PC_DIR)/lib" > /dev/null && \
		$(MAKE) >/dev/null  && \
		$(MAKE) install >/dev/null;

	rm $(TOOL_DIR)/out/pc/* -rf
	cp $(PC_DIR)/sbin/mkfs.ubifs $(TOOL_DIR)/out/pc/
	cp $(PC_DIR)/sbin/ubinize $(TOOL_DIR)/out/pc/
	cp $(PC_DIR)/sbin/mkfs.jffs2 $(TOOL_DIR)/out/pc/
	file $(TOOL_DIR)/out/pc/*|grep ELF|cut -d ':' -f 1|xargs strip
endif

clean:
	rm $(PC_DIR) -rf;
	rm $(BOARD_DIR) -rf;
	rm $(TOOL_DIR)/out -rf

distclean: clean

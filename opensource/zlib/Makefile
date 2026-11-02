ZLIB_NAME:=zlib-1.2.12
ZLIB_TAR_BALL:=zlib-1.2.12.tar.gz

PREFIX := $(shell pwd)

PC_DIR := $(shell pwd)/$(ZLIB_NAME)_pc
BOARD_DIR := $(shell pwd)/$(ZLIB_NAME)_board

OSDRV_CROSS ?= aarch64-otv02-linux-gnu
OSDRV_CROSS_CFLAGS := -fstack-protector-strong -fPIC
OSDRV_CROSS_LDFLAGS := -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack

CROSS_CC :=
ifneq ($(LLVM),)
CROSS_CC = CC=clang LD=ld.lld AS=clang AR=llvm-ar NM=llvm-nm STRIP=llvm-strip OBJCOPY=llvm-objcopy \
	       OBJDUMP=llvm-objdump READELF=llvm-readelf RANLIB=llvm-ranlib
else
CROSS_CC = CC=$(OSDRV_CROSS)-gcc
endif

LOCAL_CFLAGS := -fPIC
LOCAL_LDFLAGS := -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack
GCC_VERSION_GE49 := $(shell echo `gcc -dumpversion | cut -f1-2 -d.` \>= 4.9 | sed -e 's/\./*100+/g' | bc)
ifeq ($(GCC_VERSION_GE49),1)
LOCAL_CFLAGS += -fstack-protector-strong
else
LOCAL_CFLAGS += -fstack-protector-all
endif

all: zlib_pc  zlib_board

prepare:
ifneq ($(PREFIX)/out, $(wildcard $(PREFIX)/out))
	mkdir $(PREFIX)/out -p
	mkdir $(PREFIX)/out/pc -p
	mkdir $(PREFIX)/out/board -p
endif

zlib_pc: $(PC_DIR)/lib/libz.a
$(PC_DIR)/lib/libz.a : prepare
ifneq ($(wildcard $(PC_DIR)/$(ZLIB_NAME)), $(PC_DIR)/$(ZLIB_NAME))
	rm $(PC_DIR) -rf
	mkdir $(PC_DIR)
	tar -xf $(ZLIB_TAR_BALL) -C $(PC_DIR)
ifneq ($(wildcard $(ZLIB_NAME).patch), )
	pushd $(PC_DIR)/$(ZLIB_NAME); patch -p1 < ../../$(ZLIB_NAME).patch; popd
endif
	cd $(PC_DIR)/$(ZLIB_NAME)/ && \
		CFLAGS="$(LOCAL_CFLAGS)" LDFLAGS="$(LOCAL_LDFLAGS)" ./configure --prefix="$(PC_DIR)" && \
		$(MAKE) CC=gcc 1>/dev/null && \
		$(MAKE) install CC=gcc 1>/dev/null
	rm $(PREFIX)/out/pc/* -rf
	cp $(PC_DIR)/include $(PREFIX)/out/pc -r
	cp $(PC_DIR)/lib $(PREFIX)/out/pc -r
	cp $(PC_DIR)/share $(PREFIX)/out/pc -r
endif

zlib_board: $(BOARD_DIR)/lib/libz.a
$(BOARD_DIR)/lib/libz.a : prepare
ifneq ($(wildcard $(BOARD_DIR)/$(ZLIB_NAME)), $(BOARD_DIR)/$(ZLIB_NAME))
	rm $(BOARD_DIR) -rf
	mkdir $(BOARD_DIR)
	tar -xf $(ZLIB_TAR_BALL) -C $(BOARD_DIR)
ifneq ($(wildcard $(ZLIB_NAME).patch), )
	pushd $(BOARD_DIR)/$(ZLIB_NAME); patch -p1 < ../../$(ZLIB_NAME).patch; popd
endif
	cd $(BOARD_DIR)/$(ZLIB_NAME)/ && \
		prefix=zlib_install $(CROSS_CC) CFLAGS="$(OSDRV_CROSS_CFLAGS)" \
		LDFLAGS="$(OSDRV_CROSS_LDFLAGS)" ./configure && \
		$(MAKE) 1>/dev/null && \
		$(MAKE) install 1>/dev/null;
	rm $(PREFIX)/out/board/* -rf
	cp $(BOARD_DIR)/$(ZLIB_NAME)/zlib_install/* $(PREFIX)/out/board -r
endif

clean:
	rm out -rf;
	rm $(ZLIB_NAME)_* -rf
distclean: clean

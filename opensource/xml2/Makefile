XML2_NAME:=libxml2-2.9.0
XML2_TAR_BALL:=libxml2-2.9.0.tar.xz

PREFIX := $(shell pwd)

PC_DIR := $(shell pwd)/$(XML2_NAME)_pc
BOARD_DIR := $(shell pwd)/$(XML2_NAME)_board

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

all:  xml2_board

prepare:
ifneq ($(PREFIX)/out, $(wildcard $(PREFIX)/out))
	rm $(PREFIX)/out -rf
	mkdir $(PREFIX)/out
	mkdir $(PREFIX)/out/board
endif

xml2_board: $(BOARD_DIR)/lib/libxml2.a
$(BOARD_DIR)/lib/libxml2.a : prepare
ifneq ($(wildcard $(BOARD_DIR)/$(XML2_NAME)), $(BOARD_DIR)/$(XML2_NAME))
	rm $(BOARD_DIR) -rf
	mkdir $(BOARD_DIR)
	tar -xf $(XML2_TAR_BALL) -C $(BOARD_DIR)
	cd $(BOARD_DIR)/$(XML2_NAME)/ && sed -i '1282 i\aarch64*) basic_machine=$1;;' config.sub
	cd $(BOARD_DIR)/$(XML2_NAME)/ && \
		CC=$(OSDRV_CROSS)-gcc CFLAGS="$(OSDRV_CROSS_CFLAGS)" \
		LDFLAGS="$(OSDRV_CROSS_LDFLAGS)" ./configure  prefix=$(BOARD_DIR)/$(XML2_NAME)/xml2_install  --host=$(OSDRV_CROSS) \
		--without-regexps --without-iso8859x --without-xinclude --without-http --without-zlib --without-pattern \
		--without-lzma --without-trio --without-html --without-xptr --without-writer --without-schemas \
		--without-schematron --without-threads --without-valid --without-python --without-push --without-ftp \
		--without-xpath --without-legacy --without-icu --without-catalog --without-modules --enable-static && > /dev/null \
		$(MAKE) all >/dev/null && \
		$(MAKE) install >/dev/null

	cp $(BOARD_DIR)/$(XML2_NAME)/xml2_install/* $(PREFIX)/out/board -r
endif

clean:
	rm out -rf;
	rm $(XML2_NAME)_* -rf
distclean: clean

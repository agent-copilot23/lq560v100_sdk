E2FSPROGS:=e2fsprogs-1.46.4
E2FSPROGS_TAR:=$(E2FSPROGS).tar.xz

PREFIX := $(shell pwd)

PC_DIR := $(shell pwd)/$(E2FSPROGS)_pc
BOARD_DIR := $(shell pwd)/$(E2FSPROGS)_board

OSDRV_CROSS ?= aarch64-otv02-linux-gnu
OSDRV_CROSS_CFLAGS := -fstack-protector-strong -fPIC
OSDRV_CROSS_LDFLAGS := -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack

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

all: e2fsprogs_pc  e2fsprogs_board

prepare:
ifneq ($(PREFIX)/out, $(wildcard $(PREFIX)/out))
	mkdir $(PREFIX)/out
	mkdir $(PREFIX)/out/pc
	mkdir $(PREFIX)/out/board
endif

e2fsprogs_pc: $(PC_DIR)/mkfs.ext4
$(PC_DIR)/mkfs.ext4: $(PC_DIR)/sbin/mkfs.ext4
ifneq ($(PC_DIR), $(wildcard $(PC_DIR)))
	@test -d $(@D) || mkdir -p $(@D)
	install -vs -t $(PC_DIR) $<
	@test -d $(PC_DIR)/contrib || mkdir -p $(PREFIX)/out/pc/contrib
	@test -d $(PC_DIR)/debugfs || mkdir -p $(PREFIX)/out/pc//debugfs
	cp -rf $(PC_DIR)/$(E2FSPROGS)/contrib/populate-extfs.sh $(PREFIX)/out/pc/contrib
	cp -rf $(PC_DIR)/$(E2FSPROGS)/debugfs/debugfs $(PREFIX)/out/pc/debugfs
	cp -rf $(PC_DIR)/sbin $(PREFIX)/out/pc/
	file $(PREFIX)/out/pc/sbin/*|grep ELF|cut -d ':' -f 1|xargs strip
	file $(PREFIX)/out/pc/debugfs/*|grep ELF|cut -d ':' -f 1|xargs strip
endif

$(PC_DIR)/sbin/mkfs.ext4: prepare
ifneq ($(PC_DIR), $(wildcard $(PC_DIR)))
	mkdir $(PC_DIR)
	tar -xf $(E2FSPROGS_TAR) -C $(PC_DIR)
ifneq ($(wildcard $(E2FSPROGS).patch), )
	pushd $(PC_DIR)/$(E2FSPROGS); patch -p1 < ../../$(E2FSPROGS).patch; popd
endif

	cd $(PC_DIR)/$(E2FSPROGS) && \
		./configure --prefix="$(PC_DIR)" CFLAGS="$(LOCAL_CFLAGS)" \
		LDFLAGS="$(LOCAL_LDFLAGS)" --disable-tls \
		--with-udev-rules-dir=$(PC_DIR)/obj/rootfs/etc/udev/rules.d \
		--with-systemd-unit-dir=$(PC_DIR)/obj/rootfs/lib \
		--with-crond-dir=$(PC_DIR)/obj/rootfs/etc/cron.d > /dev/null && \
		$(MAKE) >/dev/null  && \
		$(MAKE) install >/dev/null
endif

e2fsprogs_board: $(BOARD_DIR)/sbin/mkfs.ext4
$(BOARD_DIR)/sbin/mkfs.ext4: prepare
ifneq ($(BOARD_DIR), $(wildcard $(BOARD_DIR)))
	mkdir $(BOARD_DIR)
	tar -xf $(E2FSPROGS_TAR) -C $(BOARD_DIR)
ifneq ($(wildcard $(E2FSPROGS).patch), )
	pushd $(BOARD_DIR)/$(E2FSPROGS); patch -p1 < ../../$(E2FSPROGS).patch; popd
endif

	cd $(BOARD_DIR)/$(E2FSPROGS) && \
		./configure --host=$(OSDRV_CROSS) $(CLANG_CROSS) CFLAGS="$(OSDRV_CROSS_CFLAGS)" \
		LDFLAGS="$(OSDRV_CROSS_LDFLAGS)" --prefix=$(BOARD_DIR)  --disable-tls \
		--with-udev-rules-dir=$(BOARD_DIR)/obj/rootfs/etc/udev/rules.d \
		--with-systemd-unit-dir=$(BOARD_DIR)/obj/rootfs/lib \
		--with-crond-dir=$(BOARD_DIR)/obj/rootfs/etc/cron.d > /dev/null && \
		$(MAKE) CFLAGS="-static" LDFLAGS="-static" >/dev/null && \
		$(MAKE) install >/dev/null

	cp $(BOARD_DIR)/sbin  $(PREFIX)/out/board -rf
	file $(PREFIX)/out/board/sbin/*|grep ELF|cut -d ':' -f 1|xargs $(OSDRV_CROSS)-strip
endif

clean:
	rm out -rf;
	rm $(E2FSPROGS)_* -rf
distclean: clean

CC = gcc
RM = -rm -rf
GZIP_VER_DIR = gzip-1.11

LOCAL_CFLAGS := -fPIC
LOCAL_LDFLAGS := -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack,-s
GCC_VERSION_GE49 := $(shell echo `gcc -dumpversion | cut -f1-2 -d.` \>= 4.9 | sed -e 's/\./*100+/g' | bc)
ifeq ($(GCC_VERSION_GE49),1)
LOCAL_CFLAGS += -fstack-protector-strong
else
LOCAL_CFLAGS += -fstack-protector-all
endif

all: gzip

gzip:
ifneq ($(GZIP_VER_DIR), $(wildcard $(GZIP_VER_DIR)))
	tar -xf $(GZIP_VER_DIR).tar.xz
	pushd $(GZIP_VER_DIR);patch -p1 < ../gzip.patch;popd

	pushd $(GZIP_VER_DIR);autoreconf -f -i -s && ./configure CFLAGS="$(LOCAL_CFLAGS)" LDFLAGS="$(LOCAL_LDFLAGS)";$(MAKE) || exit "$$?";popd
	mkdir bin -p
	cp $(GZIP_VER_DIR)/gzip bin;
	strip bin/gzip;
endif

clean:
	@$(RM) -rf *.bak *~ bin
	@$(RM) -rf $(GZIP_VER_DIR)

.PHONY: all clean

distclean: clean
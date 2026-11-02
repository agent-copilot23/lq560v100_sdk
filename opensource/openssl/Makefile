
BUILD_DIR      := $(shell pwd)
OPENSSL_VER_DIR = openssl-3.2.0
RM = -rm -rf
OPENSSL_OUT=$(BUILD_DIR)/../../main/tools/pc/image_tool/third_party/openssl

CROSS_COMPILE=

.PHONY: all openssl

all: openssl
ifneq ($(OPENSSL_VER_DIR), $(wildcard $(OPENSSL_VER_DIR)))
	@tar -xf $(OPENSSL_VER_DIR).tar.gz
	@mkdir ${BUILD_DIR}/out -p
	@pushd $(OPENSSL_VER_DIR);./config -static --static --prefix=${BUILD_DIR}/out >/dev/null && $(MAKE) >/dev/null 2>&1 && $(MAKE) install > /dev/null;popd
	@strip ${BUILD_DIR}/out/bin/openssl
	@cp ${BUILD_DIR}/out/bin/openssl $(OPENSSL_OUT)
endif


clean:
	@${RM} $(OPENSSL_VER_DIR)
	@${RM} ${BUILD_DIR}/out
	@${RM} $(OPENSSL_OUT)

distclean: clean


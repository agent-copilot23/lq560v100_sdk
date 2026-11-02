LOCAL_TOP_DIR ?= $(shell pwd)
LOCAL_TAR_BALL := cJSON-1.7.18.tar.gz
LOCAL_NAME := cJSON-1.7.18
LOCAL_INSTALL := out
TARGET_PATH :=$(LOCAL_TOP_DIR)/../../main/out/lib
OSDRV_CROSS ?= aarch64-otv02-linux-gnu-gcc
OSDRV_CROSS_CFLAGS := -fstack-protector-strong -fPIE
OSDRV_CROSS_LDFLAGS := -Wl,-z,relro -Wl,-z,now -Wl,-z,noexecstack -pie

LIBA=libcjson.a
LIBS=libcjson.so

all:
ifeq ($(LOCAL_TOP_DIR)/$(LOCAL_NAME), $(wildcard $(LOCAL_TOP_DIR)/$(LOCAL_NAME)))
	@rm   $(LOCAL_TOP_DIR)/$(LOCAL_NAME) -rf;
endif
	@tar -xzf $(LOCAL_TOP_DIR)/$(LOCAL_TAR_BALL) -C $(LOCAL_TOP_DIR);
ifneq ($(wildcard $(LOCAL_NAME).patch), )
	pushd $(LOCAL_TOP_DIR); patch -p1 < ./$(LOCAL_NAME).patch; popd
endif
	@mkdir -p $(LOCAL_TOP_DIR)/$(LOCAL_INSTALL);
	@cd $(LOCAL_TOP_DIR)/$(LOCAL_NAME)/ && $(MAKE)  CC=$(OSDRV_CROSS)  > /dev/null && \
	$(MAKE) PREFIX=$(LOCAL_INSTALL) DESTDIR=$(LOCAL_TOP_DIR)/  install   > /dev/null;
	@cp  $(LOCAL_TOP_DIR)/$(LOCAL_NAME)/$(LIBA)  $(TARGET_PATH);
	@cp  $(LOCAL_TOP_DIR)/$(LOCAL_NAME)/$(LIBS)  $(TARGET_PATH);
	@cp -rd $(LOCAL_TOP_DIR)/$(LOCAL_INSTALL)/include/*  $(TARGET_PATH)/../include/;

.PHONY: clean
clean:
	@rm $(LOCAL_TOP_DIR)/$(LOCAL_NAME) -rf;
	@rm $(LOCAL_TOP_DIR)/$(LOCAL_INSTALL) -rf;
	@rm $(TARGET_PATH)/$(LIBA) -rf;
	@rm $(TARGET_PATH)/$(LIBS) -rf;

.PHONY: distclean
distclean:
	@rm $(LOCAL_TOP_DIR)/$(LOCAL_NAME) -rf;
	@rm $(LOCAL_TOP_DIR)/$(LOCAL_INSTALL) -rf;
	@rm $(TARGET_PATH)/$(LIBA) -rf;
	@rm $(TARGET_PATH)/$(LIBS) -rf;



CUR_DIR = $(shell pwd)
SDK_DIR = $(CUR_DIR)/..
COMMON_DIR = $(SDK_DIR)/common

COMMON_INC_FILES = $(notdir $(wildcard $(COMMON_DIR)/include/*))

.PHONY: clean src

all: install 

clean: uninstall 

install:
	@mkdir -p out && mkdir -p out/include && cp $(COMMON_DIR)/include/* out/include/

uninstall:
ifneq ($(wildcard out/include),)
	@echo "sdk common files: $(COMMON_INC_FILES)"
	@cd out/include && rm $(COMMON_INC_FILES) -f
endif

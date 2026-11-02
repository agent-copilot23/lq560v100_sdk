CUR_DIR = $(shell pwd)

ifeq ($(MAIN_CFG), )
    MAIN_DIR = $(CUR_DIR)/../../
    MAIN_CFG := $(MAIN_DIR)/build/main_cfg.mak
    include $(MAIN_CFG)
endif


MAIN_COMMON_FILES = $(notdir $(wildcard $(CUR_DIR)/include/*))
MAIN_COMMON_KOS = $(notdir $(wildcard $(CUR_DIR)/ko/*))

.PHONY:install install_clean securec securec_clean

all: install securec

clean: securec_clean install_clean

install:
	@mkdir -p $(REL_INC) && cd include && cp * $(REL_DIR)/include -rf
	@mkdir -p $(REL_KO) && cd ko && cp * $(REL_DIR)/ko -rf

install_clean:
ifneq ($(wildcard $(REL_DIR)/include),)
	@cd $(REL_DIR)/include && rm $(MAIN_COMMON_FILES) -f
endif
ifneq ($(wildcard $(REL_DIR)/ko),)
	@cd $(REL_DIR)/ko && rm $(MAIN_COMMON_KOS) -f
endif
	
securec:
	@cd securec; $(MAKE)

securec_clean:
	@cd securec; $(MAKE) clean


CUR_DIR = $(shell pwd)

ifeq ($(MAIN_CFG), )
    MAIN_DIR = $(CUR_DIR)/../../..
    #MAIN_CFG := $(MAIN_DIR)/build/main_cfg.mak
    include $(MAIN_CFG)
endif


.PHONY:clean all
all:
	@echo -e "\e[0;32;1m--Compiling 'securec'... Configs as follow:\e[0;36;1m"
	@echo ---- CROSS=$(CROSS)
	@echo ---- OT_ARCH=$(OT_ARCH), OT_CHIP=$(OT_CHIP), CVER=$(CVER), OT_DBG=$(OT_DBG)
	@echo -e "\e[0m"
	@cd src; $(MAKE)

# --------------clean -----------------
clean:
	@cd src; $(MAKE) clean


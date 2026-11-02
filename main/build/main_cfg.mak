
export MAIN_CFG

ROOT_DIR = ${MAIN_DIR}/..

include $(ROOT_DIR)/cfg.mak

MAIN_BUILD_DIR = $(MAIN_DIR)/build
include $(MAIN_BUILD_DIR)/cfg.mak

ifeq ($(BUILD_ENV_VAR), )
    BUILD_ENV_VAR:=$(MAIN_BUILD_DIR)/env_var.mak
    include $(BUILD_ENV_VAR)
endif
ifeq ($(CFLAG_OPTIONS), )
    CFLAG_OPTIONS:=$(MAIN_BUILD_DIR)/cflag_option.mak
    include $(CFLAG_OPTIONS)
endif

# include the config file of platform
include $(MAIN_BUILD_DIR)/linux_env.mak

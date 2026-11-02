CUR_DIR = $(shell pwd)

MAIN_DIR = $(CUR_DIR)/../../../../../../
ifeq ($(MAIN_CFG), )
    MAIN_CFG := $(MAIN_DIR)/build/main_cfg.mak
    include $(MAIN_CFG)
endif

# user have to impl TARGET SRC_DIR LIB_DIR OBJ_DIR INSTLL_HEADERS CUR_INC
TARGET=ot_vo_serdes

SRC_DIR=$(CUR_DIR)
LIB_DIR=$(CUR_DIR)/../../libs
OBJ_DIR = $(CUR_DIR)/obj

INSTALL_HEADERS = $(wildcard $(CUR_DIR)/../../include/*.h)

CUR_INC := -I$(CUR_DIR)/include
CUR_INC += -I$(CUR_DIR)/../../include
CUR_INC += -I$(CUR_DIR)/../drivers/include
CUR_INC += -I$(CUR_DIR)/../../../common/include
CUR_INC += -I$(SDK_COMMON_DIR)
CUR_INC += -I$(MEDIA_COMMON_DIR)
CUR_INC += -I$(MAIN_COMMON_DIR)

# LIBS_LD_CFLAGS += -lot_serdes_common

# user have to impl CFLAGS LDFLAGS
CFLAGS += $(LIBS_CFLAGS)
LDFLAGS += $(LIBS_LD_CFLAGS)


include $(MAIN_DIR)/build/lib_template.mak


CUR_DIR = $(shell pwd)

MAIN_DIR = $(CUR_DIR)/../../../../..
ifeq ($(MAIN_CFG), )
    MAIN_CFG := $(MAIN_DIR)/build/main_cfg.mak
    include $(MAIN_CFG)
endif

EXTRA_CFLAGS := -I$(PWD)
EXTRA_CFLAGS += -I$(PWD)/../include/
EXTRA_CFLAGS += -I$(PWD)/include/
EXTRA_CFLAGS += -I$(MAIN_DIR)/src/common/securec/include/

ifeq ($(CONFIG_OT_SYS_SMMU_SUPPORT),y)
EXTRA_CFLAGS += -DCONFIG_OT_SYS_SMMU_SUPPORT
endif

ifeq ($(CONFIG_OT_PROC_SHOW_SUPPORT),y)
EXTRA_CFLAGS += -DCONFIG_OT_PROC_SHOW_SUPPORT
endif

SRCS := media_mem.c smr_userdev.c ot_allocator.c smr_init.c
SRCS_O := $(SRCS)

TARGET   := $(KO_PREFIX)_smr
DEPEND_SYMBOL := $(MAIN_DIR)/src/kal//src/linux/kernel/obj/Module.symvers
#$(TARGET)-$(CONFIG_CMA) += cma_allocator.o

include $(MAKE_DRV_FILE)

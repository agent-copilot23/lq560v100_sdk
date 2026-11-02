# env_var.mak
###############################################################################
export VER_X ?= 1
export VER_Y ?= 0
export VER_Z ?= 0
export VER_P ?= 0
export VER_B ?= 10

export PRODUCT      ?= IPC
export OMP_BUILD    ?= n

export KO_PREFIX?=ot
export ARCH_DIR?=$(CONFIG_OT_ARCH)
export EXT_DRV?=$(CONFIG_OT_ARCH)
export INTER_DRV?=$(CONFIG_OT_ARCH)

export OT_SUBARCH?=$(CONFIG_OT_SUBARCH)
export OT_CHIP?=$(CONFIG_OT_CHIP_TYPE)
export ARM_ARCH?=$(CONFIG_ARM_ARCH_TYPE)

# 32bit or 64bit
export ARCH=arm64

export OT_ARCH?=$(CONFIG_OT_ARCH)
ifeq ($(OT_ARCH), )
    $(error OT_ARCH not defined! Please check!)
endif

# uclibc or glibc
export LIBC?=$(CONFIG_LIBC_TYPE)
ifeq ($(LIBC), )
    $(error LIBC not defined! Please check!)
endif

# linux
export OSTYPE?=$(CONFIG_OS_TYPE)
export KERNEL_NAME?=$(CONFIG_KERNEL_VERSION)

# Whether support the debug information. OT_DEBUG or OT_RELEASE or AXIS
export OT_RLS_MODE?=$(CONFIG_OT_RLS_MODE)

# cpu type single/big-little
export CPU_TYPE?=$(CONFIG_CPU_TYPE)

# -g
export OT_GDB?=$(CONFIG_OT_GDB)

# gcov & asan
export OT_ASAN?=$(CONFIG_OT_ASAN)
export OT_GCOV?=$(CONFIG_OT_GCOV)


# CROSS
export CROSS_COMPILE?=$(CONFIG_OT_CROSS)
export CROSS?=$(CONFIG_OT_CROSS_LIB)
export ARCH_LIBNAME?=$(CONFIG_OT_SUBARCH)

# FPGA board or not
ifeq ($(CONFIG_OT_FPGA), y)
    export FPGA?=y
else
    export FPGA?=n
endif

ifeq ($(FPGA),y)
    export OT_FPGA:=OT_FPGA
    export FPGA_FLAG:=1
else
    export ASIC_FLAG:=0
    export OT_FPGA:=OT_XXXX
endif

#*******************************************************************************
#Set the sdk source code and include path
export SDK_PATH         := $(ROOT_DIR)/..
export OPEN_SOURCE_PATH := $(SDK_PATH)/open_source
export KAL_ROOT         := $(MAIN_DIR)/src/kal
export MAIN_COMMON_DIR  := $(MAIN_DIR)/src/common
export MAIN_COMPONENT_DIR  := $(MAIN_DIR)/src/component
export SECUREC_ROOT     := $(MAIN_COMMON_DIR)/securec

#*******************************************************************************
#Set the cbb source code path
export REL_DIR := $(MAIN_DIR)/out
export REL_OBJ := $(REL_DIR)/obj
export REL_INC := $(REL_DIR)/include
export REL_LIB := $(REL_DIR)/lib
export REL_KO := $(REL_DIR)/ko
export REL_DATA := $(REL_DIR)/data

export COMMON_INCLUDE := -I$(KAL_ROOT)/include
COMMON_INCLUDE += -I$(SECUREC_ROOT)/include
COMMON_INCLUDE += -I$(REL_INC)

SDK_COMMON_DIR=$(ROOT_DIR)/common
SDK_COMMON_INCLUDE=$(SDK_COMMON_DIR)/include
SDK_COMMON_CHIP_INCLUDE=$(SDK_COMMON_DIR)/src/include

MAIN_COMMON_INCLUDE=$(MAIN_COMMON_DIR)/include
MAIN_COMMON_CHIP_INCLUDE=$(MAIN_COMMON_DIR)/src/include

COMPONENT_DIR=$(MAIN_DIR)/src/component

MEDIA_DIR=$(COMPONENT_DIR)/media
MEDIA_COMMON_DIR=$(MEDIA_DIR)/common
MEDIA_COMMON_INCLUDE=$(MEDIA_COMMON_DIR)/include
MEDIA_COMMON_CHIP_INCLUDE=$(MEDIA_COMMON_DIR)/src/include

COMMON_INCLUDE += -I$(SECUREC_ROOT)/include
COMMON_INCLUDE += -I$(SDK_COMMON_INCLUDE)
COMMON_INCLUDE += -I$(SDK_COMMON_CHIP_INCLUDE)
COMMON_INCLUDE += -I$(MAIN_COMMON_INCLUDE)
COMMON_INCLUDE += -I$(MAIN_COMMON_CHIP_INCLUDE)
COMMON_INCLUDE += -I$(MEDIA_COMMON_INCLUDE)
COMMON_INCLUDE += -I$(MEDIA_COMMON_CHIP_INCLUDE)


export MAKE_FILE        := Makefile.$(OSTYPE)

export MAKE_DRV_FILE :=  $(MAIN_DIR)/build/kernel_template.mak
export MAKE_LIB_FILE := $(MAIN_DIR)/build/lib_template.mak
export MAKE_MACRO_FILE := $(MAIN_DIR)/build/macro_utils.mak

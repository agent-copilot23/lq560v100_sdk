# 0 -- gcc
CVERX=0

# 2 -- glibc
CVERY=2

ifeq ($(FPGA),n)
    export CVER?=.V$(CVERX)$(CVERY)
else
    export CVER?=_fpga.V$(CVERX)$(CVERY)
endif

##############################################################################
# Allocated the kernel position
export KERNEL_ROOT?=$(MAIN_DIR)/src/platform/linux/$(KERNEL_NAME)
##############################################################################

# Define cross compiler
export CLANG_CROSS :=
export CC:=$(CROSS)gcc
export AR:=$(CROSS)ar
export STRIP:=$(CROSS)strip
export OBJCOPY:=$(CROSS)objcopy

export REL_LIB := $(REL_DIR)/lib
export REL_KO  := $(REL_DIR)/ko
export REL_SAMPLE  := $(REL_DIR)/sample
export REL_OBJ := $(REL_DIR)/obj
export REL_INI := $(REL_DIR)/init


LIBS_LD_CFLAGS += -ldl -rdynamic
OMP_CFLAGS += -Wno-date-time
DRV_CFLAGS += -Wno-date-time


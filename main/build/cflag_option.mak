#cflag_option/mak

# secure CFLAGS
###############################################################################
export SECURE_LIBS_CFLAGS ?= -fstack-protector-strong -fPIC
export SECURE_EXEC_CFLAGS ?= -fPIE
ifneq ($(CONFIG_LLVM), y)
SECURE_EXEC_CFLAGS += -pie
endif
export SECURE_LD_CFLAGS   ?= -Wl,-z,relro -Wl,-z,noexecstack
ifneq ($(OT_GDB), y)
SECURE_EXEC_CFLAGS += -s
SECURE_LD_CFLAGS += -Wl,-z,now,-s
endif
###############################################################################

###############################################################################
# Common CFLAGS, build flags for all OMP libs and drivers
# -Wno-unused-parameter -Wno-missing-field-initializers: Too many false positives with these compilation options
OMP_EXTRA_CFLAGS = -Wextra -Wno-unused-parameter -Wno-missing-field-initializers

export OMP_CFLAGS := -Wall $(OMP_EXTRA_CFLAGS) -fsigned-char $(COMMON_INCLUDE)

# Driver CFLAGS, build flags for linux kernel kbuild EXTRA_CFLAGS drivers
# -Wno-sign-compare: Kernel include has many magic usage
export DRV_CFLAGS := -Wall $(OMP_EXTRA_CFLAGS) -Wno-sign-compare -fsigned-char $(COMMON_INCLUDE)

# Libs CFLAGS, build flags for linux libs
export LIBS_CFLAGS := -Wall $(OMP_EXTRA_CFLAGS) -fsigned-char -Wno-sign-compare $(COMMON_INCLUDE) $(SECURE_LIBS_CFLAGS)

# Executable program CFLAGS, build flags for linux libs
export LIBS_EXEC_CFLAGS := $(SECURE_EXEC_CFLAGS)

# Libs LDFLAGS, link flags for linux libs
export LIBS_LD_CFLAGS := -fno-common $(SECURE_LD_CFLAGS)
###############################################################################

# common CFLAGS
###############################################################################
OMP_CFLAGS     += -DVER_X=$(VER_X) -DVER_Y=$(VER_Y) -DVER_Z=$(VER_Z) -DVER_P=$(VER_P) -DVER_B=$(VER_B)
OMP_CFLAGS     += -D$(CONFIG_USER_BIT) -D$(CONFIG_KERNEL_BIT) -D$(OT_RLS_MODE)
LIBS_CFLAGS    += -ffunction-sections -fdata-sections -O2
ifneq ($(CONFIG_LLVM), y)
LIBS_CFLAGS    += -fno-aggressive-loop-optimizations
LIBS_LD_CFLAGS += -fno-aggressive-loop-optimizations
endif
###############################################################################

# debug CFLAGS
###############################################################################
ifeq ($(OT_GDB), y)
    OMP_CFLAGS += -g
    DRV_CFLAGS += -g
    LIBS_CFLAGS += -g -fPIC -fno-omit-frame-pointer -O0
endif

ifeq ($(OT_ASAN), y)
ASAN_FLAGS := -g -fsanitize=address -fno-omit-frame-pointer -fsanitize=leak
LIBS_CFLAGS += $(ASAN_FLAGS)
LIBS_LD_CFLAGS += $(ASAN_FLAGS) -lasan -lstdc++
endif

ifeq ($(OT_GCOV), y)
GCOV_FLAGS := -g -fprofile-arcs -ftest-coverage
LIBS_CFLAGS += $(GCOV_FLAGS)
LIBS_LD_CFLAGS += $(GCOV_FLAGS) -lgcov
endif
###############################################################################


# CPU CFLAGS for libs
###############################################################################
ifeq ($(OT_ARCH),$(filter $(OT_ARCH), lq560v100plus))
    LIBS_CFLAGS    += -mcpu=cortex-a55
    LIBS_LD_CFLAGS += -mcpu=cortex-a55
endif
###############################################################################

# BITS CFLAGS for mmap64
###############################################################################
ifeq ($(CONFIG_KERNEL_BIT),KERNEL_BIT_64)
    LIBS_CFLAGS     += -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE
    LIBS_LD_CFLAGS  += -D_FILE_OFFSET_BITS=64 -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE
endif
###############################################################################

ifeq ($(CONFIG_OT_VDEC_SUPPORT), y)
    OMP_CFLAGS += -D$(CONFIG_VDEC_IP)
endif

include cfg.mk

export CROSS_COMPILE ?= aarch64-otv02-linux-gnu-
ifneq ($(LLVM),)
export CC := clang
export LD := ld.lld
export AS := clang
export AR := llvm-ar
export NM := llvm-nm
export STRIP := llvm-strip
export OBJCOPY := llvm-objcopy
export OBJDUMP := llvm-objdump
export READELF := llvm-readelf
export RANLIB := llvm-ranlib
else
export CC := $(CROSS_COMPILE)gcc
export AR := $(CROSS_COMPILE)ar
export RANLIB := $(CROSS_COMPILE)ranlib
export LD := $(CROSS_COMPILE)ld
export OBJCOPY := $(CROSS_COMPILE)objcopy
export OBJDUMP := $(CROSS_COMPILE)objdump
endif
CSRC  = $(wildcard boot/*.c wildcard common/*.c)
SSRC  = $(wildcard boot/*.S)

CSRC += partition/partition_unpack.o

OBJS := $(patsubst %.c,%.o,$(CSRC) )
OBJS += $(patsubst %.S,%.o,$(SSRC) )

export CFLAGS := -fno-builtin -fno-common
CFLAGS += -Wall
CFLAGS += -I$(PWD)/include/
CFLAGS += -I$(PWD)/drivers/share_drivers/
CFLAGS += -I$(PWD)/drivers/share_drivers/include/
CFLAGS += -I$(PWD)/drivers/uart/
CFLAGS += -I$(PWD)/drivers/spi_flash/spi_nand/
CFLAGS += -I$(PWD)/drivers/spi_flash/spi_nor/
CFLAGS += -I$(PWD)/drivers/emmc/
CFLAGS += -I$(PWD)/drivers/pf8x/
CFLAGS += -I$(PWD)/partition/include/
CFLAGS += -I$(PWD)/common/

ifdef CFG_DEBUG_INFO
CFLAGS += -DCFG_DEBUG_INFO
endif

ifdef CFG_PMIC_SET
CFLAGS += -DCFG_PMIC_SET
endif

ifdef CONFIG_STL_TEST
CFLAGS += -DCONFIG_STL_TEST
endif

ifdef CFG_QUICKBOOT_SVB_WITHOUT_TEMPERATURE
CFLAGS += -DCFG_QUICKBOOT_SVB_WITHOUT_TEMPERATURE
endif

ifdef CFG_QUICKBOOT_ENABLE_EMMC_QUICK_MODE
CFLAGS += -DCFG_EMMC_QUICKBOOT_MODE
endif

CFLAGS += -DCFG_RAM_BASE_ADDR=$(RAM_BASE)

ifneq ($(LLVM),)
CFLAGS += -DLLVM_COMPILER
endif

CFLAGS += -march=armv8-a

CFLAGS += -c

CFLAGS += -Os -mstrict-align -fomit-frame-pointer -fno-exceptions -fno-asynchronous-unwind-tables -fno-unwind-tables -fstack-protector-strong

TARGET = otfl
BIN_PATH = pub

LDSCRIPT := linker.lds
LDFLAGS := -Bstatic -T $(LDSCRIPT) -Ttext $(TEXT_BASE)
LDFLAGS += drivers/libdrv.a

.SILENT:

default:
	$(MAKE) -C drivers/
	$(MAKE) -C boot/
	$(MAKE) -C common/
	$(MAKE) -C partition/
	sed -e 's/TEXT_BASE/$(TEXT_BASE)/' \
		-e 's/RAM_BASE/$(RAM_BASE)/' \
		$(LDSCRIPT).mk > $(LDSCRIPT)
	echo "  $(LD) $(OBJS) $(LDFLAGS) -Map $(TARGET).map -o $(TARGET) $(OBJCOPY) -O binary $(TARGET) $(TARGET).bin"
	mkdir -p $(BIN_PATH)
	$(LD) $(OBJS) $(LDFLAGS) -Map $(BIN_PATH)/$(TARGET).map -o $(BIN_PATH)/$(TARGET)
	$(OBJCOPY) -O binary $(BIN_PATH)/$(TARGET) $(BIN_PATH)/$(TARGET).bin
	$(OBJDUMP) -D $(BIN_PATH)/$(TARGET) > $(BIN_PATH)/$(TARGET).asm

clean:
	$(MAKE) -C drivers/ clean
	$(MAKE) -C boot/ clean
	$(MAKE) -C common/ clean
	$(MAKE) -C partition/ clean
	rm -rf $(BIN_PATH)
	rm -rf $(LDSCRIPT)


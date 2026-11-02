CUR_DIR = $(shell pwd)

ROOT_DIR = $(CUR_DIR)
include $(ROOT_DIR)/cfg.mak

$(warning "CHIP: $(CHIP)")
$(warning "CHIP_FAMILY: $(CHIP_FAMILY)")

.PHONY: mcu main tools mcu_clean main_clean

all: mcu main tools

clean: mcu_clean main_clean

mcu:
#	@cd mcu && $(MAKE)
	@echo "todo: mcu $(MAKE) "

mcu_clean:
#	@cd mcu && $(MAKE) clean
	@echo "todo: mcu $(MAKE) clean"


main:
	@cd main && $(MAKE)

main_clean:
	@cd main && $(MAKE) clean

tools: main
	@cd tools && $(MAKE)


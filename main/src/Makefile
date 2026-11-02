

.PHONY: clean common common_clean component component_clean kal kal_clean ext_drv ext_drv_clean safelib safelib_clean

all: common kal ext_drv component safelib

clean: common_clean kal_clean component_clean ext_drv_clean safelib_clean

common:
	@echo "~~~~~~~~~~Start build $@~~~~~~~~~~"
	@cd $@ && $(MAKE)

kal: common
	@echo "~~~~~~~~~~Start build $@~~~~~~~~~~"
	@cd $@ && $(MAKE)

ext_drv: common
	@echo "~~~~~~~~~~Start build $@~~~~~~~~~~"
	@cd $@ && $(MAKE)

component: common
	@echo "~~~~~~~~~~Start build $@~~~~~~~~~~"
	@cd $@ && $(MAKE)

safelib: common component
	@echo "~~~~~~~~~~Start build $@~~~~~~~~~~"
	@cd $@ && $(MAKE)

common_clean kal_clean ext_drv_clean component_clean safelib_clean:
	@echo "~~~~~~~~~~Start clean $(subst _clean,,$@)~~~~~~~~~~"
	@cd $(subst _clean,,$@) && $(MAKE) clean

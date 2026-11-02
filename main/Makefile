CUR_DIR = $(shell pwd)

.PHONY: clean platform src rootfs userfs upgrade datafs src_clean common common_clean platform_clean script script_clean otfl otfl_clean stllib stllib_clean

all: otfl platform src script upgrade datafs

clean: src_clean common_clean stllib_clean platform_clean script_clean otfl_clean openssl_clean

.NOTPARALLEL: otfl platform

otfl: openssl_pc
	@cd src/platform && $(MAKE) otflboot_build

otfl_clean:
	@cd src/platform && $(MAKE) otflboot_clean

ifeq ($(SECUREBOOT_BUILD), y)
platform: stllib openssl_pc
else
platform: stllib
endif
	@cd src/platform && $(MAKE) platform

platform_clean:
	@cd src/platform && $(MAKE) clean

src: platform
	@cd src && $(MAKE)

src_clean:
	@cd src && $(MAKE) clean

common:
	@$(MAKE) -f build/sdk_common.mak

common_clean:
	@$(MAKE) -f build/sdk_common.mak clean

script:
	@mkdir -p out/ko && cp script/load_lq560v100* out/ko/

script_clean:
	@rm -rf out/ko/load_lq560v100

rootfs: src
	@pushd src/platform; $(MAKE) rootfs_build;popd

userfs: src script
	@pushd src/platform; $(MAKE) userfs_build;popd

upgrade: rootfs userfs
	@pushd src/platform; $(MAKE) upgrade;popd

datafs: src
	@pushd src/platform; $(MAKE) datafs_build;popd

stllib:
	@cd src/safelib/a55_stl && $(MAKE)

stllib_clean:
	@cd src/safelib/a55_stl && $(MAKE) clean

OPENSSL_DIR:= $(CUR_DIR)/../opensource/openssl
openssl_pc:
	$(MAKE) -C $(OPENSSL_DIR)

openssl_clean:
	$(MAKE) -C $(OPENSSL_DIR) clean

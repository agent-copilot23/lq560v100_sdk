DATAFS_BASE	:= base
DATAFS_OUT	:= datafs

all: $(DATAFS_OUT)

$(DATAFS_OUT): FORCE
ifeq ($@, $(wildcard $@))
	@rm -rf $@
endif
	@cp -rf $(DATAFS_BASE) $@

clean:
	@rm -rf $(DATAFS_OUT)

.PHONY: FORCE
FORCE:

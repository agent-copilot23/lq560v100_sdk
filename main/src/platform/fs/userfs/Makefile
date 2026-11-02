USERFS_BASE	:= base
USERFS_OUT	:= userfs

all: $(USERFS_OUT)

$(USERFS_OUT): FORCE
ifeq ($@, $(wildcard $@))
	@rm -rf $@
endif
	@cp -rf $(USERFS_BASE) $@

clean:
	@rm -rf $(USERFS_OUT)

.PHONY: FORCE
FORCE:

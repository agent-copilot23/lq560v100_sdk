ENVTXT ?=
SIZE ?= 0x40000 # 256KB env.bin as default

ifneq ($(MAKECMDGOALS),clean)
ifeq ($(ENVTXT),)
$(error please specify plain text env by argument "ENVTXT=")
endif
endif

ABSPATH_ENV_TXT=$(shell cd $(PWD);readlink -f $(ENVTXT))
ENV_SIZE := $(SIZE)
ENV_BIN := $(CURDIR)/env.bin

.PHONY: all
all: env
.PHONY: env
env: $(ENV_BIN)

$(ENV_BIN): FORCE
	@mkenvimage -s $(ENV_SIZE) -o $(ENV_BIN) $(ABSPATH_ENV_TXT)
	@chmod +r $(ENV_BIN)
	@echo "Generate $(notdir $(ENV_BIN))"

.PHONY: clean
clean:
	rm -f $(ENV_BIN)

PHONY: FORCE
FORCE:

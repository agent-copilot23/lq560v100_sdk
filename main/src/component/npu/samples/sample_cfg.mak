ifeq ($(MAIN_CFG), )
    MAIN_CFG := $(MAIN_DIR)/build/main_cfg.mak
    include $(MAIN_CFG)
endif

INC_FLAGS := -I$(REL_INC)

CFLAGS += -L$(REL_LIB)
CFLAGS += -Wall $(INC_FLAGS) -D$(OT_ARCH) -Werror
CFLAGS += -lstdc++
CFLAGS += -lpthread -ldl
CFLAGS += $(LIBS_CFLAGS)
CFLAGS += $(LIBS_EXEC_CFLAGS)
CUR_DIR=$(shell pwd)
MAIN_DIR = $(CUR_DIR)/../../../..
ifeq ($(MAIN_CFG), )
    MAIN_CFG := $(MAIN_DIR)/build/main_cfg.mak
    include $(MAIN_CFG)
endif

CUR_SRCS := $(wildcard *.c)
TARGET := $(CUR_SRCS:%.c=%)

TARGET_PATH ?= $(PWD)

# target source
OBJS  := $(CUR_SRCS:%.c=%.o)

CFLAGS += $(COMM_INC)
CFLAGS += -I$(REL_INC)

DEP_LIBS += $(REL_LIB)/libot_proc.a
DEP_LIBS += $(LIBS_LD_CFLAGS)

.PHONY : clean all

all: $(TARGET)

$(TARGET):$(COMM_OBJ) $(OBJS)
	$(CC) $(CFLAGS) -lpthread -lm -o $(TARGET_PATH)/$@ $^ -Wl,--start-group $(DEP_LIBS) -Wl,--end-group


clean:
	@rm -f $(TARGET_PATH)/$(TARGET)
	@rm -f $(OBJS)
	@rm -f $(COMM_OBJ)
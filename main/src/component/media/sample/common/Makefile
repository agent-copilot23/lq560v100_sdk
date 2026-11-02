CUR_DIR=$(shell pwd)
MAIN_DIR = $(CUR_DIR)/../../../../../
include ../sample_cfg.mak

TARGET=sample_common
LIB=lib$(TARGET).a

CUR_SRC += $(wildcard *.c)
SNS_SRC = $(wildcard  ./data/**/*.c)
CUR_OBJ := $(CUR_SRC:%.c=%.o)
CUR_OBJ += $(SNS_SRC:%.c=%.o)
CUR_INC := -I$(COMMON_DIR)
CUR_INC += -I$(MAIN_DIR)/src/component/smr/include
CUR_INC += -I$(MAIN_DIR)/src/component/media/vrb/src/include
CUR_INC += -I$(MAIN_DIR)/src/component/media/vo/include
CUR_INC += -I$(MAIN_DIR)/src/component/media/vo/src/include
CUR_INC += -I$(MAIN_DIR)/src/component/gfb/include
CUR_INC += -I$(MAIN_DIR)/src/component/oge/include
CUR_INC += -I$(MAIN_DIR)/../opensource/json/out/include/cjson

ARFLAGS=rcv

.PHONY : clean all

all: $(CUR_OBJ) $(LIB)

clean:
	@rm $(CUR_OBJ) -f
	@rm $(LIB) -f

$(LIB): $(CUR_OBJ)
	@$(AR) $(ARFLAGS) $@ $(CUR_OBJ)

%.o:%.c
	@$(CC) $(CUR_INC) $(CFLAGS) -c $< -o $@
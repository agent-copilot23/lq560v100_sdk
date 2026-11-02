CUR_DIR=$(shell pwd)
MAIN_DIR = $(CUR_DIR)/../../../../..
include $(CUR_DIR)/../sample_cfg.mak

CUR_SRCS := $(wildcard *.c)
TARGET := $(CUR_SRCS:%.c=%)

TARGET_PATH ?= $(PWD)

# target source
OBJS  := $(CUR_SRCS:%.c=%.o)

CFLAGS += $(COMM_INC)

DEP_LIBS += $(SYS_LIBS)
DEP_LIBS += $(LIBS_LD_CFLAGS)
DEP_LIBS += $(REL_LIB)/libot_vo.a
DEP_LIBS += $(VI_LIBS)
DEP_LIBS += $(VPROC_LIBS)

.PHONY : clean all

all: $(TARGET)

$(TARGET):$(COMM_OBJ) $(OBJS)
	$(CC) $(CFLAGS) -lpthread -lm -o $(TARGET_PATH)/$@ $^ -Wl,--start-group $(DEP_LIBS) $(SENSOR_LIBS) $(AUDIO_LIBA)  $(INIPARSER_LIB)  $(REL_LIB)/libsecurec.a -Wl,--end-group


clean:
	@rm -f $(TARGET_PATH)/$(TARGET)
	@rm -f $(OBJS)
	@rm -f $(COMM_OBJ)

CUR_DIR=$(shell pwd)
MAIN_DIR = $(CUR_DIR)/../../../../..
include $(CUR_DIR)/../sample_cfg.mak

CUR_SRCS := $(wildcard *.c)
TARGET := $(CUR_SRCS:%.c=%)

TARGET_PATH ?= $(PWD)

# target source
OBJS  := $(CUR_SRCS:%.c=%.o)

CFLAGS += $(COMM_INC)
CFLAGS += -I$(CUR_DIR)/../../ao/src/include
CFLAGS += -I$(MAIN_DIR)/../common/include

DEP_LIBS += $(SYS_LIBS)
DEP_LIBS += $(LIBS_LD_CFLAGS)
DEP_LIBS += $(AO_LIBS)
DEP_LIBS += $(AUDIO_LIBA)
DEP_LIBS += $(VI_LIBS)
DEP_LIBS += $(REL_LIB)/libot_vo.a
DEP_LIBS += $(VPROC_LIBS)
DEP_LIBS += $(SAMPLE_COMMON_LIB)

.PHONY : clean all

all: $(TARGET)

$(TARGET):$(COMM_OBJ) $(OBJS)
	$(CC) $(CFLAGS) -lpthread -lm -o $(TARGET_PATH)/$@ $^ -Wl,--start-group $(DEP_LIBS) $(SENSOR_LIBS) $(AUDIO_LIBA)  $(INIPARSER_LIB)  $(REL_LIB)/libsecurec.a -Wl,--end-group


clean:
	@rm -f $(TARGET_PATH)/$(TARGET)
	@rm -f $(OBJS)
	@rm -f $(COMM_OBJ)

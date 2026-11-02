CUR_DIR=$(shell pwd)
MAIN_DIR = $(CUR_DIR)/../../../..
include $(CUR_DIR)/../../media/sample/sample_cfg.mak

CUR_SRCS := $(wildcard *.c)
TARGET := sample_avp_dpu

TARGET_PATH ?= $(PWD)

# target source
OBJS  := $(CUR_SRCS:%.c=%.o)

CFLAGS += $(COMM_INC)
CFLAGS += -I$(REL_INC)

DEP_LIBS += $(SYS_LIBS)
DEP_LIBS += $(LIBS_LD_CFLAGS)
DEP_LIBS += $(REL_LIB)/libot_avp_dpu_match.a
DEP_LIBS += $(REL_LIB)/libot_avp_dpu_rect.a
DEP_LIBS += $(REL_LIB)/libot_avp_dpu_impl.a
DEP_LIBS += $(REL_LIB)/libsecurec.a
DEP_LIBS += $(VI_LIBS)
DEP_LIBS += $(VPROC_LIBS)
DEP_LIBS += $(REL_LIB)/libot_vo.a
DEP_LIBS += $(CUR_DIR)/../../media/sample/common/libsample_common.a

$(info GABHKDDDDDDDDDDSRCDEP = $(DEP_LIBS))

$(info GABHKDDDDDDDDDD2 = $(CFLAGS))
.PHONY : clean all

all: $(TARGET)

$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) -lpthread -lm -o $(TARGET_PATH)/$@ $^ -Wl,--start-group $(DEP_LIBS) -Wl,--end-group

clean:
	@rm -f $(TARGET_PATH)/$(TARGET)
	@rm -f $(OBJS)
	@rm -f $(COMM_OBJ)


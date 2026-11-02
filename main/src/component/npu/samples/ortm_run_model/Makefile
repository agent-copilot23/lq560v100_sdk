CUR_DIR=$(shell pwd)
MAIN_DIR = $(CUR_DIR)/../../../../../
include $(CUR_DIR)/../sample_cfg.mak

CUR_SRCS := $(wildcard *.c)
UTIL_SRCS := $(wildcard $(CUR_DIR)/../sample_utils/*.c)
TARGET := ortm_run_model

TARGET_PATH := $(PWD)

# target source
OBJS  := $(CUR_SRCS:%.c=%.o)
OBJS  += $(UTIL_SRCS:%.c=%.o)


SYS_LIBS := $(REL_LIB)/libprotobuf-c.a

SYS_LIBS += $(REL_LIB)/libot_km.a
SYS_LIBS += $(REL_LIB)/libot_smr.a
SYS_LIBS += $(REL_LIB)/libot_media_common.a
SYS_LIBS += $(REL_LIB)/libot_media_sys.a
SYS_LIBS += $(REL_LIB)/libot_otp.a
SYS_LIBS += $(REL_LIB)/libot_cipher.a

NPU_LIBS := $(REL_LIB)/libot_avp_npu_impl.a
NPU_LIBS += $(REL_LIB)/libot_avp_npu_intf.a
NPU_LIBS += $(REL_LIB)/libot_avp_npu.a

CFLAGS += -I$(CUR_DIR)/../sample_utils
CFLAGS += $(COMM_INC)
CFLAGS += -g
CFLAGS += -fPIC -no-pie


DEP_LIBS += $(SYS_LIBS)
DEP_LIBS += $(NPU_LIBS)
DEP_LIBS += $(LIBS_LD_CFLAGS)

.PHONY : clean all

all: $(TARGET)

$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) -lpthread -lm -o $(TARGET_PATH)/$@ $^ -Wl,--start-group $(DEP_LIBS) $(REL_LIB)/libsecurec.a  -Wl,--end-group

clean:
	@rm -f $(TARGET_PATH)/$(TARGET)
	@rm -f $(OBJS)

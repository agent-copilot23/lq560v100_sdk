CUR_DIR=$(shell pwd)
MAIN_DIR = $(CUR_DIR)/../../../../../
include $(CUR_DIR)/../sample_cfg.mak

CUR_SRCS := $(wildcard *.c)
UTIL_SRCS := $(wildcard $(CUR_DIR)/../sample_utils/*.c)
TARGET := sample_shared_lib

TARGET_PATH := $(PWD)

# target source
OBJS  := $(CUR_SRCS:%.c=%.o)
OBJS  += $(UTIL_SRCS:%.c=%.o)

CFLAGS += -I$(CUR_DIR)/../sample_utils
CFLAGS += -lprotobuf-c -lot_otp -lot_cipher -lot_km -lot_smr -lot_media_sys -lot_media_common  -lot_isp -lot_acs -lot_ldci -lot_dehaze -lot_stats_estimate -lot_drc -lot_bnr
CFLAGS += -lot_avp_npu -lot_avp_npu_impl -lot_avp_npu_intf

CFLAGS += $(COMM_INC)
CFLAGS += -g

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

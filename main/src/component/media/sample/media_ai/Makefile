CUR_DIR=$(shell pwd)
MAIN_DIR = $(CUR_DIR)/../../../../../
include $(CUR_DIR)/../sample_cfg.mak

CUR_SRCS := $(wildcard *.c)
# TARGET := $(CUR_SRCS:%.c=%)
TARGET := sample_media_ai

TARGET_PATH ?= $(PWD)

# target source
OBJS  := $(CUR_SRCS:%.c=%.o)

CFLAGS += $(COMM_INC)

SYS_LIBS += $(REL_LIB)/libprotobuf-c.a
SYS_LIBS += $(REL_LIB)/libot_smr.a
SYS_LIBS += $(REL_LIB)/libot_km.a
SYS_LIBS += $(REL_LIB)/libot_otp.a
SYS_LIBS += $(REL_LIB)/libot_cipher.a

NPU_LIBS := $(REL_LIB)/libot_avp_npu_impl.a
NPU_LIBS += $(REL_LIB)/libot_avp_npu_intf.a
NPU_LIBS += $(REL_LIB)/libot_avp_npu.a

CVE_LIBS := $(REL_LIB)/libot_avp_cve.a
CVE_LIBS += $(REL_LIB)/libot_avp_cve_intf.a
CVE_LIBS += $(REL_LIB)/libot_avp_cve_impl.a


DEP_LIBS += $(SYS_LIBS)
DEP_LIBS += $(NPU_LIBS)
DEP_LIBS += $(CVE_LIBS)
DEP_LIBS += $(VI_LIBS)
DEP_LIBS += $(VPROC_LIBS)
DEP_LIBS += $(OGE_LIBS)
DEP_LIBS += $(REL_LIB)/libot_vo.a
DEP_LIBS += $(LIBS_LD_CFLAGS)
DEP_LIBS += $(SAMPLE_COMMON_LIB)
DEP_LIBS += $(MIPI_RX_LIBS)
DEP_LIBS += $(MIPI_TX_LIBS)
SAMPLE_COMMON_DIR:= $(PWD)/../common/
CFLAGS += -I$(CUR_DIR)/../vo
CFLAGS += -fPIC -no-pie -Werror

.PHONY : clean all

all: $(TARGET)

$(TARGET):$(COMM_OBJ) $(OBJS)
	$(CC) $(CFLAGS) -lpthread -lm -o $(TARGET_PATH)/$@ $^ -Wl,--start-group $(DEP_LIBS) $(SENSOR_LIBS) $(AUDIO_LIBA)  $(INIPARSER_LIB)  $(REL_LIB)/libsecurec.a -Wl,--end-group


clean:
	@rm -f $(TARGET_PATH)/$(TARGET)
	@rm -f $(OBJS)
	@rm -f $(COMM_OBJ)

cleanstream:
	@rm -f *.h264
	@rm -f *.h265

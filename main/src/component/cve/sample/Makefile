CUR_DIR=$(shell pwd)
MAIN_DIR = $(CUR_DIR)/../../../../
ifeq ($(MAIN_CFG), )
    MAIN_CFG := $(MAIN_DIR)/build/main_cfg.mak
    include $(MAIN_CFG)
endif



CUR_SRCS := $(wildcard *.c)
TARGET := sample_cve

TARGET_PATH := $(PWD)

# target source
OBJS  := $(CUR_SRCS:%.c=%.o)

SYS_LIBS := $(REL_LIB)/libot_media_common.a
SYS_LIBS += $(REL_LIB)/libot_smr.a
SYS_LIBS += $(REL_LIB)/libot_media_sys.a
CVE_LIBS := $(REL_LIB)/libot_avp_cve.a
CVE_LIBS += $(REL_LIB)/libot_avp_cve_intf.a
CVE_LIBS += $(REL_LIB)/libot_avp_cve_impl.a

INC_FLAGS := -I$(REL_INC)

CFLAGS += -Wall $(INC_FLAGS) -D$(OT_ARCH)
CFLAGS += -lstdc++ -lpthread -lm -ldl -g
CFLAGS += $(LIBS_CFLAGS)
CFLAGS += $(LIBS_EXEC_CFLAGS)
CFLAGS += $(COMM_INC)
CFLAGS += -I$(MAIN_DIR)/src/component/smr/include

DEP_LIBS += $(SYS_LIBS)
DEP_LIBS += $(CVE_LIBS)
DEP_LIBS += $(LIBS_LD_CFLAGS)

.PHONY : clean all

all: $(TARGET)

$(TARGET):$(OBJS)
	$(CC) $(CFLAGS) -lpthread -lm -o $(TARGET_PATH)/$@ $^ -Wl,--start-group $(DEP_LIBS) $(REL_LIB)/libsecurec.a  -Wl,--end-group

clean:
	@rm -f $(TARGET_PATH)/$(TARGET)
	@rm -f $(OBJS)

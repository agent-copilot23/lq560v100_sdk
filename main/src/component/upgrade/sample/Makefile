CUR_DIR=$(shell pwd)
MAIN_DIR = $(CUR_DIR)/../../../..
include $(MAIN_CFG)
MINIZIP=$(CUR_DIR)/minizip
SECURE=$(CUR_DIR)/secure
TARGET := sample_upgrade
TARGET_PATH := $(CUR_DIR)
CUR_SRCS := $(wildcard *.c)
CUR_SRCS += $(MINIZIP)/miniunz.c $(MINIZIP)/unzip.c $(MINIZIP)/ioapi.c
CUR_SRCS += $(wildcard $(SECURE)/*.c)

CFLAGS += -I$(CUR_DIR)/
CFLAGS += -I$(CUR_DIR)/../include -I$(CUR_DIR)/../src  $(COMMON_INCLUDE)
CFLAGS += -I$(CUR_DIR)/minizip/
CFLAGS += -I$(CUR_DIR)/secure/include/
CFLAGS += -I$(MAIN_DIR)/../opensource/zlib/out/board/include
CFLAGS += -I$(MAIN_DIR)/../opensource/xml2/out/board/include/libxml2

CFLAGS += -I$(REL_INC)
DEP_LIBS += $(REL_LIB)/libot_otp.a
DEP_LIBS += $(REL_LIB)/libot_cipher.a
DEP_LIBS += $(REL_LIB)/libot_km.a
DEP_LIBS += $(REL_LIB)/libot_smr.a
DEP_LIBS += $(REL_LIB)/libot_bc.a
DEP_LIBS += $(REL_LIB)/libsecurec.a

LD_FLAGS += -L$(MAIN_DIR)/../opensource/xml2/out/board/lib/ -lxml2 -lm
LD_FLAGS += -Wl,-Bstatic -L$(MAIN_DIR)/../opensource/zlib/out/board/lib/  -lz
LD_FLAGS += -Wl,-Bdynamic $(LIBS_LD_CFLAGS)

# target source
CUR_OBJS  := $(CUR_SRCS:%.c=%.o)

CUR_LDFLAGS += $(COMM_INC)

.PHONY : clean all $(EXT_CLEAN)

all: $(TARGET)

$(warning echo "srcs: $(CUR_SRCS)")
$(warning echo "objs: $(CUR_OBJS)")

$(TARGET): $(CUR_OBJS)
	@$(CC) $(LD_FLAGS) -o $(TARGET_PATH)/$@ $^ -Wl,--start-group $(DEP_LIBS) -Wl,--end-group $(LD_FLAGS)

clean: $(EXT_CLEAN)
	@rm -f $(TARGET_PATH)/$(TARGET)
	@rm -f $(CUR_OBJS)

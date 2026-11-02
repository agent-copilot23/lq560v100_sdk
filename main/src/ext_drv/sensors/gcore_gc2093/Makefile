#
# sensor lib Makefile
#
CUR_DIR = $(shell pwd)

MAIN_DIR = $(CUR_DIR)/../../../../
ifeq ($(MAIN_CFG), )
    MAIN_CFG := $(MAIN_DIR)/build/main_cfg.mak
    include $(MAIN_CFG)
endif

LIBPATH = ../lib
OBJPATH = ./obj

ARFLAGS = rcv
ARFLAGS_SO = $(LIBS_LD_CFLAGS)
ARFLAGS_SO += -shared -fPIC -o
CFLAGS  += -Wall -fPIC -D$(OT_FPGA)
CFLAGS  += -O2
CFLAGS  += $(LIBS_CFLAGS)

ifeq ($(ISP_INI_CONFIG), y)
CFLAGS += -D INIFILE_CONFIG_MODE
endif


ifeq ($(CONFIG_JPEGEDCF), y)
     CFLAGS += -D ENABLE_JPEGEDCF
endif

CUR_INC := ./
INC := -I$(REL_INC) -I$(CUR_INC)
INC += -I$(MAIN_DIR)/src/component/media/vi/include
INC += -I$(MAIN_DIR)/src/component/media/vi/include/3a
INC += -I$(MAIN_DIR)/src/component/media/common/include
INC += -I$(MAIN_DIR)/src/component/driver/mipi-rx/include
DEPEND_FILE := $(foreach file,$(subst -I, ,$(CUR_INC)), $(wildcard $(file)/*.h))

COMPILE = $(CC) $(CFLAGS) $(DFLAGS)

$(OBJPATH)/%.o: ./%.c $(DEPEND_FILE)
	@[ -e $(LIBPATH) ] || mkdir -p $(LIBPATH)
	@[ -e $(OBJPATH) ] || mkdir -p $(OBJPATH)
	@$(COMPILE) -o $@ -c $< $(INC)

SRCS = $(wildcard ./*.c)
OBJS = $(SRCS:%.c=%.o)
OBJS := $(OBJS:./%=$(OBJPATH)/%)

LIB_NAME := libsns_gc2093
TARGETLIB := $(LIBPATH)/$(LIB_NAME).a
TARGETLIB_SO := $(LIBPATH)/$(LIB_NAME).so

all:$(TARGETLIB)
$(TARGETLIB):$(OBJS)
	@($(AR) $(ARFLAGS) $(TARGETLIB) $(OBJS))
	@($(CC) $(ARFLAGS_SO) $(TARGETLIB_SO) $(OBJS))
	@mkdir -p $(REL_LIB)
	@cp $(TARGETLIB_SO) $(REL_LIB)
	@cp $(TARGETLIB) $(REL_LIB)
clean:
	@$(RM) -rf $(TARGETLIB) $(OBJS)
	@$(RM) -rf $(LIBPATH) $(OBJPATH)
	@$(RM) -rf $(REL_LIB)/$(LIB_NAME).a
	@$(RM) -rf $(REL_LIB)/$(LIB_NAME).so
aa:
	echo "$(INC)"

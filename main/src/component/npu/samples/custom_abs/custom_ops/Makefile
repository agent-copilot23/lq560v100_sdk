CUR_DIR = $(shell pwd)

MAIN_DIR = $(CUR_DIR)/../../../../../../
ifeq ($(MAIN_CFG), )
    MAIN_CFG := $(MAIN_DIR)/build/main_cfg.mak
    include $(MAIN_CFG)
endif

# user have to impl TARGET SRC_DIR LIB_DIR OBJ_DIR INSTLL_HEADERS CUR_INC
TARGET=ot_avp_npu_custom_opsets_extern

SRC_DIR=$(CUR_DIR)
LIB_DIR=$(CUR_DIR)/../../../libs
OBJ_DIR=$(CUR_DIR)/obj


CUR_INC := -I$(CUR_DIR)
CUR_INC += -I$(CUR_DIR)/impl
CUR_INC += -I$(SDK_COMMON_DIR)
CUR_INC += -I$(MAIN_COMMON_DIR)

# user have to impl CFLAGS LDFLAGS
CFLAGS += $(LIBS_CFLAGS) -Werror -fPIC
LDFLAGS += $(LIBS_LD_CFLAGS)


# include $(MAIN_DIR)/build/lib_template.mak
LIB_S=$(LIB_DIR)/lib$(TARGET).so

CUR_SRCS := $(shell find $(SRC_DIR)/impl/ -name "*.c")
CUR_OBJS := $(CUR_SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

ARFLAGS=rcv

.PHONY : clean all prepare install uninstall

all: prepare $(LIB_S) install

clean: uninstall
	@rm $(OBJ_DIR) -rf
	@rm $(LIB_S) -f

prepare:
	@echo -e "\e[0;32;1m--Compiling lib$(LIB_NAME)... Configs as follow:\e[0;36;1m"
	@echo "---- COMPILER=$(CC)"
	@echo "---- INC_DIR=$(CUR_INC)"
	@echo "---- CFLAGS=$(CFLAGS)"
	@echo "---- LDFLAGS=$(LDFLAGS)"
	@echo -e "\e[0m"
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(LIB_DIR)

install:
	@mkdir -p $(REL_LIB);cp $(LIB_S) $(REL_LIB)

uninstall:
	@cd $(REL_LIB) && rm  $(notdir $(LIB_S)) -f

# libot_avp_npu.a/libot_avp_npu.so
$(LIB_S): $(CUR_OBJS)
	$(CC) $(LDFLAGS) -shared -o $@  $(CUR_OBJS) $(LIB_DIR)/libot_avp_npu_custom_utils.a

$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CUR_INC) $(CFLAGS) -c $< -o $@
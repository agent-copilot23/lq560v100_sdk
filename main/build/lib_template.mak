
CFLAGS += -fPIC
LIB_A=$(LIB_DIR)/lib$(TARGET).a
LIB_S=$(LIB_DIR)/lib$(TARGET).so

CUR_SRC := $(shell find $(SRC_DIR) -name "*.c")
CUR_OBJS := $(CUR_SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
#$(warning "echo src: $(CUR_SRC) obj: $(CUR_OBJS)")
#$(warning "intall libs: $(INSTALL_LIBS) ")

ARFLAGS=rcv

.PHONY : clean all prepare install uninstall

all: prepare $(LIB_A) $(LIB_S) install

clean: uninstall
	@rm $(OBJ_DIR) -rf
	@rm $(LIB_A) $(LIB_S) -f

prepare:
	@echo -e "\e[0;32;1m--Compiling lib$(LIB_NAME)... Configs as follow:\e[0;36;1m"
	@echo "---- COMPILER=$(CC)"
	@echo "---- INC_DIR=$(CUR_INC)"
	@echo "---- CFLAGS=$(CFLAGS)"
	@echo "---- LDFLAGS=$(LDFLAGS)"
	@echo -e "\e[0m"
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(LIB_DIR)

install: $(LIB_A) $(LIB_S)
	@mkdir -p $(REL_LIB);cp $(LIB_A) $(LIB_S) $(INSTALL_LIBS) $(REL_LIB)
ifdef INSTALL_HEADERS
	@mkdir -p $(REL_INC);cp $(INSTALL_HEADERS) $(REL_INC)
endif
ifdef INSTALL_KOS
	@mkdir -p $(REL_KO);cp $(INSTALL_KOS) $(REL_KO)
endif

uninstall:
ifneq ($(wildcard $(REL_LIB)),)
	@cd $(REL_LIB) && rm  $(notdir $(LIB_A)) $(notdir $(LIB_S)) $(notdir $(INSTALL_LIBS)) -f
endif
ifneq ($(wildcard $(REL_INC)),)
ifdef INSTALL_HEADERS
	@cd $(REL_INC) && rm  $(notdir $(INSTALL_HEADERS)) -f
endif
endif
ifneq ($(wildcard $(REL_KO)),)
ifdef INSTALL_KOS
	@cd $(REL_KO) && rm  $(notdir $(INSTALL_KOS)) -f
endif
endif

$(LIB_A): $(CUR_OBJS)
	$(AR) $(ARFLAGS) $@ $(CUR_OBJS)

$(LIB_S): $(CUR_OBJS)
	$(CC) $(LDFLAGS) -shared -o $@  $(CUR_OBJS)


$(OBJ_DIR)/%.o:$(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CUR_INC) $(CFLAGS) -c $< -o $@


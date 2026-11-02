# target source
CUR_OBJS  := $(CUR_SRCS:%.c=%.o)

CUR_LDFLAGS += $(COMM_INC)

CUR_LDFLAGS += $(LIBS_LD_CFLAGS) 

.PHONY : clean all $(EXT_CLEAN)

all: $(TARGET)

$(warning echo "srcs: $(CUR_SRCS)")
$(warning echo "objs: $(CUR_OBJS)")

$(TARGET): $(CUR_OBJS)
	@$(CC) $(LD_FLAGS) -o $(TARGET_PATH)/$@ $^ -Wl,--start-group $(DEP_LIBS) $(REL_LIB)/libsecurec.a -Wl,--end-group

clean: $(EXT_CLEAN)
	@rm -f $(TARGET_PATH)/$(TARGET)
	@rm -f $(CUR_OBJS)

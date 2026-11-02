TARGET := mk_partition_env

CUR_SRCS := partition_env_tool.c utils.c
# target source
CUR_OBJS  := $(CUR_SRCS:%.c=%.o)

CFLAGS += -Iinclude -I/usr/include/libxml2 -g3
LD_FLAGS +=  -lxml2

.PHONY : clean all

all: $(TARGET)

$(warning echo "srcs: $(CUR_SRCS)")
$(warning echo "objs: $(CUR_OBJS)")

$(TARGET): $(CUR_OBJS)
	@$(CC) -o $@ $^  $(LD_FLAGS)

clean:
	@rm -f $(TARGET)
	@rm -f $(CUR_OBJS)
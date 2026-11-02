CSRC  = $(wildcard src/*.c)

CFLAGS += -nostdinc

#.SILENT:

default:
	echo "  CC    $(CSRC)"
	echo "$(CC) $(CFLAGS) $(CSRC)"
	$(CC) $(CFLAGS) $(CSRC)

clean:
	rm src/*.o test/*.o *.o -f

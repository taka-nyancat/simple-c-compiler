CFLAGS=-std=c11 -g -static
SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)

simple-cc: $(OBJS)
	$(CC) -o simple-cc $(OBJS) $(LDFLAGS)

$(OBJS): simple-cc.h

test: simple-cc
	./test.sh

clean:
	rm -f simple-cc *.o *~ tmp*

.PHONY: test clean
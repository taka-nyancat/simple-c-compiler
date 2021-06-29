CFLAGS=-std=c11 -g -static

simple-cc: simple-cc.c

test: simple-cc
		./test.sh
clean:
		rm -rf simple-cc *.o *~ tmp*

.PHONY: test clean
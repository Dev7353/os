CC=clang
CFLAGS=-Wall -pedantic -Werror -pthread

.PHONY: all clean

all: mthread

clean: 
	$(RM) *.o mthread

mthread: mthread.c
	$(CC) $(CFLAGS) $< -o $@

CC=clang
CFLAGS=-Wall -pedantic -Werror -g

.PHONY: all clean

all: mthread work

clean: 
	$(RM) *.o mthread work

mthread: mthread.c
	$(CC) $(CFLAGS) -pthread $< -o $@

work: work.c
	$(CC) $(CFLAGS) -lrt $< -o $@

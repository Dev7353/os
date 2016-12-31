CC=clang
CFLAGS=-Wall -pedantic -Werror -g
.PHONY: all clemae an

all: schedcheck

clean: 
	$(RM) -rf *.o schedcheck

schedcheck: schedcheck.c
	$(CC) $(CFLAGS) -lpthread $< -o $@
	





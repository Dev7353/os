CC=clang
CFLAGS=-Wall -pedantic -Werror -g
.PHONY: all clean

all: mutex

clean: 
	$(RM) -rf *.o mutex

mutex: mutex.c
	$(CC) $(CFLAGS) -lpthread $< -o $@



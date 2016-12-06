CC=clang
CFLAGS=-Wall -pedantic -Werror -g
.PHONY: all clean

all: mutex event

clean: 
	$(RM) -rf *.o mutex event

mutex: mutex.c
	$(CC) $(CFLAGS) -lpthread $< -o $@
	
event: event.c
	$(CC) $(CFLAGS) -lpthread $< -o $@



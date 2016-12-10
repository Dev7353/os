CC=clang
CFLAGS=-Wall -pedantic -Werror -g
.PHONY: all clean

all: mutex event prodcon

clean: 
	$(RM) -rf *.o mutex event prodcon output

mutex: mutex.c
	$(CC) $(CFLAGS) -lpthread $< -o $@
	
event: event.c
	$(CC) $(CFLAGS) -lpthread $< -o $@
	
prodcon: prodcon.c buffer.c prodcon-api.c
	$(CC) $(CFLAGS) -lpthread -lm $^ -o $@
	




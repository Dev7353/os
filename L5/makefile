CC=clang
CFLAGS=-Wall -pedantic -Werror -g
.PHONY: all clemae an

all: schedcheck scheduler

clean: 
	$(RM) -rf *.o schedcheck client scheduler *.eps measure*

schedcheck: schedcheck.c
	$(CC) $(CFLAGS) -lpthread $< -o $@

scheduler: scheduler-api.c scheduler.c 
	$(CC) $(CFLAGS) -lpthread $^ -o $@
		
	





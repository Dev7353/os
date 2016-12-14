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
	
prodcon: prodcon.c buffer.c prodcon-api.c busyloop.c
	$(CC) $(CFLAGS) -lpthread -lm $^ -o $@
	
test: 
	make clean
	make prodcon
	@echo '------------------------------------------------------------------------'
	./prodcon -i dict -p 80 -c 33 -L 100 -t 0 -r 0 -R 1 -a 12 -v -o output
	@echo '------------------------------------------------------------------------'
	@echo '>>FINISH<<'
	




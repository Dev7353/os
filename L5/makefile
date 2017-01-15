CC=clang
CFLAGS=-Wall -pedantic -Werror -g
.PHONY: all clemae an

all: schedcheck scheduler

clean: 
	$(RM) -rf *.o schedcheck client scheduler *.eps measure*

schedcheck: schedcheck.c
	$(CC) $(CFLAGS) -lpthread $< -o $@

scheduler: scheduler-api.c time-api.c scheduler.c 
	$(CC) $(CFLAGS) -lpthread $^ -o $@

test: 
	make clean
	make
	@echo 'START TEST'
	@echo '-----------------------------------------------------------------------'
	./scheduler --cn 2 --dn 5 --mn 0 --ce 4 --de 10 --me 3 --ct 4 --dt 2 --mt 5 --e 2 --E 6 --dish 8 --file measure 
	@echo '-----------------------------------------------------------------------'
	@echo 'END TEST'

test2: 
	make clean
	make
	@echo 'START TEST 2'
	@echo '-----------------------------------------------------------------------'
	./scheduler --cn 2 --dn 3 --mn 1 --ce 4 --de 10 --me 15 --ct 4 --dt 2 --mt 0 --e 4 --E 6 --dish 4 --file measure2 
	@echo '-----------------------------------------------------------------------'
	@echo 'END TEST'
	





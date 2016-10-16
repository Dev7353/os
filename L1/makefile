### .........  MAKEFILE .........................####
###                                              ####
### .......generic Makefile .....................####
CC=clang
CFLAGS=-Wall -pedantic -Werror -g
LDFLAGS=-lm
objects := $(patsubst %.c,%,$(wildcard *.c))
all: $(objects)
%: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

# CleanUp .......................................#

clean:
	rm -rf *.o $(objects) *.dSYM

### .........  MAKEFILE .........................####
###                                              ####
### .......generic Makefile .....................####
CC=clang
CFLAGS=-Wall -pedantic -Werror -g
LDFLAGS=-lm

all: hello

%: %.c
        $(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

# CleanUp .......................................#

clean:
        rm -f *.o hello

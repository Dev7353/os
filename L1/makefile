### .........  MAKEFILE .........................####
###                                              ####
### .......generic Makefile .....................####
CC=clang
CFLAGS=-Wall -pedantic -Werror -g
LDFLAGS=-lm
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)

ulMenuStack: $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

# CleanUp .......................................#

clean:
	rm -rf *.o ulMenuStack *.dSYM

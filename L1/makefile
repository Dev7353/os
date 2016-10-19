### .........  MAKEFILE .........................####
###                                              ####
### .......generic Makefile .....................####
CC=clang
CFLAGS=-Wall -pedantic -Werror -g
SOURCES = $(wildcard *.c)
OBJECTS = $(SOURCES:.c=.o)
TARGET = ulMenuStack

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
	./$@

clean:
	rm -rf *.o $(TARGET) *.dSYM

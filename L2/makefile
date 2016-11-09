CC=clang
CFLAGS=-Wall -ansi -pedantic -Werror -g 

SOURCES = $(TARGET).c $(OBJECTS:.o=.c)
HEADERS = $(OBJECTS:.o=.h)	 

.PHONY: all clean	
all: forkzombie myexec gentest1
clean:
	$(RM) forkzombie myexec $(TARGET).o $(OBJECTS) ids

forkzombie: forkzombie.c
myexec: myexec.c
gentest1: gentest1

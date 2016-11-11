CC=clang
CFLAGS=-Wall -ansi -pedantic -Werror -g 

SOURCES = $(TARGET).c $(OBJECTS:.o=.c)
HEADERS = $(OBJECTS:.o=.h)	 

.PHONY: all clean	
all: forkzombie myexec gentest1 forksqrt
clean:
	$(RM) forkzombie myexec forksqrt $(TARGET).o $(OBJECTS) ids

forkzombie: forkzombie.c
myexec: myexec.c
gentest1: gentest1
forksqrt: forksqrt.c inih/ini.c

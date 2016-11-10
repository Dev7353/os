CC=clang
CFLAGS=-Wall -pedantic -Werror -g 

SOURCES = $(TARGET).c $(OBJECTS:.o=.c)
HEADERS = $(OBJECTS:.o=.h)	 

.PHONY: all clean	
all: forkzombie myexec  gentest1 
clean:
	$(RM) -rf forkzombie myexec $(TARGET).o $(OBJECTS) ids *.dSYM gentest1

forkzombie: forkzombie.c
myexec: myexec.c
gentest1: gentest1.c genstacklib.c

CC=clang
CFLAGS=-Wall -pedantic -Werror -g

EXECUTIVES=myexec forkzombie gentest1 gentest2

TESTS_SOURCE = gentest1.c gentest2.c

.PHONY: all clean preparation genstacklib gentest	

all: preparation  genstacklib gentest

clean:
	$(RM) -rf *.o *.a ids *.dSYM $(EXECUTIVES)

preparation: 
	$(CC) $(CFLAGS) myexec.c -o myexec
	$(CC) $(CFLAGS) forkzombie.c -o forkzombie

genstacklib:
	$(CC) $(CFLAGS) -c -fpic genstacklib.c
	ar rs lib$@.a genstacklib.o
	 
gentest: 
	$(CC) $(CFLAGS) -c gentest1.c
	$(CC) gentest1.o -L. -lgenstacklib -o gentest1
	
	$(CC) $(CFLAGS) -c gentest2.c
	$(CC) gentest2.o -L. -lgenstacklib -o gentest2

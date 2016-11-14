CC=clang
CFLAGS=-Wall -pedantic -Werror -g

EXECUTIVES=myexec forkzombie gentest1 gentest2 gentest3 forksqrt

.PHONY: all clean preparation genstacklib gentest	

all: preparation  genstacklib gentest forksqrt

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
	
	$(CC) $(CFLAGS) -c gentest3.c
	$(CC) gentest3.o -L. -lgenstacklib -o gentest3

forksqrt: forksqrt.c inih/ini.c

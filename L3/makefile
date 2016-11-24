CC=clang
CFLAGS=-Wall -pedantic -Werror -g
.PHONY: all clean

all: mthread work matrix

clean: 
	$(RM) -rf *.o mthread work matrix *.dSYM result matr.*

mthread: mthread.c
	$(CC) $(CFLAGS) -lpthread $< -o $@

work: work.c
	$(CC) $(CFLAGS) -lrt $< -o $@

matrix: multiply_matrix.c matrix.c
	$(CC) $(CFLAGS) -lpthread -lrt $^ -o $@

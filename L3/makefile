CC=clang
CFLAGS=-Wall -pedantic -Werror -g

.PHONY: all clean

all: mthread work matrix

clean: 
	$(RM) *.o mthread work matrix 

mthread: mthread.c
	$(CC) $(CFLAGS) -pthread $< -o $@

work: work.c
	$(CC) $(CFLAGS) -lrt $< -o $@

matrix: matrix.c multiply_matrix.c

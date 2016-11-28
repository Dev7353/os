CC=clang
CFLAGS=-Wall -pedantic -Werror -g
.PHONY: all clean

all: mthread work matrix forksqrt

clean: 
<<<<<<< HEAD
	$(RM) -rf *.o *.dat mthread work matrix forksqrt *.dSYM result* matr.*
=======
	$(RM) -rf *.o mthread work matrix forksqrt *.dSYM
>>>>>>> racecondition

mthread: mthread.c
	$(CC) $(CFLAGS) -lpthread $< -o $@

work: work.c
	$(CC) $(CFLAGS) -lrt -lm $< -o $@

matrix: multiply_matrix.c matrix.c
	$(CC) $(CFLAGS) -lpthread -lrt $^ -o $@

forksqrt: forksqrt.c ini.c

<<<<<<< HEAD
test_matrix:
	make clean
	@echo '------------------------------------------------------------------------'
	@echo '[WARNING] Be sure you have numpy installed. Otherwise it wont work.'
	@echo '------------------------------------------------------------------------'
	make matrix
	@echo '------------------------------------------------------------------------'
	@echo 'Generate two 1000x1000 Matrices with values in range of -100 -> 100'
	@echo '------------------------------------------------------------------------'	
	python matr-gen.py 1000 1000 -100 100
	python matr-gen.py 1000 1000 -100 100
	@echo '------------------------------------------------------------------------'
	@echo '>>>Single Thread<<<'
	@echo '------------------------------------------------------------------------'
	./matrix -a matr.0 -b matr.1
	mv result result_single
	@echo '------------------------------------------------------------------------'
	@echo '>>>Multithread<<<'
	@echo '------------------------------------------------------------------------'
	./matrix -a matr.0 -b matr.1 -p -t 100
	diff result result_single
	@echo '------------------------------------------------------------------------'
	@echo '>>>FINISH<<<'
=======
matrix: matrix.c multiply_matrix.c

forksqrt: forksqrt.c ini.c
>>>>>>> racecondition

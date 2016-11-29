# BSYS L3
## Code Files

### further stuff
* makefile
* multiplyMatrix_h
	* API for multiplyMatrix
* work_test.sh 
	* tests the work.c and write real and measured execution time into a file.

### C
* mthread
	* Multithreaded sleep with random range
* work
	* Busyloop. Takes 2 Arguments: sec msec. Then wait active.
* multiplyMatrix
	* Implementation of multiplyMatrix.h
* matrix
	* Client for using multiplyMatrix
* gen_inputsqrt
	* generates random numbers and saves into file 
	
### Python
* matr-gen
	* generates matrix in size nxn 
* racecondition
	* increments a global variable with specified algorithm
* forksqrt
	* calculates root of number with heron method
* cyforksqrt
	* cythonized version of forksqrt
* measure
	* measures time for c library sqrt function, cythonized heron method, python heron method and python math sqrt function
* setup
	* created cythonized module 
	
## Additional sources
http://www.csee.wvu.edu/~jdm/classes/cs550/notes/tech/mutex/Peterson2.html
https://en.wikipedia.org/wiki/Peterson%27s_algorithm
https://docs.python.org/3/library/functions.html#any
http://stackoverflow.com/questions/3310049/proper-use-of-mutexes-in-python
https://www.tutorialspoint.com/python/python_multithreading.htm

https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
http://www.programmingsimplified.com/c-program-multiply-matrices


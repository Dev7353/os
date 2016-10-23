# BSYS L1

## Code files

### C
* ulMenuStack.c
	* Displays a menu where the user can select a certain menu point until he presses 6 and quits it
	* Invalid inputs will be caugth. The user will be notified that the input is invalid and can choose again.
	* In that case for example when the user try to push/pop on a uninitialized Stack then a notify message appears with the additional information why his input is 		 invalid and what he should do next. 
	* The user can only create one Stack. If he tries to create another one then an error message appears and he can choose between the other options. 
	* The user can create(1) and delete(5) a Stack, use push(2) and pop(3) operations and read the current number of elements(4). For quitting(6)
	* Allocate memory for the structure. The memory will set free when the stack is created (stack_created variable). If not then you cannot free the memory because there is no allocated memory.  
* ulstack.c
	* Contains the implementation for the ulstack.h file.
	* ULStackNew() allocate memory for the stackpointer with the start capacity of 4 elements with type unsigned long.
	* ULStackDispose() free the memory of the stackpointer (if its possible) and then reset the allocLenght, logLenght and stackpointer to 0 or NULL
	* ULStackPush() push a unsigned long value on the stachpointer. If the numbers of pushed values(logLength) equals the number of allowed values(allocLength) then increase the size of the stackpointer dynamically, increment logLength and duplicate the allocLenght. 
	* ULStackPop() remove a value from the top of the stackpointer and return the value. Decrease the logLength.
	* GetULStackNumberOfElements() return the logLength variable
	* The stack_created variable is initialized as integer to check if the stack is created.
* ulstack.h
	* Contains the prototypes of the functions (above described).
	* Contains the Structure for the Stack. 
	* Contains the start capacity and the global variable stack_created.

### Python
* menu.py
	* Displays a menu where the user can select a certain menu point until he presses 'q' and quits it
	* Invalid inputs will be caught and they don't end the program
	* Can return the python version, CPU-info and system info
	* Sends logging info/warnings/errors to mygetopt.log
* mygetopt.py
	* Executes certain functions, depending on the parameters that was given when having called this module from the command line.
	* Can return the git token of the current commit, the information that menu.py can also return and info on various memory usage stats given by the statm file in each program's PID folder
	* Is able to send the output to a specified file with filename instead of the command line
	* Sends logging info/warnings/errors to mygetopt.log
* helper.py
	* Helper module that gets used in menu.py and mygetopt.py. 
	* Contains functions that return the requested information.

# BSYS L1

## Code files

### C
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

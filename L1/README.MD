# BSYS L1

## Code files

### C
* ulMenuStack.c
* ulstack.c
* ulstack.h

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
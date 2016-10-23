# Based on lecture Systemprograming Chapter 5.
CC=clang
CFLAGS=-Wall -pedantic -Werror -g 			# Warns strict on possible failures and check if the ansi c standard is used

TARGET = ulMenuStack
OBJECTS = ulstack.o
SOURCES = $(TARGET).c $(OBJECTS:.o=.c)
HEADERS = $(OBJECTS:.o=.h)	 

.PHONY: all clean					# Pseudotargets to call explicit functions instead of running the whole makefile
all: $(TARGET)						# All possible endresults

clean:
	$(RM) $(TARGET) $(TARGET).o $(OBJECTS) depend 	# Delete the Target program, the object files and the depend file

depend: $(SOURCES) $(HEADERS)				# Definition of dependency rules. After simple changes the makefile build just the changed parts 
	$(CC) $(CFLAGS) -MM $(SOURCES) > $@

$(TARGET): $(TARGET).o $(OBJECTS)			# The Target consists of the object files 
	$(CC) $(LDFLAGS) $^ -o $@			# Links the object files and put the result in the target

include depend

CC=clang
CFLAGS=-Wall -pedantic -Werror -g

TARGET = ulMenuStack
OBJECTS = ulstack.o
SOURCES = $(TARGET).c $(OBJECTS:.o=.c)
HEADERS = $(OBJECTS:.o=.h)

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: all clean
all: $(TARGET)

clean:
	$(RM) $(TARGET) $(TARGET).o $(OBJECTS) depend

depend: $(SOURCES) $(HEADERS)
	$(CC) $(CPPFLAGS) -MM $(SOURCES) > $@

$(TARGET): $(TARGET).o $(OBJECTS)
	$(CC) $(LDFLAGS) $^ -o $@

include depend

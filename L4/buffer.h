#ifndef BUFFER_H
#define BUFFER_H

typedef int bool;
#define true 1
#define false 0
typedef struct {

	char** queue;
	int elements;
	bool isEmpty;
	bool isFull;
	int storage; //number of maximum Strings in buffer
	int stringLength; //number of maximum chars in one String

} Buffer;

void initBuffer(Buffer* buffer, int rows, int cols);
void add(Buffer* buffer, char* element, int row);
char* pop(Buffer* buffer, int row);
void destroyBuffer(Buffer* buffer);
#endif

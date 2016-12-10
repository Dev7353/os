#ifndef PRODCON_API_H
#define PRODCON_API_H
#include "buffer.h"

void readStdin(Buffer* b);
void readFile(Buffer* b, char* filename);
void printBuffer(Buffer* buffer);
#endif

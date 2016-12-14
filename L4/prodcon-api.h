#ifndef PRODCON_API_H
#define PRODCON_API_H
#include "buffer.h"

void readStdin(Buffer* b);
void readFile(Buffer* b, char* filename);
void printBuffer(Buffer* buffer);
void printIds(int threads);
int nextThread(int threads, int access[]);
bool threadsAreDone(int threads, int access[]);

bool verbose;
char* output;
int busyLoopFactor;

int consumerThreads;
bool* accessConsumer; 
int operations;

int producerThreads;
bool* accessProducer;
int additions;
int turn;
bool complete;
#endif

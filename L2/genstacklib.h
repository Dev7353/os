#ifndef GENSTACKLIB_H
#define GENSTACKLIB_H

# define STACK_CAPACITY 4

extern int stack_created;

typedef struct {
    void *elems;
    int elemSize;
    int logLength;
    int allocLength;
    void (*free)(void*);
} genStack;

void GenStackNew(genStack *s, int elemSize, void (*free)(void*));
void GenStackDispose(genStack *s);
void GenStackPush(genStack *s, const void *elemAddr);
void GenStackPop(genStack *s, void *elemAddr);
#endif

#ifndef GENSTACKLIB_H
#define GENSTACKLIB_H
#define GenStackInitialAllocationSize 4
#define true 0
#define false 1
/*http://stackoverflow.com/questions/1921539/using-boolean-values-in-c*/
typedef int bool;
extern bool stack_created;

typedef struct {
    void *elems;
    int elemSize;
    int logLength;
    int allocLength;
    void (*freefn)(void*);
} genStack;

void GenStackNew(genStack *s, int elemSize, void (*freefn)(void*));
void GenStackDispose(genStack *s);
void GenStackPush(genStack *s, const void *elemAddr);
void GenStackPop(genStack *s, void *elemAddr);
bool GenStackEmpty(const genStack *s);
#endif

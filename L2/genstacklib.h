#ifndef GENSTACKLIB_H
#define GENSTACKLIB_H
#define GenStackInitialAllocationSize 4
#define true 1
#define false 0

extern int stack_created;
/*http://stackoverflow.com/questions/1921539/using-boolean-values-in-c*/
typedef int bool

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
bool GenStackEmpty(const genStack *s);
#endif

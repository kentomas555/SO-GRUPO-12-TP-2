#ifndef STACK_H
#define STACK_H

#include <stdint.h>
#include <stddef.h>

#define STACK_SIZE 4096

void startStack(void* ptr);
void* createStack();
void freeStack(void * ptr);

extern void* createProcessStackframe(int argc, char** argv, void * currentRSP, void * program);
#endif 
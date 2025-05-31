#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdlib.h>

typedef struct MemoryManagerCDT * MemoryManagerADT;

MemoryManagerADT createMemoryManager(void * startMM);

void * allocMemory(size_t memoryToAllocate);

void freeMemory(void * freeAddress);

#endif
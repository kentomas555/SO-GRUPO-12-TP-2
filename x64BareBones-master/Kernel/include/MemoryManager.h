#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdlib.h>

typedef struct MemoryManagerCDT * MemoryManagerADT;

MemoryManagerADT createMemoryManager(void * startMM);

void * allocMemory(MemoryManagerADT mm, size_t memoryToAllocate);

void freeMemory(MemoryManagerADT mm, void * freeAddress);

#endif
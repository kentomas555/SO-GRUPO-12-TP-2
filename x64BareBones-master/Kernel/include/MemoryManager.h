#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdlib.h>
#include <stdint.h>

typedef struct MemoryManagerCDT * MemoryManagerADT;

void createMemoryManager();

void * allocMemory(size_t memoryToAllocate);

void freeMemory(void * freeAddress);

#endif
#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdlib.h>
#include <stdint.h>

typedef struct MemoryManagerCDT * MemoryManagerADT;

typedef struct memoryState{
    uint64_t total;
    uint64_t reserved;
    uint64_t free;
    
}memoryState;

void createMemoryManager();

void * allocMemory(size_t memoryToAllocate);

void freeMemory(void * freeAddress);

memoryState * getMemoryState();

#endif
#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER_H

#include <stdlib.h>
#include <stdint.h>
#include "lib.h"

typedef struct MemoryManagerCDT * MemoryManagerADT;

typedef struct memoryState{
    uint64_t total;
    uint64_t reserved;
    uint64_t free;
}memoryState;

int64_t *getSharedMemory();

// #ifdef BUDDY_MODE 
//     void createMemoryManagerBuddy();
// #else
//     void createMemoryManagerFreeList();
// #endif

void createMemoryManager();

void * allocMemory(size_t memoryToAllocate);

void freeMemory(void * freeAddress);

memoryState * getMemState();

uint64_t getCurrent();

#endif
#include "MemoryManager.h"

#define BLOCK_SIZE 0x1000        
#define BLOCK_QTY  0x1000       

typedef struct MemoryManagerCDT {
    void * freeArray[BLOCK_QTY]; 
    size_t totalSize;             
    size_t freeMemory;           
    size_t usedMemory;           
    size_t currentBlock;          
} MemoryManagerCDT;

static void * const mmControlStart = (void *) 0x900000;
static void * const heapStartAddress = (void *) ((char *)0x900000 + sizeof(struct MemoryManagerCDT));

static MemoryManagerADT mm = NULL;

void createMemoryManager() {
    mm = (MemoryManagerADT) mmControlStart;

    for (int i = 0; i < BLOCK_QTY; i++) {
        mm->freeArray[i] = (void *)((char *)heapStartAddress + BLOCK_SIZE * i);
    }

    size_t memorySize = BLOCK_QTY * BLOCK_SIZE;
    mm->totalSize = memorySize;
    mm->freeMemory = memorySize;
    mm->usedMemory = 0;
    mm->currentBlock = 0;
}

void * allocMemory(size_t memoryToAllocate) {
    if (mm == NULL || memoryToAllocate > BLOCK_SIZE || mm->currentBlock >= BLOCK_QTY) {
        return NULL;
    }

    void * block = mm->freeArray[mm->currentBlock++];

    mm->freeMemory -= BLOCK_SIZE;
    mm->usedMemory += BLOCK_SIZE;

    return block;
}

void freeMemory(void * freeAddress) {
    if (mm == NULL || mm->currentBlock == 0) {
        return;
    }

    mm->currentBlock--;
    mm->freeArray[mm->currentBlock] = freeAddress;

    mm->usedMemory -= BLOCK_SIZE;
    mm->freeMemory += BLOCK_SIZE;
}



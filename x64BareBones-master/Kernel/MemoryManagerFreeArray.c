#include "MemoryManager.h"

#define BLOCK_SIZE 0x1000        
#define BLOCK_QTY  0x1000    

#define HEAP_BASE_ADDRESS ((uintptr_t)0x700000)

typedef struct MemoryBlock {
    void* address;
    uint8_t isFree;
} MemoryBlock;

typedef struct MemoryManagerCDT {
    MemoryBlock blocks[BLOCK_QTY]; 
    size_t totalSize;             
    size_t freeMemory;           
    size_t usedMemory;           
    size_t currentBlock;          
} MemoryManagerCDT;

static MemoryManagerCDT memoryManager;

void createMemoryManager() {
    memoryManager.totalSize = BLOCK_SIZE * BLOCK_QTY;
    memoryManager.freeMemory = memoryManager.totalSize;
    memoryManager.usedMemory = 0;

    for (int i = 0; i < BLOCK_QTY; i++) {
        memoryManager.blocks[i].address = (void*)(HEAP_BASE_ADDRESS + i * BLOCK_SIZE);
        memoryManager.blocks[i].isFree = 1;
    }
}

void* allocMemory(size_t size) {
    if (size > BLOCK_SIZE || size == 0 || memoryManager.freeMemory < size)
        return NULL;

    for (int i = 0; i < BLOCK_QTY; i++) {
        if (memoryManager.blocks[i].isFree) {
            memoryManager.blocks[i].isFree = 0;
            memoryManager.usedMemory += BLOCK_SIZE;
            memoryManager.freeMemory -= BLOCK_SIZE;
            return memoryManager.blocks[i].address;
        }
    }

    return NULL;
}

void freeMemory(void* ptr) {
    for (int i = 0; i < BLOCK_QTY; i++) {
        if (memoryManager.blocks[i].address == ptr && !memoryManager.blocks[i].isFree) {
            memoryManager.blocks[i].isFree = 1;
            memoryManager.usedMemory -= BLOCK_SIZE;
            memoryManager.freeMemory += BLOCK_SIZE;
            return;
        }
    }
}



// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/MemoryManager.h"
#include <stdint.h>

#define BLOCK_SIZE 0x1000        // 4 KB per block
#define BLOCK_QTY  0x1000        // 4096 blocks = 16MB total

typedef struct MemoryBlock {
    size_t isUsed;
    void *address;
} MemoryBlock;

typedef struct MemoryManagerCDT {
    MemoryBlock memoryArray[BLOCK_QTY];
    size_t totalSize;
    size_t freeMemory;
    size_t usedMemory;
    size_t usedBlocks;
} MemoryManagerCDT;

int64_t *getSharedMemory() {
    static int64_t shared = 0;
    return &shared;
}

#define MM_BASE_ADDRESS 0x700000
#define MM_STRUCT_SIZE  (sizeof(MemoryManagerCDT))
static void * const mmControlStart = (void *) MM_BASE_ADDRESS;
static void * const heapStartAddress = (void *) (MM_BASE_ADDRESS + MM_STRUCT_SIZE);

static MemoryManagerADT mManager = NULL;

void createMemoryManager() {
    if (mManager != NULL) return; // Avoid reinitialization

    mManager = (MemoryManagerADT) mmControlStart;
    memset(mManager, 0, sizeof(MemoryManagerCDT));   // Clear struct for safety

    for (int i = 0; i < BLOCK_QTY; i++) {
        mManager->memoryArray[i].address = (void *)((uintptr_t)heapStartAddress + BLOCK_SIZE * i);
        mManager->memoryArray[i].isUsed = 0;
    }

    size_t memorySize = BLOCK_QTY * BLOCK_SIZE;
    mManager->totalSize = memorySize;
    mManager->freeMemory = memorySize;
    mManager->usedMemory = 0;
    mManager->usedBlocks = 0;
}

void * allocMemory(size_t memoryToAllocate) {
    if (mManager == NULL || memoryToAllocate > BLOCK_SIZE) {
        return NULL;
    }

    for (int i = 0; i < BLOCK_QTY; i++) {
        if (!mManager->memoryArray[i].isUsed) {
            mManager->memoryArray[i].isUsed = 1;
            mManager->usedBlocks++;
            mManager->usedMemory += BLOCK_SIZE;
            mManager->freeMemory -= BLOCK_SIZE;
            return mManager->memoryArray[i].address;
        }
    }
    return NULL;
}

void freeMemory(void *freeAddress) {
    if (mManager == NULL || freeAddress == NULL || mManager->usedBlocks == 0)
        return;

    for (int i = 0; i < BLOCK_QTY; i++) {
        MemoryBlock *block = &mManager->memoryArray[i];

        if (block->isUsed && block->address == freeAddress) {

            block->isUsed = 0;

            block->address = (void *)((char *)heapStartAddress + i * BLOCK_SIZE);

            mManager->usedBlocks--;
            mManager->usedMemory -= BLOCK_SIZE;
            mManager->freeMemory += BLOCK_SIZE;

            return;
        }
    }
}

static memoryState staticStateBuffer;

memoryState * getMemState() {
    if (mManager == NULL) return NULL;

    uint64_t total = 0;
    uint64_t used = 0;
    uint64_t free = 0;
    for (int i = 0; i < BLOCK_QTY; i++){
        total++;
        if(mManager->memoryArray[i].isUsed){
            used++;
        } else {
            free++;
        }
    }
    
    staticStateBuffer.total = total;
    staticStateBuffer.reserved = used;
    staticStateBuffer.free = free;

    return &staticStateBuffer;
}

uint64_t getCurrent() {
    return (mManager != NULL) ? mManager->usedBlocks : 0;
}

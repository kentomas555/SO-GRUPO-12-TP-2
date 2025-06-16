// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/MemoryManager.h"
#include "../include/videoDriver.h"
#include "../include/nativeLibrary.h"


#define MAX_ORDER 20 
#define MIN_ORDER 6   
#define RELATIVE_MAX (MAX_ORDER - MIN_ORDER + 1)  
#define MIN_BLOCK_SZ (1ULL << MIN_ORDER) 
#define CURRENT_BLOCKSIZE(order) (1ULL << order)
#define ALIGN_UP(addr, align) (((addr) + ((align) - 1)) & ~((align) - 1))


typedef struct Block{
	uint32_t order;
	struct Block * next;
}Block;

typedef struct MemoryManagerCDT {
	Block * freeList[RELATIVE_MAX];
	uint64_t totalBlocks;
    uint64_t totalSize;
    uint64_t usedMemory;
    uint64_t freeMemory;
}  MemoryManagerCDT;

static void * const mManagerControlStart = (void *) 0x700000;
static void * const heapStartAddress = (void *) ((char *)0x700000 + sizeof(struct MemoryManagerCDT));
static MemoryManagerADT mManager = NULL;

static int smallestOrder(size_t size){ 
	if (size <= MIN_BLOCK_SZ){
        return MIN_ORDER;
    }

	for (int i = MIN_ORDER; i <= MAX_ORDER; i++){ 
		if(CURRENT_BLOCKSIZE(i) >= size){
			return i; 
		}
	}
	return -1;
}
#define ALIGN_UP(addr, align) (((addr) + ((align) - 1)) & ~((align) - 1))
void createMemoryManager() {
    void * alignedmManagerAddr = (void *)ALIGN_UP((uintptr_t)mManagerControlStart, MIN_BLOCK_SZ);
    mManager = (MemoryManagerADT)alignedmManagerAddr;

    memset(mManager, 0, sizeof(MemoryManagerCDT)); 

	for (int i = 0; i < RELATIVE_MAX; i++) {
        mManager->freeList[i] = NULL;
    }
	mManager->totalBlocks = 0;  
    mManager->totalSize = CURRENT_BLOCKSIZE(MAX_ORDER); 
    mManager->usedMemory = 0;
    mManager->freeMemory = mManager->totalSize;

    Block * firstBlock = (Block *)ALIGN_UP((uintptr_t)heapStartAddress, CURRENT_BLOCKSIZE(MAX_ORDER));


	memset(firstBlock, 0, CURRENT_BLOCKSIZE(MAX_ORDER));
   
	firstBlock->order = MAX_ORDER; 
	firstBlock->next = NULL;
	mManager->freeList[MAX_ORDER - MIN_ORDER] = (Block*)firstBlock;

}



void * allocMemory(size_t memoryToAllocate){
    
	int i = smallestOrder(memoryToAllocate);

	if(i == -1){
		return NULL;
	}

	int currentOrder = i; 


	while (currentOrder < MAX_ORDER && mManager->freeList[currentOrder - MIN_ORDER] == NULL) {
        currentOrder++;
    }
    
    if (currentOrder > MAX_ORDER) {
        return NULL;
    }

   
    Block *block = mManager->freeList[currentOrder - MIN_ORDER]; 
    mManager->freeList[currentOrder - MIN_ORDER] = block->next;

    while (currentOrder > i) { 
        currentOrder--; 

        size_t size = CURRENT_BLOCKSIZE(currentOrder);
        Block *buddy2 = (Block *)((char *)block + size);

        buddy2->order = currentOrder;
        buddy2->next = mManager->freeList[currentOrder - MIN_ORDER];
        mManager->freeList[currentOrder - MIN_ORDER] = buddy2;

        block->order = currentOrder;
    }


    block->next = NULL;
    Block * chosen = block;

    chosen->next = block->next;
    chosen->order = block->order;

    mManager->totalBlocks++;
    mManager->usedMemory += CURRENT_BLOCKSIZE(currentOrder);
    mManager->freeMemory -= CURRENT_BLOCKSIZE(currentOrder);

    return (void *)((char *)chosen + sizeof(Block));

}


void freeMemory(void * freeAddress) {
	if(freeAddress == NULL){
		return;
	} 
    
	
	Block * block = (Block *)((char *)freeAddress - sizeof(Block));
	uint64_t order = block->order;

    if (block->order < MIN_ORDER || block->order > MAX_ORDER) {
        return;
    }

    mManager->totalBlocks--;
    mManager->usedMemory -= CURRENT_BLOCKSIZE(order);
    mManager->freeMemory += CURRENT_BLOCKSIZE(order);

    int merged = 0;

	while (order < MAX_ORDER) {
        size_t blockSize = CURRENT_BLOCKSIZE(order);
        uintptr_t buddyAddr = ((uintptr_t)block) ^ blockSize;
        Block *prev = NULL;
        Block *curr = mManager->freeList[order - MIN_ORDER];
        merged = 0; 

      
        while (curr != NULL) {
            if ((uintptr_t)curr == buddyAddr && curr->order == order) {
                
                if (prev == NULL) {
                    mManager->freeList[order - MIN_ORDER] = curr->next;
                } else {
                    prev->next = curr->next;
                }
                Block *merge = (Block *)((uintptr_t)block < (uintptr_t)curr ? block : curr);
                merge->order = order + 1;
                block = merge; 
                merged++;
                order++;
                break; 
            }
            prev = curr;
            curr = curr->next;
        }
        if(merged == 0){
            break;
        }
    }

    block->next = mManager->freeList[order - MIN_ORDER];
    mManager->freeList[order - MIN_ORDER] = block;
    return;
}


memoryState * getMemState(){
    memoryState * memState = allocMemory(sizeof(memoryState));
    if(memState == NULL){
        return NULL;
    }
    memState->total = mManager->totalSize;
    memState->reserved = mManager->usedMemory;
    memState->free = mManager->freeMemory;
    return memState;	
}

int64_t *getSharedMemory() {
    static int64_t shared = 0;
    return &shared;
}


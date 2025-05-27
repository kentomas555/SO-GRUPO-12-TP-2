#include "MemoryManager.h"

#define BLOCK_SIZE 0x1000 
#define BLOCK_QTY 0x1000 

typedef struct MemoryManagerCDT {
	size_t totalSize;
	size_t freeMemory;
	size_t usedMemory;
	size_t currentBlock;
	void * freeArray[BLOCK_QTY];
} MemoryManagerCDT;

MemoryManagerADT createMemoryManager(void * startMM) {
	MemoryManagerCDT * mm = (MemoryManagerCDT *) startMM;
	for (int i = 0; i < BLOCK_QTY; i++){
		mm->freeArray[i] = (void *) (startMM + BLOCK_SIZE * i); 
	}
	size_t memorySize = BLOCK_QTY * BLOCK_SIZE;
	mm->totalSize = memorySize;
	mm->freeMemory = memorySize;
	mm->usedMemory = 0;
	mm->currentBlock = 0;

	//return (MemoryManagerADT) mm;
	return mm;
}

void * allocMemory(MemoryManagerADT mm, size_t memoryToAllocate) {
	if(mm == NULL || memoryToAllocate > BLOCK_SIZE || mm->currentBlock >= BLOCK_QTY) {
		return NULL;
	}
	mm->freeMemory -= BLOCK_SIZE;
	mm->usedMemory += BLOCK_SIZE;

	return mm->freeArray[mm->currentBlock++];
}

void freeMemory(MemoryManagerADT mm, void * freeAddress){
	if(mm == NULL){
		return;
	}

	if(mm->currentBlock > 0) {
		mm->currentBlock--;
		mm->freeArray[mm->currentBlock] = freeAddress;
		mm->usedMemory -= BLOCK_SIZE;
		mm->freeMemory += BLOCK_SIZE;
	}
}


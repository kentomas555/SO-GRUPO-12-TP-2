#include "MemoryManager.h"
#define FREE 0
#define USED 1

typedef struct memoryNode {
	uint8_t isUsed;
	size_t memorySize;
	struct memoryNode * next;
	struct memoryNode * previous;
} memoryNode;

typedef struct MemoryManagerCDT {
	size_t totalSize;
	size_t freeMemory;
	size_t usedMemory;
	memoryNode * firstMemoryAddress;
} MemoryManagerCDT;

MemoryManagerADT createMemoryManager(void *const restrict memoryForMemoryManager, void *const restrict managedMemory, uint64_t memorySize) {
	MemoryManagerADT memoryManager = (MemoryManagerADT) memoryForMemoryManager;

	firstMemoryBlock = (memoryNode *) managedMemory;
	firstMemoryBlock->isUsed = FREE;
	firstMemoryBlock->memorySize = 0;
	firstMemoryBlock->next = NULL;
	firstMemoryBlock->previous = NULL;

	memoryManager->firstMemoryAddress = firstMemoryBlock;
	memoryManager->totalSize = memorySize;
	memoryManager->freeMemory = memorySize;
	memoryManager->usedMemory = 0;
	return memoryManager;
}

void *allocMemory(MemoryManagerADT const restrict memoryManager, const size_t memoryToAllocate) {
	memoryNode * allocation = memoryManager->firstMemoryAddress;

	memoryManager->freeMemory -= memoryToAllocate;
	memoryManager->usedMemory += memoryToAllocate;

	return (void *) allocation;
}

void *freeMemory(MemoryManagerADT const restrict memoryManager, const size_t memoryToFree){
	return 0;
}
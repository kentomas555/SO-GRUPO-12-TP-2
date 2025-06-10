// #include "MemoryManager.h"
// #include "videoDriver.h"

// #define MAX_ORDER 20 //tamaño total de memoria
// #define MIN_ORDER 5   // tamaño minimo de bloque
// #define MIN_BLOCK_SZ (1ULL << MIN_ORDER) //tamaño minimo del bloque
// #define CURRENT_BLOCKSIZE(order) ((1ULL << (order))*MIN_BLOCK_SZ) //calcula el tamaño correcto del block en base a su potencia

// typedef struct Block{
// 	int size;
// 	int order;
// 	struct Block * next;
// }Block;

// typedef struct MemoryManagerCDT {
// 	Block * freeList[MAX_ORDER];
// 	int totalBlocks;
//     int totalSize;
//     int usedMemory;
//     int freeMemory;
// } MemoryManagerCDT;

// //NODOS -> bloques

// static void * const mmControlStart = (void *) 0x700000;
// static void * const heapStartAddress = (void *) ((char *)0x700000 + sizeof(struct MemoryManagerCDT));
// static MemoryManagerADT mm = NULL;

// static int smallestOrder(int size){
// 	if (size <= MIN_BLOCK_SZ){
//         return 0;
//     }

// 	for (int i = 0; i <= MAX_ORDER - MIN_ORDER; i++){
// 		if(CURRENT_BLOCKSIZE(i) >= size){
// 			return i;
// 		}
// 	}
// 	return -1;
// }

// void createMemoryManager() {
// 	mm = (MemoryManagerADT) mmControlStart;

// 	for (int i = 0; i < MAX_ORDER; i++) {
//         mm->freeList[i] = NULL;
//     }
// 	mm->totalBlocks = 0;  
//     mm->totalSize = CURRENT_BLOCKSIZE(MAX_ORDER - 1);
//     mm->usedMemory = 0;
//     mm->freeMemory = CURRENT_BLOCKSIZE(MAX_ORDER - 1);
// 	Block * firstBlock = (Block *)(((uintptr_t)(heapStartAddress) + (MIN_BLOCK_SZ - 1)) & ~(MIN_BLOCK_SZ - 1));
// 	memset(firstBlock, 0, CURRENT_BLOCKSIZE(MAX_ORDER - 1));
//     firstBlock->size = CURRENT_BLOCKSIZE(MAX_ORDER - 1);
// 	firstBlock->order = MAX_ORDER - 1;
// 	firstBlock->next = NULL;
// 	mm->freeList[MAX_ORDER - 1] = (Block*)firstBlock;
// }

// void * allocMemory(size_t memoryToAllocate){
// 	int i = smallestOrder(memoryToAllocate);
// 	if(i == -1){
// 		return NULL;
// 	}

// 	int currentOrder = i;

// 	while (currentOrder < MAX_ORDER - 1 && mm->freeList[currentOrder] == NULL) {
//         currentOrder++; //no hay bloques disponibles en el orden actual, me fijo en un orden mas grande
//     }

//     if (currentOrder > MAX_ORDER - 1) {
//         return NULL; // no hay bloques disponibles
//     }

// 	while(currentOrder > i){ // divido el bloque en dos hasta llegar al minimo orden
// 		Block * auxBlock = mm->freeList[currentOrder];
// 		mm->freeList[currentOrder] = auxBlock->next; // saco el bloque de la lista
// 		currentOrder--; //me muevo un orden menor

// 		size_t size = CURRENT_BLOCKSIZE(currentOrder);
//         Block *auxBuddy1 = auxBlock;
//         Block *auxBuddy2 = (Block *)((char *)auxBlock + size);

//         auxBuddy1->order = currentOrder;
//         auxBuddy2->order = currentOrder;

//         // Add buddy2 back to the free list
// 		auxBuddy2->size = size;
//         auxBuddy2->next = mm->freeList[currentOrder];
//         mm->freeList[currentOrder] = auxBuddy2;

//         auxBuddy1->next = mm->freeList[currentOrder];
//         // Use buddy1 in the next iteration or as the final block
//         mm->freeList[currentOrder] = auxBuddy1;

// 	}

// 	// Remove the chosen block from the free list
//     Block * chosen = mm->freeList[currentOrder];
//     mm->freeList[currentOrder] = chosen->next;

//     chosen->next = NULL;
//     chosen->order = currentOrder;
//     chosen->size = CURRENT_BLOCKSIZE(currentOrder); // optional

//     mm->totalBlocks++;
//     mm->usedMemory += CURRENT_BLOCKSIZE(currentOrder);
//     mm->freeMemory -= CURRENT_BLOCKSIZE(currentOrder);
//     return (void *)chosen;
// }

// // void * allocMemory(size_t memoryToAllocate){
// //     int i = smallestOrder(memoryToAllocate);
// //     if(i == -1){
// //         return NULL;
// //     }

// //     int currentOrder = i;

// //     while (currentOrder < MAX_ORDER - 1 && mm->freeList[currentOrder] == NULL) {
// //         currentOrder++; // No hay bloques disponibles en el orden actual
// //     }

// //     if (currentOrder > MAX_ORDER - 1 || mm->freeList[currentOrder] == NULL) {
// //         return NULL; // No hay bloques disponibles
// //     }

// //     // // Dividir bloques hasta llegar al tamaño deseado
// //     // while (currentOrder > i) {
// //     //     Block *auxBlock = mm->freeList[currentOrder];
// //     //     mm->freeList[currentOrder] = auxBlock->next; // Sacar de la lista
// //     //     currentOrder--;

// //     //     size_t size = CURRENT_BLOCKSIZE(currentOrder);
// //     //     Block *auxBuddy1 = auxBlock;
// //     //     Block *auxBuddy2 = (Block *)((char *)auxBlock + size);

// //     //     auxBuddy1->order = currentOrder;
// //     //     auxBuddy2->order = currentOrder;
// //     //     auxBuddy2->size = size;

// //     //     // Solo el buddy que **NO** vas a usar se agrega a la lista libre
// //     //     auxBuddy2->next = mm->freeList[currentOrder];
// //     //     mm->freeList[currentOrder] = auxBuddy2;

// //     //     // auxBuddy1 se usa en la siguiente iteración (no se agrega a la lista)
// //     // }

// //     // // Sacar el bloque final de la lista
// //     // Block *chosen = auxBuddy1;
// //     // mm->freeList[currentOrder] = chosen->next;

// //     // chosen->next = NULL;
// //     // chosen->order = currentOrder;
// //     // chosen->size = CURRENT_BLOCKSIZE(currentOrder);

// //     // mm->totalBlocks++;
// //     // mm->usedMemory += chosen->size;
// //     // mm->freeMemory -= chosen->size;

// //     // return (void *)chosen;

// //     Block *auxBlock = NULL;

// // while (currentOrder > i) {
// //     if (!auxBlock) {
// //         auxBlock = mm->freeList[currentOrder];
// //         mm->freeList[currentOrder] = auxBlock->next;
// //     }

// //     currentOrder--;

// //     size_t size = CURRENT_BLOCKSIZE(currentOrder);
// //     Block *auxBuddy1 = auxBlock;
// //     Block *auxBuddy2 = (Block *)((char *)auxBlock + size);

// //     auxBuddy1->order = currentOrder;
// //     auxBuddy2->order = currentOrder;
// //     auxBuddy2->size = size;

// //     // Push buddy2 to free list
// //     auxBuddy2->next = mm->freeList[currentOrder];
// //     mm->freeList[currentOrder] = auxBuddy2;

// //     auxBlock = auxBuddy1; // Continue splitting this one
// // }

// // // Now auxBlock is the chosen one
// // Block *chosen = auxBlock;
// // chosen->next = NULL;
// // chosen->order = currentOrder;
// // chosen->size = CURRENT_BLOCKSIZE(currentOrder);

// // // Update memory stats
// // mm->totalBlocks++;
// // mm->usedMemory += chosen->size;
// // mm->freeMemory -= chosen->size;

// // return (void *)chosen;

// // }





// // void freeMemory(void * freeAddress) {
// //     nativeBigPrintf("entre al free", 300, 200);
// // 	if(freeAddress == NULL){
// // 		return;
// // 	}
	
// // 	Block * block = (Block*)freeAddress;
// // 	int order = block->order;

// // 	while (order < MAX_ORDER) {
// //         size_t blockSize = CURRENT_BLOCKSIZE(order);
// //         uintptr_t buddyAddr = ((uintptr_t)block) ^ blockSize;
// //         Block *prev = NULL;
// //         Block *curr = mm->freeList[order];

// //         // Look for buddy in the free list
// //         while (curr != NULL) {
// //             if ((uintptr_t)curr == buddyAddr) {
// //                 // Found buddy — remove it from free list
// //                 if (prev == NULL) {
// //                     mm->freeList[order] = curr->next;
// //                 } else {
// //                     prev->next = curr->next;
// //                 }

// //                 // Merge block and buddy
// //                 Block *merge = (Block *)((uintptr_t)block < (uintptr_t)curr ? block : curr);
// //                 merge->order = order + 1;
// //                 block = merge; // Continue with merged block
// //                 order++;
// //                 continue;
// //             }
// //             prev = curr;
// //             curr = curr->next;
// //         }

// //         // Buddy not free — just insert block into the free list
// //         block->next = mm->freeList[order];
// //         mm->freeList[order] = block;
// //         mm->totalBlocks--;
// //         mm->usedMemory -= CURRENT_BLOCKSIZE(order);
// //         mm->freeMemory += CURRENT_BLOCKSIZE(order);
// //         nativeBigPrintf("sali del free", 300, 220);
// //         return;
// //     }
// // }

// void freeMemory(void * freeAddress) {
// 	if(freeAddress == NULL){
// 		return;
// 	}
	
// 	Block * block = (Block*)freeAddress;
// 	int order = block->order;

// 	while (order < MAX_ORDER) {
//         size_t blockSize = CURRENT_BLOCKSIZE(order);
//         uintptr_t buddyAddr = ((uintptr_t)block) ^ blockSize;
//         Block *prev = NULL;
//         Block *curr = mm->freeList[order];

//         // Look for buddy in the free list
//         while (curr != NULL) {
//             if ((uintptr_t)curr == buddyAddr && curr->order == order) {
//                 // Found buddy — remove it from free list
//                 if (prev == NULL) {
//                     mm->freeList[order] = curr->next;
//                 } else {
//                     prev->next = curr->next;
//                 }

//                 // Merge block and buddy
//                 Block *merge = (Block *)((uintptr_t)block < (uintptr_t)curr ? block : curr);
//                 merge->order = order + 1;
//                 block = merge; // Continue with merged block
//                 order++;
//                 goto continue_merge;
//             }
//             prev = curr;
//             curr = curr->next;
//         }

//         // Buddy not free — just insert block into the free list
//         block->next = mm->freeList[order];
//         mm->freeList[order] = block;
//         mm->totalBlocks--;
//         mm->usedMemory -= CURRENT_BLOCKSIZE(order);
//         mm->freeMemory += CURRENT_BLOCKSIZE(order);
//         return;

// continue_merge:;
//     }
// }


// memoryState * getMemState(){
//     memoryState * memState = allocMemory(sizeof(memoryState));
//     if(memState == NULL){
//         return NULL;
//     }
//     memState->total = mm->totalSize;
//     memState->reserved = mm->usedMemory;
//     memState->free = mm->freeMemory;
//     return memState;	
// }

// uint64_t getCurrent();


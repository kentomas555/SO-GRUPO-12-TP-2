// #include "MemoryManager.h"
// #include "videoDriver.h"
// #include "nativeLibrary.h"

// #define MAX_ORDER 20 //tamaño total de memoria
// #define MIN_ORDER 5   // tamaño minimo de bloque
// #define RELATIVE_MAX (MAX_ORDER - MIN_ORDER + 1)  // => 20 - 5 + 1= 16 levels NO TENDRIA QUE ENTRAR AL 15, DEL 0 AL 14 ENTRA
// #define MIN_BLOCK_SZ (1ULL << MIN_ORDER) //tamaño minimo del bloque
// //#define CURRENT_BLOCKSIZE(order) (MIN_BLOCK_SZ << (order))  //calcula el tamaño correcto del block en base a su potencia

// #define CURRENT_BLOCKSIZE(order) (1ULL << order)

// #define ALIGN_UP(addr, align) (((addr) + ((align) - 1)) & ~((align) - 1))

// typedef struct Block{
// 	uint64_t size;
// 	uint64_t order;
//     // void * address;
// 	struct Block * next;
// }Block;

// typedef struct MemoryManagerCDT {
// 	Block * freeList[RELATIVE_MAX]; // del 0 - 15 inclusive
// 	uint64_t totalBlocks;
//     uint64_t totalSize;
//     uint64_t usedMemory;
//     uint64_t freeMemory;
// }  MemoryManagerCDT;//__attribute__((aligned(MIN_BLOCK_SZ)))

// //NODOS -> bloques

// static void * const mmControlStart = (void *) 0x700000;
// static void * const heapStartAddress = (void *) ((char *)0x700000 + sizeof(struct MemoryManagerCDT));
// static MemoryManagerADT mm = NULL;

// static char auxBuff[10];

// static int smallestOrder(size_t size){ // usamos indices relativos. freeList[0] <-> donde se guarda MIN_ORDER
// 	if (size <= MIN_BLOCK_SZ/*2^5*/){
//         return MIN_ORDER;
//     }

// 	for (int i = MIN_ORDER; i <= MAX_ORDER/*16*/; i++){ // del 5 al 20 inclusive -> 15 potencias
// 		if(CURRENT_BLOCKSIZE(i) >= size){
// 			return i; // en el primer alloc retorna 5
// 		}
// 	}
// 	return -1;
// }

// void createMemoryManager() {
// 	//mm = (MemoryManagerADT) mmControlStart;
//     //mm = (MemoryManagerADT)(((uintptr_t)(mmControlStart) + (MIN_BLOCK_SZ - 1)) & ~(MIN_BLOCK_SZ - 1));
//     void * alignedMmAddr = (void *)ALIGN_UP((uintptr_t)mmControlStart, MIN_BLOCK_SZ);
//     mm = (MemoryManagerADT)alignedMmAddr;

//     memset(mm, 0, sizeof(MemoryManagerCDT)); // setea todo en 0 en vez de basura

// 	for (int i = 0; i < RELATIVE_MAX; i++) {
//         mm->freeList[i] = NULL;
//     }
// 	mm->totalBlocks = 0;  
//     mm->totalSize = CURRENT_BLOCKSIZE(MAX_ORDER); //indice 20
//     mm->usedMemory = 0;
//     mm->freeMemory = mm->totalSize;/*CURRENT_BLOCKSIZE(MAX_ORDER - MIN_ORDER);*/

//     uintptr_t mmEndAddr = (uintptr_t)mm + sizeof(MemoryManagerCDT);
//     Block * firstBlock = (Block *)ALIGN_UP(mmEndAddr, MIN_BLOCK_SZ);
//     //Block * firstBlock = (Block *)(((uintptr_t)(heapStartAddress) + (MIN_BLOCK_SZ - 1)) & ~(MIN_BLOCK_SZ - 1));

// 	memset(firstBlock, 0, CURRENT_BLOCKSIZE(MAX_ORDER - MIN_ORDER));
//     firstBlock->size = mm->totalSize;//CURRENT_BLOCKSIZE(MAX_ORDER - MIN_ORDER);
// 	firstBlock->order = MAX_ORDER; // NO HAY QUE USARLO DE INDICE EN EL FREELIST
// 	firstBlock->next = NULL;
// 	mm->freeList[MAX_ORDER - MIN_ORDER] = (Block*)firstBlock;

//     // while(1){     //ESTO PRINTEA BIEN
//     //      itoaBase(mm->totalSize, auxBuff, 10);
//     // nativeBigPrintf("TotalSize: ", 160, 100);
//     // nativeBigPrintf(auxBuff, 300, 100);

//     // itoaBase(mm->freeMemory, auxBuff, 10);
//     // nativeBigPrintf("FreeMem: ", 160, 120);
//     // nativeBigPrintf(auxBuff, 300, 120);

//     // //MAXORDER - MINORDER = 15
//     // //

//     // itoaBase(mm->freeList[MAX_ORDER - MIN_ORDER]->order, auxBuff, 10); // it prints 20
//     // nativeBigPrintf("FirstBlock: ", 160, 140);
//     // nativeBigPrintf(auxBuff, 300, 140);
//     // }
// }

// void * allocMemory(size_t memoryToAllocate){
// 	int i = smallestOrder(memoryToAllocate); // retorna 5 al primer alloc

//     //  itoaBase(mm->totalSize, auxBuff, 10);
//     // nativeBigPrintf("TotalSize: ", 160, 100);
//     // nativeBigPrintf(auxBuff, 300, 100);

//     // itoaBase(mm->freeMemory, auxBuff, 10);
//     // nativeBigPrintf("FreeMem: ", 160, 120);
//     // nativeBigPrintf(auxBuff, 300, 120);

//     // //size_t alignedAddress = (((uintptr_t)(memoryToAllocate) + (MIN_BLOCK_SZ - 1)) & ~(MIN_BLOCK_SZ - 1));

//     // itoaBase(mm->freeList[MAX_ORDER - MIN_ORDER]->order, auxBuff, 10); // it prints 18
//     // nativeBigPrintf("FirstOrder: ", 160, 140);
//     // nativeBigPrintf(auxBuff, 300, 140);

// 	if(i == -1){
// 		return NULL;
// 	}

//     char auxBuff[10];

// 	int currentOrder = i; // en primera vuelta es 5

//     //MAX_ORDER - MIN_ORDER = 15
//     //RELATIVE_MAX = 16

// 	while (currentOrder < MAX_ORDER && mm->freeList[currentOrder] == NULL) {
//         currentOrder++; //no hay bloques disponibles en el orden actual, me fijo en un orden mas grande
//     }
//     //sale del while con currentOrder = 20
//     if(MIN_ORDER > i || MAX_ORDER < i){
//         itoaBase(currentOrder, auxBuff, 10);
//         nativeBigPrintf("ERROR ALLOC", 540, 220);
//         return NULL;
//     }
    

//     if (currentOrder > MAX_ORDER) {
//         return NULL; // no hay bloques disponibles
//     }

//     //Block *auxBuddy1 = NULL;
//     Block *block = mm->freeList[currentOrder]; // firstBlock
//     mm->freeList[currentOrder] = block->next; // nULL

//     // Split blocks until we reach the desired size
//     while (currentOrder > i) { // en primera vuelta 20 > 5
//         currentOrder--; // 19

//         size_t size = CURRENT_BLOCKSIZE(currentOrder);
//         Block *buddy2 = (Block *)((char *)block + size);

//         buddy2->order = currentOrder;
//         buddy2->size = size;
//         buddy2->next = mm->freeList[currentOrder];
//         mm->freeList[currentOrder] = buddy2;

//         block->order = currentOrder;
//         block->size = size;
//         // itoaBase(block->order, auxBuff, 10);
//         // nativeBigPrintf("Block order: ", 160, 340);
//         // nativeBigPrintf(auxBuff, 360, 340);
//     }

//     //salimos con block de order = 5(MIN_ORDER) y size = MIN_BLOCK_SIZE
//     //currentOrder = 5 = i

//     block->next = NULL;

// 	// Remove the chosen block from the free list
//     Block * chosen = block;
//     //mm->freeList[currentOrder] = chosen->next;

//     chosen->next = block->next;
//     chosen->order = block->order;
//     chosen->size = block->size; // optional

//     mm->totalBlocks++;
//     mm->usedMemory += CURRENT_BLOCKSIZE(currentOrder);
//     mm->freeMemory -= CURRENT_BLOCKSIZE(currentOrder);

// //     itoaBase((uint64_t)chosen, auxBuff, 16);
// // nativeBigPrintf("Block ptr: ", 160, 320);
// // nativeBigPrintf(auxBuff, 360, 320);

// // itoaBase(chosen->order, auxBuff, 10);
// // nativeBigPrintf("Block order: ", 160, 340);
// // nativeBigPrintf(auxBuff, 360, 340);
//     return (void *)((char *)chosen + sizeof(Block));

// }



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
//     // itoaBase((uint64_t)freeAddress, auxBuff, 16);
//     // nativeBigPrintf(auxBuff, 400, 420);
    
    
	
// 	Block * block = (Block *)((char *)freeAddress - sizeof(Block));
// 	uint64_t order = block->order;

//     // itoaBase(block->order, auxBuff, 10);
//     //     nativeBigPrintf(auxBuff, 400, 400);

//     if (block->order < MIN_ORDER || block->order > MAX_ORDER) {
//         // itoaBase(block->order, auxBuff, 10);
//         // nativeBigPrintf(auxBuff, 400, 400);
//         return;
//     }

//     mm->totalBlocks--;
//     mm->usedMemory -= CURRENT_BLOCKSIZE(order);
//     mm->freeMemory += CURRENT_BLOCKSIZE(order);

//     int merged = 0;

// 	while (order < MAX_ORDER) {
//         size_t blockSize = CURRENT_BLOCKSIZE(order);
//         uintptr_t buddyAddr = ((uintptr_t)block) ^ blockSize;
//         Block *prev = NULL;
//         Block *curr = mm->freeList[order];
//         merged = 0; //seteo en 0 para chequear el block en el nuevo order

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
//                 merged++;
//                 order++;
//                 break; // buddy found, exiting the while(curr != NULL), keep searching on higher order
//             }

//             prev = curr;
//             curr = curr->next;
//         }

//         if(merged == 0){
//             break;
//         }
//     }

//     block->next = mm->freeList[order];
//     mm->freeList[order] = block;
//     return;
// }


// memoryState * getMemState(){
//     memoryState * memState = allocMemory(sizeof(memoryState));
    
//     // itoaBase(mm->totalSize, auxBuff, 10);
//     // nativeBigPrintf(auxBuff, 540, 220);
//     if(memState == NULL){
//         return NULL;
//     }
//     memState->total = mm->totalSize;
//     memState->reserved = mm->usedMemory;
//     memState->free = mm->freeMemory;
//     return memState;	
// }

// uint64_t getCurrent();


#include "MemoryManager.h"
#include "videoDriver.h"
#include "nativeLibrary.h"

#define MAX_ORDER 20 //tamaño total de memoria
#define MIN_ORDER 6   // tamaño minimo de bloque
#define RELATIVE_MAX (MAX_ORDER - MIN_ORDER + 1)  // => 20 - 5 + 1= 16 levels NO TENDRIA QUE ENTRAR AL 15, DEL 0 AL 14 ENTRA
#define MIN_BLOCK_SZ (1ULL << MIN_ORDER) //tamaño minimo del bloque
//#define CURRENT_BLOCKSIZE(order) (MIN_BLOCK_SZ << (order))  //calcula el tamaño correcto del block en base a su potencia

#define CURRENT_BLOCKSIZE(order) (1ULL << order)

#define ALIGN_UP(addr, align) (((addr) + ((align) - 1)) & ~((align) - 1))


typedef struct Block{
	//uint64_t size;
	uint32_t order;
    // void * address;
	struct Block * next;
    //char padding[8];
}Block;

typedef struct MemoryManagerCDT {
	Block * freeList[RELATIVE_MAX]; // del 0 - 15 inclusive
	uint64_t totalBlocks;
    uint64_t totalSize;
    uint64_t usedMemory;
    uint64_t freeMemory;
}  MemoryManagerCDT;

//NODOS -> bloques

static void * const mmControlStart = (void *) 0x700000;
static void * const heapStartAddress = (void *) ((char *)0x700000 + sizeof(struct MemoryManagerCDT));
static MemoryManagerADT mm = NULL;

static char auxBuff[100];

static int smallestOrder(size_t size){ // usamos indices relativos. freeList[0] <-> donde se guarda MIN_ORDER
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
    void * alignedMmAddr = (void *)ALIGN_UP((uintptr_t)mmControlStart, MIN_BLOCK_SZ);
    mm = (MemoryManagerADT)alignedMmAddr;

    memset(mm, 0, sizeof(MemoryManagerCDT)); // setea todo en 0 en vez de basura

	for (int i = 0; i < RELATIVE_MAX; i++) {
        mm->freeList[i] = NULL;
    }
	mm->totalBlocks = 0;  
    mm->totalSize = CURRENT_BLOCKSIZE(MAX_ORDER); //indice 20
    mm->usedMemory = 0;
    mm->freeMemory = mm->totalSize;/*CURRENT_BLOCKSIZE(MAX_ORDER - MIN_ORDER);*/

    uintptr_t mmEndAddr = (uintptr_t)mm + sizeof(MemoryManagerCDT);
    Block * firstBlock = (Block *)ALIGN_UP(mmEndAddr, CURRENT_BLOCKSIZE(MAX_ORDER));
    //Block * firstBlock = (Block *)(((uintptr_t)(heapStartAddress) + (MIN_BLOCK_SZ - 1)) & ~(MIN_BLOCK_SZ - 1));

	memset(firstBlock, 0, CURRENT_BLOCKSIZE(MAX_ORDER));
    //firstBlock->size = mm->totalSize;//CURRENT_BLOCKSIZE(MAX_ORDER - MIN_ORDER);
	firstBlock->order = MAX_ORDER; // NO HAY QUE USARLO DE INDICE EN EL FREELIST
	firstBlock->next = NULL;
	mm->freeList[MAX_ORDER - MIN_ORDER] = (Block*)firstBlock;

}



void * allocMemory(size_t memoryToAllocate){
    
	int i = smallestOrder(memoryToAllocate); // retorna 5 al primer alloc

	if(i == -1){
		return NULL;
	}

    char auxBuff[100];

	int currentOrder = i; // en primera vuelta es 5

    //MAX_ORDER - MIN_ORDER = 15
    //RELATIVE_MAX = 16

	while (currentOrder < MAX_ORDER && mm->freeList[currentOrder - MIN_ORDER] == NULL) {
        currentOrder++; //no hay bloques disponibles en el orden actual, me fijo en un orden mas grande
    }
     
    //sale del while con currentOrder = 20
    if(MIN_ORDER > i || MAX_ORDER < i){
        itoaBase(currentOrder, auxBuff, 10);
        nativeBigPrintf("ERROR ALLOC", 540, 220);
        return NULL;
    }
    

    if (currentOrder > MAX_ORDER) {
        return NULL; // no hay bloques disponibles
    }

    //Block *auxBuddy1 = NULL;
    Block *block = mm->freeList[currentOrder - MIN_ORDER]; // firstBlock
    mm->freeList[currentOrder - MIN_ORDER] = block->next; // nULL

    // Split blocks until we reach the desired size
    while (currentOrder > i) { // en primera vuelta 20 > 5
        currentOrder--; // 19

        size_t size = CURRENT_BLOCKSIZE(currentOrder);
        Block *buddy2 = (Block *)((char *)block + size);

        buddy2->order = currentOrder;
        buddy2->next = mm->freeList[currentOrder - MIN_ORDER];
        mm->freeList[currentOrder - MIN_ORDER] = buddy2;

        block->order = currentOrder;
    }

    //salimos con block de order = 5(MIN_ORDER) y size = MIN_BLOCK_SIZE
    //currentOrder = 5 = i

    block->next = NULL;


    Block * chosen = block;
 

    chosen->next = block->next;
    chosen->order = block->order;


    mm->totalBlocks++;
    mm->usedMemory += CURRENT_BLOCKSIZE(currentOrder);
    mm->freeMemory -= CURRENT_BLOCKSIZE(currentOrder);

   

// itoaBase(sizeof(Block), auxBuff, 10);
// nativeBigPrintf("MINBLOCK: ", 160, 340);
// nativeBigPrintf(auxBuff, 360, 340);
// itoaBase(MIN_BLOCK_SZ, auxBuff, 10);
// nativeBigPrintf("MINBLOCSIZE: ", 160, 360);
// nativeBigPrintf(auxBuff, 360, 360);
    return (void *)((char *)chosen + sizeof(Block));

}


void freeMemory(void * freeAddress) {
	if(freeAddress == NULL){
		return;
	} 
    
	
	Block * block = (Block *)((char *)freeAddress - sizeof(Block));
	uint64_t order = block->order;

//     if (((uintptr_t)block & (CURRENT_BLOCKSIZE(order) - 1)) != 0) {
//     nativeBigPrintf("NO ALIGN", 100, 200);
//     itoaBase((uintptr_t)block, auxBuff, 16);
//     nativeBigPrintf(auxBuff, 300, 200);
// }

    if (block->order < MIN_ORDER || block->order > MAX_ORDER) {
        // itoaBase(block->order, auxBuff, 10);
        // nativeBigPrintf(auxBuff, 400, 400);
        return;
    }

    mm->totalBlocks--;
    mm->usedMemory -= CURRENT_BLOCKSIZE(order);
    mm->freeMemory += CURRENT_BLOCKSIZE(order);

    int merged = 0;

	while (order < MAX_ORDER) {
        size_t blockSize = CURRENT_BLOCKSIZE(order);
        uintptr_t buddyAddr = ((uintptr_t)block) ^ blockSize;
        Block *prev = NULL;
        Block *curr = mm->freeList[order - MIN_ORDER];
        merged = 0; //seteo en 0 para chequear el block en el nuevo order

      
        while (curr != NULL) {
            if ((uintptr_t)curr == buddyAddr && curr->order == order) {
                
                if (prev == NULL) {
                    mm->freeList[order - MIN_ORDER] = curr->next;
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

    block->next = mm->freeList[order - MIN_ORDER];
    mm->freeList[order - MIN_ORDER] = block;
    return;
}


memoryState * getMemState(){
    memoryState * memState = allocMemory(sizeof(memoryState));
    if(memState == NULL){
        return NULL;
    }
    memState->total = mm->totalSize;
    memState->reserved = mm->usedMemory;
    memState->free = mm->freeMemory;
    return memState;	
}

uint64_t getCurrent();


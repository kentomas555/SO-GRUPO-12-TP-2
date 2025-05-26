#include <stdint.h>
#include <lib.h>
#include <moduleLoader.h>
#include <idtLoader.h>
#include <videoDriver.h>
#include <soundDriver.h>
#include <clockTime.h>
#include <MemoryManager.h>

MemoryManagerADT getKernelMem();

//MemoryManagerADT createMemoryManager(void * heapStartAddress);
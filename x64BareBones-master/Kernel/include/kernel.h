#include <stdint.h>
#include <lib.h>
#include <moduleLoader.h>
#include <idtLoader.h>
#include <videoDriver.h>
#include <soundDriver.h>
#include <clockTime.h>
#include <MemoryManager.h>
#include <stack.h>
#include <scheduler.h>

MemoryManagerADT getKernelMem();

extern void _hlt();

//MemoryManagerADT createMemoryManager(void * heapStartAddress);
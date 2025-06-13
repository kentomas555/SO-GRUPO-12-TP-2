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
#include <interrupts.h>
#include <semaphore.h>
#include <fileDescriptors.h>

MemoryManagerADT getKernelMem();

// extern void _hlt();
// extern void _cli(void);
// extern void _sti(void);

//MemoryManagerADT createMemoryManager(void * heapStartAddress);
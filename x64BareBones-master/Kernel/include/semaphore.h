#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "MemoryManager.h"
#include "nativeLibrary.h"
#include "lib.h"
#include "scheduler.h"
#include "LinkedList.h"
#include "process.h"

#define MAX_SEM_CHAR 20

#define MAX_SEM_QTY 40

typedef struct sem_t {
  uint32_t value;
  //char semName[MAX_SEM_CHAR];
  int occupied;
  uint16_t id;
  uint8_t lock;
  LinkedListADT blockedQueue;
} sem_t;

// void initializeSemaphores();
uint64_t semInit(int32_t id,uint32_t value);
uint64_t semDestroy(int id);
void semPost(int id);
void semWait(int id);

extern void acquire(uint8_t * lock);
extern void release(uint8_t * lock);


#endif
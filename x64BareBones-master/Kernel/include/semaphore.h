#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "MemoryManager.h"
#include "nativeLibrary.h"
#include "lib.h"
#include "scheduler.h"
#include "LinkedList.h"

#define MAX_SEM_CHAR 20

#define MAX_SEM_QTY 15

typedef struct sem_t {
  uint32_t value;
  char semName[MAX_SEM_CHAR];
  int occupied;
  uint16_t id;
  uint8_t lock;
  LinkedListADT blockedQueue;
} sem_t;

void initializeSemaphores();
sem_t * semInit(char * semName, uint32_t value);
uint64_t semDestroy(char * semName);
void semPost(char * semName);
void semWait(char * semName);

extern void acquire(uint8_t * lock);
extern void release(uint8_t * lock);


#endif
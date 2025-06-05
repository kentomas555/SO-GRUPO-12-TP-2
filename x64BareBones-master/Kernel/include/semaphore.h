#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include "MemoryManager.h"

typedef int sem_t;

void initializeSemaphores();
uint64_t semInit(sem_t sem, int value);
uint64_t semDestroy(sem_t sem);
void semPost(sem_t sem);
void semWait(sem_t sem);

#endif
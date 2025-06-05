#include "semaphore.h"

static semaphoresADT semaphores;

void initializeSemaphores(){
  for (int i = 0; i < MAX_SEM_QTY; i++){
    semaphores->semaphores->occupied = 0;
    semaphores->semaphores->semName[0] = '\0';
    semaphores->semaphores[i].value = 0;
    semaphores->semaphores[i].lock = 1;
  }
  return;
}

uint64_t semInit(char * semName, int value){
    for (int i = 0; i < MAX_SEM_QTY; i++) {
        if (semaphores->semaphores[i].occupied && strCompare(semaphores->semaphores[i].semName, semName) == 0) {
            return (uint64_t)(&semaphores->semaphores[i]); // Ya existe
        }
    }

    //MEJORAR ESTA FUNCION
    for (int i = 0; i < MAX_SEM_QTY; i++) {
        if (!semaphores->semaphores[i].occupied) {
            semaphores->semaphores[i].occupied = 1;
            semaphores->semaphores[i].value = value;
            //strcpy
            memcpy(semaphores->semaphores[i].semName, semName, MAX_SEM_CHAR - 1);
            semaphores->semaphores[i].semName[MAX_SEM_CHAR - 1] = '\0';
            semaphores->semaphores->blockedQueue = initializeLinkedList();
            semaphores->semaphores[i].id = i;
            return (uint64_t)(&semaphores->semaphores[i]);
            
        }
    }
    return NULL; // No hay lugar
}

uint64_t semDestroy(sem_t * sem){
    if (sem == NULL || sem->occupied)
        return -1;

    sem->occupied = 0;
    sem->value = 0;
    sem->semName[0] = '\0';
    return 0;
}

void semPost(sem_t * sem){
  if (sem == NULL || !sem->occupied)
    return;
  acquire(&(sem->lock));
  sem->value++;
  release(&(sem->lock));
  int * pid = NULL;
  if(!isEmpty(sem->blockedQueue)){
    pid = (int *)dequeue(semaphores->semaphores[sem->id].blockedQueue);
    unblockProcess(*pid);
  }
  return;
}

void semWait(sem_t * sem){
  if (sem->occupied){
    acquire(&(sem->lock));
    if(sem->value > 0){
      sem->value--;
      release(&(sem->lock));
    } else {
      int pid = getCurrentPID();

      Node * node = allocMemory(sizeof(Node));
      if(node == NULL){
        return;
      }
      node->info = (void*)&pid;
      queue(semaphores->semaphores[sem->id].blockedQueue, node);
      release(&(sem->lock));
      blockProcess(pid);
      yield();
    }
  } 
}


#include "semaphore.h"

sem_t semaphores[MAX_SEM_QTY];

void initializeSemaphores(){
  for (int i = 0; i < MAX_SEM_QTY; i++){
    semaphores[i].occupied = 0;
    semaphores[i].semName[0] = '\0';
    semaphores[i].value = 0;
    semaphores[i].lock = 1;
  }
  return;
}

uint64_t semInit(char * semName, int value){
    for (int i = 0; i < MAX_SEM_QTY; i++) {
        if (semaphores[i].occupied && strCompare(semaphores[i].semName, semName) == 0) {
            return (uint64_t)(&semaphores[i]); // Ya existe
        }
    }

    //MEJORAR ESTA FUNCION
    for (int i = 0; i < MAX_SEM_QTY; i++) {
        if (!semaphores[i].occupied) {
            semaphores[i].occupied = 1;
            semaphores[i].value = value;
            //strcpy
            memcpy(semaphores[i].semName, semName, MAX_SEM_CHAR - 1);
            semaphores[i].semName[MAX_SEM_CHAR - 1] = '\0';
            semaphores[i].blockedQueue = initializeLinkedList();
            semaphores[i].id = i;
            return (uint64_t)(&semaphores[i]);
            
        }
    }
    return 0; // No hay lugar
}

static sem_t * getSemaphore(char * semName){
  if (semName == NULL)
    return NULL;

  for (int i = 0; i < MAX_SEM_QTY; i++) {
    if (semaphores[i].occupied && strCompare(semaphores[i].semName, semName) == 0) {
      return &semaphores[i];
    }
  }

  return NULL; 
}

uint64_t semDestroy(char * semName){
  sem_t * sem = getSemaphore(semName);
    if (sem == NULL || sem->occupied)
        return -1;
    sem->occupied = 0;
    sem->value = 0;
    sem->semName[0] = '\0';
    freeList(sem->blockedQueue);
    return 0;
}

void semPost(char * semName){
  sem_t * sem = getSemaphore(semName);
  if (sem == NULL || !sem->occupied)
    return;
  acquire(&(sem->lock));
  sem->value++;
  release(&(sem->lock));
  int * pid = NULL;
  if(!isEmpty(sem->blockedQueue)){
    pid = (int *)dequeue(semaphores[sem->id].blockedQueue);
    unblockProcess(*pid);
  }
  return;
}

void semWait(char * semName){
  sem_t * sem = getSemaphore(semName);
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
      queue(semaphores[sem->id].blockedQueue, node);
      release(&(sem->lock));
      blockProcess(pid);
      yield();
    }
  } 
}

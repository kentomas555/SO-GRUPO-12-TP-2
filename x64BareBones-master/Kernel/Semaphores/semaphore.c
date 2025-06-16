#include "semaphore.h"
#include "../include/videoDriver.h"
#include "scheduler.h"

static sem_t * semaphores[MAX_SEM_QTY] = {NULL};

uint64_t semInit(int32_t id,uint32_t value){

  if(id > MAX_SEM_QTY || id <= 0){
    return -1;
  }
  
  if(semaphores[id-1] == NULL){
    sem_t * semaphore = allocMemory(sizeof(sem_t));

    semaphore->value = value;
    semaphore->blockedQueue = initializeLinkedList(); 
    semaphore->lock = 1;
    semaphore->id = id;
    semaphores[semaphore->id-1] = semaphore;
    return semaphore->id;
  }
  // semaphore->refCount++;
  return id;
}

static sem_t * getSemaphore(int id){

  if(id <= 0 || id > MAX_SEM_QTY){
  
    return NULL;
  }

  return semaphores[id - 1];
}

uint64_t semDestroy(int id){
  sem_t * sem = getSemaphore(id);
    if (sem == NULL)
        return -1;
    freeList(sem->blockedQueue);
    //freeMemory(sem);
    //semaphores[id-1] = NULL;
    return 0;
}


void semPost(int id){


  sem_t * sem = getSemaphore(id);
  if (sem == NULL){
    return;
  }


  acquire(&(sem->lock));

  sem->value++;
 
  release(&(sem->lock));


  if(!isEmpty(sem->blockedQueue)){
    Pid pid = (Pid)(intptr_t)dequeue(sem->blockedQueue);
    unblockProcess(pid);
  }

  return;
}


void semWait(int id){


  sem_t * sem = getSemaphore(id);

  if (sem == NULL){
    return;
  }
  
    acquire(&(sem->lock));
    
    if(sem->value > 0){
      sem->value--;
     
      release(&(sem->lock));
    } else {
      Pid pid = (Pid) getCurrentPID();

      Node * node = allocMemory(sizeof(Node));
      if(node == NULL){
        return;
      }
    
      node->info = (void *)(uintptr_t)pid;
    


      push(sem->blockedQueue, node);
      release(&(sem->lock));
      
      setToblock(pid);

      yield();
    }
}
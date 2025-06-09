#include "semaphore.h"

static sem_t * semaphores[MAX_SEM_QTY] = {NULL};

static int availableID = 0;

// void initializeSemaphores(){
//   for (int i = 0; i < MAX_SEM_QTY; i++){
//     //semaphores[i] = allocMemory(sizeof(sem_t));
//     semaphores[i]->occupied = 0;
//     //semaphores[i]->semName[0] = '\0';
//     semaphores[i]->lock = 1;
//     semaphores[i]->id = i;
//   }
//   return;
// }


static int getAvailableSemID(){
  if(availableID >= MAX_SEM_QTY){
    return -1;
  }
  return availableID++;
}

sem_t * semInit(uint32_t value){

  if(availableID >= MAX_SEM_QTY){
    return NULL;
  }
  
  sem_t * semaphore = allocMemory(sizeof(sem_t));

  semaphore->value = value;
  semaphore->blockedQueue = initializeLinkedList();
  semaphore->lock = 1;
  semaphore->id = getAvailableSemID();

  semaphores[semaphore->id] = semaphore;

  return semaphore;
}

static sem_t * getSemaphore(int id){

  if(id >= MAX_SEM_QTY - 1){
    return NULL;
  }
  return semaphores[id];
}

uint64_t semDestroy(int id){
  sem_t * sem = getSemaphore(id);
    if (sem == NULL)
        return -1;
    freeList(sem->blockedQueue); // hay que hacer bien free
    freeMemory(sem);
    availableID--;
    return 0;
}

void semPost(int id){
  sem_t * sem = getSemaphore(id);
  if (sem == NULL)
    return;
  acquire(&(sem->lock));
  sem->value++;
  release(&(sem->lock));
  if(!isEmpty(sem->blockedQueue)){
    Node * auxNode = (Node *)dequeue(semaphores[sem->id]->blockedQueue); // funciona como un "pop"
    Pid pid = (Pid)(uint64_t)(auxNode->info); // a chequear
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
      node->info = (void*)(uint64_t)pid; // chequear si realmente es asi, deberia
      push(semaphores[sem->id]->blockedQueue, node);
      release(&(sem->lock));
      blockProcess(pid);
      yield();
    }
}

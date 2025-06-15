#include "semaphore.h"
#include "../include/videoDriver.h"
#include "scheduler.h"

static sem_t * semaphores[MAX_SEM_QTY] = {NULL};

static char aux[30];
static int y = 100;

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
  return id;
}

static sem_t * getSemaphore(int id){

  if(id >= MAX_SEM_QTY - 1){
    // nativeBigPrintf("id es mayor a MAXSEMQTY", 300, y);
    // y += 20;
    // itoaBase(id, aux, 10);
    // nativeBigPrintf(aux, 300, y);
    // y += 20;
    return NULL;
  }
  // nativeBigPrintf("retorno bien, este es el semID", 300, y);
  // y += 20;
  // itoaBase(semaphores[id - 1]->id, aux, 10);
  // nativeBigPrintf(aux, 300, y);
  // y += 20;
  return semaphores[id - 1];
}

uint64_t semDestroy(int id){
  sem_t * sem = getSemaphore(id);
    if (sem == NULL)
        return -1;
    freeList(sem->blockedQueue);
    freeMemory(sem);
    semaphores[id-1] = NULL;
    return 0;
}

// void semPost(int id){
//   // nativeBigPrintf("entre al semPost", 300, y);
//   // y += 20;
//   sem_t * sem = getSemaphore(id);
//   if (sem == NULL){
//     // nativeBigPrintf("semPostId es NULL y retorne", 300, y);
//     // y += 20;
//     return;
//   }
  
//   acquire(&(sem->lock));
//   sem->value++;
//   release(&(sem->lock));


//   if(!isEmpty(sem->blockedQueue)){
//     Node * auxNode = (Node *)dequeue(semaphores[sem->id]->blockedQueue); 
//     Pid pid = (Pid)(uint64_t)(auxNode->info->PID); // a chequear
//     unblockProcess(pid);
//   //   nativeBigPrintf("semPost desbloquee proceso dentro del !isEmpty", 300, y);
//   // y += 20;
//   }
//   // nativeBigPrintf("retorne semPost", 300, y);
//   // y += 20;
//   return;
// }

void semPost(int id){
  //nativeBigPrintf("semPost start", 300, y);
  //y += 20;

  sem_t * sem = getSemaphore(id);
  if (sem == NULL){
    nativeBigPrintf("semPost: sem is NULL", 300, y);
    y += 20;
    char aux[10];
    itoaBase(id, aux, 10);
    nativeBigPrintf(aux, 300, y);
    y += 20;
    return;
  }

  // nativeBigPrintf("semPost: acquired semaphore pointer", 300, y);
  // y += 20;

  acquire(&(sem->lock));
  // nativeBigPrintf("semPost: after acquire(lock)", 300, y);
  // y += 20;

  sem->value++;
  // nativeBigPrintf("semPost: incremented value", 300, y);
  // y += 20;

  release(&(sem->lock));
  // nativeBigPrintf("semPost: after release(lock)", 300, y);
  // y += 20;

  if(!isEmpty(sem->blockedQueue)){
    // nativeBigPrintf("semPost: blockedQueue not empty", 300, y);
    // y += 20;
    //Node * auxNode = (Node *)dequeue(sem->blockedQueue); //////////////////// DEVOLVIA INFO, NO NODE *
    Pid pid = (Pid)dequeue(sem->blockedQueue);
    // nativeBigPrintf("semPost: dequeued node", 300, y);
    // y += 20;
    
    // nativeBigPrintf("semPost: about to unblock process", 300, y);
    // y += 20;
    // itoaBase(pid, aux, 10);
    // nativeBigPrintf("PID to unblock:", 300, y);
    // y += 20;
    // nativeBigPrintf(aux, 300, y);
    // y += 20;
    unblockProcess(pid);
    // nativeBigPrintf("semPost: process unblocked", 300, y);
    // y += 20;
  }
  // nativeBigPrintf("semPost: done", 300, y);
  // y += 20;
  return;
}


void semWait(int id){
  // nativeBigPrintf("entre al semWait", 300, y);
  // y += 20;
  // itoaBase(id, aux, 10);
  // nativeBigPrintf(aux, 300, y);
  // y += 20;

  sem_t * sem = getSemaphore(id);

  if (sem == NULL){
    // nativeBigPrintf("semID no valido", 300, y);
    // y += 20;
    return;
  }
  
    acquire(&(sem->lock));
    
    if(sem->value > 0){
      sem->value--;
      // nativeBigPrintf("semValue > 0", 300, y);
      // y += 20;
      release(&(sem->lock));
    } else {
      Pid pid = (Pid) getCurrentPID();

      Node * node = allocMemory(sizeof(Node));
      if(node == NULL){
        return;
      }
      //node->info = (void*)(uint64_t)pid; // chequear si realmente es asi, deberia
      //node->info = (void *)(uint64_t)(uintptr_t)pid;  // semWait
      node->info = (void *)(uintptr_t)pid;

//       itoaBase(node->info, aux, 10);
// nativeBigPrintf("PID to block:", 300, y);
// y += 20;
// nativeBigPrintf(aux, 300, y);
// y += 20;

      push(sem->blockedQueue, node);
      release(&(sem->lock));

      
      
      //blockProcess(pid);
      setToblock(pid);
      
     
      // nativeBigPrintf("bloquee al proceso en semWait antes del yield", 300, y);
      // y += 20;
      yield();
    }
}
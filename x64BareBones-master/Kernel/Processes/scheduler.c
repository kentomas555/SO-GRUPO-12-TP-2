#include "../include/scheduler.h"
#include "../include/process.h"
#include "../include/MemoryManager.h"
#include "../include/LinkedList.h"


typedef struct SchedulerCDT{
  Node * processes[MAX_PROCESSES];
  LinkedListADT readyList;
  uint16_t currentPID;
  uint16_t currentPPID;
  //uint16_t nextPID;
  uint16_t processQty;
  uint16_t foregroundPID;
} SchedulerCDT;

SchedulerCDT * scheduler = NULL;

Pid availablePidValue = IDLE_PID;

void startScheduler() {
  scheduler = allocMemory(sizeof(SchedulerCDT));
  for (int i = 0; i < MAX_PROCESSES; i++){
    scheduler->processes[i] = NULL;
  }
  scheduler->currentPID = 0;
  scheduler->currentPPID = 0;
  //scheduler->nextPID = 0;
  scheduler->processQty = 0;
  scheduler->foregroundPID = 0;
  scheduler->readyList = initializeLinkedList();
}

void * schedule(void * currentRSP){
  //retornar siguiente proceso
  
  Node * currentRunning = scheduler->processes[scheduler->currentPID];
  
  //ceder al siguiente proceso
  if(currentRunning != NULL && ((PCB*)currentRunning->info)->status == RUNNING){

    //uso del priority del proceso
    if(((PCB*)currentRunning->info)->roundsLeft > 0){
      ((PCB*)currentRunning->info)->roundsLeft--;
      return currentRSP; // o currentRSP (ver, deberia ser lo mismo)
    }

    if(scheduler->currentPID != IDLE_PID){
      ((PCB*)currentRunning->info)->rsp = currentRSP;
      remove(scheduler->readyList, currentRunning); //dequeue(readyList);
      queue(scheduler->readyList, currentRunning);
    }
    
    ((PCB*)currentRunning->info)->status = READY;
  }

  Pid nextProcessPID = getNextProcess();
    
  return switchContext(nextProcessPID);
}

Pid getNextProcess(){
  listIterator(scheduler->readyList);
  Node * auxNode = (Node*) listNext(scheduler->readyList);
  if(auxNode == NULL){
    return IDLE_PID;
  }
  return ((PCB*)auxNode->info)->PID;
}

void * switchContext(Pid pid){
  // if(pid == IDLE_PID){ // ver caso especial idle

  // }
  Node * processAux = scheduler->processes[pid];
  PCB * nextPCB = (PCB *)processAux->info;
  nextPCB->roundsLeft = (int *)nextPCB->priority;
  nextPCB->status = RUNNING;
  scheduler->currentPID = pid;
  scheduler->currentPPID = nextPCB->parentPID;
  return nextPCB->rsp;
}

int getCurrentPID(){
   return scheduler->currentPID;
}

int getCurrentPPID(){
   return scheduler->currentPPID;
}

int getProcessQty(){
  return scheduler->processQty;
}

//SYSCALL para crear proceso
uint64_t onCreateProcess(char * processName, void * processProgram, char** args, Priority priority, int16_t fds[]){
  PCB * myNewProcess = createProcess(processName, processProgram, args, priority, fds);
  if(myNewProcess == NULL){
    return -1;
  }
  int currentPID = getCurrentPID();

  myNewProcess->parentPID = currentPID;

  myNewProcess->PID = availablePidValue;
  availablePidValue++;

  Node * node = allocMemory(sizeof(Node));
  if(node == NULL){
    return -1;
  }
  node->info = (void*)myNewProcess;

  scheduler->processes[myNewProcess->PID] = node;
  scheduler->processQty++;

  if(myNewProcess->PID != IDLE_PID){
    queue(scheduler->readyList, node);
  }  
  
  return 0;
}

void randomFunction(){
    printArray("Random function '_' \n");
    while (1)
        ;
}

uint64_t createDummyProcess(){
    int dummyFD[] = {0, 1};
    return onCreateProcess("DUMMY Function", (void *)randomFunction, NULL, AVERAGE_PRIORITY, dummyFD);
}


int blockProcess(Pid pid){
  PCB * pcb = scheduler->processes[pid];
  if(pcb->status == BLOCKED || pcb->status == KILLED || pcb->status == NULL){
    return -1;
  }
  pcb->status = BLOCKED;
  return 1;
}

int unblockProcess(Pid pid){
  PCB * pcb = scheduler->processes[pid];
  if(pcb->status != BLOCKED){
    return -1;
  }
  pcb->status = READY;
  return 1;
}

int getProcessPriority(Pid pid){
  PCB * pcb = scheduler->processes[pid];
  return pcb->priority;
}

int increaseProcessPriority(Pid pid){
  PCB * pcb = scheduler->processes[pid];
  if(pcb->priority == HIGHEST_PRIORITY){
    return pcb->priority;
  }
  pcb->priority++;
  return pcb->priority;
}

int decreaseProcessPriority(Pid pid){
  PCB * pcb = scheduler->processes[pid];
  if(pcb->priority == LOWEST_PRIORITY){
    return pcb->priority;
  }
  pcb->priority--;
  return pcb->priority;
}

//TODO: fix proper free
uint64_t killProcess(Pid pid){
  PCB * pcb = scheduler->processes[pid];
  if(pcb == NULL || pid == 0 || pid == 1){
    return -1;
  }
  pcb->status = KILLED;
  freeMemory(pcb);
  scheduler->processes[pid] = NULL;
}

void waitChilds(){
  return;
}

void printProcesses(){
  return;
}
#include "../include/scheduler.h"
#include "../include/process.h"
#include "../include/MemoryManager.h"
#include "../include/LinkedList.h"

typedef struct SchedulerCDT{
  PCB * processes[MAX_PROCESSES];
  LinkedListADT readyList;
  uint16_t currentPID;
  uint16_t currentPPID;
  //uint16_t nextPID;
  uint16_t processQty;
  uint16_t foregroundPID;
} SchedulerCDT;

SchedulerCDT * scheduler;

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
  return;
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
  scheduler->processes[myNewProcess->PID] = myNewProcess;
  scheduler->processQty++;

  node * Node = allocMemory(sizeof(node));
  if(Node == NULL){
    return -1;
  }
  Node->info = (void*)myNewProcess;
  queue(scheduler->readyList, Node);
  
  return 0;
}

void randomFunction(){
    printArray("Random function '_' \n");
    while (TRUE)
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

int increaseProcessPriority(Pid pid){
  PCB * pcb = scheduler->processes[pid];
  if(pcb->priority == HIGHEST_PRIORITY){
    return pcb->priority;
  }
  pcb->priority++;
  pcb->quantum += QUANTUM;
  return pcb->priority;
}

int decreaseProcessPriority(Pid pid){
  PCB * pcb = scheduler->processes[pid];
  if(pcb->priority == LOWEST_PRIORITY){
    return pcb->priority;
  }
  pcb->priority--;
  pcb->quantum -= QUANTUM;
  return pcb->priority;
}

void * switchContext(Pid pid){

}

uint64_t killProcess(Pid pid){
  PCB * pcb = scheduler->processes[pid];
  if(pcb == NULL){
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
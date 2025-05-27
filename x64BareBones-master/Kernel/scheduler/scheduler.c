#include "../include/scheduler.h"

typedef struct {
    Pid pid;
    Priority priority;
    int quantum;
    ProcessStatus status;
    void * currentRSP;
} processControlBlock;

processControlBlock processControlBlockTable[MAX_PROCESSES];

int currentProcess = 0;

int processesQty = 0;

void startScheduler(){
  
}

uint64_t createProcess(char * processName){
  //MALLOC para el proceso
  void * newStack = allocMemory(sizeof(processControlBlock));
  if(newStack == NULL){
    //error display
    return -1;
  }

  if(processesQty >= MAX_PROCESSES){
    return -1;
  }

  int availableSlot = 0;

  int flag = 1;
  for (int i = 0; i < MAX_PROCESSES && flag; i++){
    if(processControlBlockTable[i] == NULL){
      availableSlot = i;
      flag = 0;
    }
  }
  
  processControlBlock * newP;
  
  newP->priority = DEFAULT_PRIO;
  newP->pid = availableSlot;
  newP->status = READY;
  newP->currentRSP = newStack;
  newP->quantum = QUANTUM * DEFAULT_PRIO;

  processControlBlockTable[availableSlot] = newP;

  return processControlBlockTable[availableSlot]->pid;
}

int getCurrentPID(){
   return processControlBlockTable[currentProcess]->pid;
}

int blockProcess(Pid pid){
  processControlBlock * pcb = processControlBlockTable[pid];
  if(pcb->status == BLOCKED || pcb->status == FINISHED || pcb->status == KILLED, pcb->status == NULL){
    return -1;
  }
  pcb->status = BLOCKED;
  return 1;
}

int unblockProcess(Pid pid){
  processControlBlock * pcb = processControlBlockTable[pid];
  if(pcb->status != BLOCKED){
    return -1;
  }
  pcb->status = READY;
  return 1;
}

int increaseProcessPriority(Pid pid){
  processControlBlock * pcb = processControlBlockTable[pid];
  if(pcb->priority == HIGHEST_PRIORITY){
    return pcb->priority;
  }
  pcb->priority++;
  pcb->quantum += QUANTUM;
  return pcb->priority;
}

int decreaseProcessPriority(Pid pid){
  processControlBlock * pcb = processControlBlockTable[pid];
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

}

void waitChilds(){

}

void printProcesses(){

}
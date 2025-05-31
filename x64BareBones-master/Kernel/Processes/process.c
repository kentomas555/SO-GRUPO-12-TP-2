#include "../include/scheduler.h"
#include "../include/process.h"
#include "../include/MemoryManager.h"
#include "../include/stack.h"

uint64_t createProcess(char * processName, void * processProgram, char** args, Priority priority, int16_t fds[]){
  if(getProcessQty() >= MAX_PROCESSES || processName == NULL || processProgram == NULL || priority > HIGHEST_PRIORITY || priority < LOWEST_PRIORITY || fds == NULL){
    return -1;
  }

  processControlBlock * newPCB;

  //Copy name to PCB

  //TODO: fds
  for (int i = 0; i < FDS; i++){
    newPCB->fds[i] = i;
  }

  //Arg configs

  //TODO: see way to give pid to process
  
  newPCB->status = READY;
  newPCB->priority = DEFAULT_PRIO;
  newPCB->quantum = QUANTUM * newPCB->priority;
  newPCB->currentRSP = createProcessStackframe(); //TODO: proper call to createProcessStackFrame

  return newPCB->pid;
}
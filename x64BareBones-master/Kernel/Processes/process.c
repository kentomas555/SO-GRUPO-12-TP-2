#include "../include/scheduler.h"
#include "../include/process.h"
#include "../include/MemoryManager.h"
#include "../include/stack.h"
#include "../include/lib.h"

PCB * createProcess(char * processName, void * processProgram, char** args, Priority priority, int16_t fds[]){
  if(getProcessQty() >= MAX_PROCESSES || processName == NULL || processProgram == NULL || priority > HIGHEST_PRIORITY || priority < LOWEST_PRIORITY || fds == NULL){
    return -1;
  }

  PCB * newPCB;

  newPCB->rsp = createStack();

  //Copy name to PCB
  int nameLength = strlen(processName);
  newPCB->processName = allocMemory(nameLength + 1);
  if(newPCB->processName == NULL){
    return -1;
  }
  memcpy(newPCB->processName, processName, nameLength);
  newPCB->processName[nameLength] = '\0';
  
  //TODO: fds
  for (int i = 0; i < FDS; i++){
    newPCB->fds[i] = i;
  }

  //Arg configs
  newPCB->argc = countArguments(args);
  newPCB->argv = args;
  //TODO: see way to give pid to process
  
  
  newPCB->status = READY;
  newPCB->priority = DEFAULT_PRIO;
  newPCB->rsp = createProcessStackframe(newPCB->argc, newPCB->argv, newPCB->rsp, processProgram); //TODO: proper call to createProcessStackFrame
  return newPCB;
}
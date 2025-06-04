#include "../include/scheduler.h"
#include "../include/process.h"
#include "../include/MemoryManager.h"
#include "../include/stack.h"
#include "../include/lib.h"

//void * processProgram

PCB * createProcess(char * processName, mainFunc processProgram, char** args, Priority priority, int16_t fds[]){
  
  if(getProcessQty() >= MAX_PROCESSES || processName == NULL || processProgram == NULL || priority > HIGHEST_PRIORITY || priority < LOWEST_PRIORITY || fds == NULL){
    return NULL;
  }

  PCB * newPCB = allocMemory(sizeof(PCB));
  if(newPCB == NULL){
    return NULL;
  }

  newPCB->rbp = createStack();
  if(newPCB->rbp == NULL) {
    return NULL;
  }

  //Copy name to PCB
  int nameLength = strlen(processName);
  newPCB->processName = allocMemory(nameLength + 1);
  if(newPCB->processName == NULL){
    return NULL;
  }

  memcpy(newPCB->processName, processName, nameLength);
  newPCB->processName[nameLength] = '\0';
  
  //TODO: fds VER PARA SEMAPHORES
  newPCB->fds[0] = 0;
  newPCB->fds[1] = 1;

  //Arg configs
  newPCB->argc = countArguments((void **)args);
  newPCB->argv = args;
  
  newPCB->status = READY;
  newPCB->priority = priority;
  newPCB->roundsLeft = 0;

  newPCB->rsp = createProcessStackframe(newPCB->argc, newPCB->argv, newPCB->rbp, processProgram); //TODO: proper call to createProcessStackFrame

  return newPCB;
}
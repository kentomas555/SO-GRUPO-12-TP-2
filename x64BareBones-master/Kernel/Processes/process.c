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
  ///newPCB->argv = args;

  int argc = newPCB->argc;
  if (argc > 0) {
    newPCB->argv = allocMemory(sizeof(char *) * (argc + 1));  // +1 for NULL
    if(newPCB->argv == NULL){
      return NULL;
    }
    for (int i = 0; i < argc; i++) {
        int len = strlen(args[i]);
        newPCB->argv[i] = allocMemory(len + 1);
        memcpy(newPCB->argv[i], args[i], len);
        newPCB->argv[i][len] = '\0';
    }
    newPCB->argv[argc] = NULL;
  } else {
    newPCB->argv = NULL;
  }

  //setup children
  for(int i = 0; i < MAX_PROCESSES; i++){
    newPCB->children[i] = -1;
  }
  newPCB->childrenQty = 0;

  
  newPCB->status = READY;
  newPCB->priority = priority;
  newPCB->roundsLeft = 0;
  newPCB->retValue = 0;
  newPCB->waitingPID = -1;

  newPCB->rsp = createProcessStackframe(newPCB->argc, newPCB->argv, newPCB->rbp, processProgram); //TODO: proper call to createProcessStackFrame

  return newPCB;
}
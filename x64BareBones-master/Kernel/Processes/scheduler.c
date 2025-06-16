// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/scheduler.h"
#include "../include/process.h"
#include "../include/MemoryManager.h"
#include "../include/LinkedList.h"
#include "../include/kernel.h"
#include "../include/nativeLibrary.h"

typedef struct FinishedProcess{
  Pid finishedPid;
  int retVal;
}FinishedProcess;

typedef struct SchedulerCDT{
  Node * processes[MAX_PROCESSES];
  LinkedListADT readyList;
  Pid currentPID;
  Pid currentPPID;
  int finishedProcessCount;
  int waitBlockedProcesses;
  Pid waitBlockProcessPID[MAX_PROCESSES];
  FinishedProcess finisheddProcesses[MAX_PROCESSES];
  uint16_t processQty;
  uint16_t foregroundPID;
} SchedulerCDT;

SchedulerCDT * scheduler = NULL;

static Pid availablePidValue = IDLE_PID;

static uint8_t addChildren(Pid pid);

static uint64_t getAvailablePIDValue();


//void startScheduler(PCB * shell, PCB * idle)
void startScheduler() {
  scheduler = allocMemory(sizeof(SchedulerCDT));
  for (int i = 0; i < MAX_PROCESSES; i++){
    scheduler->processes[i] = NULL;
  }
  scheduler->currentPID = 0;
  scheduler->currentPPID = 0;
  scheduler->processQty = 0;
  scheduler->foregroundPID = 0;
  //
  scheduler->finishedProcessCount = 0;
  //
  scheduler->waitBlockedProcesses = 0;
  scheduler->readyList = initializeLinkedList();
}

static void * runIdleProcess(){
  Node * firstProcess = scheduler->processes[IDLE_PID]->info;
  ((PCB*)firstProcess->info)->status = RUNNING;
  return ((PCB*)firstProcess->info)->rsp; 
}

static Pid toCleanupPID = -1;

void freePCB(PCB *pcb) {
    if (pcb == NULL) return;

    if (pcb->argv != NULL) {
        for (int i = 0; pcb->argv[i] != NULL; i++) {
            freeMemory(pcb->argv[i]);
        }
        freeMemory(pcb->argv);
    }

    if (pcb->processName != NULL)
        freeMemory(pcb->processName);

    if (pcb->rbp != NULL)
        freeStack(pcb->rbp); 
    freeMemory(pcb); 
}



void * schedule(void * currentRSP){
  while(scheduler->waitBlockedProcesses != 0 && getCurrentPID() != scheduler->waitBlockProcessPID[scheduler->waitBlockedProcesses-1]){
    blockProcess(scheduler->waitBlockProcessPID[scheduler->waitBlockedProcesses-1]);
    scheduler->waitBlockedProcesses--;
  }
 
  if (toCleanupPID != -1) {
      killProcess(toCleanupPID);
      toCleanupPID = -1;
      return schedule(currentRSP);
  }
  
  if(isEmpty(scheduler->readyList)){
    return runIdleProcess();
  }
  
  Node * currentRunning = scheduler->processes[scheduler->currentPID];
  
  
  if(currentRunning != NULL && ((PCB*)currentRunning->info)->status == RUNNING){

    if(((PCB*)currentRunning->info)->roundsLeft > 0){
      ((PCB*)currentRunning->info)->roundsLeft--;
      return currentRSP;
    }

    if(scheduler->currentPID != IDLE_PID){
      ((PCB*)currentRunning->info)->rsp = currentRSP;
      removeFromQueue(scheduler->readyList, currentRunning);
      queue(scheduler->readyList, currentRunning);
    }
    
    ((PCB*)currentRunning->info)->status = READY;
  } 

  

  if (currentRunning != NULL && ((PCB*)currentRunning->info)->status == KILLED && ((PCB*)currentRunning->info)->pendingCleanUp) {

    toCleanupPID = scheduler->currentPID;
    ((PCB*)currentRunning->info)->pendingCleanUp = 0;
    removeFromQueue(scheduler->readyList, currentRunning); 

  }
  
  Pid nextProcessPID; 
  nextProcessPID = getNextProcess();
  PCB * nextPCB = (PCB*)scheduler->processes[nextProcessPID]->info;
  if(nextPCB->isForeground){
    scheduler->foregroundPID = nextPCB->PID;
  }
  return switchContext(nextProcessPID);
}

void breakpoint(){
  return;
}

void setToblock(Pid pid){
  scheduler->waitBlockProcessPID[scheduler->waitBlockedProcesses++] = pid;
  return;
}


int waitPID(Pid pid){
  if (pid == IDLE_PID || pid == SHELL_PID || pid >= MAX_PROCESSES)
    return -1;

  PCB *caller = (PCB *)scheduler->processes[scheduler->currentPID]->info;

  int isChild = 0;
  for (int i = 0; i < caller->childrenQty; i++) {
    if (caller->children[i] == pid) {
      isChild = 1;
      break;
    }
  }

  if (!isChild){
    return -1;
  }


  for (int i = 0; i < scheduler->finishedProcessCount; i++) {
    if (scheduler->finisheddProcesses[i].finishedPid == pid) {
      int ret = scheduler->finisheddProcesses[i].retVal;
      
      for (int j = i; j < scheduler->finishedProcessCount - 1; j++) {
        scheduler->finisheddProcesses[j] = scheduler->finisheddProcesses[j + 1];
      } 

      scheduler->finishedProcessCount--;    
      return ret;
      }
  }

  caller->waitingPID = pid;
  // if(caller->status == BLOCKED){
  //   forceTimerTick();
  //   yield();
  // }
  // else {
  //   setToblock(caller->PID);
  //   forceTimerTick();
  //   yield();
  // }
  setToblock(caller->PID);
  forceTimerTick();
  yield();

  caller->waitingPID = -1;
  return caller->retValue;
 }

Pid getNextProcess(){
  listIterator(scheduler->readyList);
  Node * auxNode; 
  while ((auxNode = (Node*) listNext(scheduler->readyList)) != NULL) {
    PCB * pcb = (PCB *)auxNode->info;
    if (pcb != NULL) {
      if (pcb->status == READY)
        return pcb->PID;
    }
  }
  return IDLE_PID;
}

void * switchContext(Pid pid){
  Node * processAux = scheduler->processes[pid];
  if(processAux == NULL){
    return runIdleProcess();
  }
  PCB * nextPCB = (PCB *)processAux->info;
  
  nextPCB->roundsLeft = (int)nextPCB->priority;
  nextPCB->status = RUNNING;
  scheduler->currentPID = pid;
  scheduler->currentPPID = nextPCB->parentPID;

  if(nextPCB->isForeground){
    scheduler->foregroundPID = nextPCB->PID;
  }
  
  return nextPCB->rsp;
}

uint64_t exitProcess(int retValue){

  PCB * pcb = (PCB*)scheduler->processes[getCurrentPID()]->info;
  
  pcb->status = KILLED;
  pcb->retValue = retValue;

  pcb->pendingCleanUp = 1;

  scheduler->finisheddProcesses[scheduler->finishedProcessCount].finishedPid = pcb->PID;
  scheduler->finisheddProcesses[scheduler->finishedProcessCount].retVal = retValue;
  scheduler->finishedProcessCount++;

  PCB *parent = (PCB*)scheduler->processes[pcb->parentPID]->info;


  if(parent != NULL){
    if (parent->waitingPID == pcb->PID) {
      if(pcb->isForeground){
       
        parent->waitingPID = -1;
      }
      parent->retValue = pcb->retValue;
      unblockProcess(parent->PID);
    }
  }
  
  yield();
  return 1;
}

int isForeground(Pid pid){
  PCB * pcb = (PCB*)scheduler->processes[pid]->info;
  return pcb->PID == scheduler->foregroundPID;
  //return pcb->isForeground;
}

int getForegroundPID(){
  return scheduler->foregroundPID;
}

uint64_t killProcess(Pid pid){
  
  
  if(pid == IDLE_PID || pid == SHELL_PID || pid >= MAX_PROCESSES){
    return -1;
  }

  PCB * pcb = (PCB*)scheduler->processes[pid]->info;
  if(pcb == NULL){
    return -1;
  }

  Node * auxNode = scheduler->processes[pid];
  
  if(pcb->status == READY){
    removeFromQueue(scheduler->readyList, auxNode);
  }

  if(pcb->childrenQty > 0){
    for(int i=0; i<pcb->childrenQty; i++){
      Pid childPid = pcb->children[i];
      PCB *child = scheduler->processes[childPid]->info;

      if (child->status != KILLED) {
        child->parentPID = pcb->parentPID;
      }

      PCB *parent = scheduler->processes[child->parentPID]->info;
      parent->children[parent->childrenQty] = child->PID;
      parent->childrenQty++;
    }
  }

  PCB * parentPCB = (PCB*)scheduler->processes[pcb->parentPID]->info;
  
  if(parentPCB != NULL){
    if(parentPCB->childrenQty > 0){
      parentPCB->childrenQty--;
    }
  }
   

  if(pcb->status == RUNNING){
    pcb->retValue = -1;
    pcb->pendingCleanUp = 1;
    pcb->status = KILLED;
    forceTimerTick();
  }
  
  
  freePCB(pcb);
  freeMemory(scheduler->processes[pid]);
  scheduler->processQty--;
  scheduler->processes[pid] = NULL;


  if (parentPCB != NULL && parentPCB->status == BLOCKED) {
    unblockProcess(parentPCB->PID);
  }

  return 0;
}

Pid getCurrentPID(){
   return scheduler->currentPID;
}

Pid getCurrentPPID(){
   return scheduler->currentPPID;
}

int getProcessQty(){
  return scheduler->processQty;
}

//SYSCALL para crear proceso

static uint8_t addChildren(Pid pid){
  PCB *parent = (PCB *)scheduler->processes[pid]->info;
  if (parent->childrenQty >= MAX_PROCESSES) {
    return 0; // no hay más espacio para hijos
  }
  parent->children[parent->childrenQty++] = availablePidValue;
  return 1;
}

static uint64_t getAvailablePIDValue(){
  for (uint64_t i = 0; i < MAX_PROCESSES; i++) {
    if (scheduler->processes[i] == NULL) {
      return i;
    }
  }
  return MAX_PROCESSES+1;
}

uint64_t onCreateProcess(char * processName, mainFunc processProgram, char** args, Priority priority, int16_t fds[]){
  availablePidValue = getAvailablePIDValue();
  if(availablePidValue == MAX_PROCESSES+1){
    return -1;
  }

  PCB * myNewProcess = createProcess(processName, processProgram, args, priority, fds);
  if(myNewProcess == NULL){
    return -1;
  }

  Pid currentPID = getCurrentPID();
  myNewProcess->parentPID = currentPID;
  if(availablePidValue != SHELL_PID && availablePidValue != IDLE_PID && !addChildren(currentPID)){
    return -1;
  }

  myNewProcess->PID = availablePidValue;

  if((PCB*)(scheduler->processes[myNewProcess->parentPID]) != NULL){

    PCB * parentPCB = (PCB*)(scheduler->processes[myNewProcess->parentPID]->info);
    if(myNewProcess->fds[0] == STDIN){
      if (parentPCB->PID == scheduler->foregroundPID) {

        parentPCB->waitingPID = myNewProcess->PID;
        setToblock(parentPCB->PID);
      }
      scheduler->foregroundPID = myNewProcess->PID;
    }
  }

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

  return myNewProcess->PID;
}

void dummyFunction(){
    while (1)
        ;
}

uint64_t createDummyProcess(){
    int dummyFD[] = {0, 1};
    return onCreateProcess("Dummy Process", (void *)dummyFunction, NULL, AVERAGE_PRIORITY, (int16_t *)dummyFD);
}

int blockProcess(Pid pid){

  Node * processToBeBlocked = scheduler->processes[pid];
  PCB * pcb = (PCB*)scheduler->processes[pid]->info;
  
  if(pcb == NULL || pcb->status == KILLED || pid == SHELL_PID || pid == IDLE_PID){
    return -1;
  }
  if(pcb->status != BLOCKED){
    removeFromQueue(scheduler->readyList, processToBeBlocked);
  }  
  pcb->status = BLOCKED;
  
  return 1;
}

int unblockProcess(Pid pid){
  Node * processToBeUnblocked = scheduler->processes[pid];
  PCB * pcb = (PCB*)scheduler->processes[pid]->info;
  if(pcb->status != BLOCKED){
    return -1;
  }

  pcb->status = READY;
  queue(scheduler->readyList, processToBeUnblocked);
     
  //yield();
      return 1;
}

int getProcessPriority(Pid pid){
  PCB * pcb = (PCB*)scheduler->processes[pid]->info;
  return pcb->priority;
}

int increaseProcessPriority(Pid pid){
  PCB * pcb = (PCB*)scheduler->processes[pid]->info;
  if(pcb->priority == HIGHEST_PRIORITY){
    return pcb->priority;
  }
  pcb->priority++;
  return pcb->priority;
}

int decreaseProcessPriority(Pid pid){
  PCB * pcb = (PCB*)scheduler->processes[pid]->info;
  if(pcb->priority == LOWEST_PRIORITY){
    return pcb->priority;
  }
  pcb->priority--;
  return pcb->priority;
}

int nice(Pid pid,Priority priority){
  PCB * pcb = (PCB*)scheduler->processes[pid]->info;
  if(priority > HIGHEST_PRIORITY || priority < LOWEST_PRIORITY){
    return -1;
  }
  pcb->priority = priority;
  return 1;
}

void yield(){
  int pid = getCurrentPID();
  ((PCB *)scheduler->processes[pid]->info)->roundsLeft = 0;
  forceTimerTick();
}

processesToPrint * printProcesses(){
  processesToPrint * psList = allocMemory(sizeof(processesToPrint));
  if(psList == NULL){
    return NULL;
  }
  int i = 0;
  //psList->cantProcess = scheduler->processQty;
  for (int j = 0; j < MAX_PROCESSES; j++){
    if(scheduler->processes[j] != NULL){
      psList->names[i] = ((PCB*)scheduler->processes[j]->info)->processName;
      psList->PIDs[i] = ((PCB*)scheduler->processes[j]->info)->PID;
      psList->Priority[i] = (uint8_t)((PCB*)scheduler->processes[j]->info)->priority;
      psList->Status[i] = (uint8_t)((PCB*)scheduler->processes[j]->info)->status;
      psList->childrens[i] = (uint8_t)((PCB*)scheduler->processes[j]->info)->childrenQty;
      psList->rspList[i] = ((PCB*)scheduler->processes[j]->info)->rsp;
      psList->rbpList[i] = ((PCB*)scheduler->processes[j]->info)->rbp;
      psList->PPIDs[i] = ((PCB*)scheduler->processes[j]->info)->parentPID;
      i++;
    }
  }

  psList->cantProcess = i;
  return psList;
}

int getReadFD(Pid pid){
  return ((PCB*)scheduler->processes[pid]->info)->fds[0];
}

int getWriteFD(Pid pid){
  return ((PCB*)scheduler->processes[pid]->info)->fds[1];
}
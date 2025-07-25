#include "../include/scheduler.h"
#include "../include/process.h"
#include "../include/MemoryManager.h"
#include "../include/LinkedList.h"
#include "../include/kernel.h"
#include "../include/nativeLibrary.h"


typedef struct SchedulerCDT{
  Node * processes[MAX_PROCESSES];
  LinkedListADT readyList;
  Pid currentPID;
  Pid currentPPID;
  //uint16_t nextPID;
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
  scheduler->readyList = initializeLinkedList();
}

static void * runIdleProcess(){
  Node * firstProcess = scheduler->processes[IDLE_PID]->info;
  ((PCB*)firstProcess->info)->status = RUNNING;
  return ((PCB*)firstProcess->info)->rsp; 
}

static Pid toCleanupPID = -1;

void * schedule(void * currentRSP){
  
  if (toCleanupPID != -1) {
    Node * deadNode = scheduler->processes[toCleanupPID];
    if (deadNode != NULL) {
      if(((PCB*)deadNode->info)->childrenQty > 0){
        for(int i=0; i<((PCB*)deadNode->info)->childrenQty; i++){
          Pid childPid = ((PCB*)deadNode->info)->children[i];
          PCB *child = scheduler->processes[childPid]->info;

          if (child->status != KILLED) {
              child->parentPID = SHELL_PID;
          }
          PCB *parent = scheduler->processes[child->parentPID]->info;
          parent->children[parent->childrenQty] = child->PID;
          parent->childrenQty++;
        }
      }

      removeFromQueue(scheduler->readyList, deadNode);
      freeStack(((PCB*)deadNode->info)->rbp);
      freeMemory(deadNode->info);
      freeMemory(deadNode);
      PCB * parentPCB = (PCB*)scheduler->processes[((PCB*)deadNode->info)->parentPID]->info;
      if(parentPCB->childrenQty > 0){
        parentPCB->childrenQty--;
      } 
      scheduler->processes[toCleanupPID] = NULL;
      scheduler->processQty--;
    }
    toCleanupPID = -1;
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
  else if (currentRunning != NULL && ((PCB*)currentRunning->info)->status == KILLED) {
    toCleanupPID = scheduler->currentPID;
  }

  Pid nextProcessPID = getNextProcess();    
  return switchContext(nextProcessPID);
}

Pid getNextProcess(){
  listIterator(scheduler->readyList);
  Node * auxNode; 
  while ((auxNode = (Node*) listNext(scheduler->readyList)) != NULL) {
    PCB * pcb = (PCB *)auxNode->info;
    if (pcb != NULL && pcb->status != KILLED) {
      return ((PCB*)auxNode->info)->PID;
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
  return nextPCB->rsp;
}

uint64_t exitProcess(){
  //_cli();
  PCB * pcb = (PCB*)scheduler->processes[getCurrentPID()]->info;
  pcb->status = KILLED;
  yield();
  //_sti();
  return 0;
}

//TODO: fix proper free
uint64_t killProcess(Pid pid){
  
  PCB * pcb = (PCB*)scheduler->processes[pid]->info;
  if(pcb == NULL || pid == IDLE_PID || pid == SHELL_PID || pid >= MAX_PROCESSES){
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
        child->parentPID = SHELL_PID;
      }
      PCB *parent = scheduler->processes[child->parentPID]->info;
      parent->children[parent->childrenQty] = child->PID;
      parent->childrenQty++;

    }
  }

  PCB * parentPCB = (PCB*)scheduler->processes[pcb->parentPID]->info;
  if(parentPCB->childrenQty > 0){
    parentPCB->childrenQty--;
  }  

  if(pcb->status == RUNNING){
    pcb->retValue = -1;
    pcb->status = KILLED;
    forceTimerTick();
    return 0;
  }
  //////////////
  freeMemory(pcb->processName);
  for(int i = 0; i < pcb->argc; i++){
    freeMemory(pcb->argv[i]);
  }
  freeStack(pcb->rbp);
  freeMemory(pcb);
  //////////////

  freeMemory(scheduler->processes[pid]);
  scheduler->processQty--;
  scheduler->processes[pid] = NULL;
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
  if(pcb->status == BLOCKED || pcb->status == KILLED || pcb == NULL || pid == SHELL_PID || pid == IDLE_PID){
    return -1;
  }
  removeFromQueue(scheduler->readyList, processToBeBlocked);
  pcb->status = BLOCKED;
  if(pid == scheduler->currentPID) {
    yield();
  }
  return 1;
}

int unblockProcess(Pid pid){
  Node * processToBeUnblocked = scheduler->processes[pid];
  PCB * pcb = (PCB*)scheduler->processes[pid]->info;
  if(pcb->status != BLOCKED){
    return -1;
  }
  queue(scheduler->readyList, processToBeUnblocked);
  pcb->status = READY;
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



int waitPID(Pid pid){
  if(scheduler->processes[pid] != NULL && pid != IDLE_PID && pid != SHELL_PID){
    ((PCB*)scheduler->processes[scheduler->currentPID]->info)->waitingPID = pid;
    blockProcess(pid);
    return ((PCB*)scheduler->processes[pid]->info)->retValue;
  }
  return 0;
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
  psList->cantProcess = scheduler->processQty;
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
  // psList->processQty = (uint32_t)scheduler->processQty;
  psList->cantProcess = i;
  return psList;
}
#include "../include/scheduler.h"
#include "../include/process.h"
#include "../include/MemoryManager.h"
#include "../include/LinkedList.h"
#include "../include/kernel.h"


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

static int firstScheduleCall = 1;

static Pid availablePidValue = IDLE_PID;


static void idleKernel(){
    while(1){
		_hlt();
    }
}

//void startScheduler(PCB * shell, PCB * idle)
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

  int16_t fds[2] = {0,1};
	char * args[] = {NULL};
  onCreateProcess("idle", (mainFunc)idleKernel, args, LOWEST_PRIORITY, fds);
}

void * schedule(void * currentRSP){

  if(firstScheduleCall){
    firstScheduleCall = 0;
    Node * firstProcess = scheduler->processes[IDLE_PID]->info;
    return ((PCB*)firstProcess->info)->rsp;
  }
  
  Node * currentRunning = scheduler->processes[scheduler->currentPID];
 
  if(currentRunning != NULL && ((PCB*)currentRunning->info)->status == RUNNING){

    if(((PCB*)currentRunning->info)->roundsLeft > 0){
      ((PCB*)currentRunning->info)->roundsLeft--;
      return currentRSP; // o currentRSP (ver, deberia ser lo mismo)
    }

    if(scheduler->currentPID != IDLE_PID){
      ((PCB*)currentRunning->info)->rsp = currentRSP;
      removeFromQueue(scheduler->readyList, currentRunning); //dequeue(readyList);
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
  nextPCB->roundsLeft = (int)nextPCB->priority;
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
//void * processProgram
uint64_t onCreateProcess(char * processName, mainFunc processProgram, char** args, Priority priority, int16_t fds[]){
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

  // if(myNewProcess->PID != IDLE_PID){
  //   queue(scheduler->readyList, node);
  // }  

  queue(scheduler->readyList, node);

  return 0;
}

void randomFunction(){
    while (1)
        ;
}

uint64_t createDummyProcess(){
    int dummyFD[] = {0, 1};
    return onCreateProcess("DUMMY Function", (void *)randomFunction, NULL, AVERAGE_PRIORITY, (int16_t *)dummyFD);
}


int blockProcess(Pid pid){
  PCB * pcb = scheduler->processes[pid]->info;
  if(pcb->status == BLOCKED || pcb->status == KILLED || pcb == NULL){
    return -1;
  }
  pcb->status = BLOCKED;
  return 1;
}

int unblockProcess(Pid pid){
  PCB * pcb = scheduler->processes[pid]->info;
  if(pcb->status != BLOCKED){
    return -1;
  }
  pcb->status = READY;
  return 1;
}

int getProcessPriority(Pid pid){
  PCB * pcb = scheduler->processes[pid]->info;
  return pcb->priority;
}

int increaseProcessPriority(Pid pid){
  PCB * pcb = scheduler->processes[pid]->info;
  if(pcb->priority == HIGHEST_PRIORITY){
    return pcb->priority;
  }
  pcb->priority++;
  return pcb->priority;
}

int decreaseProcessPriority(Pid pid){
  PCB * pcb = scheduler->processes[pid]->info;
  if(pcb->priority == LOWEST_PRIORITY){
    return pcb->priority;
  }
  pcb->priority--;
  return pcb->priority;
}

//TODO: fix proper free
uint64_t killProcess(Pid pid){
  PCB * pcb = scheduler->processes[pid]->info;
  if(pcb == NULL || pid == 0 || pid == 1){
    return -1;
  }
  pcb->status = KILLED;
  freeMemory(pcb);
  scheduler->processes[pid] = NULL;
  return 0;
}

void waitChilds(){
  return;
}

void printProcesses(){
  return;
}
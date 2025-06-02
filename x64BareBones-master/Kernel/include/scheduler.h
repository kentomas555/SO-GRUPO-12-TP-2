#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include <stdint.h>


#define QUANTUM 5 

#define MAX_PROCESSES 128

#define DEFAULT_PRIO 3

typedef int Pid;
typedef enum {LOWEST_PRIORITY = 1, LOW_PRIORITY, AVERAGE_PRIORITY, HIGH_PRIORITY, HIGHEST_PRIORITY} Priority;
typedef enum {READY = 0, RUNNING, BLOCKED, KILLED} ProcessStatus;

void startScheduler();
int getCurrentPID();
int getCurrentPPID();
int getProcessQty();
int blockProcess(Pid pid);
int unblockProcess(Pid pid);

void * schedule(void * currentRSP);

int increaseProcessPriority(Pid pid);
int decreaseProcessPriority(Pid pid);

void * switchContext(Pid pid);

uint64_t killProcess(Pid pid); 
void waitChilds();

void printProcesses();

#endif
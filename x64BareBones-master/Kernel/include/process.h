#ifndef PROCESS_H
#define PROCESS_H

#include "scheduler.h"
#define FDS 2
#define IDLE_PID 0

typedef struct PCB{
    Pid PID;
    Pid parentPID;
    Priority priority;
    char * processName;
    int fds[FDS];
    ProcessStatus status;

    Pid children[MAX_PROCESSES];
    int childrenQty;
    int retValue;
    Pid waitingPID;

    void * rsp;
    void * rbp;

    int argc;
    char **argv;

    int roundsLeft;
} PCB;


PCB * createProcess(char * processName, mainFunc processProgram, char** args, Priority priority, int16_t fds[]);//void * processProgram


#endif
#ifndef PROCESS_H
#define PROCESS_H

#include "scheduler.h"
#define FDS 2

typedef struct PCB{
    Pid PID;
    Pid parentPID;
    Priority priority;
    char * processName;
    int fds[FDS];
    ProcessStatus status;

    void * rsp;
    void * rbp;

    int argc;
    char **argv;
} PCB;

#endif
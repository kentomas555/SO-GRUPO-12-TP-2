#include "scheduler.h"
#define FDS 3

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
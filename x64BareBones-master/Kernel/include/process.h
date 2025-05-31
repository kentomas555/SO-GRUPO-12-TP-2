#define FDS 3

typedef struct {
    Pid pid;
    Pid ppid;
    Priority priority;
    int quantum;
    char * processName;
    int fds[FDS];
    ProcessStatus status;
    void * currentRSP;
} processControlBlock;
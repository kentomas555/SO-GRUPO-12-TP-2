#include "../include/pipes.h"
#include "../include/MemoryManager.h"

typedef struct PipeCDT {
    char buffer[PIPE_BUFFER_SIZE];
    int readPos;
    int writePos;
    Pid writerPID;
    Pid readerPID;
    int readFD;
    int writeFD;
    sem_t * semRead;
    sem_t * semWrite;
    sem_t * mutex;

} PipeCDT;

PipeCDT * pipeTable[MAX_PIPES];




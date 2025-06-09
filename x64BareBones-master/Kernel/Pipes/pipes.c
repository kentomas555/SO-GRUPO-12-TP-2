#include "../include/pipes.h"
#include "../include/MemoryManager.h"

typedef struct PipeCDT {
    char buffer[PIPE_BUFFER_SIZE];
    int pipeID; //ID funciona tmb como indice para el array
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

static PipeCDT * pipeArray[MAX_PIPES] = {NULL};

static PipeCDT * createPipe(int pipeID, int fd[2]){
    if(pipeArray[pipeID] != NULL){
        return pipeArray[pipeID];
    }
    PipeCDT * pipe = allocMemory(sizeof(PipeCDT));
    if(pipe == NULL){
        return NULL;
    }
    
    memset(pipe->buffer, 0, PIPE_BUFFER_SIZE);
    pipe->pipeID = pipeID;
    pipe->readPos = 0;
    pipe->writePos = 0;
    pipe->writerPID = -1;
    pipe->readerPID = -1;
    pipe->readFD = fd[0];
    pipe->writeFD = fd[1];
    pipe->semRead =
    pipe->semWrite = 
    pipe->mutex = 
    
}






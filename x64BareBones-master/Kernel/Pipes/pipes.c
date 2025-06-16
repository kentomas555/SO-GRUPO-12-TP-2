// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "../include/pipes.h"
#include "../include/videoDriver.h"

#define READ_SEMAPHORE_ID 1
#define WRITE_SEMAPHORE_ID 2
#define MUTEX_SEMAPHORE_ID 3


typedef struct PipeCDT {
    char buffer[PIPE_BUFFER_SIZE];
    int pipeID; //ID funciona tmb como indice para el array
    int readPos;
    int writePos;
    Pid writerPID;
    Pid readerPID;
    int16_t readFD;
    int16_t writeFD;
    int semRead;
    int semWrite;
    int mutex;

} PipeCDT;

static PipeCDT * pipeArray[MAX_PIPES] = {NULL};

static int existPipe(int pipeID){
    if(pipeArray[pipeID-3] != NULL){
        return 1;
    }
    return 0;
}

int64_t createPipe(int pipeID, int16_t fd[2]){
    if(pipeID < 3){
        return -1;
    }

    if(existPipe(pipeID)){
        return pipeID;
    }

    PipeCDT * pipe = allocMemory(sizeof(PipeCDT));
    if(pipe == NULL){
        return -1;
    }
    
    memset(pipe->buffer, 0, PIPE_BUFFER_SIZE);
    pipe->pipeID = pipeID;
    pipe->readPos = 0;
    pipe->writePos = 0;
    pipe->writerPID = 1;
    pipe->readerPID = 1;
    pipe->readFD = fd[0];
    pipe->writeFD = fd[1];
    pipe->semRead = semInit(READ_SEMAPHORE_ID + (pipeID - 3) * 3, 0);
    pipe->semWrite = semInit(WRITE_SEMAPHORE_ID + (pipeID - 3) * 3, PIPE_BUFFER_SIZE);
    pipe->mutex = semInit(MUTEX_SEMAPHORE_ID + (pipeID - 3) * 3, 1);

  
    pipeArray[pipeID-3] = pipe;
    return pipe->pipeID;
}

void destroyPipe(int pipeID){
    if(!existPipe(pipeID)){
        return;
    }
    semDestroy(pipeArray[pipeID - 3]->semRead);
    semDestroy(pipeArray[pipeID - 3]->semWrite);
    semDestroy(pipeArray[pipeID - 3]->mutex);
    pipeArray[pipeID - 3]->readPos = 0;
    pipeArray[pipeID - 3]->writePos = 0;
    memset(pipeArray[pipeID - 3]->buffer, 0, PIPE_BUFFER_SIZE);
    
    freeMemory((void *)pipeArray[pipeID-3]);
    pipeArray[pipeID-3] = NULL;
}

uint64_t writePipe(int pipeID, const char * source){ 
 
    if (!existPipe(pipeID) || source == NULL){
        return 0;
    } 

 
    PipeCDT *pipe = pipeArray[pipeID - 3]; 

    if (pipe->readerPID == -1) {
        return 0; 
    }

    uint64_t written = 0;


    for (int i = 0; source[i] != '\0'; i++) {
        
        // if (pipe->readerPID == -1) {
        //     break; 
        // }

        semWait(pipe->semWrite);
 
        // if (pipe->readerPID == -1) {
        //     semPost(pipe->semWrite);  // clean up before exiting
        //     break;
        // }

        semWait(pipe->mutex);
    
        pipe->buffer[pipe->writePos] = source[i];
        pipe->writePos = (pipe->writePos + 1) % PIPE_BUFFER_SIZE;
        written++;
      
        semPost(pipe->mutex);     // sale de la región crítica
        semPost(pipe->semRead);   // habilita lectura
    }
    



    //if (pipe->readerPID != -1) {
        semWait(pipe->semWrite);
        semWait(pipe->mutex);
        pipe->buffer[pipe->writePos] = '\0';
        pipe->writePos = (pipe->writePos + 1) % PIPE_BUFFER_SIZE;
        semPost(pipe->mutex);
        semPost(pipe->semRead);
    //}

  
    return written;
}

uint64_t readPipe(int pipeID, char * destination){


    if(!existPipe(pipeID) || destination == NULL){
        return 0;
    }

    PipeCDT *pipe = pipeArray[pipeID - 3];
    char c;

    if (pipe->writerPID == -1 && pipe->readPos == pipe->writePos) {
        return 0; 
    }

   
    semWait(pipe->semRead); 
    semWait(pipe->mutex);   

       

    c = pipe->buffer[pipe->readPos];
    pipe->readPos = (pipe->readPos + 1) % PIPE_BUFFER_SIZE;
        
    *destination = c;


    semPost(pipe->mutex);    
    semPost(pipe->semWrite); 

   
    return 1;
}



void closePipeEnd(int pipeID, int isReader) {

    if (!existPipe(pipeID)) {
        return;
    }    

    PipeCDT *pipe = pipeArray[pipeID - 3];
    if (isReader) {
        pipe->readerPID = -1;

    } else {
        pipe->writerPID = -1;
      
    }

    if (pipe->readerPID == -1 && pipe->writerPID == -1) {
        destroyPipe(pipeID);
    }
}



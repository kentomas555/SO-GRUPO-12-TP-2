#include "../include/pipes.h"
#include "../include/videoDriver.h"

#define READ_SEMAPHORE_ID 1
#define WRITE_SEMAPHORE_ID 2
#define MUTEX_SEMAPHORE_ID 3

static char bufferAux[30];
static int y = 300;

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
    if(pipeID < 2){
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
    pipe->writerPID = -1;
    pipe->readerPID = -1;
    pipe->readFD = fd[0];
    pipe->writeFD = fd[1];
    pipe->semRead = semInit(READ_SEMAPHORE_ID + (pipeID - 3) * 3, 1);
    pipe->semWrite = semInit(WRITE_SEMAPHORE_ID + (pipeID - 3) * 3, 1);
    pipe->mutex = semInit(MUTEX_SEMAPHORE_ID + (pipeID - 3) * 3, 1);

    nativeBigPrintf("Pipe creadooo", 300, 460);
    pipeArray[pipeID-3] = pipe;
    return pipe->pipeID;
}

void destroyPipe(int pipeID){
    if(!existPipe(pipeID)){
        return;
    }
    freeMemory((void *)pipeArray[pipeID-3]);
    pipeArray[pipeID-3] = NULL;
}

uint64_t writePipe(int pipeID, const char * source){
    nativeBigPrintf("Escribooo", 300, 450);
    // if (!existPipe(pipeID) || source == NULL){
    //     return 0;
    // } 

    //TESTING
    if (!existPipe(pipeID)){
        nativeBigPrintf("existPipeId", 350, 450);
        return 0;
    } 
    if (source == NULL){
        nativeMediumPrintf("source", 370, 450);
        return 0;
    } 
    //nativeMediumPrintf(source, 370, 450);

    //END TESTING

    PipeCDT *pipe = pipeArray[pipeID - 3];

    uint64_t written = 0;
    int X = 200;

    // for (int i = 0; source[i] != '\0'; i++) {
        
        semWait(pipe->semWrite);
        semWait(pipe->mutex);
        //y = 500;
        //nativeBigPrintf(source, 300, y);
        memcpy(pipe->buffer, source, strlen(source));
        // printMediumChar(pipe->buffer, 300 , y);
        // y += 20;
        //pipe->buffer[pipe->writePos] = source[i];
        pipe->writePos = (pipe->writePos + 1) % PIPE_BUFFER_SIZE;
        written++;
        // itoaBase(pipe->writePos, bufferAux, 10);
        // nativeBigPrintf(bufferAux, X, 240);
        X++;
        semPost(pipe->mutex);     // sale de la región crítica
        semPost(pipe->semRead);   // habilita lectura
    // }

    return written;
}

uint64_t readPipe(int pipeID, char * destination){
    //nativeBigPrintf("entreeeeeeee", 300, 400);
    if(!existPipe(pipeID) || destination == NULL){
        return 0;
    }

    PipeCDT *pipe = pipeArray[pipeID - 3];
    uint64_t read = 0;
    char c;
    //nativeBigPrintf("entreee", 300, 420);
    do {
        semWait(pipe->semRead);   // espera a que haya algo para leer
        semWait(pipe->mutex);     // entra a la región crítica


        c = pipe->buffer[pipe->readPos];
        pipe->readPos = (pipe->readPos + 1) % PIPE_BUFFER_SIZE;
        destination[read++] = c;
        // nativeBigPrintf("ANTES DE PRINT", 200, 180);
        // nativeBigPrintf(pipe->buffer, 200, 220);
        y += 10;
        semPost(pipe->mutex);     // sale de la región crítica
        semPost(pipe->semWrite);  // habilita escritura
    } while (c != '\0');
        nativeBigPrintf(destination, 200, 200);
    return read;
}






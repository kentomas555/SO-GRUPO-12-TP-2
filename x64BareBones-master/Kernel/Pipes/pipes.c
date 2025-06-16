#include "../include/pipes.h"
#include "../include/videoDriver.h"

#define READ_SEMAPHORE_ID 1
#define WRITE_SEMAPHORE_ID 2
#define MUTEX_SEMAPHORE_ID 3

static char bufferAux[30];
static int y = 100;
//static int y = 300;

static int lengthAux = 0;

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
    pipe->writerPID = 1;
    pipe->readerPID = 1;
    pipe->readFD = fd[0];
    pipe->writeFD = fd[1];
    pipe->semRead = semInit(READ_SEMAPHORE_ID + (pipeID - 3) * 3, 0);
    pipe->semWrite = semInit(WRITE_SEMAPHORE_ID + (pipeID - 3) * 3, PIPE_BUFFER_SIZE);
    pipe->mutex = semInit(MUTEX_SEMAPHORE_ID + (pipeID - 3) * 3, 1);

    //nativeBigPrintf("Pipe creadooo", 300, 460);
    pipeArray[pipeID-3] = pipe;
    return pipe->pipeID;
}

void destroyPipe(int pipeID){
    nativeBigPrintf("ENTRE AL DESTROY", 300, y);
    y += 20;
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

uint64_t writePipe(int pipeID, const char * source){ // WRITE ANDA BIEN, HAY QUE VER EL READPIPE
    // nativeBigPrintf("entre al writePipe", 300, y);
    // y += 20;
    if (!existPipe(pipeID) || source == NULL){
        return 0;
    } 

    lengthAux = 0;

    // TESTING
    // if (!existPipe(pipeID)){
    //     nativeBigPrintf("no existe pipeID", 350, 450);
    //     return 0;
    // } 
    // if (source == NULL){
    //     nativeMediumPrintf("source es NULL", 370, 450);
    //     return 0;
    // } 
    // nativeMediumPrintf(source, 370, 450);  SOURCE ESTA BIEN

    // END TESTING

    PipeCDT *pipe = pipeArray[pipeID - 3]; // PIPE ESTA BIEN

    if (pipe->readerPID == -1) {
        return 0; // Reader closed, avoid blocking forever
    }
    // itoaBase(pipe->pipeID, bufferAux, y);
    // y += 20;
    // nativeBigPrintf(bufferAux, 300, y);
    // y += 20;

    uint64_t written = 0;
    //int X = 200;

    for (int i = 0; source[i] != '\0'; i++) {
        
        if (pipe->readerPID == -1) {
            break;  // Reader disappeared while writing
        }

        // nativeBigPrintf("FOR waiting semWrite", 300, y);
        // y += 20;
        semWait(pipe->semWrite);
        // nativeBigPrintf("FOR got semWrite", 300, y);
        // y += 20;
        if (pipe->readerPID == -1) {
            semPost(pipe->semWrite);  // clean up before exiting
            break;
        }

        semWait(pipe->mutex);
        //nativeBigPrintf("estoy en el for", 300, y);
        //memcpy(pipe->buffer, source, strlen(source));
        // printMediumChar(pipe->buffer, 300 , y);
        //y += 20;
        //nativeBigPrintf(source[i], 300, y);
        //y += 20;
        pipe->buffer[pipe->writePos] = source[i];
        pipe->writePos = (pipe->writePos + 1) % PIPE_BUFFER_SIZE;
        written++;
        // itoaBase(written, bufferAux, 10);
        // nativeBigPrintf(bufferAux, 300, y);
        // y += 20;

        // itoaBase(pipe->writePos, bufferAux, 10);
        // nativeBigPrintf(bufferAux, X, 240);
        //X++;
        semPost(pipe->mutex);     // sale de la región crítica
        semPost(pipe->semRead);   // habilita lectura
    }
    

    // After the for loop
    // nativeBigPrintf("waiting semWrite", 300, y);
    // y += 20;

    // semWait(pipe->semWrite);
    // semWait(pipe->mutex);
    // pipe->buffer[pipe->writePos] = '\0';
    // pipe->writePos = (pipe->writePos + 1) % PIPE_BUFFER_SIZE;
    // semPost(pipe->mutex);
    // semPost(pipe->semRead);


    if (pipe->readerPID != -1) {
        semWait(pipe->semWrite);
        semWait(pipe->mutex);
        pipe->buffer[pipe->writePos] = '\0';
        pipe->writePos = (pipe->writePos + 1) % PIPE_BUFFER_SIZE;
        semPost(pipe->mutex);
        semPost(pipe->semRead);
    }

    // itoaBase(written, bufferAux, 10);
    // nativeBigPrintf(bufferAux, 300, y);
    // y += 20;

    // itoaBase(pipe->pipeID, bufferAux, y);
    // y += 20;
    //nativeBigPrintf(pipe->buffer, 300, y);
    //y += 20;

    //lengthAux = written;

    return written;
}

uint64_t readPipe(int pipeID, char * destination){
    //nativeBigPrintf("entre al readPipe", 300, y);
    //y += 20;

    // itoaBase(lengthAux, bufferAux, 10);
    // nativeBigPrintf("lengthAux=", 300, y);
    // nativeBigPrintf(bufferAux, 500, y);
    // y += 20;

    if(!existPipe(pipeID) || destination == NULL){
        return 0;
    }

    PipeCDT *pipe = pipeArray[pipeID - 3];
    char c;

    if (pipe->writerPID == -1 && pipe->readPos == pipe->writePos) {
        //semPost(pipe->mutex);
        //semPost(pipe->semWrite);  // habilita escritura
        return 0;  // EOF, nothing to read, writer closed
    }

    //nativeBigPrintf("antes del do", 300, y);
    //y += 20;
    //nativeBigPrintf("entreee", 300, 420);
    // do {
        semWait(pipe->semRead);   // espera a que haya algo para leer
        semWait(pipe->mutex);     // entra a la región crítica

        // After the for loop
        //nativeBigPrintf("en el do", 300, y);
        //y += 20;
    // semWait(pipe->semRead);
    // nativeBigPrintf("got semWrite", 300, y);
    // y += 20;

    // semWait(pipe->mutex); 

        c = pipe->buffer[pipe->readPos];
        pipe->readPos = (pipe->readPos + 1) % PIPE_BUFFER_SIZE;
        
        *destination = c;

//         nativeBigPrintf("before semPost(mutex)", 300, y);
// y += 20;
// semPost(pipe->mutex);
// nativeBigPrintf("after semPost(mutex)", 300, y);
// y += 20;

// nativeBigPrintf("before semPost(semWrite)", 300, y);
// y += 20;
// semPost(pipe->semWrite);
// nativeBigPrintf("after semPost(semWrite)", 300, y);
// y += 20;

        semPost(pipe->mutex);     // sale de la región crítica
        semPost(pipe->semWrite);  // habilita escritura

        //nativeBigPrintf("dps del semPost en el do", 0, y);
        //y += 20;
        
    // } while (c != '\0');

    // for(int i = 0; i < pipe->writePos; i++){
    //     semWait(pipe->semRead);   // espera a que haya algo para leer
    //     semWait(pipe->mutex);     // entra a la región crítica

    //     c = pipe->buffer[pipe->readPos];
    //     pipe->readPos = (pipe->readPos + 1) % PIPE_BUFFER_SIZE;
    //     destination[read++] = c;
    
    //     semPost(pipe->mutex);     // sale de la región crítica
    //     semPost(pipe->semWrite);  // habilita escritura
    // }

    //lengthAux = 0;

    // nativeBigPrintf("antes del print dest", 0, y);
    // y += 20;
    
    // //nativeBigPrintf(destination, 0, 200);
    // static int x = 0;
    // nativeBigPrintf(destination, y, x);
    // if(y == 500){
    //     x += 50;
    // }
    // y += 20;
    return 1;
}



void closePipeEnd(int pipeID, int isReader) {
    nativeBigPrintf("dentro del closePIPE", 300, y);
    y+=20;

    if (!existPipe(pipeID)) {
        nativeBigPrintf("dentro del !existPipe", 300, y);
    y+=20;
        return;
    }    

    PipeCDT *pipe = pipeArray[pipeID - 3];
    if (isReader) {
        nativeBigPrintf("CLOSED READER", 300, y);
        y+=20;
        pipe->readerPID = -1;
    
        semPost(pipe->semWrite);  // in case writer is stuck
    } else {
        nativeBigPrintf("CLOSED WRITER", 300, y);
        y+=20;
        pipe->writerPID = -1;

        semPost(pipe->semRead);
    }

    if (pipe->readerPID == -1 && pipe->writerPID == -1) {
        destroyPipe(pipeID);  // Only destroy when both ends are closed
    }
}



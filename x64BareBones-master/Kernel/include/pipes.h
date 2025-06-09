#ifndef PIPES_H
#define PIPES_H

#include "MemoryManager.h"
#include "scheduler.h"
#include "process.h"

#define MAX_PIPES 20
#define PIPE_BUFFER_SIZE 128


uint64_t openPipe(int pipeID, int mode);
uint64_t closePipe(int pipeID);
uint64_t writePipe(int pipeID);
uint64_t readPipe(int pipeID);



#endif
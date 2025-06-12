#ifndef PIPES_H
#define PIPES_H

#include "MemoryManager.h"
#include "scheduler.h"
#include "semaphore.h"
#include "process.h"

#define MAX_PIPES 20
#define PIPE_BUFFER_SIZE 128

int64_t createPipe(int pipeID, int fd[2]);
void destroyPipe(int pipeID);
uint64_t writePipe(int pipeID, const char * source);
uint64_t readPipe(int pipeID, char * destination);

#endif
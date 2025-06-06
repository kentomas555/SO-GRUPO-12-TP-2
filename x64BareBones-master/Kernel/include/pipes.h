#ifndef PIPES_H
#define PIPES_H

#include "MemoryManager.h"
#include "scheduler.h"
#include "process.h"

#define MAX_PIPES 20

uint64_t createPipe(char * pipeName);


#endif
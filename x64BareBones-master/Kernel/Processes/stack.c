// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include "../include/stack.h"
#include "../include/process.h"
#include "../include/scheduler.h"

#define STACK_SIZE 4096

static void* stackPtrs[MAX_PROCESSES];
static void* start;
static int current = 0;

void startStack(void* ptr) {
    start = ptr;

    for (int i = 0; i < MAX_PROCESSES; i++) {
        stackPtrs[i] = (void*)((uint8_t*)start + STACK_SIZE * (MAX_PROCESSES - i)); //(MAX_PROCESSES - i)
    }
    current = 0;
}

void* createStack() {
    if (current < MAX_PROCESSES) {
        return stackPtrs[current++];
    }
    return NULL;
}

void freeStack(void* ptr) {
    if (current > 0) {
        current--;
        stackPtrs[current] = ptr;
    }
}

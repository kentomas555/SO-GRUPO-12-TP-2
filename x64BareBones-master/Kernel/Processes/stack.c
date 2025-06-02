#include <stdint.h>
#include "../include/stack.h"
#include "../include/process.h"
#include "../include/scheduler.h"

#define STACK_SIZE 4096

void* stackPtrs[MAX_PROCESSES];
void* start;
int current;

void startStack(void* ptr){
    start = ptr;
    current = 0;
    void* aux; 
    for(int i = 0; i < MAX_PROCESSES; i++){
        aux = start + STACK_SIZE*(i+1);
        stackPtrs[i] = aux;
    }
}

void* createStack(){
    if (current < MAX_PROCESSES){
        return stackPtrs[current++];
    } else {
        return NULL;
    }
}

void freeStack(void * ptr){
    stackPtrs[--current] = ptr;
}

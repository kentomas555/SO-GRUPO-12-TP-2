#ifndef TEST_UTIL_H
#define TEST_UTIL_H

#include <stdint.h>
#include "syscall.h"
#include "globalLib.h"

#define MINOR_WAIT 10000000 // TODO: Change this value to prevent a process from flooding the screen
#define WAIT 100000000      // TODO: Change this value to make the wait long enough to see theese processes beeing run at least twice

uint32_t GetUint();
uint32_t GetUniform(uint32_t max);
uint8_t memcheck(void *start, uint8_t value, uint32_t size);
int64_t satoi(char *str);
void bussy_wait(uint64_t n);
void endless_loop();
void endless_loop_print();

#endif
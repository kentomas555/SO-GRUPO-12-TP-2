#ifndef LIBASM_H
#define LIBASM_H

#include <stdarg.h>
#include <stdint.h>

uint64_t syscall(uint64_t id, ...);

#endif
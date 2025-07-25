#ifndef NATIVELIBRARY_H
#define NATIVELIBRARY_H

typedef struct
{
    uint64_t rax;
    uint64_t rbx;
    uint64_t rcx;
    uint64_t rdx;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t r8;
    uint64_t r9;
    uint64_t r10;
    uint64_t r11;
    uint64_t r12;
    uint64_t r13;
    uint64_t r14;
    uint64_t r15;
    uint64_t rip;
    uint64_t cs;
    uint64_t rflags;
    uint64_t rsp;
    uint64_t ss;
    uint64_t rbp;
}registerState;

char nativeGetChar();
void conditionalGetChar(char* charBuffer);

int strCompare(char *str1, char *str2);
void itoaBase(uint64_t value, char * buffer, uint32_t base);

void printf(char *string, int x, int y, char size);
void printc(char c, int x, int y, char size);
void charDelete(uint32_t bgColor, int x, int y, char size);

void nativeShowAllRegisters();

#endif

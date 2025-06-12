#ifndef GLOBAL_LIB_H
#define GLOBAL_LIB_H

#include <stdint.h>
#include <stddef.h>
#include "../include/test_util.h"

#define MEMORY_SIZE

#define MAX_PID_LENGTH 3
#define MAX_PRIORITY_LENGTH 8

#define HIGHEST_PRIO 4
#define HIGH_PRIO 3
#define AVERAGE_PRIO 2
#define LOW_PRIO 1
#define LOWEST_PRIO 0

typedef int (*mainFunc)(int argc, char **args);

/*====== SCREEN FUNCTIONS ======*/

void ClearScreen(uint64_t HexColor);
void NewLine();
void getBuffer(char * buffer, int size);
void largerFontSize();
void smallerFontSize();

/*====== AUXILIAR FUNCTIONS ======*/

void itoaBase(uint64_t value, char * buffer, uint32_t base);
int strCompare(char *str1, char *str2);
int strCompareFirstComand(char *str1, char *str2);
void nextX(int i);

/*====== PRINT TIME ======*/

void printCurrentTime();

/*====== SOUNDS ======*/

void sound(uint64_t frecuencia , uint64_t duración );

/*====== WAIT ======*/

void wait(int ticks);

/*============================================*/
/*================== TP2-SO ==================*/
/*============================================*/



/*====== PRINT PROCESSES ======*/

void handlePrintMemState();

void handleBlock(char * buffer);
void handleNice(char * buffer);
void handleGetPid();
void printProcesses();
void handleKill(char * buffer);
void handleLoop(char * buffer);

/*====== INPUT CONTROLLER ======*/

void handleCat(char * buffer);
void handleWC(char * buffer);
void handleFilter(char * buffer);

/*======  ======*/
void handlePhylo();

/*====== TESTS ======*/

void handleMemoryManagerTest();
void handleProcessTest();
void handlePriorityTest();
void handleSyncroTest();
void handleNoSyncroTest();


/*====== EXCEPTION TRIGGERS ======*/

void zeroDivisionTrigger();
void invalidOpcodeTrigger();

void throw_zero_division();
void throw_invalid_opcode();

typedef struct Command {
    char* name_id;
    void (*func)(char *);
    char* desc;
    char* usage;
} Command;

#endif // GLOBAL_LIB_H
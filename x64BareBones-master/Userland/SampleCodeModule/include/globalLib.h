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

#define PROCESS 1
#define NOT_PROCESS 0

typedef int (*mainFunc)(int argc, char **args);


#ifndef COMMAND_H
typedef struct Command {
    char* name;
    mainFunc func;
    char* desc;
    char* usage;
    int8_t isProcess;
} Command;
#endif

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
int strlen(const char *str);
char * strchr(char *string, char toFind);
char * strtok(char *str, char *delim);
int countArguments(void **argc);

/*====== PRINT TIME ======*/

void printCurrentTime(int argc, char **args);

/*====== SOUNDS ======*/

void sound(uint64_t frecuencia , uint64_t duración );

/*====== WAIT ======*/

void wait(int ticks);

/*============================================*/
/*================== TP2-SO ==================*/
/*============================================*/



/*====== PRINT PROCESSES ======*/

void handlePrintMemState();

void handleBlock(int argc, char **args);
void handleNice(int argc, char **args);
void handleGetPid(int argc, char **args);
void printProcesses(int argc, char **args);
void handleKill(int argc, char **args);
void handleLoop(int argc, char **args);

/*====== INPUT CONTROLLER ======*/

void handleCat(char * buffer);
void handleWC(char * buffer);
void handleFilter(char * buffer);

/*======  ======*/
void handlePhylo(int argc, char **args);
//void executeUser(char * name, mainFunc func, char *args[], int16_t *fds[], int8_t processFlag);
void executeUser(Command command,char *args[], int16_t *fds[]);



/*====== TESTS ======*/

void handleMemoryManagerTest(int argc, char **args);
void handleProcessTest(int argc, char **args);
void handlePriorityTest(int argc, char **args);
void handleSyncroTest(int argc, char **args);
void handleNoSyncroTest(int argc, char **args);


/*====== EXCEPTION TRIGGERS ======*/

void zeroDivisionTrigger(int argc, char **args);
void invalidOpcodeTrigger(int argc, char **args);

void throw_zero_division(int argc, char **args);
void throw_invalid_opcode(int argc, char **args);



#endif // GLOBAL_LIB_H
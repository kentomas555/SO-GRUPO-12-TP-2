#ifndef GLOBAL_LIB_H
#define GLOBAL_LIB_H

#include <stdint.h>
#include <stddef.h>

#define MAX_PROCESSES 128


typedef struct processesToPrint{
    uint16_t cantProcess;
    char * names[MAX_PROCESSES];
    uint8_t PIDs[MAX_PROCESSES];
    uint8_t PPIDs[MAX_PROCESSES];
    uint8_t Priority[MAX_PROCESSES];
    uint8_t Status[MAX_PROCESSES];
    void * rspList[MAX_PROCESSES];
    void * rbpList[MAX_PROCESSES];
    
}processesToPrint;


/*====== SCREEN FUNCTIONS ======*/

void ClearScreen(uint64_t HexColor);
void NewLine();
void getBuffer(char * buffer, int size);
void largerFontSize();
void smallerFontSize();

/*====== AUXILIAR FUNCTIONS ======*/

void itoaBase(uint64_t value, char * buffer, uint32_t base);
int strCompare(char *str1, char *str2);

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

void printProcesses();

/*====== TESTS ======*/

void memoryManagerTest();
void processTest();
void priorityTest();
void syncroTest();
void noSyncroTest();


/*====== EXCEPTION TRIGGERS ======*/

void zeroDivisionTrigger();
void invalidOpcodeTrigger();

void throw_zero_division();
void throw_invalid_opcode();

#endif // GLOBAL_LIB_H
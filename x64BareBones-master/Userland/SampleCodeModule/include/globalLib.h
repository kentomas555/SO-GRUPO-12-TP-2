#ifndef GLOBAL_LIB_H
#define GLOBAL_LIB_H

#include <stdint.h>
#include <stddef.h>

#define MAX_PROCESSES 128

typedef struct
{
    uint8_t seconds;
    uint8_t minutes;
    int8_t hours;
    uint8_t day;
    uint8_t month;
    uint8_t year;
}date;

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

typedef int Pid;

typedef enum {LOWEST_PRIORITY = 0, LOW_PRIORITY, AVERAGE_PRIORITY, HIGH_PRIORITY, HIGHEST_PRIORITY} Priority;


void moveCurrentX(int newX);
void moveCurrentY(int newY);

void ClearScreen(uint64_t HexColor);
void drawRectangle(int x , int y, int height, int width, uint64_t hexColor);
void drawCircle(int x , int y, int radius, uint64_t hexColor);

void registers();

void printf(char *string);
void printfPos(char *string, int x, int y , char fontSizePos);
void NewLine();
char getChar();
char getKeyDown();
void getBuffer(char * buffer, int size);

int HorizontalOffset(char fontSize);
void largerFontSize();
void smallerFontSize();

void itoaBase(uint64_t value, char * buffer, uint32_t base);
int strCompare(char *str1, char *str2);
int seconds_elapsed();

void printCurrentTime();

void sound(uint64_t frecuencia , uint64_t duración );
void wait(int ticks);

//TP2-SO//
void * allocMemoryUser(uint32_t size);
int getpid();
int createNewProcess(char * processName, void * processProgram, char** args, Priority priority, int16_t fds[]);
int createDummyProcess();
int blockProcess(Pid pid);
int unblockProcess(Pid pid);
int getPriority(Pid pid);
int increasePriority(Pid pid);
int decreasePriority(Pid pid);
void printProcesses(void);
void hlt();

//EXCEPTION TRIGGERS:
void zeroDivisionTrigger();
void invalidOpcodeTrigger();

void throw_zero_division();
void throw_invalid_opcode();

#endif // GLOBAL_LIB_H
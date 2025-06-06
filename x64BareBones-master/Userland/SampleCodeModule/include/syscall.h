#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdint.h>
#include <libasm.h>

/*====== READ SYSCALL ======*/

char getChar();
char getKeyDown();

/*====== WRITE SYSCALL ======*/

void setCurrentX(int newX);
void setCurrentY(int newY);
int getCurrentX();
int getCurrentY();
int getFontSize();
void increaseFontSize();
void decreaseFontSize();

void printf(char *string);
void printfPos(char *string, int x, int y , char fontSizePos);

/*====== INIT DYSPLAY SYSCALL ======*/

void initDisplay(uint64_t HexColor);

/*====== RECTANGLE SYSCALL ======*/

void drawRectangle(int x , int y, int height, int width, uint64_t hexColor);

/*====== CIRCLE SYSCALL ======*/

void drawCircle(int x , int y, int radius, uint64_t hexColor);

/*====== SECONDS ELAPSED SYSCALL ======*/

int seconds_elapsed();

/*====== SOUNDS SYSCALL ======*/

void soundOn(uint64_t frecuencia);
void soundOff();

/*====== TICKS ELAPSED SYSCALL ======*/

int ticks_elapsed();

/*====== DATE SYSCALL =======*/

typedef struct date{
    uint8_t seconds;
    uint8_t minutes;
    int8_t hours;
    uint8_t day;
    uint8_t month;
    uint8_t year;
}date;

void getDate(date * time);

/*====== MEMORY MANAGER ======*/

typedef int Pid;

typedef enum {LOWEST_PRIORITY = 0, LOW_PRIORITY, AVERAGE_PRIORITY, HIGH_PRIORITY, HIGHEST_PRIORITY} Priority;

void * allocMemoryUser(uint32_t size);
void freeMemoryUser();
int getpid();

/*====== NEW PROCESS SYSCALL ======*/

int createNewProcess(char * processName, void * processProgram, char** args, Priority priority, int16_t fds[]);
int createDummyProcess();

/*====== EXISTING PROCESS SYSCALL ======*/

int blockProcess(Pid pid);
int unblockProcess(Pid pid);
int getPriority(Pid pid);
int increasePriority(Pid pid);
int decreasePriority(Pid pid);

/*====== HLT SYSCALL ======*/

void hlt();

/*====== SEMAPHORE SYSCALLS======*/

void * semInit(char * semName, int32_t value);
void semDestroy(char * semName);
void semPost(char * semName);
void semWait(char * semName);

/*====== PIPE SYSCALLS ======*/


int64_t my_getpid();
int64_t my_create_process(char *name, uint64_t argc, char *argv[]);
int64_t my_nice(uint64_t pid, uint64_t newPrio);
int64_t my_kill(uint64_t pid);
int64_t my_block(uint64_t pid);
int64_t my_unblock(uint64_t pid);
int64_t my_sem_open(char *sem_id, uint64_t initialValue);
int64_t my_sem_wait(char *sem_id);
int64_t my_sem_post(char *sem_id);
int64_t my_sem_close(char *sem_id);
int64_t my_yield();
int64_t my_wait(int64_t pid);

#endif

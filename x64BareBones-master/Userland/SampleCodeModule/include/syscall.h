#ifndef SYSCALL_H
#define SYSCALL_H

#include <stdint.h>
#include <libasm.h>

/*=========================*/
/*====== SYSCALL IDS ======*/
/*=========================*/

#define SYSCALL_READ 3
#define SYSCALL_WRITE 4
#define SYSCALL_INIT_DISPLAY 5
#define SYSCALL_DRAW_RECTANGLE 6
#define SYSCALL_DRAW_CIRCLE 7
#define SYSCALL_SECONDS_ELAPSED 8
#define SYSCALL_SOUND_ON 9
#define SYSCALL_SOUND_OFF 10
#define SYSCALL_TICKS_ELAPSED 11
#define SYSCALL_FREE_SPACE 12
#define SYSCALL_DATE 13

#define SYSCALL_ALLOC_MEMORY 14
#define SYSCALL_FREE_MEMORY 15
#define SYSCALL_MEMORY_INFO 16
#define SYSCALL_GETCURRENTBLOCK 40

#define SYSCALL_GET_PID 17
#define SYSCALL_CREATE_PROCESS 18
#define SYSCALL_CREATE_DUMMY_PROCESS 19
#define SYSCALL_EXIT 20
#define SYSCALL_BLOCK_PROCESS 21
#define SYSCALL_UNBLOCK_PROCESS 22
#define SYSCALL_KILL_PROCESS 23
#define SYSCALL_YIELD 24
#define SYSCALL_LIST_PROCESSES 25
#define SYSCALL_GET_PRIORITY 26
#define SYSCALL_INCREASE_PRIORITY 27
#define SYSCALL_DECREASE_PRIORITY 28
#define SYSCALL_NICE 29
#define SYSCALL_WAITPID 30
#define SYSCALL_HLT 31

#define SYSCALL_SEM_INIT 32
#define SYSCALL_SEM_DESTROY 33
#define SYSCALL_SEM_POST 34
#define SYSCALL_SEM_WAIT 35

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

typedef struct memoryState{
    uint64_t total;
    uint64_t reserved;
    uint64_t free;
}memoryState;

memoryState * getMemoryState();

uint64_t getCurrentBlock();

/*====== NEW PROCESS SYSCALL ======*/

int getpid();
int createNewProcess(char * processName, void * processProgram, char** args, Priority priority, int16_t fds[]);
int createDummyProcess();

/*====== EXISTING PROCESS SYSCALL ======*/

#define MAX_PROCESSES 128

typedef struct processesList{
    uint16_t cantProcess;
    char * names[MAX_PROCESSES];
    uint8_t PIDs[MAX_PROCESSES];
    uint8_t PPIDs[MAX_PROCESSES];
    uint8_t Priority[MAX_PROCESSES];
    uint8_t Status[MAX_PROCESSES];
    uint8_t childrens[MAX_PROCESSES];
    void * rspList[MAX_PROCESSES];
    void * rbpList[MAX_PROCESSES];
    //uint32_t processQty;
    
}processesList;

int exitProcess();
int killProcess(Pid pid);
void yield();
int blockProcess(Pid pid);
int unblockProcess(Pid pid);
processesList * getProcesses();
int getPriority(Pid pid);
int increasePriority(Pid pid);
int decreasePriority(Pid pid);
int nice(Pid pid, Priority priority);
int waitPID(Pid pid);

/*====== HLT SYSCALL ======*/

void hlt();

/*====== SEMAPHORE SYSCALLS======*/

typedef struct Node {
    void * info;
    struct Node * next;
    struct Node * previous;
} Node;

typedef struct LinkedListCDT {
    Node * current;
    Node * first;
    Node * last;
    int size;
} LinkedListCDT;

typedef struct LinkedListCDT * LinkedListADT;

typedef struct sem_t {
  uint32_t value;
  //char semName[MAX_SEM_CHAR];
  int occupied;
  uint16_t id;
  uint8_t lock;
  LinkedListADT blockedQueue;
} sem_t;

uint64_t semInit(int32_t id, uint32_t value);
uint64_t semDestroy(int id);
void semPost(int id);
void semWait(int id);

/*====== PIPE SYSCALLS ======*/


#endif

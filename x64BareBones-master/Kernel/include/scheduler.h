#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdint.h>
#include <fileDescriptors.h>


#define QUANTUM 5 

#define MAX_PROCESSES 128

#define DEFAULT_PRIO 3

#define SHELL_PID 1
#define IDLE_PID 0
#define BACKGROUND 0
#define FOREGROUND 1

typedef int (*mainFunc)(int argc, char **args);

typedef int Pid;
typedef enum {LOWEST_PRIORITY = 0, LOW_PRIORITY, AVERAGE_PRIORITY, HIGH_PRIORITY, HIGHEST_PRIORITY} Priority;
typedef enum {READY = 0, RUNNING, BLOCKED, KILLED} ProcessStatus;

typedef struct processesToPrint{
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
    
} processesToPrint;

void startScheduler();
uint64_t onCreateProcess(char * processName, mainFunc processProgram, char** args, Priority priority, int16_t fds[]); //void * processProgram
uint64_t createDummyProcess();
int getProcessPriority(Pid pid);
int getCurrentPID();
int getCurrentPPID();
int getProcessQty();
int blockProcess(Pid pid);
int unblockProcess(Pid pid);
void setToblock(Pid pid);

void * schedule(void * currentRSP);
Pid getNextProcess();
void * switchContext(Pid pid);

int increaseProcessPriority(Pid pid);
int decreaseProcessPriority(Pid pid);
int nice(Pid pid,Priority priority);

void * switchContext(Pid pid);

uint64_t exitProcess(int retValue);
uint64_t killProcess(Pid pid); 
int waitPID(Pid pid);
void yield();

processesToPrint * printProcesses();

extern void _cli(void);
extern void _sti(void);
extern void forceTimerTick(void);

//AGREGADO
int getReadFD(Pid pid);
int getWriteFD(Pid pid);

#endif
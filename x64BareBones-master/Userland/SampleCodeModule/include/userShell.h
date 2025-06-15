#ifndef USERSHELL_H
#define USERSHELL_H

#define PROCESS_PIPE_ID 5

#include "globalLib.h"
#ifndef COMMAND_H
typedef struct Command {
    char* name;
    void (*func) (int argc, char **args); /*mainFunc*/
    char* desc;
    char* usage;
    int8_t isProcess;
} Command;
#endif

void startShell();
void shellEngine();
void bufferInterpreter();
void help();
void changeColor();
void handleLoopNoParams();
Command getCommand(int index);

#endif
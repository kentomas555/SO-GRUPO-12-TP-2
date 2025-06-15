#include <stdarg.h>
#include <stdint.h>
#include <globalLib.h>
#include <syscall.h>
#include <snakeApp.h>
#include <phylo.h>


#define MAX_ARGS 2
char shellBuffer[48];
int bgColorIndex = 0;


//void handleCommand(shellBuffer, backgroundFD);

void help(int argc, char **args);
void clearScreenCommand(int argc, char **args);
void changeColor(int argc, char **args);

int foregroundFD[] = {0, 1};
int backgroundFD[] = {-1, -1};

static int runInBackground = 0;

Command commands[] = {
    {"HELP", (void (*)(int,  char **))help, "Imprime todos los comandos disponibles", NULL, PROCESS},
    {"CLEAR", (void (*)(int,  char **))clearScreenCommand, "Limpia la consola", NULL, NOT_PROCESS},
    {"LARGER", (void (*)(int,  char **))largerFontSize, "Agranda la fuente", NULL, NOT_PROCESS},
    {"SMALLER", (void (*)(int,  char **))smallerFontSize, "Achica la fuente", NULL, NOT_PROCESS},
    {"COLOR", (void (*)(int,  char **))changeColor, "Cambia el color de la consola", NULL, NOT_PROCESS},
    {"TIME", (void (*)(int,  char **))printCurrentTime, "Imprime la hora y fecha actual", NULL, PROCESS}, 
    {"SNAKE", (void (*)(int,  char **))startGame, "Comienza el juego", NULL, NOT_PROCESS},
    {"ZERODIV", (void (*)(int,  char **))zeroDivisionTrigger, "Causa una division por cero", NULL, NOT_PROCESS},
    {"INVOPCODE", (void (*)(int,  char **))invalidOpcodeTrigger, "Causa una instruccion invalida", NULL, NOT_PROCESS},
    {"MEM", (void (*)(int,  char **))handlePrintMemState, "Imprime el estado de memoria", NULL, PROCESS},
    {"GETPID", (void (*)(int,  char **))handleGetPid, "Imprime el PID del proceso actual", NULL, PROCESS},
    {"PS", (void (*)(int,  char **))printProcesses, "Imprime los procesos actuales", NULL, PROCESS},
    {"CREATEDUMMY", (void (*)(int,  char **))createDummyProcess, "Crea un proceso dummy", NULL, NOT_PROCESS},
    {"LOOP", (void (*)(int,  char **))handleLoop, "Ejecuta un loop", "LOOP <segundos>", PROCESS},
    {"KILL", (void (*)(int,  char **))handleKill, "Mata un proceso", "KILL <pid>", NOT_PROCESS},
    {"NICE", (void (*)(int,  char **))handleNice, "Cambia prioridad", "NICE <pid> <prio>", NOT_PROCESS},
    {"BLOCK", (void (*)(int,  char **))handleBlock, "Bloquea proceso", "BLOCK <pid>", NOT_PROCESS},
    {"CAT", (void (*)(int,  char **))handleCat, "Imprime stdin tal como se recibe", NULL, PROCESS},
    {"WC", (void (*)(int,  char **))handleWC, "Cuenta las lineas del input", NULL, PROCESS},
    {"FILTER", (void (*)(int,  char **))handleFilter, "Filtra vocales del input", NULL, PROCESS},
    {"PHYLO", (void (*)(int,  char **))startPhylo, "Proceso filosofos", NULL, NOT_PROCESS},
    {"TESTMM", (void (*)(int,  char **))handleMemoryManagerTest, "Test memory manager", NULL, NOT_PROCESS},
    {"TESTPROCESS", (void (*)(int,  char **))handleProcessTest, "Test de procesos", NULL, PROCESS},
    {"TESTPRIO", (void (*)(int,  char **))handlePriorityTest, "Test de prioridades", NULL, PROCESS},
    {"TESTSYNC", (void (*)(int,  char **))handleSyncroTest, "Test sincronizacion", NULL, PROCESS},
    {"TESTNOSYNC", (void (*)(int,  char **))handleNoSyncroTest, "Test sin sincronizacion", NULL, NOT_PROCESS},
    {"TESTPIPE", (void (*)(int,  char **))handlePipeTest, "Test de prioridades", NULL, PROCESS},
    {NULL, NULL, NULL, NULL}
};

void help(int argc, char **args) {
    printf("Comandos disponibles:");
    NewLine(); 
    NewLine();
    for (int i = 0; commands[i].name != NULL; i++) {
        printf(commands[i].name);
        setCurrentX(getFontSize()*100);    
        printf(commands[i].desc);
        NewLine();
        if(commands[i].usage != NULL){
            printf(commands[i].usage);
            NewLine();
        }
    }
    NewLine();
    printf("Use '&' para ejecutar en background. Ej: TESTSYNC &");
    NewLine();
    /*TESTING*/
    char auxbuf[20];
    itoaBase(getWriteFD(getpid()),auxbuf,10 );
    printf(auxbuf);
    NewLine();
    /*END TESTING*/
}



void changeColor(int argc, char **args){
    if (bgColorIndex >= 3){
        bgColorIndex = 0;
    } else {
        bgColorIndex++;
    }

    uint64_t newColor; 

    if(bgColorIndex == 0){
        newColor = 0x000000FF;
    } else if (bgColorIndex == 1){
        newColor = 0x00000000;
    } else if (bgColorIndex == 2){
        newColor = 0x00FF0000;
    } else {
        newColor = 0x0000FF00;
    }

    ClearScreen(newColor);
}

void clearScreenCommand(int argc, char **args) {
    if(bgColorIndex == 0){
        ClearScreen(0x000000FF);
    } else if (bgColorIndex == 1){
        ClearScreen(0x00000000);
    } else if (bgColorIndex == 2){
        ClearScreen(0x00FF0000);
    } else {
        ClearScreen(0x0000FF00);
    }
}

Command getCommand(int index){
    return commands[index];
}

void bufferInterpreter(){
    NewLine();

    char *leftName;
    char *rightName;
    char *pipePos = strchr(shellBuffer, '|');

    //printf(pipePos);
    if (pipePos != NULL) {
       
        // Split into two commands
        *pipePos = 0;
        char *leftCmd = shellBuffer;
        char *rightCmd = pipePos + 1;

        // Trim spaces
        while (*leftCmd == ' ') leftCmd++;
        while (*rightCmd == ' ') rightCmd++;

        // Parse left command
        leftName = strtok(leftCmd, " ");
        char *leftArgs = strtok(NULL, "");

        // Parse right command
        rightName = strtok(rightCmd, " ");
        char *rightArgs = strtok(NULL, "");

        // Find commands in table
        int leftIdx = -1, rightIdx = -1;
        for (int i = 0; commands[i].name != NULL; i++) {
            if (strCompare(leftName, commands[i].name)) leftIdx = i;
            if (strCompare(rightName, commands[i].name)) rightIdx = i;
        }

       
        if (leftIdx == -1 || rightIdx == -1) {
            printf("Comando no reconocido en pipe. Use HELP para mas informacion");
            NewLine(); 
            NewLine();
            return;
        }

        // char leftI[5];
        // itoaBase(leftIdx, leftI, 10);

         int16_t fds[2] = {0, 1};
        printf("Pipe Created");
        NewLine();
        int pipeID = createPipeUser(PROCESS_PIPE_ID, fds);

        if (pipeID <= 2) {
            printf("Error creando pipe");
            NewLine(); 
            NewLine();
            return;
        }


        char *leftArgArr[] = {leftArgs, NULL};
        int16_t leftFds[] = {0, PROCESS_PIPE_ID};
        //createNewProcess(commands[leftIdx].name, (mainFunc)commands[leftIdx].func, leftArgArr, AVERAGE_PRIORITY, leftFds);
        Pid leftPID = executeUser(commands[leftIdx], leftArgArr, leftFds);

        // Launch right command (stdin <- pipe read)
        //char *rightArgArr[] = {rightArgs, NULL};
        char *rightArgArr[] = {rightArgs, NULL};
        int16_t rightFds[] = {PROCESS_PIPE_ID, 1};
        //createNewProcess(commands[rightIdx].name, (mainFunc)commands[rightIdx].func, rightArgArr, AVERAGE_PRIORITY, rightFds);
        Pid rightPID = executeUser(commands[rightIdx], rightArgArr, rightFds);


        // ESTO DENTRO DE FILTER, CAT, ... ------------------------------------------

        
        // int16_t fds[2] = {0, 1};
        // printf("Pipe Created");
        // NewLine();
        // int pipeID = createPipeUser(PROCESS_PIPE_ID, fds);

        // if (pipeID <= 2) {
        //     printf("Error creando pipe");
        //     NewLine(); 
        //     NewLine();
        //     return;
        // }

        
        // char *leftArgArr[] = {leftArgs, NULL};
        // int16_t leftFds[] = {0, pipeID};
        // //createNewProcess(commands[leftIdx].name, (mainFunc)commands[leftIdx].func, leftArgArr, AVERAGE_PRIORITY, leftFds);
        // executeUser(commands[leftIdx], leftArgArr, leftFds);

        // ESTO DENTRO DE FILTER, CAT, ... ------------------------------------------

        // // Launch right command (stdin <- pipe read)
        // char *rightArgArr[] = {rightArgs, NULL};
        // int16_t rightFds[] = {pipeID, 1};
        // //createNewProcess(commands[rightIdx].name, (mainFunc)commands[rightIdx].func, rightArgArr, AVERAGE_PRIORITY, rightFds);
        // executeUser(commands[rightIdx], rightArgArr, rightFds);

        if(leftPID > 1){
            printf("LEFTPID");
            NewLine();
            waitPID(leftPID);
            closePipeUser(PROCESS_PIPE_ID, 0);
        }

        if(rightPID > 1){
            printf("RIGHTPID");
            NewLine();
            waitPID(rightPID);
            closePipeUser(PROCESS_PIPE_ID, 1); // aca se elimina el pipe
            printProcesses(rightArgArr, fds);
            //destroyPipeUser(pipeID);
        }
      
        // Close pipe fds in parent
        
        //printf("Pipe destroyed");
        NewLine();
        return;
    }

    // char *ownString = strchr(shellBuffer, '\"');
    // if(ownString != NULL){
    //     printf("hola");
    //     return;
    // }

    // No pipe: regular command execution
    char *commandName = shellBuffer;
    char *commandArgs = NULL;
    char *argv[MAX_ARGS];
    char *space = strchr(shellBuffer, ' ');
    if (space != NULL) {
        *space = '\0';
        commandArgs = space + 1;
        while (*commandArgs == ' ') {
            commandArgs++;
        }
        if (*commandArgs == '\0') {
            commandArgs = NULL;
        }
    }

    int argc = 0;
    if (commandArgs != NULL) {
        char *token = strtok(commandArgs, " ");
        while (token != NULL && argc < MAX_ARGS) {
            argv[argc++] = token;
            token = strtok(NULL, " ");
        }
    }
    argv[argc] = NULL;


    int16_t fds[] = {-1, 1};
    
    if(strchr(shellBuffer, '&')){
        runInBackground = 1;
        fds[0] = -1;
    }
    else{
        runInBackground = 0;
        fds[0] = 0;
    }

    if (commandName == NULL) {
        printf("No command entered.");
        NewLine();
        return;
    }

    char *args[] = {argv[0], argv[1]};
    

    for (int i = 0; commands[i].name != NULL; i++) {
        if (strCompare(commandName, commands[i].name)) {
            executeUser(commands[i], args, fds);
           
            return;
        }
    }

    printf("Comando no reconocido. Use HELP para mas informacion");
    NewLine();
    NewLine();

    // handleCommand(shellBuffer, backgroundFD);
    return;
}

void shellEngine(){
    while (1) {
        getBuffer(shellBuffer, 48);
        bufferInterpreter();
    }
}

void startShell(){
	ClearScreen(0x000000FF);
	
    printf("Welcome to the OS!");
    NewLine();
    printf("------------------------------------------------------------------------------------");
    NewLine();
	printf("Sistemas Operativos");
	NewLine();
    printf("------------------------------------------------------------------------------------");
	NewLine();
    printf("Berruti Roman, Kaneko Tomas, Kim Hwa Pyoung");
    NewLine();
    printf("------------------------------------------------------------------------------------");
    NewLine();
    printf("Usa el comando HELP para obtener ayuda");
    NewLine();
    printf("------------------------------------------------------------------------------------");
    NewLine();

    shellEngine();

}






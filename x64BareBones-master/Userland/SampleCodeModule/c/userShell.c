#include <stdarg.h>
#include <stdint.h>
#include <globalLib.h>
#include <syscall.h>
#include <snakeApp.h>
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
    {"HELP", (void (*)(int,  char **))help, "Imprime todos los comandos disponibles", NULL, NOT_PROCESS},
    {"CLEAR", (void (*)(int,  char **))clearScreenCommand, "Limpia la consola", NULL, NOT_PROCESS},
    {"LARGER", (void (*)(int,  char **))largerFontSize, "Agranda la fuente", NULL, NOT_PROCESS},
    {"SMALLER", (void (*)(int,  char **))smallerFontSize, "Achica la fuente", NULL, NOT_PROCESS},
    {"COLOR", (void (*)(int,  char **))changeColor, "Cambia el color de la consola", NULL, NOT_PROCESS},
    {"TIME", (void (*)(int,  char **))printCurrentTime, "Imprime la hora y fecha actual", NULL, NOT_PROCESS}, 
    {"SNAKE", (void (*)(int,  char **))startGame, "Comienza el juego", NULL, NOT_PROCESS},
    {"ZERODIV", (void (*)(int,  char **))zeroDivisionTrigger, "Causa una division por cero", NULL, NOT_PROCESS},
    {"INVOPCODE", (void (*)(int,  char **))invalidOpcodeTrigger, "Causa una instruccion invalida", NULL, NOT_PROCESS},
    {"MEM", (void (*)(int,  char **))handlePrintMemState, "Imprime el estado de memoria", NULL, NOT_PROCESS},
    {"GETPID", (void (*)(int,  char **))handleGetPid, "Imprime el PID del proceso actual", NULL, NOT_PROCESS},
    {"PS", (void (*)(int,  char **))printProcesses, "Imprime los procesos actuales", NULL, NOT_PROCESS},
    {"CREATEDUMMY", (void (*)(int,  char **))createDummyProcess, "Crea un proceso dummy", NULL, NOT_PROCESS},
    {"LOOP", (void (*)(int,  char **))handleLoop, "Ejecuta un loop", "LOOP <segundos>", PROCESS},
    {"KILL", (void (*)(int,  char **))handleKill, "Mata un proceso", "KILL <pid>", NOT_PROCESS},
    {"NICE", (void (*)(int,  char **))handleNice, "Cambia prioridad", "NICE <pid> <prio>", NOT_PROCESS},
    {"BLOCK", (void (*)(int,  char **))handleBlock, "Bloquea proceso", "BLOCK <pid>", NOT_PROCESS},
    {"CAT", (void (*)(int,  char **))handleCat, "Imprime stdin tal como se recibe", NULL, PROCESS},
    {"WC", (void (*)(int,  char **))handleWC, "Cuenta las lineas del input", NULL, PROCESS},
    {"FILTER", (void (*)(int,  char **))handleFilter, "Filtra vocales del input", NULL, PROCESS},
    {"PHYLO", (void (*)(int,  char **))handlePhylo, "Proceso filosofos", NULL, PROCESS},
    {"TESTMM", (void (*)(int,  char **))handleMemoryManagerTest, "Test memory manager", NULL, NOT_PROCESS},
    {"TESTPROCESS", (void (*)(int,  char **))handleProcessTest, "Test de procesos", NULL, NOT_PROCESS},
    {"TESTPRIO", (void (*)(int,  char **))handlePriorityTest, "Test de prioridades", NULL, NOT_PROCESS},
    {"TESTSYNC", (void (*)(int,  char **))handleSyncroTest, "Test sincronizacion", NULL, NOT_PROCESS},
    {"TESTNOSYNC", (void (*)(int,  char **))handleNoSyncroTest, "Test sin sincronizacion", NULL, NOT_PROCESS},
    {NULL, NULL, NULL, NULL}
};

void help(int argc, char **args) {
    printf("Comandos disponibles:");
    NewLine(); NewLine();
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

static void bufferInterpreter(){
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

        // Launch left command (stdout -> pipe write)
        char *leftArgArr[] = {leftArgs, NULL};
        int16_t leftFds[] = {0, 1};
        createNewProcess(commands[leftIdx].name, (mainFunc)commands[leftIdx].func, leftArgArr, AVERAGE_PRIORITY, leftFds);

        // Launch right command (stdin <- pipe read)
        char *rightArgArr[] = {rightArgs, NULL};
        int16_t rightFds[] = {0, 1};
        createNewProcess(commands[rightIdx].name, (mainFunc)commands[rightIdx].func, rightArgArr, AVERAGE_PRIORITY, rightFds);

        return;
       
    }

    // No pipe: regular command execution
char *commandName = shellBuffer;
char *commandArgs = NULL;
char *space = strchr(shellBuffer, ' ');
if (space != NULL) {
    //printf("DSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSSS");
    *space = '\0';
    //printf(commandName);
    setCurrentY(getCurrentY() + 20);
    commandArgs = space + 1;
    //printf(commandArgs);
    // Remove leading spaces
    while (*commandArgs == ' ') {
        commandArgs++;
    }
    if (*commandArgs == '\0') {
        commandArgs = NULL;
    }
}

// // Find the first space (argument separator)
// char *space = strchr(shellBuffer, ' ');
// if (space != NULL) {
//     // Null-terminate the command name
//     *space = '\0';
//     commandName = shellBuffer;
//     commandArgs = space + 1;
//     // Remove leading spaces in arguments
//     while (*commandArgs == ' ') commandArgs++;
//     if (*commandArgs == '\0') commandArgs = NULL; // No arguments
// } else {
//     commandName = shellBuffer;
//     commandArgs = NULL;
// }

int16_t fds[] = {-1, 1};
    
    if(strchr(shellBuffer, '&')){
        runInBackground = 1;
        fds[0] = -1;
    }
    else{
        runInBackground = 0;
        fds[0] = 1;
    }

    if (commandName == NULL) {
        printf("No command entered.");
        NewLine();
        return;
    }

    char *args[] = {commandArgs, NULL};
    

    for (int i = 0; commands[i].name != NULL; i++) {
        if (strCompare(commandName, commands[i].name)) {
            //printf(args[0]);
            //executeUser(commands[i].name, (mainFunc)commands[i].func, args, fds, (int8_t)commands[i].isProcess);
            executeUser(commands[i], args, fds);
           
            return;
        }
    }

    printf("Comando no reconocido. Use HELP para mas informacion");
    NewLine();
    NewLine();

    // if(strCompare(shellBuffer, "HELP")){
    //     help();
    // } 
    // else if (strCompare(shellBuffer, "CLEAR")){
    //     if(bgColorIndex == 0){
    //         ClearScreen(0x000000FF);
    //     } else if (bgColorIndex == 1){
    //         ClearScreen(0x00000000);
    //     } else if (bgColorIndex == 2){
    //         ClearScreen(0x00FF0000);
    //     } else {
    //         ClearScreen(0x0000FF00);
    //     }
    // } else if (strCompare(shellBuffer, "LARGER")){
    //     largerFontSize();
    // } else if (strCompare(shellBuffer, "SMALLER")){
    //     smallerFontSize();
    // } else if (strCompare(shellBuffer, "COLOR")){
    //     changeColor();
    // } else if (strCompare(shellBuffer, "TIME")){
    //     printCurrentTime();
    // } else if (strCompare(shellBuffer, "SNAKE")){
    //     startGame();
    // } else if (strCompare(shellBuffer, "ZERODIV")){
    //     zeroDivisionTrigger();
    // } else if (strCompare(shellBuffer, "INVOPCODE")){
    //     invalidOpcodeTrigger();
    // } else if (strCompare(shellBuffer, "MEM")){
    //     handlePrintMemState();
    // } else if (strCompare(shellBuffer, "GETPID")){
    //     handleGetPid();
    // } else if (strCompare(shellBuffer, "PS")){
    //     printProcesses();
    // } else if (strCompare(shellBuffer, "CREATEDUMMY")){
    //     createDummyProcess();
    // } else if (strCompareFirstComand(shellBuffer, "LOOP ")){
    //     handleLoop(shellBuffer);
    // } /* else if (strCompare(shellBuffer, "LOOP")){
    //      handleLoopNoParams();
    // } */else if (strCompareFirstComand(shellBuffer, "KILL ")){
    //     handleKill(shellBuffer);
    // } /*else if (strCompare(shellBuffer, "KILL")){
    //     handleKillNoParams();
    // }*/ else if (strCompareFirstComand(shellBuffer, "NICE ")){
    //     handleNice(shellBuffer);
    // } /*else if (strCompare(shellBuffer, "NICE")){
    //      handleNiceNoParams();
    // }*/ else if (strCompareFirstComand(shellBuffer, "BLOCK ")){
    //     handleBlock(shellBuffer);
    // } /*else if (strCompare(shellBuffer, "BLOCK")){
    //     handleBlockNoParams();
    // }*/else if (strCompareFirstComand(shellBuffer, "CAT ")){
    //     /*====TODO===*/
    //     NewLine();
    //     printf("Por implementar ...");
    //     NewLine();
    //     NewLine();
    //     handleCat(shellBuffer);
    // } else if (strCompareFirstComand(shellBuffer, "WC")){
    //     /*====TODO===*/
    //     NewLine();
    //     printf("Por implementar ...");
    //     NewLine();
    //     NewLine();
    //     handleWC(shellBuffer);
    // } else if (strCompareFirstComand(shellBuffer, "FILTER")){
    //     /*====TODO===*/
    //     NewLine();
    //     printf("Por implementar ...");
    //     NewLine();
    //     NewLine();
    //     handleFilter(shellBuffer);
    // } else if (strCompare(shellBuffer, "PHYLO")){
    //     /*====TODO===*/
    //     NewLine();
    //     printf("Por implementar ...");
    //     NewLine();
    //     NewLine();
    //     handlePhylo();
    // } else if (strCompare(shellBuffer, "TESTMM")){
    //     handleMemoryManagerTest();
    // } else if (strCompare(shellBuffer, "TESTPROCESS")){
    //     handleProcessTest();
    // } else if (strCompare(shellBuffer, "TESTPRIO")){
    //     handlePriorityTest();
    // } else if (strCompare(shellBuffer, "TESTSYNC")){
    //     handleSyncroTest();
    // } else if (strCompare(shellBuffer, "TESTNOSYNC")){
    //     handleNoSyncroTest();
    // } else {
    //     printf("Comando no reconocido. Use HELP para mas informacion");
    //     NewLine();
    //     NewLine();
    // }
    NewLine();

    // int runInBackground = 0;
    // int len = strLen(input);
    // if (input[len - 1] == '&') {
    //     runInBackground = 1;
    //     input[len - 1] = 0;
    //     input[len - 2] = 0;
    //     handleCommand(shellBuffer, foregroundFD);
    // }
    // handleCommand(shellBuffer, backgroundFD);
    return;
}



// static void bufferInterpreter() {
//     NewLine();
//     char *input = shellBuffer;

//     int runInBackground = 0;
//     int len = strlen(input);
//     if (len > 0 && input[len - 1] == '&') {
//         runInBackground = 1;
//         input[len - 1] = 0;
//     }

//     // Check for pipe
//     char *pipePos = strchr(input, '|');
//     if (pipePos != NULL) {
//         // Split into two commands
//         *pipePos = 0;
//         char *leftCmd = input;
//         char *rightCmd = pipePos + 1;

//         // Trim spaces
//         while (*leftCmd == ' ') leftCmd++;
//         while (*rightCmd == ' ') rightCmd++;

//         // Parse left command
//         char *leftName = strtok(leftCmd, " ");
//         char *leftArgs = strtok(NULL, "");

//         // Parse right command
//         char *rightName = strtok(rightCmd, " ");
//         char *rightArgs = strtok(NULL, "");

//         // Find commands in table
//         int leftIdx = -1, rightIdx = -1;
//         for (int i = 0; commands[i].name != NULL; i++) {
//             if (strCompare(leftName, commands[i].name)) leftIdx = i;
//             if (strCompare(rightName, commands[i].name)) rightIdx = i;
//         }

//         if (leftIdx == -1 || rightIdx == -1) {
//             printf("Comando no reconocido en pipe. Use HELP para mas informacion");
//             NewLine(); NewLine();
//             return;
//         } 

// HASTA ACA ANDA BIEN ----------------------------------------------------------------------------------------

//         // Create pipe (replace with your syscall if needed)
//         int fds[2];
//         if (sys_pipe(fds) != 0) {
//             printf("Error creando pipe");
//             NewLine(); NewLine();
//             return;
//         }

//         // Launch left command (stdout -> pipe write)
//         char *leftArgArr[] = {leftArgs, NULL};
//         int16_t leftFds[] = {0, fds[1]};
//         onCreateProcess(commands[leftIdx].name, (mainFunc)commands[leftIdx].func, leftArgArr, AVERAGE_PRIORITY, leftFds);

//         // Launch right command (stdin <- pipe read)
//         char *rightArgArr[] = {rightArgs, NULL};
//         int16_t rightFds[] = {fds[0], 1};
//         onCreateProcess(commands[rightIdx].name, (mainFunc)commands[rightIdx].func, rightArgArr, AVERAGE_PRIORITY, rightFds);

//         // Close pipe fds in parent
//         sys_close(fds[0]);
//         sys_close(fds[1]);
//         return;
//     }

//     // No pipe: regular command execution
//     char *commandName = strtok(input, " ");
//     char *commandArgs = strtok(NULL, "");

//     if (commandName == NULL) {
//         printf("No command entered.");
//         NewLine();
//         return;
//     }

//     for (int i = 0; commands[i].name != NULL; i++) {
//         if (strCompare(commandName, commands[i].name)) {
//             if (runInBackground) {
//                 int16_t fds[] = {0, 1};
//                 char *args[] = {commandArgs, NULL};
//                 onCreateProcess(commands[i].name, (mainFunc)commands[i].func, args, AVERAGE_PRIORITY, fds);
//             } else {
//                 commands[i].func(commandArgs);
//             }
//             return;
//         }
//     }

//     printf("Comando no reconocido. Use HELP para mas informacion");
//     NewLine();
//     NewLine();
// }


// FUNCIONES AGREGADAS PARA AYUDA _________________________________________________________________
// void execCommand(char * commandName, int * fd){
//     for(commands[i].name != NULL){
//         if (strCompare(commandName, commands[i].name)){
//             (commands[i].func)((void *)fd);
//             return;
//         }
//     }
//     printf("Comando no reconocido. Use HELP para mas informacion");
//     NewLine();
// }

// void handleCommand(char * buffer, int * fd){
//     // Separar nombre y argumentos
//     char * firstCommand;
//     char * secondCommand = NULL;
//     char * commandArgs[] = {0};

//     //Checkear y si encuentra un " " o " | " terminar
//     //EJ " " -> KILL 2 entonces KILL = firstCommand
//     //Si help | cat -> help = firstCommand y cat = secondCommand

//     //FOR como el que esta abajo, y en fds de createProcess, se deberia llamarlo con fd

//     if (strcmp(str, "")==0) {
//         return;
//     }
//     argC = parse_command_arg(str, arguments);
//     char * cmd = arguments[0];
//     if (argC > 1) {
//         if(strcmp(arguments[1], "b") == 0){
//             foreground = FALSE;
//             str = cmd;
//         }
//         if (strcmp(arguments[1], "|") == 0){
//             pipe = TRUE;
//             processWithPipeExec(arguments, argC);
//             return;
//         }
//     }
//     if(pipe == FALSE){
//         int fd[2] = {0, 1};
//         execute_command(cmd, fd);
//     }

//     //agregado (de un tp)

// void pipe_impl(char * args[], int argC) {

//     int fds[2];
//     // if(usys_open_pipe(fds) == -1){
//     //     print_error("Error al crear el pipe.\n");
//     //     return;
//     // }
//     int writerProcess[2];
//     writerProcess[0] = 0;
//     writerProcess[1] = fds[1];

//     int readerProcess[2];
//     readerProcess[0] = fds[0];
//     readerProcess[1] = 1;

//     execCommand(args[0], writerProcess);
//     execCommand(args[2], readerProcess);
// }
//     //agregado end

// }

// FUNCIONES AGREGADAS PARA AYUDA _________________________________________________________________

static void shellEngine(){
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


#include <stdarg.h>
#include <stdint.h>
#include <globalLib.h>
#include <syscall.h>
#include <snakeApp.h>
char shellBuffer[48];
int bgColorIndex = 0;

//void handleCommand(shellBuffer, backgroundFD);

void help();
void clearScreenCommand();
void changeColor();

int foregroundFD[] = {0, 1};
int backgroundFD[] = {-1, -1};

Command commands[] = {
    {"HELP", help, "Imprime todos los comandos disponibles", NULL},
    {"CLEAR", clearScreenCommand, "Limpia la consola", NULL},
    {"LARGER", largerFontSize, "Agranda la fuente", NULL},
    {"SMALLER", smallerFontSize, "Achica la fuente", NULL},
    {"COLOR", changeColor, "Cambia el color de la consola", NULL},
    {"TIME", printCurrentTime, "Imprime la hora y fecha actual", NULL}, 
    {"SNAKE", startGame, "Comienza el juego", NULL},
    {"ZERODIV", zeroDivisionTrigger, "Causa una division por cero", NULL},
    {"INVOPCODE", invalidOpcodeTrigger, "Causa una instruccion invalida", NULL},
    {"MEM", handleMemoryManagerTest, "Imprime el estado de memoria", NULL},
    {"GETPID", handleGetPid, "Imprime el PID del proceso actual", NULL},
    {"PS", printProcesses, "Imprime los procesos actuales", NULL},
    {"CREATEDUMMY", (void (*)(char *))createDummyProcess, "Crea un proceso dummy", NULL},
    {"LOOP", handleLoop, "Ejecuta un loop", "LOOP <segundos>"},
    {"KILL", handleKill, "Mata un proceso", "KILL <pid>"},
    {"NICE", handleNice, "Cambia prioridad", "NICE <pid> <prio>"},
    {"BLOCK", handleBlock, "Bloquea proceso", "BLOCK <pid>"},
    {"CAT", handleCat, "Imprime stdin tal como se recibe", NULL},
    {"WC", handleWC, "Cuenta las lineas del input", NULL},
    {"FILTER", handleFilter, "Filtra vocales del input", NULL},
    {"PHYLO", handlePhylo, "Proceso filosofos", NULL},
    {"TESTMM", handleMemoryManagerTest, "Test memory manager", NULL},
    {"TESTPROCESS", handleProcessTest, "Test de procesos", NULL},
    {"TESTPRIO", handlePriorityTest, "Test de prioridades", NULL},
    {"TESTSYNC", handleSyncroTest, "Test sincronizacion", NULL},
    {"TESTNOSYNC", handleNoSyncroTest, "Test sin sincronizacion", NULL},
    {NULL, NULL, NULL, NULL}
};

void help() {
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



void changeColor(){
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

void clearScreenCommand() {
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
    if(strCompare(shellBuffer, "HELP")){
        help();
    } else if (strCompare(shellBuffer, "CLEAR")){
        if(bgColorIndex == 0){
            ClearScreen(0x000000FF);
        } else if (bgColorIndex == 1){
            ClearScreen(0x00000000);
        } else if (bgColorIndex == 2){
            ClearScreen(0x00FF0000);
        } else {
            ClearScreen(0x0000FF00);
        }
    } else if (strCompare(shellBuffer, "LARGER")){
        largerFontSize();
    } else if (strCompare(shellBuffer, "SMALLER")){
        smallerFontSize();
    } else if (strCompare(shellBuffer, "COLOR")){
        changeColor();
    } else if (strCompare(shellBuffer, "TIME")){
        printCurrentTime();
    } else if (strCompare(shellBuffer, "SNAKE")){
        startGame();
    } else if (strCompare(shellBuffer, "ZERODIV")){
        zeroDivisionTrigger();
    } else if (strCompare(shellBuffer, "INVOPCODE")){
        invalidOpcodeTrigger();
    } else if (strCompare(shellBuffer, "MEM")){
        handlePrintMemState();
    } else if (strCompare(shellBuffer, "GETPID")){
        handleGetPid();
    } else if (strCompare(shellBuffer, "PS")){
        printProcesses();
    } else if (strCompare(shellBuffer, "CREATEDUMMY")){
        createDummyProcess();
    } else if (strCompareFirstComand(shellBuffer, "LOOP ")){
        handleLoop(shellBuffer);
    } /* else if (strCompare(shellBuffer, "LOOP")){
         handleLoopNoParams();
    } */else if (strCompareFirstComand(shellBuffer, "KILL ")){
        handleKill(shellBuffer);
    } /*else if (strCompare(shellBuffer, "KILL")){
        handleKillNoParams();
    }*/ else if (strCompareFirstComand(shellBuffer, "NICE ")){
        handleNice(shellBuffer);
    } /*else if (strCompare(shellBuffer, "NICE")){
         handleNiceNoParams();
    }*/ else if (strCompareFirstComand(shellBuffer, "BLOCK ")){
        handleBlock(shellBuffer);
    } /*else if (strCompare(shellBuffer, "BLOCK")){
        handleBlockNoParams();
    }*/else if (strCompareFirstComand(shellBuffer, "CAT ")){
        /*====TODO===*/
        NewLine();
        printf("Por implementar ...");
        NewLine();
        NewLine();
        handleCat(shellBuffer);
    } else if (strCompareFirstComand(shellBuffer, "WC")){
        /*====TODO===*/
        NewLine();
        printf("Por implementar ...");
        NewLine();
        NewLine();
        handleWC(shellBuffer);
    } else if (strCompareFirstComand(shellBuffer, "FILTER")){
        /*====TODO===*/
        NewLine();
        printf("Por implementar ...");
        NewLine();
        NewLine();
        handleFilter(shellBuffer);
    } else if (strCompare(shellBuffer, "PHYLO")){
        /*====TODO===*/
        NewLine();
        printf("Por implementar ...");
        NewLine();
        NewLine();
        handlePhylo();
    } else if (strCompare(shellBuffer, "TESTMM")){
        handleMemoryManagerTest();
    } else if (strCompare(shellBuffer, "TESTPROCESS")){
        handleProcessTest();
    } else if (strCompare(shellBuffer, "TESTPRIO")){
        handlePriorityTest();
    } else if (strCompare(shellBuffer, "TESTSYNC")){
        handleSyncroTest();
    } else if (strCompare(shellBuffer, "TESTNOSYNC")){
        handleNoSyncroTest();
    } else {
        printf("Comando no reconocido. Use HELP para mas informacion");
        NewLine();
        NewLine();
    }
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


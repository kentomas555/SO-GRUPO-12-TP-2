#include <stdarg.h>
#include <stdint.h>
#include <globalLib.h>
#include <syscall.h>
#include <snakeApp.h>
char shellBuffer[48];
int bgColorIndex = 0;

static void help(){
    printf("Comandos disponibles:");
    NewLine();
    NewLine();
    printf("CLEAR - Limpia la consola");
    NewLine();
    printf("LARGER - Agranda la fuente");
    NewLine();
    printf("SMALLER - Achica la fuente");
    NewLine();
    printf("COLOR - Cambia el color de la consola");
    NewLine();
    printf("TIME - Imprime la hora y fecha actual");
    NewLine();
    printf("SNAKE - Comienza el juego");
    NewLine();
    printf("ZERODIV - Causa una excepcion: divide por cero");
    NewLine();
    printf("INVOPCODE - Causa una excepcion : invalid opcode");
    NewLine();
    printf("HELP - Imprime todos los comandos disponibles");
    NewLine();
    NewLine();
    printf("Comandos para TP2 SO 1C2025:");
    NewLine();
    NewLine();
    printf("MEM - Imprime el estado de memoria");
    NewLine();
    printf("GETPID - Imprime el PID del proceso corriendo");
    NewLine();
    printf("PS - Imprime los procesos");
    NewLine();
    printf("CREATEDUMMY - Crea un proceso DUMMY para testear");
    NewLine();
    printf("LOOP - Imprime su ID con un saludo cada n segundos.");
    NewLine();
    printf("KILL - Mata un proceso dado su ID");
    NewLine();
    printf("    uso: KILL (process pid)");
    NewLine();
    printf("NICE - Cambia la prioridad de un proceso");
    NewLine();
    printf("    uso: NICE (process pid) (new priority)");
    NewLine();
    printf("BLOCK - Cambia el estado de un proceso entre bloqueado y listo");
    NewLine();
    printf("    uso: BLOCK (process pid)");
    NewLine();
    printf("CAT - Imprime el stdin tal como lo recibe");
    NewLine();
    printf("WC - Cuenta la cantidad de líneas del input");
    NewLine();
    printf("FILTER - Filtra las vocales del input");
    NewLine();
    printf("PHYLO - Comienza el proceso de Filosofos comensales");
    NewLine();
    printf("TESTMM - Test del Memory Manager");
    NewLine();
    printf("TESTPROCESS - Test de procesos");
    NewLine();
    printf("TESTPRIO - Test para prioridades");
    NewLine();
    printf("TESTSYNC - Test para synchro");
    NewLine();
    printf("TESTNOSYNC - TEST para no synchro");
    NewLine();
    NewLine();
    printf("Para ver el valor de los registros, presione TAB");
    NewLine();
    NewLine();
    
}

static void changeColor(){
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

static void handleKillNoParams(){
    NewLine();
    printf("Faltan parametros");
    NewLine();
    printf("Ejemplo de llamada: KILL (PID)");
    NewLine();
    NewLine();
}

static void handleNiceNoParams(){
    NewLine();
    printf("Faltan parametros");
    NewLine();
    printf("Ejemplo de llamada: NICE (PID) (PRIORITY)");
    NewLine();
    NewLine();
}

static void handleBlockNoParams(){
    NewLine();
    printf("Faltan parametros");
    NewLine();
    printf("Ejemplo de llamada: BLOCK (PID)");
    NewLine();
    NewLine();
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
        /*====TODO===*/
        NewLine();
        printf("Por implementar ...");
        NewLine();
        NewLine();
        handleLoop(shellBuffer);
    } else if (strCompareFirstComand(shellBuffer, "KILL ")){
        handleKill(shellBuffer);
    } else if (strCompare(shellBuffer, "KILL")){
        handleKillNoParams();
    } else if (strCompareFirstComand(shellBuffer, "NICE ")){
        handleNice(shellBuffer);
    } else if (strCompare(shellBuffer, "NICE")){
        handleNiceNoParams();
    } else if (strCompareFirstComand(shellBuffer, "BLOCK ")){
        handleBlock(shellBuffer);
    } else if (strCompare(shellBuffer, "BLOCK")){
        handleBlockNoParams();
    } else if (strCompareFirstComand(shellBuffer, "CAT ")){
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
        /*====TODO===*/
        NewLine();
        printf("Por implementar ...");
        NewLine();
        NewLine();
        handleMemoryManagerTest();
    } else if (strCompare(shellBuffer, "TESTPROCESS")){
        NewLine();
        printf("Por implementar ...");
        NewLine();
        NewLine();
        handleProcessTest();
    } else if (strCompare(shellBuffer, "TESTPRIO")){
        handlePriorityTest();
    } else if (strCompare(shellBuffer, "TESTSYNC")){
        /*====TODO===*/
        NewLine();
        printf("Por implementar ...");
        NewLine();
        NewLine();
        handleSyncroTest();
    } else if (strCompare(shellBuffer, "TESTNOSYNC")){
        /*====TODO===*/
        NewLine();
        printf("Por implementar ...");
        NewLine();
        NewLine();
        handleNoSyncroTest();
    } else {
        printf("Comando no reconocido. Use HELP para mas informacion");
        NewLine();
        NewLine();
    }
}

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


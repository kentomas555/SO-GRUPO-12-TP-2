#include <stdarg.h>
#include <stdint.h>
#include <globalLib.h>
#include <snakeApp.h>
char shellBuffer[48];
int bgColorIndex = 0;

static void help(){
    printf("Accediste a la funcion HELP");
    NewLine();
    printf("Comandos disponibles:");
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
    //printf("OBSOLETO: REGISTERS - Imprime cada registro y su contenido");
    //NewLine();
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
    printf("TESTALLOC - Test para alocar memoria con MM");
    NewLine();
    printf("GETPID - Imprime el PID del proceso corriendo");
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
    //} else if(strCompare(shellBuffer, "REGISTERS")){
    //    registers();
    } else if (strCompare(shellBuffer, "SNAKE")){
        startGame();
    } else if (strCompare(shellBuffer, "ZERODIV")){
        zeroDivisionTrigger();
    } else if (strCompare(shellBuffer, "INVOPCODE")){
        invalidOpcodeTrigger();
    } else if (strCompare(shellBuffer, "TESTALLOC")){
        allocMemoryUser(10);
    } else if (strCompare(shellBuffer, "GETPID")){
        ;
    }else if (strCompare(shellBuffer, "CREATEDUMMY")){
        ;
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
	printf("Arquitectura de Computadoras");
	NewLine();
    printf("------------------------------------------------------------------------------------");
	NewLine();
    printf("Huerta Joaquin, Kaneko Tomas, Valentinuzzi Antonio");
    NewLine();
    printf("------------------------------------------------------------------------------------");
    NewLine();
    printf("Usa el comando HELP para obtener ayuda");
    NewLine();
    printf("------------------------------------------------------------------------------------");
    NewLine();

    shellEngine();

}


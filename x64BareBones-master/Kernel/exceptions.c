#include <stdint.h>
#include <nativeLibrary.h>
#include <videoDriver.h>
#include <interrupts.h>

char* allRegisterNames[20] = {"RAX = ","RBX = ","RCX = ","RDX = ","RSI = ","RDI = ","R8 = ","R9 = ","R10 = ", "R11 = ",
							 "R12 = ","R13 = ","R14 = ","R15 = ","RIP = ", "CS = ", "FLAGS = ", "RSP = ", "SS = ", "RBP = "};

registerState currentState;

static void zero_division();
static void niy();
static void invalid_opcode();

static void (*exceptions[])() = {
	zero_division,   	//EXCEPTION 0 - ZERO DIVISION
	niy, 				//EXCEPTION 1 - Not Implemented Yet
	niy, 				//EXCEPTION 2 - Not Implemented Yet
	niy, 				//EXCEPTION 3 - Not Implemented Yet
	niy, 				//EXCEPTION 4 - Not Implemented Yet
	niy, 				//EXCEPTION 5 - Not Implemented Yet
	invalid_opcode   	//EXCEPTION 6 - INVALID OPCODE
};


void exceptionDispatcher(int exception) {
	initializeDisplay(0x000000FF);

	(exceptions[exception])();
	
	nativeShowAllRegisters();
	printf("PRESS KEY TO CONTINUE", 0 , 16 + 21 * 16, 3);
	
	char input = 0;
	_sti();
	while (input == 0){
		input = nativeGetChar();
	}
}

static void zero_division() {
	printf("EXCEPTION: ZERO DIVISION", 0, 0, 3);
}

static void niy(){
	printf("ERROR: EXCEPTION NOT IMPLEMENTED YET", 0, 0, 3);
}

static void invalid_opcode(){
	printf("EXCEPTION: INVALID OPCODE", 0, 0, 3);
}

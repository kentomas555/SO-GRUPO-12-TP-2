// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <stdint.h>
#include <nativeLibrary.h>
#include <videoDriver.h>
#include <keybord.h>
#include <interrupts.h>
#include <syscallDispatcher.h>

static char* allRegisterNames[20] = {"RAX = ","RBX = ","RCX = ","RDX = ","RSI = ","RDI = ","R8 = ","R9 = ","R10 = ", "R11 = ",
							 "R12 = ","R13 = ","R14 = ","R15 = ","RIP = ", "CS = ", "FLAGS = ", "RSP = ", "SS = ", "RBP = "};

static registerState currentState;

char nativeGetChar(){
    char ret = syscallDispatcher(3);
    while (ret == -1){
        ret = syscallDispatcher(3);
    }
    return ret;
}

void conditionalGetChar(char* charBuffer){
    if(isKeyAvailable()){
        *charBuffer = getKeyBuffer();
        resetKeyBuffer();
        resetKeyAvailable(); 
    } else {
        *charBuffer = 0;
    } 
}

int strCompare(char *str1, char *str2) {
    while (*str1 != 0 && *str2 != 0) {
        if (*str1 != *str2) {
            return 0;
        }
        str1++;
        str2++;
    }
    return (*str1 == 0 && *str2 == 0) ? 1 : 0;
}

void itoaBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

}

//For printing

void printf(char *string, int x, int y, char size){
    if(size == 3){
        nativeBigPrintf(string, x, y);
    } else if (size == 1){
        nativePrintf(string, x, y);
    } else {
        nativeMediumPrintf(string, x, y);
    }
}

void printc(char c, int x, int y, char size){
    if(size == 3){
        printBigChar(c, x, y);
    } else if (size == 1){
        printChar(c, x, y);
    } else {
        printMediumChar(c, x, y);
    }
}

void charDelete(uint32_t bgColor, int x, int y, char size){
    if(size == 1){
      drawRectangle(x, y, 8, 8, bgColor);
    } else if (size == 2){
        drawRectangle(x, y, 8, 12, bgColor);
    } else {
        drawRectangle(x, y, 16, 16, bgColor);
    }
}

void obtainRegisters(registerState* state){
    uint64_t * regs = getAllRegisters();

    state->rax = regs[0];
    state->rbx = regs[1];
    state->rcx = regs[2];
    state->rdx = regs[3];
    state->rsi = regs[4];
    state->rdi = regs[5];
    state->r8 = regs[6];
    state->r9 = regs[7];
    state->r10 = regs[8];
    state->r11 = regs[9];
    state->r12 = regs[10];
    state->r13 = regs[11];
    state->r14 = regs[12];
    state->r15 = regs[13];
    state->rip = regs[14];
    state->cs = regs[15];
    state->rflags = regs[16];
    state->rsp = regs[17];
    state->ss = regs[18];
    state->rbp = regs[19];
}

void nativeShowAllRegisters(){
	char localbuffer[32];
	obtainRegisters(&currentState);
	for (int i = 0; i < 20; i++){
		printf(allRegisterNames[i], 0, 16 + i * 16, 3);
	}
	itoaBase(currentState.rax, localbuffer, 16);
	printf(localbuffer, 16 * 8, 16, 3);
	itoaBase(currentState.rbx, localbuffer, 16);
	printf(localbuffer, 16 * 8, 16 * 2, 3);
	itoaBase(currentState.rcx, localbuffer, 16);
	printf(localbuffer, 16 * 8, 16 * 3, 3);
	itoaBase(currentState.rdx, localbuffer, 16);
	printf(localbuffer, 16 * 8, 16 * 4, 3);
	itoaBase(currentState.rsi, localbuffer, 16);
	printf(localbuffer, 16 * 8, 16 * 5, 3);
	itoaBase(currentState.rdi, localbuffer, 16);
	printf(localbuffer, 16 * 8, 16 * 6, 3);
	
	itoaBase(currentState.r8, localbuffer, 16);
	printf(localbuffer, 16 * 8, 16 * 7, 3);
	itoaBase(currentState.r9, localbuffer, 16);
	printf(localbuffer, 16 * 8, 16 * 8, 3);
	itoaBase(currentState.r10, localbuffer, 16);
	printf(localbuffer, 16 * 8, 16 * 9, 3);
	itoaBase(currentState.r11, localbuffer, 16);
	printf(localbuffer, 16 * 8, 16 * 10, 3);
	itoaBase(currentState.r12, localbuffer, 16);
	printf(localbuffer, 16 * 8, 16 * 11, 3);
	itoaBase(currentState.r13, localbuffer, 16);
	printf(localbuffer, 16 * 8, 16 * 12, 3);
	itoaBase(currentState.r14, localbuffer, 16);
	printf(localbuffer, 16 * 8, 16 * 13, 3);
	itoaBase(currentState.r15, localbuffer, 16);
	printf(localbuffer, 16 * 8, 16 * 14, 3);
	
	itoaBase(currentState.rip, localbuffer, 16);
	printf(localbuffer, 16 * 8, 16 * 15, 3);
	itoaBase(currentState.cs, localbuffer, 16);
	printf(localbuffer, 16 * 8, 16 * 16, 3);
	itoaBase(currentState.rflags, localbuffer, 16);
	printf(localbuffer, 16 * 8, 16 * 17, 3);
	itoaBase(currentState.rsp, localbuffer, 16);
	printf(localbuffer, 16 * 8, 16 * 18, 3);
	itoaBase(currentState.ss, localbuffer, 16);
	printf(localbuffer, 16 * 8, 16 * 19, 3);
	itoaBase(currentState.rbp, localbuffer, 16);
	printf(localbuffer, 16 * 8, 16 * 20, 3);
}
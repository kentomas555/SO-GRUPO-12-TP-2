#include <clockTime.h>
#include <stdint.h>
#include <stdio.h>
#include <keybord.h>
#include <videoDriver.h>
#include <irqDispatcher.h>
#include <scheduler.h>

static uint64_t int_20(uint64_t rsp);
static void int_21();

char * v = (char*)0xB8000;
char keyAvailable = 0;
char keyBuffer = -1;

uint64_t irqDispatcher(uint64_t irq, uint64_t rsp) {
	switch (irq) {
		case 0:
			return int_20(rsp);
			break;
		case 1:
			int_21();
			break;
	}
	return 0;
}

void irs80Dispatcher(uint64_t irc80Mode){
	
}

void int_21(){
	if(getKey() <= 0x58 ){
		keyBuffer = getCharASCII(getKey());
		keyAvailable = 1; 
	}
}

char isKeyAvailable(){
	return keyAvailable;
}

void resetKeyAvailable(){
	keyAvailable = 0;
}

char getKeyBuffer(){
	return keyBuffer;
}

void resetKeyBuffer(){
	keyBuffer = -1;
}

uint64_t int_20(uint64_t rsp) {
	timer_handler();
	// return 0;
	return (uint64_t)schedule((void *)rsp);
}

void insertChar(char c){
	*v = c;
	*(v+1) = 0x0F;
}

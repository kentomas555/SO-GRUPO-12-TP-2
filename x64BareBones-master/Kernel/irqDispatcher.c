#include <clockTime.h>
#include <stdint.h>
#include <stdio.h>
#include <keybord.h>
#include <videoDriver.h>
#include <irqDispatcher.h>

static void int_20();
static void int_21();

char * v = (char*)0xB8000;
char keyAvailable = 0;
char keyBuffer = -1;

void irqDispatcher(uint64_t irq) {
	switch (irq) {
		case 0:
			int_20();
			break;
		case 1:
			int_21();
			break;
	}
	return;
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

void int_20() {
	timer_handler();
}

void insertChar(char c){
	*v = c;
	*(v+1) = 0x0F;
}

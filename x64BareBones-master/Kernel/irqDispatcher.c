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

	uint8_t keycode = getKey();
    // Handle Shift key state
    if (keycode == 0x2A || keycode == 0x36) {
        shiftPressed();  // Left or right Shift pressed
        return;
    } else if (keycode == 0xAA || keycode == 0xB6) {
        shiftNotPressed();  // Left or right Shift released
        return;
    }

	if(getKey() <= 0x58 ){
		keyBuffer = getCharASCII(getKey());
		if(keyBuffer){

		}
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

// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <clockTime.h>
#include <stdint.h>
#include <stdio.h>
#include <keybord.h>
#include <videoDriver.h>
#include <irqDispatcher.h>
#include <scheduler.h>

static uint64_t int_20(uint64_t rsp);
static void int_21();

char * variable = (char*)0xB8000;
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

static void killForegroundProcess(){
    int frPID = getForegroundPID();
    if(frPID != SHELL_PID && frPID != IDLE_PID){
		killProcess(frPID);
        yield();
        yield();
    }
   
}

void int_21(){

	uint8_t keycode = getKey();
    
    if (keycode == 0x2A || keycode == 0x36) {
        shiftPressed();  
        return;
    } else if (keycode == 0xAA || keycode == 0xB6) {
        shiftNotPressed(); 
        return;
    }

	if (keycode == 0x1D) {
        ctrlPressed(); 
        return;
    } else if (keycode == 0x9D) {
        ctrlNotPressed();
        return;
    }

	
	if(getKey() <= 0x58 ){
		keyBuffer = getCharASCII(getKey());
		if(keyBuffer == 0x03){
			killForegroundProcess();
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
	*variable = c;
	*(variable+1) = 0x0F;
}

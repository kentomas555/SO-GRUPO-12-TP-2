// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <clockTime.h>
#include <time.h>
#include <videoDriver.h>
#include <stdint.h>
#include <lib.h>

extern int getSeconds();
extern int getMinutes();
extern int getHours();
extern int getDays();
extern int getMonth();
extern int getYear();

#define UTC_3 -3

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}

void wait(uint16_t ticks){
    uint16_t aux = ticks_elapsed();
    while(ticks_elapsed() - aux < ticks){
        
    }
    return;
}

void setTickFreq(uint16_t freq) {
    
    uint32_t div = 1193180 / freq;
    outb(0x43, 0x36);                  // 0x36: Channel 0, Mode 3, Low/High byte access
    outb(0x40, (uint8_t)(div));        // Send the low byte of the divisor
    outb(0x40, (uint8_t)(div >> 8));   // Send the high byte of the divisor
}

void getTime(date *dia){
	dia->seconds = getSeconds();
	dia->minutes = getMinutes();
	dia->hours = getHours() + UTC_3;
	if(dia->hours < 0){
		dia->hours += 24;
	}
	dia->day = getDays() - ((dia->hours > 21) ? 1 : 0);
	dia->month = getMonth();
	dia->year = getYear();
}

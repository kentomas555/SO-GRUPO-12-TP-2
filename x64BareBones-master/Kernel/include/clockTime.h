#ifndef _TIME_H_
#define _TIME_H_

#include <stdint.h>

typedef struct
{
    uint8_t seconds;
    uint8_t minutes;
    int8_t hours;
    uint8_t day;
    uint8_t month;
    uint8_t year;
}date;

void timer_handler();

int ticks_elapsed();
int seconds_elapsed();

void setTickFreq(uint16_t div);
void getTime(date *dia);

void wait(uint16_t ticks);

//Extern:
void outb(uint8_t a, uint8_t b);
uint8_t inb(uint8_t a);

#endif

// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <lib.h>
#include <soundDriver.h>
#include <stdint.h>
#include <stdio.h>
#include <clockTime.h>

#include <time.h>

void soundOn(uint32_t freq){
    
    uint32_t div;
    uint8_t stat;

    div = 1193180 / freq; 
    outb(0x43, 0xB6);
    outb(0x42, (uint8_t)(div));
    outb(0x42, (uint8_t)(div >> 8));

    stat = inb(0x61);
    if (stat != (stat | 3)){
        outb(0x61, stat | 3);
    }        
}

void soundOff(){
    uint8_t stat = inb(0x61);

    outb(0x61, stat & 0xFC);
}


void sound(uint16_t freq, uint16_t duration){
    if(freq == 0){
        wait(duration);
        return;
    }
    
    soundOn(freq);

    wait(duration);  
    
    soundOff();  
    return;  
}

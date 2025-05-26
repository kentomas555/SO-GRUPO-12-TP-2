#include <syscallDispatcher.h>
#include <stdarg.h>
#include <lib.h>
#include <videoDriver.h>
#include <nativeLibrary.h>
#include <soundDriver.h>
#include <clockTime.h>
#include <interrupts.h>
#include <irqDispatcher.h>
#include <MemoryManager.h>
#include <kernel.h>

uint64_t syscallDispatcher(uint64_t id, ...)
{
    va_list args;
    va_start(args, id);
    uint64_t ret = 0;
    switch(id){
        case 3:{ //Read
            ret = getKeyBuffer();
            if (ret == 9){
                initializeDisplay(0x000000FF);
                printf("REGISTERS:",0,0,3);
                nativeShowAllRegisters();
                printf("PRESS KEY TO CONTINUE", 0 , 16 + 21 * 16, 3);
                char aux = -1;
                resetKeyBuffer();
                _sti();    
                while(aux == -1){
                    aux = getKeyBuffer();
                }
                initializeDisplay(0x000000FF);
            }
            resetKeyBuffer();
            break;
        }
        case 4:{ //Write

            int x = va_arg(args, int);
            int y = va_arg(args, int);
            
            char size = va_arg(args, int);

            char* string = va_arg(args, char*);

            printf(string, x , y, size);
            break;
        }
        case 5:{ //Initialize Display
            uint64_t hexColor = va_arg(args, uint64_t);
            initializeDisplay(hexColor);
            break;
        }
        case 6: //DrawRectangle
        {
            int x = va_arg(args, int);
            int y = va_arg(args, int);
            int width = va_arg(args, int);
            int height = va_arg(args, int);
            uint64_t hexColor = va_arg(args, uint64_t);
            drawRectangle(x, y, width, height, hexColor);
            break;
        }
        case 7:{ //DrawCircle
            int x = va_arg(args, int);
            int y = va_arg(args, int);
            int radius = va_arg(args, int);
            uint64_t hexColor = va_arg(args, uint64_t);
            drawCircle(x, y, radius, hexColor);
            break;
        }
        case 8:{ //Seconds
            ret = seconds_elapsed();
            break;
        }
        case 9:{ //SoundOn
            uint64_t frec = va_arg(args, uint64_t);
            soundOn(frec);
            break;
        }
        case 10:{ //SoundOff
            soundOff();
            break;
        }
        case 11:{ //Ticks
            ret = ticks_elapsed();
            break;
        }
        case 12:
        {
            //Free Space

            break;
        }
        case 13: //Fecha
        {
            date *aux = va_arg(args, date *);
            getTime(aux);
            break;
        }
        case 14: //allocMemory
        {
            uint32_t size = va_arg(args, uint32_t);
            ret = (uint64_t) allocMemory(getKernelMem(), size);
            break;
        }
        case 15: //freeMemory
        {
            
        }
        
    }

    va_end(args);
    return ret;
}
// ========== INCLUDES ==========
#include <stdarg.h>
#include <stdint.h>
#include <syscallDispatcher.h>
#include <lib.h>
#include <videoDriver.h>
#include <nativeLibrary.h>
#include <soundDriver.h>
#include <clockTime.h>
#include <interrupts.h>
#include <irqDispatcher.h>
#include <memoryManager.h>
#include <kernel.h>
#include <process.h>
#include <scheduler.h>
#include <stack.h>

// ========== PROTOTIPOS DE FUNCIONES ==========
static uint64_t handleReadSyscall(void);
static void handleWriteSyscall(va_list args);
static void handleInitDisplaySyscall(va_list args);
static void handleDrawRectangleSyscall(va_list args);
static void handleDrawCircleSyscall(va_list args);
static void handleSoundOnSyscall(va_list args);
static void handleDateSyscall(va_list args);
static uint64_t handleAllocMemorySyscall(va_list args);
static uint64_t handleCreateProcessSyscall(va_list args);
static uint64_t handleCreateDummyProcessSyscall(va_list args);

// ========== DISPATCHER PRINCIPAL ==========
uint64_t syscallDispatcher(uint64_t id, ...) {
    va_list args;
    va_start(args, id);
    uint64_t ret = 0;

    switch (id) {
        case SYSCALL_READ:
            ret = handleReadSyscall();
            break;
        case SYSCALL_WRITE:
            handleWriteSyscall(args);
            break;
        case SYSCALL_INIT_DISPLAY:
            handleInitDisplaySyscall(args);
            break;
        case SYSCALL_DRAW_RECTANGLE:
            handleDrawRectangleSyscall(args);
            break;
        case SYSCALL_DRAW_CIRCLE:
            handleDrawCircleSyscall(args);
            break;
        case SYSCALL_SECONDS_ELAPSED:
            ret = seconds_elapsed();
            break;
        case SYSCALL_SOUND_ON:
            handleSoundOnSyscall(args);
            break;
        case SYSCALL_SOUND_OFF:
            soundOff();
            break;
        case SYSCALL_TICKS_ELAPSED:
            ret = ticks_elapsed();
            break;
        case SYSCALL_FREE_SPACE:
            // TODO: Free Space
            break;
        case SYSCALL_DATE:
            handleDateSyscall(args);
            break;

        //Memory

        case SYSCALL_ALLOC_MEMORY:
            ret = handleAllocMemorySyscall(args);
            break;
        case SYSCALL_FREE_MEMORY:
            // TODO: freeMemory
            break;
        case SYSCALL_MEMORY_INFO:
            // TODO: memoryInfo
            break;

        //Process
        
        case SYSCALL_GET_PID:
            ret = handleCreateProcessSyscall(args);
            break;
        case SYSCALL_CREATE_PROCESS:
            ret = handleCreateProcessSyscall(args);
            break;
        case SYSCALL_CREATE_DUMMY_PROCESS:
            ret = handleCreateDummyProcessSyscall(args);
            break;
        case SYSCALL_EXIT:
            break;
        case SYSCALL_BLOCK_PROCESS:
            break;
        case SYSCALL_UNBLOCK_PROCESS:
            break;
    }

    va_end(args);
    return ret;
}

// ========== IMPLEMENTACIONES DE HANDLERS ==========

static uint64_t handleReadSyscall(void) {
    uint64_t ret = getKeyBuffer();
    if (ret == 9) {
        initializeDisplay(0x000000FF);
        printf("REGISTERS:", 0, 0, 3);
        nativeShowAllRegisters();
        printf("PRESS KEY TO CONTINUE", 0, 16 + 21 * 16, 3);
        char aux = -1;
        resetKeyBuffer();
        _sti();    
        while (aux == -1) {
            aux = getKeyBuffer();
        }
        initializeDisplay(0x000000FF);
    }
    resetKeyBuffer();
    return ret;
}

static void handleWriteSyscall(va_list args) {
    int x = va_arg(args, int);
    int y = va_arg(args, int);
    char size = va_arg(args, int);
    char* string = va_arg(args, char*);
    printf(string, x, y, size);
}

static void handleInitDisplaySyscall(va_list args) {
    uint64_t hexColor = va_arg(args, uint64_t);
    initializeDisplay(hexColor);
}

static void handleDrawRectangleSyscall(va_list args) {
    int x = va_arg(args, int);
    int y = va_arg(args, int);
    int width = va_arg(args, int);
    int height = va_arg(args, int);
    uint64_t hexColor = va_arg(args, uint64_t);
    drawRectangle(x, y, width, height, hexColor);
}

static void handleDrawCircleSyscall(va_list args) {
    int x = va_arg(args, int);
    int y = va_arg(args, int);
    int radius = va_arg(args, int);
    uint64_t hexColor = va_arg(args, uint64_t);
    drawCircle(x, y, radius, hexColor);
}

static void handleSoundOnSyscall(va_list args) {
    uint64_t frec = va_arg(args, uint64_t);
    soundOn(frec);
}

static void handleDateSyscall(va_list args) {
    date *aux = va_arg(args, date *);
    getTime(aux);
}

static uint64_t handleAllocMemorySyscall(va_list args) {
    uint32_t size = va_arg(args, uint32_t);
    return (uint64_t) allocMemory(getKernelMem(), size);
}

static uint64_t handleCreateProcessSyscall(va_list args){
    return (uint64_t) 
}

static uint64_t handleCreateDummyProcessSyscall(va_list args){
    return (uint64_t) 
}
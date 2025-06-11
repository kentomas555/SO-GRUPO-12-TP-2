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
#include <semaphore.h>

static uint64_t handleReadSyscall(void);
static void handleWriteSyscall(va_list args);
static void handleInitDisplaySyscall(va_list args);
static void handleDrawRectangleSyscall(va_list args);
static void handleDrawCircleSyscall(va_list args);
static void handleSoundOnSyscall(va_list args);
static void handleDateSyscall(va_list args);
static uint64_t handleAllocMemorySyscall(va_list args);
static void handleFreeMemorySyscall(va_list args);
static uint64_t handlePrintMem();
static uint64_t handleGetPidSyscall(void);
static uint64_t handleCreateProcessSyscall(va_list args);
static uint64_t handleCreateDummyProcessSyscall(void);
static uint64_t handleBlockProcessSyscall(va_list args);
static uint64_t handleUnblockProcessSyscall(va_list args);
static uint64_t handleKillProcessSyscall(va_list args);
static void handleYieldSyscall(void);
static uint64_t handleGetPrioritySyscall(va_list args);
static uint64_t handleIncreasePrioritySyscall(va_list args); 
static uint64_t handleDecreasePrioritySyscall(va_list args);
static int32_t handleNiceSyscall(va_list args);
static uint64_t handleWaitPidSyscall(va_list args);
static processesToPrint * handleListProcesses();
static uint64_t handleSemInitSyscall(va_list args);
static uint64_t handleSemDestroySyscall(va_list args);
static void handleSemPostSyscall(va_list args);
static void handleSemWaitSyscall(va_list args);
static uint64_t handleExitSyscall(va_list args);
//static uint64_t handleGetCurrentBlock();

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
            handleFreeMemorySyscall(args);
            break;
        case SYSCALL_MEMORY_INFO:
            ret = handlePrintMem();
            break;

        //Process
        
        case SYSCALL_GET_PID:
            ret = handleGetPidSyscall();
            break;
        case SYSCALL_CREATE_PROCESS:
            ret = handleCreateProcessSyscall(args);
            break;
        case SYSCALL_CREATE_DUMMY_PROCESS:
            ret = handleCreateDummyProcessSyscall();
            break;
        case SYSCALL_EXIT:
            ret = handleExitSyscall(args);
            break;
        case SYSCALL_BLOCK_PROCESS:
            ret = handleBlockProcessSyscall(args);
            break;
        case SYSCALL_UNBLOCK_PROCESS:
            ret = handleUnblockProcessSyscall(args);
            break;
        case SYSCALL_KILL_PROCESS:
            ret = handleKillProcessSyscall(args);
            break;
        case SYSCALL_YIELD:
            handleYieldSyscall();
            break;
        case SYSCALL_LIST_PROCESSES:
            ret = (uint64_t)handleListProcesses();
            break;
        case SYSCALL_GET_PRIORITY:
            ret = handleGetPrioritySyscall(args);
            break;
        case SYSCALL_INCREASE_PRIORITY:
            ret = handleIncreasePrioritySyscall(args); 
            break;
        case SYSCALL_DECREASE_PRIORITY:
            ret = handleDecreasePrioritySyscall(args);
            break;
        case SYSCALL_NICE:
            ret = handleNiceSyscall(args);
            break;
        case SYSCALL_WAITPID:
            ret = handleWaitPidSyscall(args);
            break;
        case SYSCALL_HLT:
            _hlt();
            break;
        case SYSCALL_SEM_INIT:
            ret = (uint64_t)handleSemInitSyscall(args);
            break;
        case SYSCALL_SEM_DESTROY:
            ret = handleSemDestroySyscall(args);
            break;
        case SYSCALL_SEM_POST:
            handleSemPostSyscall(args);
            break;
        case SYSCALL_SEM_WAIT:
            handleSemWaitSyscall(args);
            break;
        // case SYSCALL_GETCURRENTBLOCK:
        //     handleGetCurrentBlock();
        //     break;
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
    return (uint64_t) allocMemory(size);
}

static void handleFreeMemorySyscall(va_list args) {
    void * address = va_arg(args, void *);
    freeMemory(address);
    return;
}

static uint64_t handlePrintMem(){
    return (uint64_t)getMemState();
}

static uint64_t handleGetPidSyscall(void){
    return (uint64_t)getCurrentPID();
}

static uint64_t handleCreateProcessSyscall(va_list args){
    char* processName = va_arg(args, char*);
    void* processProgram = va_arg(args, void*);
    char** ProcessArgs = va_arg(args, char**);
    Priority priority = va_arg(args, Priority);
    int16_t* fds = va_arg(args, int16_t*);
    return (uint64_t) onCreateProcess( processName, processProgram, ProcessArgs, priority, fds);
}

static uint64_t handleCreateDummyProcessSyscall(void){
    return (uint64_t) createDummyProcess();
}

static uint64_t handleBlockProcessSyscall(va_list args){
    Pid pid = va_arg(args,Pid);
    return (uint64_t) blockProcess(pid);
}

static uint64_t handleUnblockProcessSyscall(va_list args){
    Pid pid = va_arg(args,Pid);
    return (uint64_t) unblockProcess(pid);
}

static uint64_t handleKillProcessSyscall(va_list args){
    Pid pid = va_arg(args,Pid);
    return (uint64_t) killProcess(pid);
}

static void handleYieldSyscall(void){
    yield();
    return;
}

static uint64_t handleGetPrioritySyscall(va_list args){
    Pid pid = va_arg(args,Pid);
    return (uint64_t) getProcessPriority(pid);
}

static uint64_t handleIncreasePrioritySyscall(va_list args){
    Pid pid = va_arg(args,Pid);
    return (uint64_t) increaseProcessPriority(pid);
}

static uint64_t handleDecreasePrioritySyscall(va_list args){
    Pid pid = va_arg(args,Pid);
    return (uint64_t) decreaseProcessPriority(pid);
}

static int32_t handleNiceSyscall(va_list args){
    Pid pid = va_arg(args,Pid);
    Priority priority = va_arg(args, Priority);
    return (uint64_t) nice(pid, priority);
}

static uint64_t handleWaitPidSyscall(va_list args){
    Pid pid = va_arg(args,Pid);
    return (uint64_t) waitPID(pid);
}

static processesToPrint * handleListProcesses(){
    return printProcesses();
}

static uint64_t handleSemInitSyscall(va_list args){
    //char* semName = va_arg(args, char*);
    int32_t id = va_arg(args, int32_t);
    int32_t value = va_arg(args, int32_t);
    return semInit(id ,value); // no recibe name
}

static uint64_t handleSemDestroySyscall(va_list args){
    //char* semName = va_arg(args, char*);
    int32_t id = va_arg(args, int32_t);
    return (uint64_t)semDestroy(id);

}

static void handleSemPostSyscall(va_list args){
    //char* semName = va_arg(args, char*);
    int32_t id = va_arg(args, int32_t);
    semPost(id);
}

static void handleSemWaitSyscall(va_list args){
    //char* semName = va_arg(args, char*);
    int32_t id = va_arg(args, int32_t);
    semWait(id);
}

static uint64_t handleExitSyscall(va_list args){
    return exitProcess();
}

// static uint64_t handleGetCurrentBlock(){
//     return getCurrent();
// }
#include <stdarg.h>
#include <lib.h>

#define SYSCALL_READ 3
#define SYSCALL_WRITE 4
#define SYSCALL_INIT_DISPLAY 5
#define SYSCALL_DRAW_RECTANGLE 6
#define SYSCALL_DRAW_CIRCLE 7
#define SYSCALL_SECONDS_ELAPSED 8
#define SYSCALL_SOUND_ON 9
#define SYSCALL_SOUND_OFF 10
#define SYSCALL_TICKS_ELAPSED 11
#define SYSCALL_FREE_SPACE 12
#define SYSCALL_DATE 13

/*========= SO SYSCALLS ==========*/
#define SYSCALL_ALLOC_MEMORY 14
#define SYSCALL_FREE_MEMORY 15
#define SYSCALL_MEMORY_INFO 16

#define SYSCALL_GET_PID 17
#define SYSCALL_CREATE_PROCESS 18
#define SYSCALL_CREATE_DUMMY_PROCESS 19
#define SYSCALL_EXIT 20
#define SYSCALL_BLOCK_PROCESS 21
#define SYSCALL_UNBLOCK_PROCESS 22
#define SYSCALL_KILL_PROCESS 23
#define SYSCALL_LIST_PROCESSES 24
#define SYSCALL_GET_PRIORITY 25
#define SYSCALL_INCREASE_PRIORITY 26
#define SYSCALL_DECREASE_PRIORITY 27
#define SYSCALL_WAIT_PID 28

static uint64_t handleReadSyscall(void);
static void handleWriteSyscall(va_list args);
static void handleInitDisplaySyscall(va_list args);
static void handleDrawRectangleSyscall(va_list args);
static void handleDrawCircleSyscall(va_list args);
static void handleSoundOnSyscall(va_list args);
static void handleDateSyscall(va_list args);
static uint64_t handleAllocMemorySyscall(va_list args);
static uint64_t handleGetPidSyscall(void);
static uint64_t handleCreateProcessSyscall(va_list args);
static uint64_t handleCreateDummyProcessSyscall(void);
static uint64_t handleBlockProcessSyscall(va_list args);
static uint64_t handleUnblockProcessSyscall(va_list args);
static uint64_t handleKillProcessSyscall(va_list args);
static uint64_t handleGetPrioritySyscall(va_list args);
static uint64_t handleIncreasePrioritySyscall(va_list args); 
static uint64_t handleDecreasePrioritySyscall(va_list args);

uint64_t syscallDispatcher(uint64_t id, ...);
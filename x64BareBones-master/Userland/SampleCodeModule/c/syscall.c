
#include <syscall.h>

int currentX = 0;
int currentY = 0;
char fontSize = 2;



/*====== READ ======*/

char getChar(){
  char ret = syscall(SYSCALL_READ);
  while (ret == -1){
    ret = syscall(SYSCALL_READ);
  }
  return ret;
}

char getKeyDown(){
    return (char)syscall(SYSCALL_READ);
}


/*====== WRITE ======*/

void setCurrentX(int newX){
    currentX = newX;
}

void setCurrentY(int newY){
    currentY = newY;    
}

int getCurrentX(){
    return currentX;
}

int getCurrentY(){
    return currentY;    
}

int getFontSize(){
  return fontSize;
}

void increaseFontSize(){
  fontSize++;
}

void decreaseFontSize(){
  fontSize--;
}

void printf(char *string){
    syscall(SYSCALL_WRITE, currentX, currentY, fontSize, string);
}

void printfPos(char *string, int x, int y, char fontSizePos ){
    syscall(SYSCALL_WRITE, x, y, fontSizePos, string);
}

/*====== INIT DISPLAY ======*/

void initDisplay(uint64_t HexColor){
  syscall(SYSCALL_INIT_DISPLAY, HexColor, 0, 0);
}

/*====== RECTANGLE ======*/

void drawRectangle(int x , int y, int height, int width, uint64_t hexColor){
    syscall(SYSCALL_DRAW_RECTANGLE, x , y, height, width,  hexColor);
}

/*====== CIRCLE  ======*/

void drawCircle(int x , int y, int radius, uint64_t hexColor){
    syscall(SYSCALL_DRAW_CIRCLE, x, y, radius, hexColor);
}

/*====== SECONDS ELAPSED ======*/

int seconds_elapsed(){
    return syscall(SYSCALL_SECONDS_ELAPSED);
}

/*====== SOUND ON AND OFF ======*/

void soundOn(uint64_t frecuencia){
    syscall(SYSCALL_SOUND_ON, frecuencia);
}

void soundOff(){
    syscall(SYSCALL_SOUND_OFF);
}

/*====== TICKS ELAPSED ======*/

int ticks_elapsed(){
    return syscall(SYSCALL_TICKS_ELAPSED);
}

/*====== DATE ======*/

void getDate(date * time){
  syscall(SYSCALL_DATE, time);
}

/*====== MEMORY MANAGER ======*/

void * allocMemoryUser(uint32_t size){
    uint64_t aux = syscall(SYSCALL_ALLOC_MEMORY, size);
    return (void *) aux;
}

void freeMemoryUser(){
    syscall(SYSCALL_FREE_MEMORY);
}

/*AGREGADO*/

memoryState * getMemoryState(){
    return (memoryState *)syscall(SYSCALL_MEMORY_INFO);
}

uint64_t getCurrentBlock(){
    return (uint64_t)syscall(SYSCALL_GETCURRENTBLOCK);
}

/*AGREGADO END*/

/*====== NEW PROCESS ======*/

int createNewProcess(char * processName, void * processProgram, char** args, Priority priority, int16_t fds[]){
    return syscall(SYSCALL_CREATE_PROCESS, processName,  processProgram, args, priority, fds);
}

int createDummyProcess(){
    return syscall(SYSCALL_CREATE_DUMMY_PROCESS);
}

/*====== EXISTING PROCESSES ======*/

Pid getpid(){
    return syscall(SYSCALL_GET_PID);
}

int blockProcess(Pid pid){
    return syscall(SYSCALL_BLOCK_PROCESS, pid);
}

int unblockProcess(Pid pid){
    return syscall(SYSCALL_UNBLOCK_PROCESS, pid);
}

int exitProcess(){
    return syscall(SYSCALL_EXIT);
}

int killProcess(Pid pid){
    return syscall(SYSCALL_KILL_PROCESS, pid);
}

processesList * getProcesses(){
    return (processesList *)syscall(SYSCALL_LIST_PROCESSES);
}

int getPriority(Pid pid){
    return syscall(SYSCALL_GET_PRIORITY, pid);
}

int increasePriority(Pid pid){
    return syscall(SYSCALL_INCREASE_PRIORITY, pid);
}

int decreasePriority(Pid pid){
    return syscall(SYSCALL_DECREASE_PRIORITY, pid);
}

int nice(Pid pid, Priority priority){
    return syscall(SYSCALL_NICE, pid, priority);
}

//FALTA WAITPID

/*====== HLT ======*/

void hlt(){
    syscall(SYSCALL_HLT);
}

/*====== SEMAPHORES ======*/

void * semInit(char * semName, int32_t value){
    return (void *)syscall(SYSCALL_SEM_INIT, semName, value);
}

void semDestroy(char * semName){
    syscall(SYSCALL_SEM_DESTROY, semName);
}

void semPost(char * semName){
    syscall(SYSCALL_SEM_POST, semName);
}

void semWait(char * semName){
    syscall(SYSCALL_SEM_WAIT, semName);
}



/*====== PIPES ======*/

// int64_t my_getpid() {
//   return 0;
// }

// int64_t my_create_process(char *name, uint64_t argc, char *argv[]) {
//   return 0;
// }

// int64_t my_nice(uint64_t pid, uint64_t newPrio) {
//   return 0;
// }

// int64_t my_kill(uint64_t pid) {
//   return 0;
// }

// int64_t my_block(uint64_t pid) {
//   return 0;
// }

// int64_t my_unblock(uint64_t pid) {
//   return 0;
// }

// int64_t my_sem_open(char *sem_id, uint64_t initialValue) {
//   return 0;
// }

// int64_t my_sem_wait(char *sem_id) {
//   return 0;
// }

// int64_t my_sem_post(char *sem_id) {
//   return 0;
// }

// int64_t my_sem_close(char *sem_id) {
//   return 0;
// }

// int64_t my_yield() {
//   return 0;
// }

// int64_t my_wait(int64_t pid) {
//   return 0;
// }

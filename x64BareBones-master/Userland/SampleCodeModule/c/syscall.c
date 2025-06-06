
#include <syscall.h>

int currentX = 0;
int currentY = 0;
char fontSize = 2;



/*====== READ ======*/

char getChar(){
  char ret = syscall(3);
  while (ret == -1){
    ret = syscall(3);
  }
  return ret;
}

char getKeyDown(){
    return (char)syscall(3);
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
    syscall(4, currentX, currentY, fontSize, string);
}

void printfPos(char *string, int x, int y, char fontSizePos ){
    syscall(4, x, y, fontSizePos, string);
}

/*====== INIT DISPLAY ======*/

void initDisplay(uint64_t HexColor){
  syscall(5, HexColor, 0, 0);
}

/*====== RECTANGLE ======*/

void drawRectangle(int x , int y, int height, int width, uint64_t hexColor){
    syscall(6, x , y, height, width,  hexColor);
}

/*====== CIRCLE  ======*/

void drawCircle(int x , int y, int radius, uint64_t hexColor){
    syscall(7, x, y, radius, hexColor);
}

/*====== SECONDS ELAPSED ======*/

int seconds_elapsed(){
    return syscall(8);
}

/*====== SOUND ON AND OFF ======*/

void soundOn(uint64_t frecuencia){
    syscall(9, frecuencia);
}

void soundOff(){
    syscall(10);
}

/*====== TICKS ELAPSED ======*/

int ticks_elapsed(){
    return syscall(11);
}

/*====== DATE ======*/

void getDate(date * time){
  syscall(13, time);
}

/*====== MEMORY MANAGER ======*/

void * allocMemoryUser(uint32_t size){
    uint64_t aux = syscall(14, size);
    return (void *) aux;
}

void freeMemoryUser(){
    syscall(15);
}

/*====== NEW PROCESS ======*/

int createNewProcess(char * processName, void * processProgram, char** args, Priority priority, int16_t fds[]){
    return syscall(18, processName,  processProgram, args, priority, fds);
}

int createDummyProcess(){
    return syscall(19);
}

/*====== EXISTING PROCESSES ======*/

Pid getpid(){
    return syscall(17);
}

int blockProcess(Pid pid){
    return syscall(21, pid);
}

int unblockProcess(Pid pid){
    return syscall(22, pid);
}

int killProcess(Pid pid){
    return syscall(23, pid);
}

int getPriority(Pid pid){
    return syscall(25, pid);
}

int increasePriority(Pid pid){
    return syscall(26, pid);
}

int decreasePriority(Pid pid){
    return syscall(27, pid);
}

/*====== HLT ======*/

void hlt(){
    syscall(29);
}

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

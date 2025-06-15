
#include <syscall.h>

int currentX = 0;
int currentY = 0;
char fontSize = 2;



/*====== READ ======*/

char getChar(){
    // char ret;
    // int fd = getReadFD(getpid());
    // if(fd == 0){
    //     ret = syscall(SYSCALL_READ);
    //     while (ret == -1){
    //         ret = syscall(SYSCALL_READ);
    //     }
    // } else {
    //     char buf[2] = {0};
    //     //printf(buf[0]);
    //     readPipeUser(fd, buf);
    //     ret = buf[0];
    //     //ret = "a";
    // }
    // return ret;
    char ret;
    int fd = getReadFD(getpid());
    if (fd == 0) {
        ret = syscall(SYSCALL_READ);
        while (ret == -1) {
            ret = syscall(SYSCALL_READ);
        }
    } else {
        char buf[2] = {0};
        if (readPipeUser(fd, buf) == 0) {
            return -1; // Pipe invalid or error
        }
        ret = buf[0];
        if (ret == '\0') {
            return -1; // Interpret null terminator as EOF
        }
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
    int fd = getWriteFD(getpid());    
    if (fd == 1) {
        syscall(SYSCALL_WRITE, currentX, currentY, fontSize, string);
    } else {
        writePipeUser(fd, string);
    }
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

void freeMemoryUser(void * address){
    syscall(SYSCALL_FREE_MEMORY, address);
}

/*AGREGADO*/

memoryState * getMemoryState(){
    return (memoryState *)syscall(SYSCALL_MEMORY_INFO);
}

// uint64_t getCurrentBlock(){
//     return (uint64_t)syscall(SYSCALL_GETCURRENTBLOCK);
// }

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

int exitProcess(int retValue){
    return syscall(SYSCALL_EXIT, retValue);
}

int killProcess(Pid pid){
    return syscall(SYSCALL_KILL_PROCESS, pid);
}

void yield(){
    syscall(SYSCALL_YIELD);
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

int waitPID(Pid pid){
    return (int)syscall(SYSCALL_WAITPID, pid);
}

/*====== HLT ======*/

void hlt(){
    syscall(SYSCALL_HLT);
}

/*====== SEMAPHORES ======*/

uint64_t semInit(int32_t id,uint32_t value){
    return syscall(SYSCALL_SEM_INIT, id, value);
}

uint64_t semDestroy(int id){
    return syscall(SYSCALL_SEM_DESTROY, (uint64_t)id);
}

void semPost(int id){
    syscall(SYSCALL_SEM_POST, (uint64_t)id);
}

void semWait(int id){
    syscall(SYSCALL_SEM_WAIT, (uint64_t)id);
}

/*====== PIPES ======*/

int64_t createPipeUser(int pipeID, int16_t fds[]){
    return syscall(SYSCALL_CREATE_PIPE, pipeID, fds);
}

void destroyPipeUser(int pipeID){
    syscall(SYSCALL_DESTROY_PIPE, pipeID);
}

uint64_t writePipeUser(int pipeID, const char * source){
    return syscall(SYSCALL_WRITE_PIPE, pipeID, source);
}

uint64_t readPipeUser(int pipeID, char * destination){
    return syscall(SYSCALL_READ_PIPE, pipeID, destination);
}

uint64_t closePipeUser(int pipeID, int isReader){
    return syscall(SYSCALL_CLOSE_PIPE, pipeID, isReader);
}

int64_t * getSharedMemory(){
    return (int64_t *)syscall(SYSCALL_SHARED_MEM);
}

/*====== FILE DESCRIPTORS ======*/

uint64_t getReadFD(Pid pid){
    return (int64_t)syscall(SYSCALL_GET_R_FD, pid);
}

uint64_t getWriteFD(Pid pid){
    return (int64_t)syscall(SYSCALL_GET_W_FD, pid);
}

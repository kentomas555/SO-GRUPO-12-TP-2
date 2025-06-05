#include <libasm.h>
#include <globalLib.h>

typedef void (*function)();

int currentX = 0;
int currentY = 0;
char fontSize = 2;

uint64_t bgColor = 0x00;

int minX = 0;
char buffer[48];

void moveCurrentX(int newX){
    currentX = newX;
}

void moveCurrentY(int newY){
    currentY = newY;    
}

//write
void printf(char *string){
    syscall(4, currentX, currentY, fontSize, string);
}

void printfPos(char *string, int x, int y, char fontSizePos ){
    syscall(4, x, y, fontSizePos, string);
}

void charDelete(int x, int y, char size){
    if(size == 1){
      drawRectangle(x, y, 8, 8, bgColor);
    } else if (size == 2){
        drawRectangle(x, y, 8, 12, bgColor);
    } else {
        drawRectangle(x, y, 16, 16, bgColor);
    }
}


//read

char getChar(){

    char ret = syscall(3);

    while (ret == -1)
    {
        ret = syscall(3);
    }
    return ret;
    
}

char getKeyDown(){
    return (char)syscall(3);
}

void getBuffer(char * buffer, int size){
    printf("$");
    currentX += HorizontalOffset(fontSize);
    printf(">");
    currentX += HorizontalOffset(fontSize);
    int aux = minX;
    minX = currentX;
    char end = 0;

    int i = 0;
    char buffAux[1];

    for(; end == 0 && i < (size - 1); i++){
        buffer[i] = getChar();
        if (buffer[i] == 0 || buffer[i] == 9 || buffer[i] == 10 || buffer[i] == 13){
            end++;
        }
        buffAux[0] = buffer[i];
        printf(buffAux);
        if (buffer[i] >= 32 && buffer[i] <=  126){
            currentX += HorizontalOffset(fontSize);
        } else if (buffer[i] == 8 && currentX > minX){
            currentX -= HorizontalOffset(fontSize);
            i--;
            buffer[i] = 32; 
            charDelete(currentX, currentY, fontSize);
            i--;
        } else if (i >= 0) {
            i--;
        }
    }
    if(buffer[i] == 9){
        ClearScreen(bgColor);
    }
    buffer[i] = 0;
    NewLine();
    minX = aux;
    currentX = minX;
}

void ClearScreen(uint64_t HexColor){
    bgColor = HexColor;
    syscall(5, HexColor, 0, 0);
    currentX = 0;
    currentY = 0;
}

void drawRectangle(int x , int y, int height, int width, uint64_t hexColor){
    syscall(6, x , y, height, width,  hexColor);
}

void printCurrentTime(){
    int xAux = currentX;
    char timeBuffer[10]; 

    date time = {0};
    
    syscall(13, &time);
    int aux = time.hours;
	itoaBase(aux, timeBuffer, 10);
    //getCurrentHours(timeBuffer);
    printf(timeBuffer);
    currentX += HorizontalOffset(fontSize) * 2;
    
    printf(":");
    currentX += HorizontalOffset(fontSize);

    syscall(13, &time);
    aux = time.minutes;
	itoaBase(aux, timeBuffer, 10);
    //getCurrentMinutes(timeBuffer);
    printf(timeBuffer);
    currentX += HorizontalOffset(fontSize) * 2;
    
    printf(":");
    currentX += HorizontalOffset(fontSize);

    syscall(13, &time);
    aux = time.seconds;
	itoaBase(aux, timeBuffer, 10);
    //getCurrentSeconds(timeBuffer);
    printf(timeBuffer);
    currentX = xAux;
    NewLine();

    syscall(13, &time);
    aux = time.day;
	itoaBase(aux, timeBuffer, 10);
    //getCurrentDay(timeBuffer);
    printf(timeBuffer);
    currentX += HorizontalOffset(fontSize) * 2;
    
    printf("/");
    currentX += HorizontalOffset(fontSize);
    
    syscall(13, &time);
    aux = time.month;
	itoaBase(aux, timeBuffer, 10);
    //getCurrentMonth(timeBuffer);
    printf(timeBuffer);
    currentX += HorizontalOffset(fontSize) * 2;
    
    printf("/");
    currentX += HorizontalOffset(fontSize);

    syscall(13, &time);
    aux = time.year;
	itoaBase(aux, timeBuffer, 10);
    //getCurrentYear(timeBuffer);
    printf(timeBuffer);
    currentX = xAux;
    NewLine();
    NewLine();
}

void drawCircle(int x , int y, int radius, uint64_t hexColor){
    syscall(7, x, y, radius, hexColor);
}

void NewLine(){
    int offset;
    if (fontSize == 3){
        offset = 16;
    } else if (fontSize == 2){
        offset = 12;
    } else {
        offset = 8;
    }
    currentY += offset;
    currentX = 0;
    if (currentY >= 750){
        ClearScreen(bgColor);
    }
}

int HorizontalOffset(char fontSize){
    if (fontSize == 3){
        return 16;
    } else {
        return 8;
    } 
}

void largerFontSize(){
    if (fontSize == 3){
        printf("No se puede agrandar mas la letra");
        NewLine();
        NewLine();
    } else {
        fontSize++;
    }
}

void smallerFontSize(){
    if (fontSize == 1){
        printf("No se puede achicar mas la letra");
        NewLine();
        NewLine();
    } else {
        fontSize--;
    }
}

void itoaBase(uint64_t value, char * buffer, uint32_t base)
{
	char *p = buffer;
	char *p1, *p2;
	uint32_t digits = 0;

	//Calculate characters for each digit
	do
	{
		uint32_t remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

}

int strCompare(char *str1, char *str2) {
    while (*str1 != 0 && *str2 != 0) {
        if (*str1 != *str2) {
            return 0;
        }
        str1++;
        str2++;
    }
    return (*str1 == 0 && *str2 == 0) ? 1 : 0;
}

int seconds_elapsed(){
    return syscall(8);
}


void soundOn(uint64_t frecuencia){
    syscall(9, frecuencia);
}

void soundOff(){
    syscall(10);
}

void sound(uint64_t frecuencia , uint64_t duración ){
    soundOn(frecuencia);
    wait(duración);
    soundOff();
}

int ticks_elapsed(){
    return syscall(11);
}

void wait(int ticks){
    int aux = ticks_elapsed();
    while(ticks_elapsed() <= aux + ticks){

    }
    return;
}

//void * allocMemory(MemoryManagerADT mm, size_t memoryToAllocate) 

void allocMemoryUser(uint32_t size){
    char allocMemoryBuffer[10]; 
    printf("entre a allocMemoryUser");
    NewLine();
    //
    uint64_t aux = syscall(14, size);
    if(aux){
        printf("Hay algo");
        NewLine();
    }
    itoaBase(aux, allocMemoryBuffer, 16);
    printf(allocMemoryBuffer);
    NewLine();
    //
    aux = syscall(14, size);
    if(aux){
        printf("Hay algo");
        NewLine();
    }
    itoaBase(aux, allocMemoryBuffer, 16);
    printf(allocMemoryBuffer);
    NewLine();
    //
    aux = syscall(14, size);
    if(aux){
        printf("Hay algo");
        NewLine();
    }
    itoaBase(aux, allocMemoryBuffer, 16);
    printf(allocMemoryBuffer);
    NewLine();
    //
    aux = syscall(14, size);
    if(aux){
        printf("Hay algo");
        NewLine();
    }
    itoaBase(aux, allocMemoryBuffer, 16);
    printf(allocMemoryBuffer);
    NewLine();
    //
    printf("entre a allocMemoryUser");
    NewLine();
}

void freeMemoryUser(){
    syscall(15);
}

Pid getpid(){
    return syscall(17);
}

int createNewProcess(char * processName, void * processProgram, char** args, Priority priority, int16_t fds[]){
    return syscall(18, processName,  processProgram, args, priority, fds);
}

int createDummyProcess(){
    return syscall(19);
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

void hlt(){
    syscall(29);
}

void printProcesses(){
    
    processesToPrint * pr = syscall(24);
    char* state[4] = {"READY", "RUNNING", "BLOCKED", "KILLED"};
    
    for(int i = 0; i < pr->cantProcess; i++){
        
        char auxBuffer[20];
        
        NewLine();
        printf("Proceso: ");
        currentX += HorizontalOffset(fontSize) * 9;
        printf(pr->names[i]);
        NewLine();
        printf("-------------");
        NewLine();
        printf(" PID: ");
        currentX += HorizontalOffset(fontSize) * 6;
        itoaBase((uint64_t)pr->PIDs[i], auxBuffer, 10);
        printf(auxBuffer);
        NewLine();
        printf(" rsp: 0x");
        currentX += HorizontalOffset(fontSize) * 8;
        itoaBase((uint64_t)pr->rspList[i], auxBuffer, 16);
        printf(auxBuffer);
        NewLine();
        printf(" rbp: 0x");
        currentX += HorizontalOffset(fontSize) * 8;
        itoaBase((uint64_t)pr->rbpList[i], auxBuffer, 16);
        printf(auxBuffer);
        NewLine();
        printf(" status: ");
        currentX += HorizontalOffset(fontSize) * 8;
        printf(state[pr->Priority[i]]);
        NewLine();
        printf(" prioridad: ");
        currentX += HorizontalOffset(fontSize) * 13;
        itoaBase((uint64_t)pr->Priority[i], auxBuffer, 10);
        printf(auxBuffer);
        NewLine();
        NewLine();
    }
}

//EXCEPTION TRIGGERS:

void zeroDivisionTrigger(){
    throw_zero_division();
}

void invalidOpcodeTrigger(){
    throw_invalid_opcode();
}
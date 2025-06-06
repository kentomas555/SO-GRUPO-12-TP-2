#include <libasm.h>
#include <globalLib.h>
#include <syscall.h>

typedef void (*function)();



uint64_t bgColor = 0x00;

int minX = 0;
char buffer[48];

//write

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

static int HorizontalOffset(char fontSize){
    if (fontSize == 3){
        return 16;
    } else {
        return 8;
    } 
}

static void nextX(int i){
    setCurrentX(getCurrentX() + HorizontalOffset(getFontSize()) * i);
}

static void prevX(int i){
    setCurrentX(getCurrentX() - HorizontalOffset(getFontSize()) * i);
}

void getBuffer(char * buffer, int size){
    printf("$");
    nextX(1);
    printf(">");
    nextX(1);
    int aux = minX;
    minX = getCurrentX();
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
            nextX(1);
        } else if (buffer[i] == 8 && getCurrentX() > minX){
            prevX(1);
            i--;
            buffer[i] = 32; 
            charDelete(getCurrentX(), getCurrentY(), getFontSize());
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
    setCurrentX(minX);
}

void ClearScreen(uint64_t HexColor){
    bgColor = HexColor;
    initDisplay(HexColor);
    setCurrentX(0);
    setCurrentY(0);
}

void printCurrentTime(){
    int xAux = getCurrentX();
    char timeBuffer[10]; 

    date time = {0};
    
    getDate(&time);
    int aux = time.hours;
	itoaBase(aux, timeBuffer, 10);
    //getCurrentHours(timeBuffer);
    printf(timeBuffer);
    nextX(2);
    
    printf(":");
    nextX(1);

    getDate(&time);
    aux = time.minutes;
	itoaBase(aux, timeBuffer, 10);
    //getCurrentMinutes(timeBuffer);
    printf(timeBuffer);
    nextX(2);
    
    printf(":");
    nextX(1);

    getDate(&time);
    aux = time.seconds;
	itoaBase(aux, timeBuffer, 10);
    //getCurrentSeconds(timeBuffer);
    printf(timeBuffer);
    setCurrentX(xAux);
    NewLine();

    getDate(&time);
    aux = time.day;
	itoaBase(aux, timeBuffer, 10);
    //getCurrentDay(timeBuffer);
    printf(timeBuffer);
    nextX(2);
    
    printf("/");
    nextX(1);
    
    getDate(&time);
    aux = time.month;
	itoaBase(aux, timeBuffer, 10);
    //getCurrentMonth(timeBuffer);
    printf(timeBuffer);
    nextX(2);
    
    printf("/");
    nextX(1);

    getDate(&time);
    aux = time.year;
	itoaBase(aux, timeBuffer, 10);
    //getCurrentYear(timeBuffer);
    printf(timeBuffer);
    setCurrentX(xAux);
    NewLine();
    NewLine();
}

void NewLine(){
    int offset;
    if (getFontSize() == 3){
        offset = 16;
    } else if (getFontSize() == 2){
        offset = 12;
    } else {
        offset = 8;
    }
    setCurrentY(getCurrentY()+offset);
    setCurrentX(0);
    if (getCurrentY() >= 750){
        ClearScreen(bgColor);
    }
}

void largerFontSize(){
    if (getFontSize() == 3){
        printf("No se puede agrandar mas la letra");
        NewLine();
        NewLine();
    } else {
        increaseFontSize();
    }
}

void smallerFontSize(){
    if (getFontSize() == 1){
        printf("No se puede achicar mas la letra");
        NewLine();
        NewLine();
    } else {
        decreaseFontSize();
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

void sound(uint64_t frecuencia , uint64_t duración ){
    soundOn(frecuencia);
    wait(duración);
    soundOff();
}

void wait(int ticks){
    int aux = ticks_elapsed();
    while(ticks_elapsed() <= aux + ticks){

    }
    return;
}

void printProcesses(){
    
    processesToPrint * pr = (processesToPrint *)syscall(24);
    char* state[4] = {"READY", "RUNNING", "BLOCKED", "KILLED"};
    
    for(int i = 0; i < pr->cantProcess; i++){
        
        char auxBuffer[20];
        
        NewLine();
        printf("Proceso: ");
        nextX(9);
        printf(pr->names[i]);
        NewLine();
        printf("-------------");
        NewLine();
        printf(" PID: ");
        nextX(6);
        itoaBase((uint64_t)pr->PIDs[i], auxBuffer, 10);
        printf(auxBuffer);
        NewLine();
        printf(" PPID: ");
        nextX(6);
        itoaBase((uint64_t)pr->PPIDs[i], auxBuffer, 10);
        printf(auxBuffer);
        NewLine();
        printf(" rsp: 0x");
        nextX(8);
        itoaBase((uint64_t)pr->rspList[i], auxBuffer, 16);
        printf(auxBuffer);
        NewLine();
        printf(" rbp: 0x");
        nextX(8);
        itoaBase((uint64_t)pr->rbpList[i], auxBuffer, 16);
        printf(auxBuffer);
        NewLine();
        printf(" status: ");
        nextX(8);
        printf(state[pr->Status[i]]);
        NewLine();
        printf(" prioridad: ");
        nextX(13);
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
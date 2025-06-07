#include <libasm.h>
#include <globalLib.h>
#include <syscall.h>
#include <tests.h>

uint64_t bgColor = 0x00;

int minX = 0;
char buffer[48];

/*====== STATIC FUNCTIONS ======*/

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

static void setX(int i){
    setCurrentX(HorizontalOffset(getFontSize()) * i);
}

static void prevX(int i){
    setCurrentX(getCurrentX() - HorizontalOffset(getFontSize()) * i);
}

/*====== SCREEN FUNCTIONS ======*/

void charDelete(int x, int y, char size){
    if(size == 1){
      drawRectangle(x, y, 8, 8, bgColor);
    } else if (size == 2){
        drawRectangle(x, y, 8, 12, bgColor);
    } else {
        drawRectangle(x, y, 16, 16, bgColor);
    }
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

/*====== AUXILIAR FUNCTION ======*/

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

int strCompareFirstComand(char *str1, char *str2) {
    while (*str1 != 0 && *str2 != 0) {
        if (*str1 != *str2) {
            return 0;
        }
        str1++;
        str2++;
        if(*str1 == ' '){
            return 1;
        }
    }
    return (*str1 == 0 && *str2 == 0) ? 1 : 0;
}

static int atoi(char * str){
    int result = 0;
    while (*str >= '0' && *str <= '9') {
        result = result * 10 + (*str - '0');
        str++;
    }
    return result;
}

/*====== PRINT TIME ======*/

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

/*====== SOUND ======*/

void sound(uint64_t frecuencia , uint64_t duración ){
    soundOn(frecuencia);
    wait(duración);
    soundOff();
}

/*====== WAIT ======*/

void wait(int ticks){
    int aux = ticks_elapsed();
    while(ticks_elapsed() <= aux + ticks){

    }
    return;
}

/*====== PROCESSES ======*/

void handleGetPid(){
    NewLine();
    char auxBuffer[20];
    itoaBase((uint64_t)getpid(), auxBuffer, 10);
    printf(auxBuffer);
    NewLine();
}

static int getPIDFromBuffer(char * buffer) {
    char pid[MAX_PID_LENGTH + 1];
    int i = 0;

    while (i < MAX_PID_LENGTH && *buffer != ' ' && *buffer != '\0') {
        if (*buffer < '0' || *buffer > '9') {
            return -1;
        }
        pid[i++] = *buffer++;
    }

    pid[i] = '\0';

    if (i == 0) {
        return -1;
    }

    return atoi(pid);
}

static int getIndex(processesList * psList, int checkPID){
    for (int i = 0; i < psList->processQty; i++){
        if(psList->PIDs[i] == checkPID){
            return i;
        }
    }
    return -1;
}

static int getPriorityFromBuffer(char * buffer) {
    char priority[MAX_PRIORITY_LENGTH + 1];
    int i = 0;

    while (i < MAX_PRIORITY_LENGTH && *buffer != ' ' && *buffer != '\0') {
        priority[i++] = *buffer++;
    }
    priority[i] = '\0';

    if (strCompare(priority, "HIGHEST")) {
        return HIGHEST_PRIO;
    } else if (strCompare(priority, "LOWEST")) {
        return LOWEST_PRIO;
    } else if (strCompare(priority, "AVERAGE")) {
        return AVERAGE_PRIO;
    } else if (strCompare(priority, "HIGH")) {
        return HIGH_PRIO;
    } else if (strCompare(priority, "LOW")) {
        return LOW_PRIO;
    } else {
        return -1;
    }
}

static int validPriority(int priority){
    if(priority >= 0 && priority < 5){
        return 1;
    }
    return 0;
}

void handleNice(char * buffer){
    char* priorityList[5] = {"LOWEST", "LOW", "AVERAGE", "HIGH", "HIGHEST"};

    while (*buffer != ' ' && *buffer != '\0') buffer++;
    if (*buffer == '\0') {
        printf("Faltan parametros");
        NewLine();
        printf("Ejemplo de llamada: NICE (PID) (PRIORIDAD)");
        NewLine();
        NewLine();
        return;
    }
    buffer++;  

    processesList * psList = getProcesses();
    int checkPID = getPIDFromBuffer(buffer);

    if (checkPID < 0) {
        printf("Formato invalido de PID");
        NewLine();
        printf("Ejemplo de llamada: NICE (PID) (PRIORIDAD)");
        NewLine();
        NewLine();
        return;
    }

    int psListIndex = getIndex(psList, checkPID);
    if (psListIndex == -1) {
        printf("No existe el PID");
        NewLine();
        NewLine();
        return;
    }

    while (*buffer != ' ' && *buffer != '\0') buffer++;
    if (*buffer == '\0') {
        printf("Falta parametro de prioridad");
        NewLine();
        NewLine();
        printf("Ejemplo de llamada: NICE (PID) (PRIORIDAD)");
        NewLine();
        printf("Prioridades: HIGHEST, HIGH, AVERAGE, LOW, LOWEST");
        NewLine();
        NewLine();
        return;
    }
    buffer++;  

    int checkPriority = getPriorityFromBuffer(buffer);
    if (!validPriority(checkPriority)) {
        printf("La prioridad es invalida.");
        NewLine();
        NewLine();
        printf("Prioridades: HIGHEST, HIGH, AVERAGE, LOW, LOWEST");
        NewLine();
        NewLine();
        return;
    }

    Priority priority = (Priority)checkPriority;
    Pid pid = (Pid)checkPID;
    nice(pid, priority);

    // Mostrar resultado
    printf("Process ");
    setX(8);
    printf(psList->names[psListIndex]);
    NewLine();
    printf("Priority change was successful!");
    NewLine();
    NewLine();
    printf("New priority: ");
    setX(14);
    printf(priorityList[priority]);
    NewLine();
    NewLine();
}

void printProcesses(){
    
    processesList * pr = getProcesses();
    char* state[4] = {"READY", "RUNNING", "BLOCKED", "KILLED"};
    char* priority[5] = {"LOWEST", "LOW", "AVERAGE", "HIGH", "HIGHEST"};

    NewLine();
    printf(" PROCESS        PID PPID STATUS  PRIO     CHILDS RSP      RBP   ");
    for(int i = 0; i < pr->cantProcess; i++){
        
        char auxBuffer[20];
        
        NewLine();
        setX(1);
        printf(pr->names[i]);
        setX(16);
        itoaBase((uint64_t)pr->PIDs[i], auxBuffer, 10);
        printf(auxBuffer);
        setX(20);
        itoaBase((uint64_t)pr->PPIDs[i], auxBuffer, 10);
        printf(auxBuffer);
        setX(25);
        printf(state[pr->Status[i]]);
        setX(33);
        printf(priority[pr->Priority[i]]);
        setX(42);
        itoaBase((uint64_t)pr->childrens[i], auxBuffer, 16);
        printf(auxBuffer);
        setX(49);
        itoaBase((uint64_t)pr->rspList[i], auxBuffer, 16);
        printf(auxBuffer);
        setX(58);
        itoaBase((uint64_t)pr->rbpList[i], auxBuffer, 16);
        printf(auxBuffer);
    }
    NewLine();
    NewLine();
}

/*====== TESTS ======*/

void memoryManagerTest(){
    // char * args[] = {MEMORY_SIZE};
    // createNewProcess("Memory Test",(mainFunc)test_mm,);
    return;
}

void processTest(){
    return;
}

void priorityTest(){
    return;
}

void syncroTest(){
    return;
}

void noSyncroTest(){
    return;
}

/*====== EXCEPTIONS TRIGGER ======*/

void zeroDivisionTrigger(){
    throw_zero_division();
}

void invalidOpcodeTrigger(){
    throw_invalid_opcode();
}
#include <libasm.h>
#include <globalLib.h>
#include <syscall.h>
#include <tests.h>
#include <phylo.h>
#include <userShell.h>

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

static void * memset(void * destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

void nextX(int i){
    setCurrentX(getCurrentX() + HorizontalOffset(getFontSize()) * i);
}

void setX(int i){
    setCurrentX(HorizontalOffset(getFontSize()) * i);
}

void prevX(int i){
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

static void *memcpy(void *dest, const void *src, int n) {
    char *d = (char *)dest;
    const char *s = (const char *)src;
    for (int i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

int strlen(const char *str) {
    int length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}

char * strchr(char *string, char toFind){
    //int len = 0;
    while(*string){
        if(*string == toFind){
            return string;
        }
        string++;
    }
    return NULL;
}

char * strtok(char *str, char *delim) {
    static char *posicion = NULL;  // Guarda la posición entre llamadas
    if (str != NULL) {
        posicion = str;
    }

    if (posicion == NULL) {
        return NULL;
    }

    // Saltar caracteres delimitadores al inicio
    while (*posicion && strchr(delim, *posicion)) {
        posicion++;
    }

    if (*posicion == '\0') {
        return NULL;
    }

    char *inicio = posicion;

    // Buscar el siguiente delimitador
    while (*posicion && !strchr(delim, *posicion)) {
        posicion++;
    }

    // Si encontramos un delimitador, lo reemplazamos por '\0' y avanzamos
    if (*posicion) {
        *posicion = '\0';
        posicion++;
    } else {
        posicion = NULL;
    }

    return inicio;
}

int countArguments(void **argc){
	int toReturn = 0;
	if(argc == NULL){
		return 0;
	}
	while(argc[toReturn] != NULL){
		toReturn++;
	}
	return toReturn;
}

/*====== PRINT TIME ======*/

void printCurrentTime(int argc, char **args){
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

void handleGetPid(int argc, char **args){
    NewLine();
    char auxBuffer[20];
    itoaBase((uint64_t)getpid(), auxBuffer, 10);
    printf(auxBuffer);
    NewLine();
}

static int getPIDFromBuffer(char * buffer) {
    //char * buffer = args[0];
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
    for (int i = 0; i < psList->cantProcess; i++){
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

void handleBlock(int argc, char **args){
    char * buffer = args[0];

    //while (*buffer != ' ' && *buffer != '\0') buffer++;
    if (*buffer == '\0') {
        printf("Faltan parametros");
        NewLine();
        printf("Ejemplo de llamada: BLOCK (PID)");
        NewLine();
        NewLine();
        return;
    }
    //buffer++;

    processesList * psList = getProcesses();
    int checkPID = atoi(buffer);/*getPIDFromBuffer(buffer);*/

    if (checkPID < 0) {
        printf("Formato invalido de PID");
        NewLine();
        printf("Ejemplo de llamada: BLOCK (PID)");
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

    int status = (int)psList->Status[psListIndex];
    Pid pid = (Pid)checkPID;
    if (status != 0 && status != 2){
        printf("El proceso elegido no esta BLOCKED ni READY");
    } else if(status == 0){
        blockProcess(pid);
        printf("Process ");
        setX(8);
        printf(psList->names[psListIndex]);
        NewLine();
        printf("Cambio de estado a BLOCKED exitosamente!");
    } else if(status == 2){
        unblockProcess(pid);
        printf("Process ");
        setX(8);
        printf(psList->names[psListIndex]);
        NewLine();
        printf("Cambio de estado a READY exitosamente!");
    }
    freeMemoryUser(psList);
    NewLine();
    NewLine();
}

void handleNice(int argc, char **args){
    char* priorityList[5] = {"LOWEST", "LOW", "AVERAGE", "HIGH", "HIGHEST"};
    char * buffer = args[0];

    //while (*buffer != ' ' && *buffer != '\0') buffer++;
    if (*buffer == '\0') {
        printf("Faltan parametros");
        NewLine();
        printf("Ejemplo de llamada: NICE (PID) (PRIORIDAD)");
        NewLine();
        NewLine();
        return;
    }
    //buffer++;  

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

    buffer = args[1];
    //while (*buffer != ' ' && *buffer != '\0') buffer++;
    //if (*buffer + 1 == '\0') {
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
    //buffer++;  

 

    int checkPriority = getPriorityFromBuffer(args[1]);
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
    printf("La prioridad fue cambiada exitosamente!");
    NewLine();
    NewLine();
    printf("New priority: ");
    setX(14);
    printf(priorityList[priority]);
    NewLine();
    NewLine();
    freeMemoryUser(psList);
}

void handlePrintMemState(int argc, char **args){
    memoryState * memState = getMemoryState();
    char aux[20];
    NewLine();
    printf("Memory state:");
    NewLine();
    NewLine();
    printf("Total size:");
    NewLine();
    itoaBase(memState->total, aux, 10); 
    printf(aux);
    NewLine();
    printf("Used memory:");
    NewLine();
    itoaBase(memState->reserved, aux, 10);
    printf(aux);
    NewLine();
    printf("Free memory:");
    NewLine();
    itoaBase(memState->free, aux, 10);
    printf(aux);
    NewLine();
    NewLine();
    printf("-----------------------------");
    freeMemoryUser(memState);
    NewLine();
    /*TESTING*/
    char auxbuf[20];
    itoaBase(getWriteFD(getpid()),auxbuf,10 );
    printf(auxbuf);
    NewLine();
    /*END TESTING*/
}

void printProcesses(int argc, char **args){
    
    processesList * pr = getProcesses();
    char* state[4] = {"READY", "RUNNING", "BLOCKED", "KILLED"};
    char* priority[5] = {"LOWEST", "LOW", "AVERAGE", "HIGH", "HIGHEST"};

    NewLine();
    printf(" PROCESS        PID PPID STATUS  PRIO     CHILDS RSP      RBP   ");
    NewLine();
    for(int i = 0; i < pr->cantProcess; i++){
        
        char auxBuffer[10];

        //printf("");
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
        itoaBase((uint64_t)pr->childrens[i], auxBuffer, 10);
        printf(auxBuffer);
        setX(49);
        itoaBase((uint64_t)pr->rspList[i], auxBuffer, 16);
        printf(auxBuffer);
        setX(58);
        itoaBase((uint64_t)pr->rbpList[i], auxBuffer, 16);
        printf(auxBuffer);
        //printf("");
    }
    // NewLine();
    // char aux[10];
    // itoaBase(getWriteFD(getpid()), aux, 10);
    // printf(aux);
    // NewLine();
    // if(getWriteFD(getpid()) == PROCESS_PIPE_ID){
    //     NewLine();
    //     printf("HOOOOOOOOOO");
    //     printCurrentTime(argc, args);
    //     closePipeUser(PROCESS_PIPE_ID, 0);
    // }


    //closePipeUser(PROCESS_PIPE_ID, 0);
    
    freeMemoryUser(pr);
    NewLine();
    NewLine();
    // /*TESTING*/    
    // char auxbuf[20];
    // itoaBase(getWriteFD(getpid()),auxbuf,10 );
    // printf(auxbuf);
    // NewLine();
    // /*END TESTING*/
}

static void loopFuction(int argc, char *argv[]){
    int lastPrinted = -1;
    int secs = satoi(argv[0]);
    while (1) {
        int current = seconds_elapsed();
        if (current != lastPrinted && current % secs == 0) {
            printf("Hello User!! I'm still here :)");
            NewLine();
            lastPrinted = current;
        }
    }
    return;
}


void handleLoop(int argc, char **args) {
    if (argc < 1 || args[0] == NULL) {
        printf("Faltan parámetros");
        NewLine();
        printf("Ejemplo de llamada: LOOP (SEGUNDOS)");
        NewLine();
        return;
    }

    int seconds = satoi(args[0]);
    if (seconds <= 0) {
        printf("El parámetro debe ser un número mayor que 0");
        NewLine();
        return;
    }

    int16_t fds[2] = {0, 1};

    char * argv[] = { args[0], NULL };

    createNewProcess("Loop Process", (mainFunc)loopFuction, argv, AVERAGE_PRIORITY, fds);
}

void handleKill(int argc, char **args){
    char * buffer = args[0];

    
    if (*buffer == '\0') {
        //printf("Faltan parametros");
        NewLine();
        //printf("Ejemplo de llamada: KILL (PID)");
        NewLine();
        NewLine();
        return;
    }
    //buffer++;  

    processesList * psList = getProcesses();
    if(psList == NULL){
        printf("NULL");
        NewLine();
        return;
    }
    
    NewLine();
    int checkPID = atoi(buffer);
    

    if (checkPID < 0) {
        printf("Formato invalido de PID");
        NewLine();
        printf("Ejemplo de llamada: KILL (PID)");
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
    Pid pid = (Pid)checkPID;
    killProcess(pid);

    // Mostrar resultado
    printf("Process ");
    setX(8);
    printf(psList->names[psListIndex]);
    NewLine();
    printf("Fue eliminado exitosamente!");
    NewLine();
    NewLine();
    freeMemoryUser(psList);
    return;
}


/*====== INPUT CONTROLLER ======*/
//MANEJAR CON PIPES

void handleCat(int argc, char **args){
    //char c;
    // printProcesses(argc, args);
    // printf("IN CAT");

    // char buffer[1024] = {0};
    // int fd = getReadFD(getpid());

    char buffer[128] = {0};
    int fd = getReadFD(getpid());

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int result = readPipeUser(fd, buffer);  // Read a chunk
        if (result <= 0 || buffer[0] == 0) break;

        printf(buffer);
        NewLine();
    }
        
    // while ((c = getChar()) != 0 && c != '\n' && c != 13) {
    //     if (c == 8) { 
    //         prevX(1);
    //         charDelete(getCurrentX(), getCurrentY(), getFontSize());
    //     } else {
    //         char buf[2] = {c, 0};
    //         printProcesses(argc, args);
    // printf("IN CAT");
    //         printf(buf);
            
    //         nextX(1);
    //     }
    // }
    NewLine();
    /*TESTING*/
    // char auxbuf[20];
    // itoaBase(getReadFD(getpid()),auxbuf,10 );
    // printf(auxbuf);
    // NewLine();
    /*END TESTING*/
    
    return;
}

void handleWC(int argc, char **args){

    char c;
    int lines = 0;
    while ((c = getChar()) != 0 && c != 4) {
        if (c == '\n' || c == 13) {
            lines++;
        }
    }
    char buffer[10];
    itoaBase(lines, buffer, 10);
    printf("Cantidad de lineas: ");
    nextX(20);
    printf(buffer);
    NewLine();
    return;
    // /*TESTING*/
    // char auxbuf[20];
    // itoaBase(getReadFD(getpid()),auxbuf,10 );
    // printf(auxbuf);
    // NewLine();
    // /*END TESTING*/
}

int isVowel(char c) {
    c |= 32; 
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

void handleFilter(int argc, char **args){
    // if(argc != 1){
    //     return;
    // }

    // int16_t fd[2] = {0,1};
    // int pipeID = createPipeUser(PROCESS_PIPE_ID, fd);

    // if (pipeID <= 2) {
    //         printf("Error creando pipe");
    //         NewLine(); 
    //         NewLine();
    //         return;
    // }

    // int16_t filterFD[2] = {0, PROCESS_PIPE_ID};
    // char * argv[] = {0};
    // int leftIdx = satoi(args[0]);
    // Command command = getCommand(leftIdx);
    // Pid rightPID = getpid();
    // Pid pid = createNewProcess(command.name, command.func, argv, AVERAGE_PRIO, filterFD);
    char c;

    // printProcesses(argc, args);
    //     NewLine();

    while ((c = getChar()) != 0 && c != EOF) {
        // printProcesses(argc, args);
        // NewLine();
        if (!isVowel(c)) {
            char buf[2] = {c, 0};
            printf(buf);
            nextX(1);
        }
    }   
    NewLine();
    
    //closePipeUser(PROCESS_PIPE_ID, 1);
    //waitPID(pid);
    
    //destroyPipeUser(PROCESS_PIPE_ID);
    printf("sali del filter");
    NewLine();
    
    //exitProcess(rightPID);
    return;
}

/*====== PHYLO ======*/

void handlePhylo(int argc, char **args){
    int16_t fds[2] = {0,1};
    char * argv[] = {NULL};
    createNewProcess("Phylo Process",(mainFunc)startPhylo, argv, HIGHEST_PRIO,fds);
    return;
}

/*====== TESTS ======*/

void handleMemoryManagerTest(int argc, char **args){
    int16_t fds[2] = {0,1};
    char * argv[] = {"512", 0};
    createNewProcess("Memory Test",(mainFunc)test_mm, argv, HIGHEST_PRIO,fds);
    return;
}

void handleProcessTest(int argc, char **args){
    int16_t fds[2] = {0,1};
    char *argv[] = {"2", 0};
    createNewProcess("Process Test",test_processes, argv, HIGH_PRIO,fds);
    return;
}

void handlePriorityTest(int argc, char **args){
    int16_t fds[2] = {0,1};
    char *argv[] = {NULL};
    createNewProcess("Priority Test",test_prio, argv, HIGH_PRIO,fds);
    return;
}

void handleSyncroTest(int argc, char **args){
    int16_t fds[2] = {0,1};
    char *argv[] = {"10", "1", 0};
    createNewProcess("Syncro Test",test_sync, argv, HIGH_PRIO,fds);
    // waitPID(pid);
    return;
}

void handleNoSyncroTest(int argc, char **args){
    int16_t fds[2] = {0,1};
    char *argv[] = {"10", "0", 0};
    createNewProcess("No Sycnro Test",test_sync, argv, HIGH_PRIO,fds);
    return;
}

uint64_t pipeReaderTest(uint64_t argc, char *argv[]){
    if (argc < 1)
        return -1;

    int n = satoi(argv[0]);
    char ch;

    for (int i = 0; i < n * 2; i++) {
        if (readPipeUser(getReadFD(getpid()), &ch) > 0) {
            char str[2] = {ch, 0};
            printf(str);
        }
    }
    return 0;
}

uint64_t pipeWriterTest(uint64_t argc, char *argv[]){
    if (argc < 1)
    return -1;

    int n = satoi(argv[0]);
    char buffer[64];
    itoaBase(getpid(), buffer, 10);

    for (int i = 0; i < n; i++) {
        writePipeUser(getWriteFD(getpid()), buffer);
        writePipeUser(getWriteFD(getpid()), "\n"); // para separar
        yield(); // Para ceder tiempo al reader
    }

    return 0;
}

void handlePipeTest(int argc, char **args){
    int16_t fds[2] = {0, 1};    
    char *argv[] = {"10", "0", 0};
    int64_t pipe = createPipeUser(25, fds);
    int16_t readerFd[2] = {25,1};
    int16_t writerFd[2] = {0,25};
    Pid pid1 = createNewProcess("Pipe reader test",pipeReaderTest, argv, HIGH_PRIO, readerFd);
    Pid pid2 = createNewProcess("Pipe writer test",pipeWriterTest, argv, HIGH_PRIO, writerFd);
    waitPID(pid1);
    waitPID(pid2);
    destroyPipeUser(pipe);
    return;
}

/*====== EXCEPTIONS TRIGGER ======*/

void zeroDivisionTrigger(int argc, char **args){
    throw_zero_division(argc, args);
}

void invalidOpcodeTrigger(int argc, char **args){
    throw_invalid_opcode(argc, args);
}

int32_t executeUser(Command command, char *args[], int16_t fds[]){
    int argc = countArguments((void*)args);
    if(command.isProcess){
        //printProcesses(argc, args);
        return createNewProcess(command.name, command.func, args, AVERAGE_PRIORITY, fds);
        
    }
    else{
        command.func(argc, args);
        return -1;
    }
}
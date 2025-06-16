#include "../include/phylo.h"

#define MIN_PHYLO 3
#define STARTING_PHYLO 5
#define MAX_PHYLO 10
#define THINKING_TIME 50000000
#define EATING_TIME 50000000
#define SEM_OFFSET 5
#define MUTEX_SEM 16
#define COUNT_MUTEX 17

#define HUNGRY 0
#define EATING 1
#define THINKING 2

static void startDining();
static void endDining();
static void addPhylo();
static void removePhylo();
static void phyloInitializationScreen();
static void eat(int phyloID);
static void think(int phyloID);
static void printState();
static void takeForks(int phyloID);
static void putForks(int phyloID);
static void test();

static Pid phylosophers[MAX_PHYLO];
static int phyloStatus[MAX_PHYLO];
static int phyloSems[MAX_PHYLO];
static int phylosEating;

int phyloQueue[MAX_PHYLO];
int waitingToEatPhylos;

void startPhylo(int argc, char **args){
    char charAux = 0;
    phyloInitializationScreen();

    while(charAux == 0){
        charAux = getChar();
        if (charAux == 9){
            phyloInitializationScreen();
        }
        if(charAux != 32 && charAux != 83){ // 32: space, 83: 'S'
            charAux = 0;
        }
    }

    if (charAux == 83){ // 'S'
        smallerFontSize();
        smallerFontSize();
        ClearScreen(0x000000FF);
        return;
    }
    ClearScreen(0x00000000);
    semInit(MUTEX_SEM, 1);
    semInit(COUNT_MUTEX, 1);
    startDining();

    while((charAux = getKeyDown()) != 83){ // While not 'S'
        switch (charAux){
        case 'A':
            addPhylo();
            break;
        case 'R':
            removePhylo();
            break;      
        default:
            printState();
            break;
        }
    }

    endDining();
    semDestroy(COUNT_MUTEX);
    semDestroy(MUTEX_SEM);
    smallerFontSize();
    ClearScreen(0x000000FF);
    return;
}

void phylo(int argc, char **args){
    int id = satoi(args[0]);
    while (1){
        think(id);
        takeForks(id);
        eat(id);
        putForks(id);
    }
    return;
}

static void addPhylo(){
    semWait(COUNT_MUTEX);
    if (phylosEating >= MAX_PHYLO){
        semPost(COUNT_MUTEX);
        return;
    }
    char *args[] = {NULL};
    int16_t fd[] = {0, 1};
    phylosophers[phylosEating] = createNewProcess("Phylosofer", phylo, args, LOW_PRIO, fd);
    phyloStatus[phylosEating] = THINKING;
    phyloSems[phylosEating] = semInit(phylosEating + SEM_OFFSET, 1);
    phylosEating++;
    printState();
    semPost(COUNT_MUTEX);
    return;
}

static void removePhylo(){
    semWait(COUNT_MUTEX);
    if (phylosEating <= MIN_PHYLO){
        semPost(COUNT_MUTEX);
        return;
    }
    phylosEating--;
    semDestroy(phylosEating + SEM_OFFSET);
    phyloSems[phylosEating] = 0;
    phyloStatus[phylosEating] = THINKING;
    killProcess(phylosophers[phylosEating]);
    printState();
    semPost(COUNT_MUTEX);
    return;
}

static void startDining(){
    phylosEating = 0;
    for (int i = 0; i < STARTING_PHYLO; i++){
        addPhylo();
    }
}

static void endDining(){
    for (int i = 0; i < phylosEating; i++){
        phyloStatus[i] = HUNGRY;
        semDestroy(phylosEating +SEM_OFFSET);
        phyloSems[phylosEating] = 0;
        killProcess(phylosophers[i]);
        phylosophers[i] = -1;
    }
    phylosEating = 0;
    NewLine();
    printf("DINING SESSION ENDED");
    NewLine();
}

static void think(int phyloID){
    bussy_wait(THINKING_TIME);
    phyloStatus[phyloID] = HUNGRY;
}

static void takeForks(int phyloID){
    semWait(MUTEX_SEM);
    test(phyloID);
    if(phyloStatus[phyloID] == EATING){
        phyloQueue[waitingToEatPhylos--];
        semPost(MUTEX_SEM);
    } else {
        semPost(MUTEX_SEM);
        semWait(phyloID + SEM_OFFSET);
    }
}

static void eat(int phyloID){
    bussy_wait(50000000);
}

static void putForks(int phyloID){
    semWait(MUTEX_SEM);
    phyloStatus[phyloID] = THINKING;
    //Test left
    test((phyloID + phylosEating - phyloID) % phylosEating);
    //Test right
    test((phyloID + 1) % phylosEating);
    semPost(MUTEX_SEM);
}

static void test(int phyloID){
    if (phyloStatus[phyloID] == HUNGRY && phyloStatus[(phyloID + phylosEating - phyloID) % phylosEating] != EATING && phyloStatus[(phyloID + 1) % phylosEating] != EATING){
        phyloStatus[phyloID] = EATING;
        semPost(phyloID);
    }
}

static void phyloInitializationScreen(){
    largerFontSize();
    largerFontSize();
    ClearScreen(0x00000000);
    NewLine();
    printf("PHYLOSOPHER'S DINING PROBLEM");
    NewLine();
    NewLine();
    printf("TO ADD PHYLOSOPHER PRESS: A");
    NewLine();
    printf("TO REMOVE PHYLOSOPHER PRESS: R");
    NewLine();
    printf("TO GO BACK TO SHELL PRESS: S");
    NewLine();
    printf("TO START PRESS THE SPACEBAR");
    NewLine();
    NewLine();
    printf("WARNING!");
    NewLine();
    printf("PRESSING TAB for viewing registers");
    NewLine();
    printf("CAUSES AN AUTOMATIC GAME OVER");
}

static void printState(){
    NewLine();
    printf("Current Phylosopher States:");
    NewLine();
    char auxBuf[20];
    for (int i = 0; i < phylosEating; i++){
        itoaBase(phylosEating, auxBuf, 10);
        if(phyloStatus[i] == EATING){
            printf(auxBuf);
            NewLine();
            printf("E ");
        } else {
            printf(auxBuf);
            NewLine();
            printf(". ");
        }
    }
    NewLine();
}

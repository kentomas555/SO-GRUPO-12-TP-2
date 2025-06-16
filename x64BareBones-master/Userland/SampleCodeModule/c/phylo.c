#include "../include/phylo.h"

#define MIN_PHYLO 3
#define STARTING_PHYLO 5
#define MAX_PHYLO 10
#define THINKING_TIME 50000000
#define SEM_OFFSET 5
#define MUTEX_SEM 16
#define COUNT_MUTEX 17

#define EATING 1
#define WAITING 0

static void startDining();
static void endDining();
static void addPhylo();
static void removePhylo();
static void phyloInitializationScreen();
static void eat(int phyloID);
static void think(int phyloID);
static void printState();

static Pid phylosofers[MAX_PHYLO];
static int phyloStatus[MAX_PHYLO];
static int phyloSems[MAX_PHYLO];
static int phylosEating;

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
        eat(id);
    }
    return;
}

static void addPhylo(){
    if (phylosEating >= MAX_PHYLO)
        return;
    char *args[] = {NULL};
    int16_t fd[] = {0, 1};
    phylosofers[phylosEating] = createNewProcess("Phylosofer", phylo, args, LOW_PRIO, fd);
    phyloStatus[phylosEating] = WAITING;
    phyloSems[phylosEating] = semInit(phylosEating + SEM_OFFSET, 1);
    phylosEating++;
    printState();
}

static void removePhylo(){
    if (phylosEating <= MIN_PHYLO)
        return;
    phylosEating--;
    killProcess(phylosofers[phylosEating]);
    printState();
}

static void startDining(){
    phylosEating = 0;
    for (int i = 0; i < STARTING_PHYLO; i++){
        addPhylo();
    }
}

static void endDining(){
    for (int i = 0; i < phylosEating; i++){
        phyloStatus[i] = WAITING;
        semDestroy(phylosEating +SEM_OFFSET);
        phyloSems[phylosEating] = 0;
        killProcess(phylosofers[i]);
        phylosofers[i] = -1;
    }
    phylosEating = 0;
    NewLine();
    printf("DINING SESSION ENDED");
    NewLine();
}

static void eat(int phyloID){
    phyloStatus[phyloID] = EATING;
    bussy_wait(THINKING_TIME);
    phyloStatus[phyloID] = WAITING;
    printState();
}

static void think(int phyloID){
    bussy_wait(THINKING_TIME);
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

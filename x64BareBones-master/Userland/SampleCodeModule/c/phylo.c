// #include "../include/phylo.h"

// #define MIN_PHYLO 3
// #define STARTING_PHYLO 5
// #define MAX_PHYLO 10
// #define THINKING_TIME 50000000
// #define EATING_TIME

// #define EATING 1
// #define WAITING 0

// static void startDining();
// static void endDining();
// static void addPhylo();
// static void removePhylo();
// static void phyloInitializationScreen();
// static void eat(int phyloID);
// static void think(int phyloID);
// // static void takeForks();
// // static void putForks();
// static void printState();

// Pid phylosofers[MAX_PHYLO];
// int phyloStatus[MAX_PHYLO];
// int phylosEating;

// void startPhylo(int argc, char **args){
//     char charAux = 0;
//     phyloInitializationScreen();
//     while(charAux != 'S' || charAux != ' '){
//         charAux = getChar();
//     }

//     if (charAux == 'S'){
//       smallerFontSize();
//       smallerFontSize();
//       ClearScreen(0x000000FF);
//       return;
//     }

//     startDining();
//     while((charAux = getChar()) != 'S'){
//       switch (charAux){
//       case 'A':
//         addPhylo();
//         break;
//       case 'R':
//         removePhylo();
//         break;      
//       default:
//         printf("******Invalid Key******");
//         NewLine();
//         NewLine();        
//         printf("A to add phylosofer");
//         NewLine();        
//         printf("R to remove phylosofer");
//         NewLine();        
//         printf("S to exit");
//         NewLine(); 
//         NewLine(); 
//         break;
//       }
//     }
//     endDining();

//     ClearScreen(0x00000000);
//     smallerFontSize();
//     smallerFontSize();
//     return;
// }

// void phylo(int argc, char **args){
//     //Semaphore
//     while (1){
//         think(getpid());
//         takeForks();
//         eat(getpid());
//         putForks();
//     }
//     return;
// }

// static void addPhylo(){
//   return;
// }

// static void removePhylo(){
//   return;
// }

// static void startDining(){
//   phylosEating = STARTING_PHYLO;
//   //Create semaphores

//   //Create phylo processes
//   return;
// }

// static void endDining(){
//   //Destroy semaphores
//   //Print some result
//   //Kill phylo processes
//   return;
// }

// static void eat(int phyloID){
//   phyloStatus[phyloID] = EATING;
//   return;
// }

// static void think(int phyloID){
//   bussy_wait(THINKING_TIME);
//   return;
// }

// static void phyloInitializationScreen(){
//     largerFontSize();
//     largerFontSize();
//     ClearScreen(0x00000000);
//     NewLine();
//     printf("PHYLOSOPHER'S DINNING PROBLEM");
//     NewLine();
//     NewLine();
//     printf("TO ADD PHYLOSOPHER PRESS: A");
//     NewLine();
//     printf("TO REMOVE PHYLOSOPHER PRESS: R");
//     NewLine();
//     printf("TO GO BACK TO SHELL PRESS: S");
//     NewLine();
//     printf("TO START PRESS THE SPACEBAR");
//     NewLine();
//     NewLine();
//     printf("WARNING!");
//     NewLine();
//     printf("PRESSING TAB for viewing registers");
//     NewLine();
//     printf("CAUSES AN AUTOMATIC GAME OVER");
// }

// static void printState(){
//   int pos = 5;
//   for (int i = 0; i < phylosEating; i++){
//     if(phyloStatus[i] == EATING){
//       printf("E ");
//     } else {
//       printf(". ");
//     }
//     pos++;
//     nextX(pos++);
//   }
//   NewLine();
//   return;
// }
#include <stdarg.h>
#include <stdint.h>
#include <globalLib.h>

void startGame(int argc, char **args);

void gameEngine();
void gameEngine2P();
void initializationScreen();
void clearGrid();
void initializeSnakeDisplay();
void resetFruit();
void loadp1();
void loadp2();
void refreshScreen();
void checkIfOver();
void checkIfOverP2();
void initializeGameOverScreen();
void interpretInput1();
void interpretInput2();
void resetInputs();
void gameOverMenu();
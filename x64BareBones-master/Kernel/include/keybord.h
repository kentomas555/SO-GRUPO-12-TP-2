#include <stdint.h>

char isKeyAvailable();
void resetKeyAvailable();
char getKeyBuffer();
void resetKeyBuffer();

uint8_t getKey();
char getChar(uint8_t Key);
char* getCharName(uint8_t Key);
char getCharASCII(uint8_t Key);
void shiftPressed();
void shiftNotPressed();

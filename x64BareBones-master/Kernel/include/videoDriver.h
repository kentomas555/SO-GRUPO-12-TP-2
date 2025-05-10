#include <stdint.h>

void putPixel(uint32_t hexColor, uint64_t x, uint64_t y);
void initializeDisplay(uint32_t bgColor);
void initializeKernelDisplay(uint32_t bgColor, int kernelDim[4]);

void printChar(char c, int x, int y);
void printMediumChar(char c, int x, int y);
void printBigChar(char c, int x, int y);

void nativePrintf(char * string, int x, int y);
void nativeBigPrintf(char * string, int x, int y);
void nativeMediumPrintf(char * string, int x, int y);

void drawRectangle(int x, int y, int width, int height, uint32_t hexColor);
void drawCircle(int topleft_x, int topleft_y, int radius, uint32_t hexColor);
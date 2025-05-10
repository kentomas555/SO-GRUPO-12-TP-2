#include <videoDriver.h>
#include <font8x8_basic.h>
#include <font12x8.h>
#include <font16x16.h>

struct vbe_mode_info_structure {
	uint16_t attributes;		// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
	uint8_t window_a;			// deprecated
	uint8_t window_b;			// deprecated
	uint16_t granularity;		// deprecated; used while calculating bank numbers
	uint16_t window_size;
	uint16_t segment_a;
	uint16_t segment_b;
	uint32_t win_func_ptr;		// deprecated; used to switch banks from protected mode without returning to real mode
	uint16_t pitch;			// number of bytes per horizontal line
	uint16_t width;			// width in pixels
	uint16_t height;			// height in pixels
	uint8_t w_char;			// unused...
	uint8_t y_char;			// ...
	uint8_t planes;
	uint8_t bpp;			// bits per pixel in this mode
	uint8_t banks;			// deprecated; total number of banks in this mode
	uint8_t memory_model;
	uint8_t bank_size;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
	uint8_t image_pages;
	uint8_t reserved0;
 
	uint8_t red_mask;
	uint8_t red_position;
	uint8_t green_mask;
	uint8_t green_position;
	uint8_t blue_mask;
	uint8_t blue_position;
	uint8_t reserved_mask;
	uint8_t reserved_position;
	uint8_t direct_color_attributes;
 
	uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
	uint32_t off_screen_mem_off;
	uint16_t off_screen_mem_size;	// size of memory in the framebuffer but not being displayed on the screen
	uint8_t reserved1[206];
} __attribute__ ((packed));

typedef struct vbe_mode_info_structure * VBEInfoPtr;

VBEInfoPtr VBE_mode_info = (VBEInfoPtr) 0x0000000000005C00;


void putPixel(uint32_t hexColor, uint64_t x, uint64_t y) {
    uint8_t * framebuffer = (uint8_t *)(uintptr_t) VBE_mode_info->framebuffer;
    uint64_t offset = (x * ((VBE_mode_info->bpp)/8)) + (y * VBE_mode_info->pitch);
    framebuffer[offset]     =  (hexColor) & 0xFF;
    framebuffer[offset+1]   =  (hexColor >> 8) & 0xFF; 
    framebuffer[offset+2]   =  (hexColor >> 16) & 0xFF;
}

//SHAPE-DRAWING FUNCTIONS:

void drawRectangle(int x, int y, int width, int height, uint32_t hexColor){
  for (int j = 0; j <= height; j++){
    int x_local = x;
    for (int i = 0; i <= width; i++){
      putPixel(hexColor, x_local+i, y+j);
    }
  }
}

void drawCircle(int topleft_x, int topleft_y, int radius, uint32_t hexColor) {
    int diameter = radius * 2;
    for (int j = 0; j <= diameter; j++) {
        int x_local = topleft_x;
        for (int i = 0; i <= diameter; i++) {
            // Calculate the distance from the center (radius, radius)
            int distanceSquared = (i - radius) * (i - radius) + (j - radius) * (j - radius);
            if (distanceSquared <= radius * radius) {
                putPixel(hexColor, x_local+i, topleft_y+j);
            } 
        }
    }
}

//DISPLAY INITIALIZATION: 

void initializeDisplay(uint32_t bgColor){
	for (int i = 0; i < VBE_mode_info->height ; i++){ //768
        for(int j = 0; j < VBE_mode_info->width; j++){ //1024
                putPixel(bgColor, j, i);
        }
    }
	
}

void initializeKernelDisplay(uint32_t bgColor, int kernelDim[4]){
	drawRectangle(70, 90, VBE_mode_info->width - 70 - 60, VBE_mode_info->height - 90 - 60, 0x00707070);
	drawRectangle(86, 78, VBE_mode_info->width - 70 - 60, VBE_mode_info->height - 90 - 60, 0x00FFFFFF);
	drawRectangle(90, 82, VBE_mode_info->width - 70 - 60 -4 -4, VBE_mode_info->height - 90 - 60 -4 -4, bgColor);
	kernelDim[0] = VBE_mode_info->width - 70 - 60 -4 -4;
	kernelDim[1] = VBE_mode_info->height - 90 - 60 -4 -4;
	kernelDim[2] = 90 + 10;
	kernelDim[3] = 82 + 10;
}

void getKernelDim(int kernelDim[4]){
	kernelDim[0] = VBE_mode_info->width - 70 - 60 -4 -4;
	kernelDim[1] = VBE_mode_info->height - 90 - 60 -4 -4;
	kernelDim[2] = 90 + 10;
	kernelDim[3] = 82 + 10;
}


//PRINTING FUNCTIONS:

void printChar(char c, int x, int y){
	int x_aux = x;
	for (int j = 0; j <= 7; j++){ 
		int local_x = x_aux;
		for (int i = 0; i <= 7; i++) {
			unsigned char bit = (font8x8_basic[(int)c][j] >> i) & 1;
			if (bit != 0){
	        	putPixel(0x00FFFFFF, local_x, y);
			}
			local_x++;
    	}
		y++;
	}
}

void printMediumChar(char c, int x, int y){
	if (c < 32 || c > 126){
		return;
	}
	int offset = (((int)c) - 32) * 12;
	int x_aux = x;
    for (int j = 0; j <= 11; j++){ 
        int local_x = x_aux;
        for (int i = 7; i >= 0; i--) {
            unsigned char bit = (font12x8[j+offset] >> i) & 1;
            if (bit != 0){
              putPixel(0x00FFFFFF, local_x, y);
            }
            local_x++;
    	}
        y++;
    }
}

void printBigChar(char c, int x, int y){
	if (c < 32 || c > 126){
		return;
	}
    int offset = (((int)c) - 32) * 32;
	int local_x = x;
    for (int j = 0; j < 32; j++) { 
        if (j % 2 == 0 && j != 0) {
            local_x = x; 
            y++; 
        }
        for (int i = 7; i >= 0; i--) {
			unsigned char bit = (font16x16[j+offset] >> i) & 1;
			if (bit != 0){
           		putPixel(0x00FFFFFF, local_x, y);
            } 
            local_x++; 
        }
    }
}

void nativePrintf(char * string, int x, int y){
	int x_aux = x;
	while(*string != 0){
		printChar(*string, x, y);
		if (*string == 10){
			y += 8;
			x = x_aux;
		} else {
			x += 8;
		}
		string++;
	}
}

void nativeBigPrintf(char * string, int x, int y){
	int x_aux = x;
	while(*string != 0){
		if( *string >= 32 && *string <=  126){
			printBigChar(*string, x, y);
		}
		if (*string == 10){
			y += 16;
			x = x_aux;
		} else {
			x += 16;
		}
		string++;
	}
}

void nativeMediumPrintf(char * string, int x, int y){
	int x_aux = x;
	while(*string != 0){
		printMediumChar(*string, x, y);
		if (*string == 10){
			y += 12;
			x = x_aux;
		} else {
			x += 8;
		}
		string++;
	}
}

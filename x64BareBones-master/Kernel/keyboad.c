#include <keybord.h>


typedef struct {
    char *key_name;   // Nombre de la tecla
    int scancode;     // Scancode en hexadecimal
    int ascii;        // Código ASCII (o -1 si no aplica)
} KeyMapping;

KeyMapping key_table[] = {
    {"ESC", 0x01, 27},
    {"F1", 0x3B, -1}, {"F2", 0x3C, -1}, {"F3", 0x3D, -1}, 
    {"F4", 0x3E, -1}, {"F5", 0x3F, -1}, {"F6", 0x40, -1},
    {"F7", 0x41, -1}, {"F8", 0x42, -1}, {"F9", 0x43, -1},
    {"F10", 0x44, -1}, {"F11", 0x57, -1}, {"F12", 0x58, -1},

    {"1", 0x02, 49}, {"2", 0x03, 50}, {"3", 0x04, 51},
    {"4", 0x05, 52}, {"5", 0x06, 53}, {"6", 0x07, 54},
    {"7", 0x08, 55}, {"8", 0x09, 56}, {"9", 0x0A, 57},
    {"0", 0x0B, 48}, {"-", 0x0C, 45}, {"=", 0x0D, 61},
    {"Backspace", 0x0E, 8},

    {"Tab", 0x0F, 9}, 
    {"Q", 0x10, 81}, {"W", 0x11, 87}, {"E", 0x12, 69},
    {"R", 0x13, 82}, {"T", 0x14, 84}, {"Y", 0x15, 89},
    {"U", 0x16, 85}, {"I", 0x17, 73}, {"O", 0x18, 79},
    {"P", 0x19, 80}, {"[", 0x1A, 91}, {"]", 0x1B, 93},
    {"Enter", 0x1C, 13},

    {"A", 0x1E, 65}, {"S", 0x1F, 83}, {"D", 0x20, 68},
    {"F", 0x21, 70}, {"G", 0x22, 71}, {"H", 0x23, 72},
    {"J", 0x24, 74}, {"K", 0x25, 75}, {"L", 0x26, 76},
    {";", 0x27, 59}, {"'", 0x28, 39}, {"`", 0x29, 96},

    {"Shift Izq.", 0x2A, -1}, {"\\", 0x2B, 92},
    {"Z", 0x2C, 90}, {"X", 0x2D, 88}, {"C", 0x2E, 67},
    {"V", 0x2F, 86}, {"B", 0x30, 66}, {"N", 0x31, 78},
    {"M", 0x32, 77}, {",", 0x33, 44}, {".", 0x34, 46},
    {"/", 0x35, 47}, {"Shift Der.", 0x36, -1},

    {"Ctrl Izq.", 0x1D, -1}, {"Alt Izq.", 0x38, -1},
    {"Espacio", 0x39, 32}, {"Alt Der.", 0x38, -1},
    {"Ctrl Der.", 0x1D, -1},

    {"Flecha Arriba", 0x48, -1},
    {"Flecha Izquierda", 0x4B, -1},
    {"Flecha Derecha", 0x4D, -1},
    {"Flecha Abajo", 0x50, -1},

    {"Insert", 0x52, -1}, {"Delete", 0x53, -1},
    {"Inicio", 0x47, -1}, {"Fin", 0x4F, -1},
    {"Re Pág", 0x49, -1}, {"Av Pág", 0x51, -1},

    {"Num Lock", 0x45, -1}, {"Bloq Mayús", 0x3A, -1}
};



char getCharASCII(uint8_t Key){
         for (int i = 0; i < sizeof(key_table) / sizeof(key_table[0]); i++) {
        if (key_table[i].scancode == Key) {
            return key_table[i].ascii;
        }
    }
    return -1; // Retornar -1 si no se encuentra
}

char* getCharName(uint8_t Key){
         for (int i = 0; i < sizeof(key_table) / sizeof(key_table[0]); i++) {
        if (key_table[i].scancode == Key) {
            return key_table[i].key_name;
        }
    }
    return 0; // Retornar -1 si no se encuentra
}
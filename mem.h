#ifndef MEM_H
#define MEM_H

#include "types.h"

#define MEM_MAP 64*1024

#define MAX_ROM_S 2*1024*1024
#define MAX_EXT_RAM_S 128*1024

#define BIOS_S 256
#define VIDEO_RAM_S 8*1024
#define EXT_RAM_S 8*1024
#define INT_RAM_S 8*1024
#define SPRITES_S 160
#define IO_S 76
#define OTHER_RAM_S 128

typedef struct {
    // Attributes
    int cartridge_type;
    int rom_size;
    int ram_size;
    int bios_active;
} MEMState;

int loadRom(char fn[]);
void dissassembleRom(byte rom[]);

void decode(byte *b, int i);

int wb(word address, byte value);
byte rb(word address);

int ww(word address, word value);
byte rw(word address);

void printMemory(byte start, byte end);

#endif

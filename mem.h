#ifndef MEM_H
#define MEM_H

#include "types.h"

// Max ROM size is 2 MBytes
#define MAX_ROM_SIZE 2*1024*1024
#define RAM_SIZE 0xFFFF

typedef struct {
    int cartridge_type;
    int rom_size;
    int bios_mapped;
} MEMState;

extern const MEMState MEM_DEFAULT;

int loadRom(char fn[]);
void dissassembleRom(byte rom[]);

void decode(byte *b, int i);

int wb(word address, byte value);
byte rb(word address);

int ww(word address, word value);
byte rw(word address);

void printMemory(byte start, byte end);

#endif

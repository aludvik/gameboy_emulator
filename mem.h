#ifndef MEM_H
#define MEM_H

// Max ROM size is 2 MBytes
#define MAX_ROM_SIZE 2*1024*1024
#define RAM_SIZE 0xFFFF

typedef unsigned int byte;
typedef unsigned int word;

typedef struct {
    int cartridge_type, 
    int rom_size, 
    int bios_mapped
} MEMState;

extern const MEMState MEM_DEFAULT;

int loadRom(char fn[]);
void dissassembleRom(byte rom[]);

void decode(byte *b, int i);

void wb(word address, byte value);
byte rb(word address);

void ww(word address, word value);
byte rw(word address);

#endif

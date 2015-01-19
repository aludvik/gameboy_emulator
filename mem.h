#ifndef MEM_H
#define MEM_H

#define MAX_ROM_SIZE 0x8000
#define RAM_SIZE 0xFFFF

// *** Repeated typedef in cpu.h ***
typedef unsigned int byte;
typedef unsigned int word;

void loadRom(char fn[], byte rom[]);
void dissassembleRom(byte rom[]);

void decode(byte *b, int i);

void wb(word address, byte value);
byte rb(word address);

#endif

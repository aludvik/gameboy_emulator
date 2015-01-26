#ifndef MEM_H
#define MEM_H

#include "types.h"

#define MEM_MAP             64*1024

#define MAX_ROM_SIZE        2*1024*1024
#define MAX_EXT_RAM_SIZE    128*1024

#define BIOS_SIZE           256
#define SWITCH_ROM_SIZE     16*1024
#define SWITCH_RAM_SIZE     8*1024
#define VIDEO_RAM_SIZE      8*1024
#define EXT_RAM_SIZE        8*1024
#define INT_RAM_SIZE        8*1024
#define SPRITE_OAM_SIZE     160
#define IO_SIZE             76
#define ZERO_RAM_SIZE       128

#define ROM_START           0x0000
#define SWITCH_ROM_START    0x4000
#define VIDEO_RAM_START     0x8000
#define EXT_RAM_START       0xA000
#define INT_RAM_START       0xC000
#define ECHO_START          0xE000
#define SPRITE_OAM_START    0xFE00
#define EMPTY1_START        0xFEA0
#define IO_START            0xFF00
#define EMPTY2_START        0xFF4C
#define ZERO_RAM_START      0xFF80
#define INTERRUPT_REGISTER  0xFFFF

typedef struct {
    int bios_active;
    
    byte cart_type;     // See gameboy docs

    int mem_controller; // 0 - None, 1 - MBC1, 2 - MBC2, 
                        // 3 - MBC3, 5 - MBC5

    int ext_ram;        // 0 - None, 1 - RAM, 2 - SRAM
    int battery;        // 0 - None, 1 - Yes

    byte rom_size;      //  0 - 256Kbit =  32KB =   2 banks
                        //  1 - 512Kbit =  64KB =   4 banks
                        //  2 -   1Mbit = 128KB =   8 banks
                        //  3 -   2Mbit = 256KB =  16 banks
                        //  4 -   4Mbit = 512KB =  32 banks
                        //  5 -   8Mbit =   1MB =  64 banks
                        //  6 -  16Mbit =   2MB = 128 banks
                        // 52 -   9Mbit = 1.1MB =  72 banks
                        // 53 -  10Mbit = 1.2MB =  80 banks
                        // 54 -  12Mbit = 1.5MB =  96 banks

    byte ram_size;      // 0 - None
                        // 1 -  16kBit =   2KB =  1 bank
                        // 2 -  64kBit =   8KB =  1 bank
                        // 3 - 256kBit =  32KB =  4 banks
                        // 4 -   1MBit = 128KB = 16 banks


    byte interrupt;
} MEMState;

typedef struct {
    byte ram_on;
    byte rom_bank;
    byte ram_bank;
    byte mode;
} MBC1State;

int loadRom(char fn[]);
void dissassembleRom(byte rom[]);
void decode(byte *b, int i);

int wb(word address, byte value);
byte rb(word address);

int ww(word address, word value);
byte rw(word address);

void printMemory(byte start, byte end);
void fillZeros(byte b[], int l);

#endif

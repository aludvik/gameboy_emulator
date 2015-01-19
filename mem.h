#define MAX_ROM_SIZE 0x8000
#define RAM_SIZE 0xFFFF

typedef unsigned int byte;

void loadRom(char fn[], byte rom[]);
void dissassembleRom(byte rom[]);

void decode(byte *b, int i);

void setByte(byte address, byte value);
byte getByte(byte address);

#include "stdio.h"

#include "emu.h"

int main(int argc, char *argv) {
    byte rom[MAX_ROM_SIZE];

    loadRom("blue.gb", rom);

    dissassembleRom(rom);

    return 0;
}

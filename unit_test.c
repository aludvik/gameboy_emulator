#include "stdio.h"

#include "emu.h"

void loadAndDissassemble();

int main(int argc, char *argv) {

    State cpu = STATE_DEFAULT;

    cpu.c = 0xFF;

    wb(0x0000, LD_B_C);

    execute(&cpu);

    printf("%#06x\n", cpu.b);

    return 0;
}

void loadAndDissassemble() {
    byte rom[MAX_ROM_SIZE];

    loadRom("blue.gb", rom);

    dissassembleRom(rom);
}

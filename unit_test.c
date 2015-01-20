#include "stdio.h"

#include "emu.h"

void memTest1();
void memTest2();
void cpuTest1();

int main(int argc, char *argv) {

    memTest2();

    return 0;
}

void memTest1() {
    byte rom[MAX_ROM_SIZE];

    loadRom("blue.gb");

    dissassembleRom(rom);
}

void memTest2() {
    char fn[30];

    printf("Enter rom name: ");
    scanf("%s", fn);
    printf("ROM loaded was %d bytes.\n", loadRom(fn));

} 

void cpuTest1() {
    CPUState cpu = CPU_DEFAULT;

    cpu.c = 0xFF;

    wb(0x0000, LD_B_C);

    execute(&cpu);

    printf("%#06x\n", cpu.b);
}

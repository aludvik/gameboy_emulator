#include "stdio.h"

#include "emu.h"

void memTest1();
void memTest2();
void cpuTest1();
void cpuTest2();

void writeBytes(word address, byte b[], int len);

int main(int argc, char *argv) {

    cpuTest2();

    return 0;
}

void writeBytes(word address, byte b[], int len) {
    int i = 0;
    while (i < len) {
        wb(address + i, b[i]);
        i++;
    }
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

// Should print 0x00ff
void cpuTest1() {
    CPUState cpu = CPU_DEFAULT;

    cpu.c = 0xFF;

    wb(0x0000, LD_B_C);

    execute(&cpu);

    printf("%#06x\n", cpu.b);
}

void cpuTest2() {
    CPUState cpu = CPU_DEFAULT;
    byte inst[3] = {0x01, 0x11, 0x22};

    printCPU(&cpu);

    writeBytes(0x0000, inst, 3);
    printMemory(0x0000, 0x0009);

    execute(&cpu);

    printCPU(&cpu);
}

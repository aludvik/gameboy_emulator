#include "stdio.h"

#include "emu.h"

void memTest1();
void cpuTest1();
void cpuTest2();

void writeBytes(word address, byte b[], int len);

int main(int argc, char *argv) {

//    memTest1();
//    cpuTest1();
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
    char fn[30];

    printf("Enter rom name: ");
    scanf("%s", fn);
    printf("ROM loaded was %d bytes.\n", loadRom(fn));

} 

// Should print 0x00ff
void cpuTest1() {

    //cpu.c = 0xFF;

    wb(0x0000, LD_B_C);

    execute();

    printCPU();
}

void cpuTest2() {
    byte inst[4] = {0x01, 0x11, 0x22, LD_B_C};

    printCPU();

    writeBytes(0x0000, inst, 4);
    printMemory(0x0000, 0x0009);

    execute();
    printCPU();

    execute();
    printCPU();
}

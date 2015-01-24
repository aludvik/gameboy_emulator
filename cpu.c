#include "emu.h"

// Endian
void reg_wpair(reg *r1, reg *r2, word value) {
    *r1 = (value & 0xFF00) >> 8;
    *r2 = value & 0x00FF;
}

word reg_rpair(reg *r1, reg *r2) {
    return *r1 + (*r2 << 8);
}

static const CPUState CPU_DEFAULT = {
    0x0000, // pc
    0x0000, // sp
    0x00,   // a
    0x00,   // b
    0x00,   // c
    0x00,   // d
    0x00,   // e
    0x00,   // f
    0x00,   // h
    0x00    // l
};

static CPUState cpu;

// Must be called before CPU functions used
void initCPU() {
    cpu = CPU_DEFAULT;
    // Do anything else that needs to be done...
}

void execute() {
    int op_len = 1,
        cycles = 4;

    // need to add flags...
    switch (rb(cpu.pc)) {
        case NOP:
            break;
        case LD_BC_d16: // Checked
            reg_wpair(&(cpu.b), &(cpu.c), rw((cpu.pc) + 1));
            op_len = 3;
            cycles = 12;
            break;
        case LD_pBC_A:
            wb(reg_rpair(&(cpu.b), &(cpu.c)), cpu.a);
            cycles = 8;
            break;
        case INC_BC:
            cpu.c = (cpu.c + 1) & 0xFF;
            // Check for overflow
            if (!cpu.c) cpu.b = (cpu.b + 1) & 0xFF;
            break;
        case INC_B: 
            cpu.b = (cpu.b + 1) & 0xFF; 
            break;
        case DEC_B: 
            cpu.b = (cpu.b - 1) & 0xFF; 
            break;
        case LD_B_d8:
            cpu.b = rb(cpu.pc + 1);
            break;
        case LD_B_B:
            cpu.b = cpu.b;
            break;
        case LD_B_C:
            cpu.b = cpu.c;
            break;
        case LD_B_D:
            cpu.b = cpu.d;
            break;
        case LD_B_E:
            cpu.b = cpu.e;
            break;
        case LD_B_H:
            cpu.b = cpu.h;
            break;
        case LD_B_L:
            cpu.b = cpu.l;
            break;
        case LD_B_HL:
            cpu.b = rb((cpu.h << 8) + cpu.l);
            break;
        case LD_B_A:
            cpu.b = cpu.a;
            break;
    }

    cpu.pc += op_len;
}

void printCPU() {
    printf("-------------------------------- CPU --\n");
    printf("| pc: %#06x", cpu.pc);
    printf("        sp: %#06x        |\n", cpu.sp);
    printf("|  a: %#04x", cpu.a);
    printf("  b: %#04x", cpu.b);
    printf("  c: %#04x", cpu.c);
    printf("  d: %#04x |\n", cpu.d);
    printf("|  e: %#04x", cpu.e);
    printf("  f: %#04x", cpu.f);
    printf("  h: %#04x", cpu.h);
    printf("  l: %#04x |\n", cpu.l);
    printf("---------------------------------------\n");
}


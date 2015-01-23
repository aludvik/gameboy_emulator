#include "emu.h"

// Endian
void reg_wpair(reg *r1, reg *r2, word value) {
    *r1 = (value & 0xFF00) >> 8;
    *r2 = value & 0x00FF;
}

word reg_rpair(reg *r1, reg *r2) {
    return *r1 + (*r2 << 8);
}

void execute(CPUState *cpu) {
    int op_len = 1,
        cycles = 4;

    switch (rb(cpu->pc)) {
        case NOP: // Untested
            break;
        case LD_BC_d16: // Untested
            reg_wpair(&(cpu->b), &(cpu->c), rw((cpu->pc) + 1));
            op_len = 3;
            cycles = 12;
            break;
        case LD_pBC_A: // Untested
            wb(reg_rpair(&(cpu->b), &(cpu->c)), cpu->a);
            cycles = 8;
            break;
        case LD_B_B: // Untested
            cpu->b = cpu->b;
            break;
        case LD_B_C: // Untested
            cpu->b = cpu->c;
            break;
        case LD_B_D: // Untested
            cpu->b = cpu->d;
            break;
        case LD_B_E: // Untested
            cpu->b = cpu->e;
            break;
        case LD_B_H: // Untested
            cpu->b = cpu->h;
            break;
        case LD_B_L: // Untested
            cpu->b = cpu->l;
            break;
        case LD_B_HL: // Untested
            // Does this work???
            cpu->b = rb(cpu->h * 0x100 + cpu->l); // consider using bit-shifts
            break; // Untested
        case LD_B_A: // Untested
            cpu->b = cpu->a;
            break;
    }

    cpu->pc += op_len;
}

void printCPU(CPUState *cpu) {
    printf("-------------------------------- CPU --\n");
    printf("| pc: %#06x", cpu->pc);
    printf("        sp: %#06x        |\n", cpu->sp);
    printf("|  a: %#04x", cpu->a);
    printf("  b: %#04x", cpu->b);
    printf("  c: %#04x", cpu->c);
    printf("  d: %#04x |\n", cpu->d);
    printf("|  e: %#04x", cpu->e);
    printf("  f: %#04x", cpu->f);
    printf("  h: %#04x", cpu->h);
    printf("  l: %#04x |\n", cpu->l);
    printf("---------------------------------------\n");
}


const CPUState CPU_DEFAULT = {
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

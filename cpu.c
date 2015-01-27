#include "emu.h"

// Endian
void rpw(reg *r1, reg *r2, word value) {
    *r1 = (value & 0xFF00) >> 8;
    *r2 = value & 0x00FF;
}

word rpr(reg r1, reg r2) {
    return r1 + (r2 << 8);
}

void rInc(reg *r) {
    *r = (*r + 1) & 0xFF; 
}

void rDec(reg *r) {
    *r = (*r - 1) & 0xFF; 
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
    byte sum;

    // need to add flags...
    // should they be reset at the beginning of
    // each computation cycle?
    switch (rb(cpu.pc)) {
        case NOP:
            break;
        case LD_BC_d16: // Checked
            rpw(&(cpu.b), &(cpu.c), rw((cpu.pc) + 1));
            op_len = 3;
            cycles = 12;
            break;
        case LD_pBC_A:
            wb(rpr(cpu.b, cpu.c), cpu.a);
            cycles = 8;
            break;
        case INC_BC:
            cpu.c = (cpu.c + 1) & 0xFF;
            // Check for overflow
            if (!cpu.c) cpu.b = (cpu.b + 1) & 0xFF;
            break;
        case INC_B: 
            rInc(&(cpu.b));
            break;
        case DEC_B: 
            rDec(&(cpu.b));
            break;
        case LD_B_d8:
            cpu.b = rb(cpu.pc + 1);
            op_len = 2;
            cycles = 8;
            break;
        case RLCA:
            // TODO: Faster way?
            if ((cpu.a >> 7) & 0x01 == 1) {
                cpu.a = ((cpu.a << 1) & 0xFF) | 1; 
                cpu.f = 1 << 4;
                // cpu.f = (cpu.f | (1 << 4)) & 0xFF;
            } else {
                cpu.a = (cpu.a << 1) & 0xFF;
                cpu.f = 0;
                // cpu.f = (cpu.f & !(1 << 4)) & 0xFF;
            }
            break;
        case LD_pa16_SP:
            ww( rw(cpu.pc + 1), cpu.sp );
            op_len = 3; cycles = 20;
        case ADD_HL_BC:
            sum = rpr(cpu.h, cpu.l) + rpr(cpu.b, cpu.c);
            rpw(&(cpu.h), &(cpu.l), sum);
            cycles = 8;
            break;
        case LD_A_pBC:
            cpu.a = rb(rpr(cpu.b, cpu.c));
            cycles = 8;
            break;
        case DEC_BC:
            // Need to handle overflow
            cycles = 8;
            break;
        case INC_C:
            rInc(&(cpu.c));
            // Z0H-
            break;
        case DEC_C:
            rDec(&(cpu.c));
            // Z1H-
            break;
        case LD_C_d8:
            cpu.c = rb(cpu.pc + 1);
            op_len = 2; cycles = 8;
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


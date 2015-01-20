#include "emu.h"

void execute(CPUState *cpu) {
    int op_len = 1;

    switch (rb(cpu->pc)) {
        case LD_B_B:
            cpu->b = cpu->b;
            break;
        case LD_B_C:
            cpu->b = cpu->c;
            break;
        case LD_B_D:
            cpu->b = cpu->d;
            break;
        case LD_B_E:
            cpu->b = cpu->e;
            break;
        case LD_B_H:
            cpu->b = cpu->h;
            break;
        case LD_B_L:
            cpu->b = cpu->l;
            break;
        case LD_B_HL:
            // Does this work???
            cpu->b = rb(cpu->h * 0x100 + cpu->l);
            break;
        case LD_B_A:
            cpu->b = cpu->a;
            break;
    }

    cpu->pc += op_len;
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

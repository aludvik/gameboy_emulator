#include "emu.h"

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

#include "cpu_helpers.h"

// Must be called before CPU functions used
void initCPU() {
    cpu = CPU_DEFAULT;
    // Do anything else that needs to be done...
}

void execute() {
    CPUUpdate update = {1, 4};
    byte sum;

    // need to add flags...
    // should they be reset at the beginning of
    // each computation cycle?
    switch (rb(cpu.pc)) {

        // --- 0x00 - 0x0F --- //
        case NOP:
            break;
        case LD_BC_d16: // Checked
            update = ld_rr_d16(&cpu.b, &cpu.c);
            break;
        case LD_pBC_A:
            update = ld_prr_A(&cpu.b, &cpu.c);
            break;
        case INC_BC:
            cpu.c = (cpu.c + 1) & 0xFF;
            // Check for overflow
            if (!cpu.c) cpu.b = (cpu.b + 1) & 0xFF;
            break;
        case INC_B: 
            inc_r(&(cpu.b));
            break;
        case DEC_B: 
            dec_r(&(cpu.b));
            break;
        case LD_B_d8:
            update = ld_r_d8(&cpu.b);
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
            break;
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
            inc_r(&(cpu.c));
            // Z0H-
            break;
        case DEC_C:
            dec_r(&(cpu.c));
            // Z1H-
            break;
        case LD_C_d8:
            update = ld_r_d8(&cpu.c);
            break;
        case RRCA:
            if ((cpu.a & 0x01) == 1) {
                cpu.a = ((cpu.a & 0xFF) >> 1) | (1 << 7);
                cpu.f = 1 << 4;
            } else {
                cpu.a = ((cpu.a & 0xFF) >> 1);
                cpu.f = 0; // TODO: Check flags
            }
            break;

        // --- 0x10 - 0x1F --- //
        
        case STOP_0:
            // Stop until button pressed
            break;
        case LD_DE_d16:
            update = ld_rr_d16(&cpu.d, &cpu.e);
            break;
        case LD_pDE_A:
            update = ld_prr_A(&cpu.d, &cpu.e);
            break;
        case INC_DE:
            cpu.e = (cpu.e + 1) & 0xFF;
            // Check for overflow
            if (!cpu.e) cpu.d = (cpu.d + 1) & 0xFF;
            break;
        case INC_D: 
            inc_r(&(cpu.d));
            break;
        case DEC_D: 
            dec_r(&(cpu.d));
            break;
        case LD_D_d8:
            update = ld_r_d8(&cpu.d);
            break;
        case RLA:
            // TODO: NO
            if (cpu.f & 0x10) {
                cpu.f = (cpu.a & 0x80) << 4;
                cpu.a = ((cpu.a & 0xFF) << 1) | 1;
            } else {
                cpu.f = (cpu.a & 0x80) << 4;
                cpu.a = ((cpu.a & 0xFF) << 1) & (!1);
            }
            break;
        case JR_r8:
            // TODO: What doe this do?
            op_len = 2;
            cycles = 12;
            break;
        case ADD_HL_DE:
            sum = rpr(cpu.h, cpu.l) + rpr(cpu.d, cpu.e);
            rpw(&(cpu.h), &(cpu.l), sum);
            cycles = 8;
            break;
        case LD_A_pDE:
            cpu.a = rb(rpr(cpu.d, cpu.e));
            cycles = 8;
            break;
        case DEC_DE:
            // Need to handle overflow
            cycles = 8;
            break;
        case INC_E:
            inc_r(&(cpu.e));
            // Z0H-
            break;
        case DEC_E:
            dec_r(&(cpu.e));
            // Z1H-
            break;
        case LD_E_d8:
            update = ld_r_d8(&cpu.e);
            break;
        case RRA:
            if (cpu.f & 0x10) {
                cpu.f = (cpu.a & 1) << 4;
                cpu.a = ((cpu.a & 0xFF) >> 1) | (1 << 7);
            } else {
                cpu.f = (cpu.a & 1) << 4;
                cpu.a = ((cpu.a & 0xFF) >> 1);
            }
            break;

        // --- 0x40 - 0x4F --- //
        case LD_B_B: ld_r_r(&cpu.b, &cpu.b); break;
        case LD_B_C: ld_r_r(&cpu.b, &cpu.c); break;
        case LD_B_D: ld_r_r(&cpu.b, &cpu.d); break;
        case LD_B_E: ld_r_r(&cpu.b, &cpu.e); break;
        case LD_B_H: ld_r_r(&cpu.b, &cpu.h); break;
        case LD_B_L: ld_r_r(&cpu.b, &cpu.l); break;
        case LD_B_pHL: update = ld_r_prr(&cpu.b, &cpu.h, &cpu.l); break;
        case LD_B_A: ld_r_r(&cpu.b, &cpu.a); break;
        case LD_C_B: ld_r_r(&cpu.c, &cpu.b); break;
        case LD_C_C: ld_r_r(&cpu.c, &cpu.c); break;
        case LD_C_D: ld_r_r(&cpu.c, &cpu.d); break;
        case LD_C_E: ld_r_r(&cpu.c, &cpu.e); break;
        case LD_C_H: ld_r_r(&cpu.c, &cpu.h); break;
        case LD_C_L: ld_r_r(&cpu.c, &cpu.l); break;
        case LD_C_pHL: update = ld_r_prr(&cpu.c, &cpu.h, &cpu.l); break;
        case LD_C_A: ld_r_r(&cpu.c, &cpu.a); break;

        // --- 0x50 - 0x5F --- //
        case LD_D_B: ld_r_r(&cpu.d, &cpu.b); break;
        case LD_D_C: ld_r_r(&cpu.d, &cpu.c); break;
        case LD_D_D: ld_r_r(&cpu.d, &cpu.d); break;
        case LD_D_E: ld_r_r(&cpu.d, &cpu.e); break;
        case LD_D_H: ld_r_r(&cpu.d, &cpu.h); break;
        case LD_D_L: ld_r_r(&cpu.d, &cpu.l); break;
        case LD_D_pHL: update = ld_r_prr(&cpu.d, &cpu.h, &cpu.l); break;
        case LD_D_A: ld_r_r(&cpu.d, &cpu.a); break;
        case LD_E_B: ld_r_r(&cpu.e, &cpu.b); break;
        case LD_E_C: ld_r_r(&cpu.e, &cpu.c); break;
        case LD_E_D: ld_r_r(&cpu.e, &cpu.d); break;
        case LD_E_E: ld_r_r(&cpu.e, &cpu.e); break;
        case LD_E_H: ld_r_r(&cpu.e, &cpu.h); break;
        case LD_E_L: ld_r_r(&cpu.e, &cpu.l); break;
        case LD_E_pHL: update = ld_r_prr(&cpu.e, &cpu.h, &cpu.l); break;
        case LD_E_A: ld_r_r(&cpu.e, &cpu.a); break;

        // --- 0x60 - 0x6F --- //
        case LD_H_B: ld_r_r(&cpu.h, &cpu.b); break;
        case LD_H_C: ld_r_r(&cpu.h, &cpu.c); break;
        case LD_H_D: ld_r_r(&cpu.h, &cpu.d); break;
        case LD_H_E: ld_r_r(&cpu.h, &cpu.e); break;
        case LD_H_H: ld_r_r(&cpu.h, &cpu.h); break;
        case LD_H_L: ld_r_r(&cpu.h, &cpu.l); break;
        case LD_H_pHL: update = ld_r_prr(&cpu.h, &cpu.h, &cpu.l); break;
        case LD_H_A: ld_r_r(&cpu.h, &cpu.a); break;
        case LD_L_B: ld_r_r(&cpu.l, &cpu.b); break;
        case LD_L_C: ld_r_r(&cpu.l, &cpu.c); break;
        case LD_L_D: ld_r_r(&cpu.l, &cpu.d); break;
        case LD_L_E: ld_r_r(&cpu.l, &cpu.e); break;
        case LD_L_H: ld_r_r(&cpu.l, &cpu.h); break;
        case LD_L_L: ld_r_r(&cpu.l, &cpu.l); break;
        case LD_L_pHL: update = ld_r_prr(&cpu.l, &cpu.h, &cpu.l); break;
        case LD_L_A: ld_r_r(&cpu.l, &cpu.a); break;
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


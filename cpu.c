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
        case NOP: break;
        case LD_BC_d16: update = ld_rr_d16(&cpu.b, &cpu.c); break;
        case LD_pBC_A: update = ld_prr_r(&cpu.b, &cpu.c, &cpu.a); break;
        case INC_BC: update = inc_rr(&cpu.b, &cpu.c); break;
        case INC_B: inc_r(&(cpu.b)); break;
        case DEC_B: dec_r(&(cpu.b)); break;
        case LD_B_d8: update = ld_r_d8(&cpu.b); break;
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
            update = (CPUUpdate) {3, 20};
            ww( rw(cpu.pc + 1), cpu.sp );
            break;
        case ADD_HL_BC: update = add_rr_rr(&cpu.h, &cpu.l, &cpu.b, &cpu.c); break;
        case LD_A_pBC: update = ld_r_prr(&cpu.a, &cpu.b, &cpu.c); break;
        case DEC_BC: update = dec_rr(&cpu.b, &cpu.c); break;
        case INC_C: inc_r(&(cpu.c)); break;
        case DEC_C: dec_r(&(cpu.c)); break;
        case LD_C_d8: update = ld_r_d8(&cpu.c); break;
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
        case LD_DE_d16: update = ld_rr_d16(&cpu.d, &cpu.e); break;
        case LD_pDE_A: update = ld_prr_r(&cpu.d, &cpu.e, &cpu.a); break;
        case INC_DE: update = inc_rr(&cpu.d, &cpu.e); break;
        case INC_D: inc_r(&(cpu.d)); break;
        case DEC_D: dec_r(&(cpu.d)); break;
        case LD_D_d8: update = ld_r_d8(&cpu.d); break;
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
        case JR_r8: update = jr(&cpu.pc); break;
        case ADD_HL_DE: update = add_rr_rr(&cpu.h, &cpu.l, &cpu.d, &cpu.e); break;
        case LD_A_pDE: update = ld_r_prr(&cpu.a, &cpu.d, &cpu.e); break;
        case DEC_DE: update = dec_rr(&cpu.d, &cpu.e); break;
        case INC_E: inc_r(&(cpu.e)); break;
        case DEC_E: dec_r(&(cpu.e)); break;
        case LD_E_d8: update = ld_r_d8(&cpu.e); break;
        case RRA:
            if (cpu.f & 0x10) {
                cpu.f = (cpu.a & 1) << 4;
                cpu.a = ((cpu.a & 0xFF) >> 1) | (1 << 7);
            } else {
                cpu.f = (cpu.a & 1) << 4;
                cpu.a = ((cpu.a & 0xFF) >> 1);
            }
            break;

        // --- 0x20 - 0x2F --- //
        case JR_NZ_r8: update = jr_f(&cpu.pc, !(cpu.f & CPU_FLAG_Z)); break;
        case LD_HL_d16: update = ld_rr_d16(&cpu.h, &cpu.l); break;
        case LD_pHLp_A: update = ld_prr_r(&cpu.h, &cpu.l, &cpu.a); inc_rr(&cpu.h, &cpu.l); break;
        case INC_HL: update = inc_rr(&cpu.h, &cpu.l); break;
        case INC_H: inc_r(&(cpu.h)); break;
        case DEC_H: dec_r(&(cpu.h)); break;
        case LD_H_d8: update = ld_r_d8(&cpu.h); break;
        case DAA: // TODO: WTH?
            break;
        case JR_Z_r8: update = jr_f(&cpu.pc, (cpu.f & CPU_FLAG_Z)); break;
        case ADD_HL_HL: update = add_rr_rr(&cpu.h, &cpu.l, &cpu.h, &cpu.l); break;
        case LD_A_pHLp: update = ld_r_prr(&cpu.a, &cpu.h, &cpu.l); inc_rr(&cpu.h, &cpu.l); break;
        case DEC_HL: update = dec_rr(&cpu.h, &cpu.l); break;
        case INC_L: inc_r(&(cpu.l)); break;
        case DEC_L: dec_r(&(cpu.l)); break;
        case LD_L_d8: update = ld_r_d8(&cpu.l); break;
        case CPL:
            cpu.a ^= 0xFF;
            cpu.f |= CPU_FLAG_H | CPU_FLAG_N;
            break;

        // --- 0x30 - 0x3F --- //
        case JR_NC_r8: update = jr_f(&cpu.pc, !(cpu.f & CPU_FLAG_C)); break;
        case LD_SP_d16: update = ld_r2_d16(&cpu.sp); break;
        case LD_pHLm_A: 
            update = ld_prr_r(&cpu.h, &cpu.l, &cpu.a); 
            dec_rr(&cpu.h, &cpu.l);
            break;
        case INC_SP: update = inc_r2(&cpu.sp); break;
        case INC_pHL: 
            wb(rrp(cpu.h, cpu.l), rb(rrp(cpu.h, cpu.h)) + 1); 
            update = (CPUUpdate) {1, 12};
            break;
        case DEC_pHL: 
            wb(rrp(cpu.h, cpu.l), rb(rrp(cpu.h, cpu.h)) - 1); 
            update = (CPUUpdate) {1, 12};
            break;
        case LD_pHL_d8: 
            wb(rrp(cpu.h, cpu.l), rb(cpu.pc + 1));
            update = (CPUUpdate) {2, 12};
            break;
        case SCF:
            cpu.f |= CPU_FLAG_C;
            cpu.f &= ~(CPU_FLAG_H | CPU_FLAG_N);
            break;
        case JR_C_r8: update = jr_f(&cpu.pc, (cpu.f & CPU_FLAG_C)); break;
        case ADD_HL_SP: update = add_rr_r2(&cpu.h, &cpu.l, &cpu.sp); break;
        case LD_A_pHLm: 
            update = ld_r_prr(&cpu.a, &cpu.h, &cpu.l); 
            dec_rr(&cpu.h, &cpu.l); 
            break;
        case DEC_SP: update = dec_r2(&cpu.sp); break;
        case INC_A: inc_r(&(cpu.a)); break;
        case DEC_A: dec_r(&(cpu.a)); break;
        case LD_A_d8: update = ld_r_d8(&cpu.a); break;
        case CCF:
            if (cpu.f & CPU_FLAG_C) cpu.f &= ~CPU_FLAG_C;
            else cpu.f |= CPU_FLAG_C;
            cpu.f &= ~(CPU_FLAG_H | CPU_FLAG_N);
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

        // --- 0x70 - 0x7F --- //
        case LD_pHL_B: update = ld_prr_r(&cpu.h, &cpu.l, &cpu.b); break;
        case LD_pHL_C: update = ld_prr_r(&cpu.h, &cpu.l, &cpu.c); break;
        case LD_pHL_D: update = ld_prr_r(&cpu.h, &cpu.l, &cpu.d); break;
        case LD_pHL_E: update = ld_prr_r(&cpu.h, &cpu.l, &cpu.e); break;
        case LD_pHL_H: update = ld_prr_r(&cpu.h, &cpu.l, &cpu.h); break;
        case LD_pHL_L: update = ld_prr_r(&cpu.h, &cpu.l, &cpu.l); break;
        case HALT: 
            // Wait until interrupt or reset, executing NOPs
            break;
        case LD_pHL_A: update = ld_prr_r(&cpu.h, &cpu.l, &cpu.a); break;
        case LD_A_B: ld_r_r(&cpu.a, &cpu.b); break;
        case LD_A_C: ld_r_r(&cpu.a, &cpu.c); break;
        case LD_A_D: ld_r_r(&cpu.a, &cpu.d); break;
        case LD_A_E: ld_r_r(&cpu.a, &cpu.e); break;
        case LD_A_H: ld_r_r(&cpu.a, &cpu.h); break;
        case LD_A_L: ld_r_r(&cpu.a, &cpu.l); break;
        case LD_A_pHL: update = ld_r_prr(&cpu.a, &cpu.h, &cpu.l); break;
        case LD_A_A: ld_r_r(&cpu.a, &cpu.a); break;
    }

    cpu.pc += update.op_len;
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


#ifndef CPU_H
#define CPU_H

typedef unsigned int reg;

typedef struct {
    // Program Counter (16-bit)
    reg pc;
    
    // Stack Pointer (16-bit)
    reg sp;

    // Registers (8-bit)
    reg a;
    reg b;
    reg c;
    reg d;
    reg e;

    // Flags (8-bit)
    // ---------------
    // 7 6 5 4 3 2 1 0
    // Z N H C 0 0 0 0
    // ---------------
    reg f;

    // Valid 16-bit combinations are 
    // AF, BC, DE, and HL

    // Memory Pointer (16-bit)
    reg h;
    reg l;
} CPUState; 

extern const CPUState CPU_DEFAULT;

// Write a word into a register pair
void reg_wpair(reg *r1, reg *r2, word value);
void reg_rpair(reg *r1, reg *r2);

void execute(CPUState *cpu);

#endif

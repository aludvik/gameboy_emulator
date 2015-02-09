#ifndef CPU_H
#define CPU_H

#include "types.h"

#define CPU_FLAG_C 0x10
#define CPU_FLAG_H 0x20
#define CPU_FLAG_N 0x40
#define CPU_FLAG_Z 0x80

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

typedef struct {
    int op_len;
    int cycles;
} CPUUpdate;

// Write a word into a register pair
void wrp(reg *r1, reg *r2, word value);
// Read a word from a register pair
word rrp(reg r1, reg r2);

void initCPU();

void execute();

void printCPU();

#endif

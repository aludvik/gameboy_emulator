typedef unsigned int byte;

typedef struct State {
    // Program Counter (16-bit)
    byte pc;
    
    // Stack Pointer (16-bit)
    byte sp;

    // Registers (8-bit)
    byte a;
    byte b;
    byte c;
    byte d;
    byte e;

    // Flags (8-bit)
    // ---------------
    // 7 6 5 4 3 2 1 0
    // Z N H C 0 0 0 0
    // ---------------
    byte f;

    // Valid 16-bit combinations are 
    // AF, BC, DE, and HL

    // Memory Pointer (16-bit)
    byte hl;
} 


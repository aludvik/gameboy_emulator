#include "stdio.h"
#include "string.h"

#include "opcodes.h"
#include "mem.h"

byte MEM[RAM_SIZE];

void loadRom(char fn[], byte rom[]){
    // Open ROM for reading
    FILE *fin;
    int i;

    fin = fopen(fn, "rb");

    // TODO: Does this need to descend or ascend???
    // Copy ROM into temporary array
    for (i = 0; i < MAX_ROM_SIZE; i++) {
        if ((rom[i] = getc(fin)) == EOF) {
            i = MAX_ROM_SIZE;
        }
    }

    // TODO: Load straight into memory
    // Cartridge ROM sits in 0000-7FFF
    //for (i = ROM_SIZE - 1; i>= 0; i--) {
    //    setByte(i, rom[i]);
    //}
}

void dissassembleRom(byte rom[]) {
    int i;
    int messages_per_line = 1;
    int lines_before_break = 40;

    for (i = 0; i < MAX_ROM_SIZE; i++) {
        printf("%#06x: ", i);
        decode(rom, i);
        printf("\t\t");
        if (!(i % messages_per_line)) printf("\n");
        if (!(i % lines_before_break)) getchar();
    }
}

// Max length of return message is NN
void decode(byte *b, int i) {
    char *msg;
    int op_len = 1;

    switch (b[i]) {
        case NOP:       msg = "NOP";        break;
        case LD_BC_d16: 
            msg = "LD BC, %#010x %#010x";
            op_len = 3;
            break;
        case LD_BC_A:   msg = "LD (BC), A"; break;
        case INC_BC:    msg = "INC BC";     break;
        case INC_B:     msg = "INC B";      break;
        case DEC_B:     msg = "DEC B";      break;
        case LD_B_d8:
            msg = "LD B, %#010x";
            op_len = 2;
            break;
        case RLCA:      msg = "RLCA";       break;
        case LD_a16_SP:
            msg = "LD %#010x %#010x, SP";
            op_len = 3;
            break;
        case ADD_HL_BC: msg = "ADD HL, BC"; break;
        case LD_A_BC:   msg = "LD A, (BC)"; break;
        case DEC_BC:    msg = "DEC BC";     break;
        case INC_C:     msg = "INC C";      break;
        case DEC_C:     msg = "DEC C";      break;
        case LD_C_d8:
            msg = "LD C, %#010x";
            op_len = 2;
            break;
        case RRCA:      msg = "RRCA";       break;
        default:
            msg = "%#010x";
    }

    if (op_len == 3) {
        printf(msg, b+1, b+2);
    } else if (op_len == 2) {
        printf(msg, b+1);
    } else {
        printf(msg);
    }
}

void setByte(byte address, byte value) {
    MEM[address] = value;
}

byte getByte(byte address) {
    // Mask the value to simulate actual memory behavior
    return MEM[address] & 0xFFFF;
}

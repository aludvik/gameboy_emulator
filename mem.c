#include "stdio.h"

#include "emu.h"

static const MEMState MEM_DEFAULT = {
    1,    // bios active
    0,    // cartridge type
    0,    // mem controller
    0,    // external ram type
    0,    // has battery
    0x00, // rom size
    0x00, // ram size
    1,    // rom bank
    1,    // ram bank
    0x00  // interrupt register
};

static MEMState MEM;

// Memory banks
static byte ROM[MAX_ROM_SIZE];          // 0000 - 7FFF (ROM uses banking)
static byte VIDEO_RAM[VIDEO_RAM_SIZE];  // 8000 - 9FFF
static byte EXT_RAM[MAX_EXT_RAM_SIZE];  // A000 - BFFF
static byte INT_RAM[INT_RAM_SIZE];      // C000 - DFFF 
// Echo of internal ram at:                E000 - FDFF
static byte SPRITE_OAM[SPRITE_OAM_SIZE];// FE00 - FE9F
// Empty                                   FEA0 - FEFF
static byte IO[IO_SIZE];                // FF00 - FF4B
// Empty                                   FF4C - FF7F
static byte ZERO_RAM[ZERO_RAM_SIZE];  // FF80 - FFFF

static const byte BIOS[BIOS_SIZE] = {   // 0000 - 00FF (Before ROM loaded)
    0x31, 0xFE, 0xFF, 0xAF, 0x21, 0xFF, 0x9F, 0x32, 
    0xCB, 0x7C, 0x20, 0xFB, 0x21, 0x26, 0xFF, 0x0E,
    0x11, 0x3E, 0x80, 0x32, 0xE2, 0x0C, 0x3E, 0xF3, 
    0xE2, 0x32, 0x3E, 0x77, 0x77, 0x3E, 0xFC, 0xE0,
    0x47, 0x11, 0x04, 0x01, 0x21, 0x10, 0x80, 0x1A, 
    0xCD, 0x95, 0x00, 0xCD, 0x96, 0x00, 0x13, 0x7B,
    0xFE, 0x34, 0x20, 0xF3, 0x11, 0xD8, 0x00, 0x06, 
    0x08, 0x1A, 0x13, 0x22, 0x23, 0x05, 0x20, 0xF9,
    0x3E, 0x19, 0xEA, 0x10, 0x99, 0x21, 0x2F, 0x99, 
    0x0E, 0x0C, 0x3D, 0x28, 0x08, 0x32, 0x0D, 0x20,
    0xF9, 0x2E, 0x0F, 0x18, 0xF3, 0x67, 0x3E, 0x64, 
    0x57, 0xE0, 0x42, 0x3E, 0x91, 0xE0, 0x40, 0x04,
    0x1E, 0x02, 0x0E, 0x0C, 0xF0, 0x44, 0xFE, 0x90, 
    0x20, 0xFA, 0x0D, 0x20, 0xF7, 0x1D, 0x20, 0xF2,
    0x0E, 0x13, 0x24, 0x7C, 0x1E, 0x83, 0xFE, 0x62, 
    0x28, 0x06, 0x1E, 0xC1, 0xFE, 0x64, 0x20, 0x06,
    0x7B, 0xE2, 0x0C, 0x3E, 0x87, 0xF2, 0xF0, 0x42, 
    0x90, 0xE0, 0x42, 0x15, 0x20, 0xD2, 0x05, 0x20,
    0x4F, 0x16, 0x20, 0x18, 0xCB, 0x4F, 0x06, 0x04, 
    0xC5, 0xCB, 0x11, 0x17, 0xC1, 0xCB, 0x11, 0x17,
    0x05, 0x20, 0xF5, 0x22, 0x23, 0x22, 0x23, 0xC9, 
    0xCE, 0xED, 0x66, 0x66, 0xCC, 0x0D, 0x00, 0x0B,
    0x03, 0x73, 0x00, 0x83, 0x00, 0x0C, 0x00, 0x0D, 
    0x00, 0x08, 0x11, 0x1F, 0x88, 0x89, 0x00, 0x0E,
    0xDC, 0xCC, 0x6E, 0xE6, 0xDD, 0xDD, 0xD9, 0x99, 
    0xBB, 0xBB, 0x67, 0x63, 0x6E, 0x0E, 0xEC, 0xCC,
    0xDD, 0xDC, 0x99, 0x9F, 0xBB, 0xB9, 0x33, 0x3E, 
    0x3c, 0x42, 0xB9, 0xA5, 0xB9, 0xA5, 0x42, 0x4C,
    0x21, 0x04, 0x01, 0x11, 0xA8, 0x00, 0x1A, 0x13, 
    0xBE, 0x20, 0xFE, 0x23, 0x7D, 0xFE, 0x34, 0x20,
    0xF5, 0x06, 0x19, 0x78, 0x86, 0x23, 0x05, 0x20, 
    0xFB, 0x86, 0x20, 0xFE, 0x3E, 0x01, 0xE0, 0x50
};

void initMEM() {
    int i;
    MEM = MEM_DEFAULT;
    fillZeros(VIDEO_RAM, VIDEO_RAM_SIZE);
    fillZeros(EXT_RAM, MAX_EXT_RAM_SIZE);
    fillZeros(INT_RAM, INT_RAM_SIZE);
    fillZeros(SPRITE_OAM, SPRITE_OAM_SIZE);
    fillZeros(IO, IO_SIZE);
    fillZeros(ZERO_RAM, ZERO_RAM_SIZE);
    // Do anything else that needs doing...
}

int loadRom(char fn[]){
    // Open ROM for reading
    FILE *fin;
    int i, end;

    fin = fopen(fn, "rb");

    // TODO: Does this need to descend or ascend???
    // Copy ROM into temporary array
    for (i = 0, end = 0; i < MAX_ROM_S && !end; i++) {
        if ((ROM[i] = getc(fin)) == EOF) {
            end = 1;
        }
    }
    
    return i - 1;
}

// Returns whether byte was written
int wb(word address, byte value) {
    int success = 1;
    if (address >= ROM_START               && address < VIDEO_RAM_START)    {
        // ROM/RAM bank switching

    } else if (address >= VIDEO_RAM_START  && address < EXT_RAM_START)      {
        VIDEO_RAM[address - VIDEO_RAM_START] = value;

    } else if (address >= EXT_RAM_START    && address < INT_RAM_START)      {
        EXT_RAM[(address - EXT_RAM_START) + (SWITCH_RAM_SIZE * MEM.ram_bank)] = value;

    } else if (address >= INT_RAM_START    && address < ECHO_START)         {
        INT_RAM[address - INT_RAM_START] = value;

    } else if (address >= ECHO_START       && address < SPRITE_OAM_START)   {
        INT_RAM[address - ECHO_START] = value;

    } else if (address >= SPRITE_OAM_START && address < EMPTY1_START)       {
        SPRITE_OAM[address - SPRITE_OAM_START] = value;

    } else if (address >= EMPTY1_START     && address < IO_START)           {
        // TODO: Verify this doesn't need to be implemented
        success = 0;

    } else if (address >= IO_START         && address < EMPTY2_START)       {
        IO[address - IO_START] = value;

    } else if (address >= EMPTY2_START     && address < ZERO_RAM_START)    {
        success = 0;

    } else if (address >= ZERO_RAM        && address < INTERRUPT_REGISTER) {
        ZERO_RAM[address - ZERO_RAM_START] = value;

    } else if (address = INTERRUPT_REGISTER) {
        MEM.interrupt = value;
    } else {
        success = 0;
    }
    return success;
}

byte rb(word address) {
    byte value// Mask the value to simulate actual memory behavior
    if (address >= ROM_START               && address < VIDEO_RAM_START)    {
        if (address < SWITCH_ROM_START) {
            if (MEM.bios_active && address < BIOS_SIZE) {
                value = BIOS[address];
            } else {
                value = ROM[address];
            }
        } else if (address >= SWITCH_ROM_START) {
            value = ROM[address + (SWITCH_ROM_SIZE * (MEM.rom_bank - 1))];
        }

    } else if (address >= VIDEO_RAM_START  && address < EXT_RAM_START)      {
        value = VIDEO_RAM[address - VIDEO_RAM_START];

    } else if (address >= EXT_RAM_START    && address < INT_RAM_START)      {
        value = EXT_RAM[(address - EXT_RAM_START) + (SWITCH_RAM_SIZE * MEM.ram_bank)];

    } else if (address >= INT_RAM_START    && address < ECHO_START)         {
        value = INT_RAM[address - INT_RAM_START];

    } else if (address >= ECHO_START       && address < SPRITE_OAM_START)   {
        value = INT_RAM[address - ECHO_START];

    } else if (address >= SPRITE_OAM_START && address < EMPTY1_START)       {
        value = SPRITE_OAM[address - SPRITE_OAM_START];

    } else if (address >= EMPTY1_START     && address < IO_START)           {
        value = 0x00;

    } else if (address >= IO_START         && address < EMPTY2_START)       {
        value = IO[address - IO_START];

    } else if (address >= EMPTY2_START     && address < ZERO_RAM_START)    {
        value = 0x00;

    } else if (address >= ZERO_RAM        && address < INTERRUPT_REGISTER) {
        value = ZERO_RAM[address - ZERO_RAM_START];

    } else if (address = INTERRUPT_REGISTER) {
        value = MEM.interrupt;
    }

    return value & 0xFF; // Memory stores bytes
}

// Returns whether word was written
int ww(word address, word value) {
    // Write the least significant byte first
    return wb(address, value & 0x00FF) || wb(address + 1, value & 0xFF00);
}

word rw(word address) {
    return rb(address) + (rb(address + 1) << 8);
}

void dissassembleRom(byte rom[]) {
    int i;
    int messages_per_line = 1;
    int lines_before_break = 40;

    for (i = 0; i < MAX_ROM_S; i++) {
        printf("%#06x: ", i);
        decode(rom, i);
        printf("\t\t");
        if (!(i % messages_per_line)) printf("\n");
        if (!(i % lines_before_break)) getchar();
    }
}

void printMemory(byte start, byte end) {
    int i;
    printf("-------------------------- MEM ----------------------------\n");
    while (start <= end) {
        printf("| %#06x: ", start);
        for (i = 0; (start + i) <= end && i < 8; i++) {
            printf("%#04x  ", rb(start + i)); 
        }
        while (i < 8) {
            printf("      ");
            i++;
        }
        printf("|\n");
        start += 8;
    }
    printf("-----------------------------------------------------------\n");
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
        case LD_pBC_A:   msg = "LD (BC), A"; break;
        case INC_BC:    msg = "INC BC";     break;
        case INC_B:     msg = "INC B";      break;
        case DEC_B:     msg = "DEC B";      break;
        case LD_B_d8:
            msg = "LD B, %#010x";
            op_len = 2;
            break;
        case RLCA:      msg = "RLCA";       break;
        case LD_pa16_SP:
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

void fillZeros(byte b[], int l){
    int i;
    for (i = 0; i < l; i++) {
        b[i] = 0x00;
    }
}


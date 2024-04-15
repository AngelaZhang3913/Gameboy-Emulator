#include <stdio.h>
#include <typedef.h>

/* -------------
    REGISTERS
 ------------- */

// 8 registers of 8-bits
// paired like AF, BC, DE, HL
// A is accumulator, F is flag register

// flag definitions - the bit in the F register
// where the flag is

#define FLAG_Z 7 // zero flag
#define FLAG_S 6 // subtract flag
#define FLAG_H 5 // half carry flag
#define FLAG_C 4 // carry flag

union Register {
    WORD wrd;
    struct {
        BYTE lo;
        BYTE hi;
    };
};

// register pairs
Register reg_AF = {0x01B0};
Register reg_BC = {0x0013};
Register reg_DE = {0x00D8};
Register reg_HL = {0x014D};

/* -------------
      PC/SP
 ------------- */

WORD program_counter = 0x100;
Register stack_pointer = {0xFFFE};

/* -------------
  INITIALIZATION
 ------------- */

// program_counter = 0x100;
// stack_pointer = 0xFFFE;
// reg_AF = 0x01B0;
// reg_BC = 0x0013;
// reg_DE = 0x00D8;
// reg_HL = 0x014D;

// ... and the special rom registers

/* -------------
     MEMORY
 ------------- */

BYTE rom[0x10000];
BYTE current_rom_bank = 1;

void write_memory(WORD address, BYTE data);

// ram banking
BYTE ram_banks[0x8000];
void initialize_ram_bank() ;
BYTE current_ram_bank = 0;

BYTE cartridge_memory[0x200000] ;

BYTE read_memory(WORD address) ;

bool test_bit(WORD word, int index) ;

bool test_bit(BYTE byte, int index) ;

bool enable_ram;
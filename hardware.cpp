#include <stdio.h>

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
Register reg_AF;
Register reg_BC;
Register reg_DE;
Register reg_HL;

/* -------------
      PC/SP
 ------------- */

WORD program_counter;
Register stack_pointer;

/* -------------
  INITIALIZATION
 ------------- */

program_counter = 0x100;
stack_pointer = 0xFFFE;
reg_AF = 0x01B0;
reg_BC = 0x0013;
reg_DE = 0x00D8;
reg_HL = 0x014D;

// ... and the special rom registers


/* -------------
     MEMORY
 ------------- */

// write memory

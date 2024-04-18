#pragma once
#include <stdio.h>
#include "typedef.h"
#include "bitmath.h"

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
extern Register reg_AF;
extern Register reg_BC;
extern Register reg_DE;
extern Register reg_HL;

/* -------------
      PC/SP
 ------------- */

extern WORD program_counter;
extern Register stack_pointer;

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

extern BYTE rom[0x10000];
extern BYTE current_rom_bank;

void write_memory(WORD address, BYTE data);

// ram banking
extern BYTE ram_banks[0x8000];
void initialize_ram_bank() ;
extern BYTE current_ram_bank;

extern BYTE cartridge_memory[0x200000] ;

BYTE read_memory(WORD address) ;

bool test_bit(WORD word, int index) ;

bool test_bit(BYTE byte, int index) ;

extern bool enable_ram;

extern bool rom_banking;

extern BYTE screen_data[160][144][3] ;

// basic internal game memory
extern BYTE game_memory[0x80000]; // don't know if this actually works

extern BYTE bitset(BYTE byte, int bit) ;

extern BYTE bitreset(BYTE byte, int bit) ;
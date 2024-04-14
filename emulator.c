#include "typdef.h"

// opcode masks (only the ones with varying bits)
// GMB 8 bit-load commands
BYTE ld_r_r_mask = 0b11000000;
BYTE ld_r_n_mask = 0b11000111;
BYTE ld_r_hl_mask = 0b11000111;
BYTE ld_hl_r_mask = 0b11111000;

// GMB 16 bit - load commands
BYTE ld_rr_nn_mask = 0b11001111;
BYTE push_rr_mask = 0b11001111;
BYTE pop_rr_mask = 0b11001111;

// GMB 8 bit - arithmetic/logical commands
BYTE add_A_r_mask = 0b11111000;
BYTE adc_A_r_mask = 0b11111000;
BYTE sub_r_mask = 0b11111000;
BYTE sbc_A_r_mask = 0b11111000;
BYTE cp_r_mask = 0b11111000;
BYTE inc_r_mask = 0b11000111;
BYTE dec_r_mask = 0b11000111;
BYTE and_r_mask = 0b11111000;
BYTE or_r_mask = 0b11111000;
BYTE xor_r_mask = 0b11111000;

// GMB 16 bit - arithmetic/logical commands
BYTE inc_rr_mask = 0b11001111;
BYTE dec_rr_mask = 0b11001111;
BYTE add_HL_rr_mask = 0b11001111;

// GMB rotate and shift commands
BYTE rlc_c_bottomHalf_mask = 0b11111000;
BYTE rl_r_bottomHalf_mask = 0b11111000;
BYTE rrc_r_bottomHalf_mask = 0b11111000;
BYTE rr_r_bottomHalf_mask = 0b11111000;
BYTE sla_r_bottomHalf_mask = 0b11111000;
BYTE swap_r_bottomHalf_mask = 0b11111000;
BYTE sra_r_bottomHalf_mask = 0b11111000;
BYTE srl_r_bottomHalf_mask = 0b11111000;

// GMB singlebit operation commands
BYTE bit_b_r_bottomHalf_mask = 0b11000000;
BYTE bit_b_hl_bottomHalf_mask = 0b11000111;
BYTE res_b_r_bottomHalf_mask = 0b11000000;
BYTE res_b_hl_bottomHalf_mask = 0b11000111;
BYTE set_b_r_bottomHalf_mask = 0b11000000;
BYTE set_b_hl_bottomHalf_mask = 0b11000111;

// GMB Jumpcommands
BYTE jp_cc_nn_mask = 0b11100111;
BYTE jr_cc_e_mask = 0b11100111;
BYTE call_cc_nn_mask = 0b11100111;
BYTE ret_cc_mask = 0b11100111;
BYTE rst_n_mask = 0b11000111;

void update() {
    int current_cycle;
    while (current_cycle < 69905) {
        int new_cycles = execute_next_opcode()
        current_cycle += new_cycles;
        update_timers(new_cycles);
        update_graphics(new_cycles);
        execute_interrupts();
    }
    render_screen();
}

int execute_next_opcode() {
    // returns the number of cycles for the instruction

}

update_timers(int cycles) {

}

execute_interrupts() {

}

render_screen() {
    
}


int execute_opcode(BYTE op) {
    // returns the number of cycles for the instruction
    switch (op) {
        // 8 BIT LOAD (total 19)
        case 0xFA : // ld A, nn
        case 0xEA : // ld nn, A
            return 16;
        
        case 0x36 : // ld HL, n
        case 0xF0 : // ld A, FF00+n
        case 0xE0 : // ld FF00+n, A
            return 12;

        case 0x0A : // ld A, BC
        case 0x1A : // ld A, DE        
        case 0x02 : // ld BC, A
        case 0x12 : // ld DE, A
        case 0xF2 : // ld A, FF00+C
        case 0xE2 : // ld FF00+C, A
        case 0x22 : // ldi HL, A
        case 0x2A : // ldi A, HL
        case 0x32 : // ldd HL, A
            return 8;
        
        // 16 BIT LOAD (total 4)
        case 0xF9 : // ld SP, HL
            return 8;
        
        // 8 BIT ARITHMETIC/LOGICAL (total 30)
        case 0x27 : // daa
        case 0x2F : // cpl
            return 4;

        case 0xC6 : // add A, n
        case 0xCE : // adc A, n
        case 0xD6 : // sub n
        case 0xDE : // sbc A
        case 0xE6 : // and n
        case 0xEE : // xor n
        case 0xF6 : // or n
        case 0xFE : // cp n
            return 8;

        case 0x86 : // add A, HL
        case 0x8E : // adc A, HL
        case 0x96 : // sub HL
        case 0x9E : // sbc A, HL
        case 0xA6 : // and HL
        case 0xAE : // xor HL
        case 0xB6 : // or HL
        case 0xBE : // cp HL
            return 8;

        case 0x34 : // inc HL
        case 0x35 : // dec HL
            return 12;

        // 16 BIT ARITHMETIC/LOGICAL (total 5)
        case 0xE8 : // add SP, dd
            return 16;
        
        case 0xF8 : // ld HL, SP+dd
            return 12;
        
        // ROTATE AND SHIFT (total 20)
        case 0x07 : // rlca
        case 0x17 : // rla
        case 0x0F : // rrca
        case 0x1F : // rra
            return 4;
        
        // CPU CONTROL
        case 0x3F : // ccf
        case 0x37 : // scf
        case 0x00 : // nop
        case 0xF3 : // di
        case 0xFB : // ei
            return 4;
            
        // case 0x76 : // halt
        // case 0x10 00 : // stop

        // JUMP COMMANDS
        case 0xC3 : // jp nn
        case 0xC9 : // ret
        case 0xD9 : // resti
            return 16;
        
        case 0xE9 : // jp HL
            return 4;

        case 0x18 : // jr PC+dd
            return 12;

        case 0xCD : // call nn
            return 24;
        
        case 0xCB : 
            return execute_extended_opcode();
    }
}

int execute_extended_opcode() {
    BYTE opcode;

    
}

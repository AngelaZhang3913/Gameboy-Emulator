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
BYTE rotate_shift_mask = 0b11111000;

// GMB singlebit operation commands
BYTE bit_mask_1 = 0b11000000;
BYTE bit_mask_2 = 0b11000111;

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

void update_timers(int cycles) {

}

void execute_interrupts() {

}

void render_screen() {
    
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
    if (op & ld_r_r_mask == 0b01000000) {
        return 4;
    } else if (op & ld_r_n_mask == 0b00000110) {
        return 8;
    } else if (op & ld_r_hl_mask == 0b01000110) {
        return 8;
    } else if (op & ld_hl_r_mask == 0b01110000) {
        return 8;
    } else if (op & ld_rr_nn_mask == 0b00000001) {
        return 12;
    } else if (op & push_rr_mask == 0b11000101) {
        return 16;
    } else if (op & pop_rr_mask == 0b11000001) {
        return 12;
    } else if (op & add_A_r_mask == 0b10000000) {
        return 4;
    } else if (op & adc_A_r_mask == 0b10001000) {
        return 4;
    } else if (op & sub_r_mask == 0b10010000) {
        return 4;
    } else if (op & sbc_A_r_mask == 0b10011000) {
        return 4;
    } else if (op & cp_r_mask == 0b10111000) {
        return 4;
    } else if (op & inc_r_mask == 0b00000100) {
        return 4;
    } else if (op & dec_r_mask == 0b00000101) {
        return 4;
    } else if (op & and_r_mask == 0b10100000) {
        return 4;
    } else if (op & or_r_mask == 0b10110000) {
        return 4;
    } else if (op & xor_r_mask == 0b10101000) {
        return 4;
    } else if (op & inc_rr_mask == 0b000000011) {
        return 8;
    } else if (op & dec_rr_mask == 0b00001011) {
        return 8;
    } else if (op & add_HL_rr_mask == 0b00001001) {
        return 8;
    } else if (op & jp_cc_nn_mask == 0b11000010) {
        // return execute_jp_cc_nn(); 
    } else if (op & jr_cc_e_mask == 0b00100000) {
        // return execute_jr_cc_e();
    } else if (op & call_cc_nn_mask == 0b11000100) {
        // return execute_call_cc_nn();
    } else if (op & ret_cc_mask == 0b11000000) {
        // return execute_ret_cc();
    } else if (op & rst_n_mask == 0b11000111) {
        return 16;
    }
}

int execute_extended_opcode() {
    BYTE opcode; // = readmemory
    
    switch(opcode) {
        case 0x06 : // rlc HL
        case 0x16 : // rl HL
        case 0x0E : // rrc HL
        case 0x1E : // rr HL
        case 0x26 : // sla HL
        case 0x36 : // swap HL
        case 0x2E : // sra HL
        case 0x3E : // srl HL
            return 16;
    }

    switch(opcode & rotate_shift_mask) {
        case 0 : // rlc r
        case 0b00010000 : // rl r
        case 0b00001000 : // rrc r
        case 0b00011000 : // rr r
        case 0b00100000 : // sla r
        case 0b00110000 : // swap r
        case 0b00101000 : // sra r
        case 0b00101000 : // srl r
            return 8;
    }

    switch(opcode & bit_mask_1) {
        case 0b01000000 : // bit b r
        case 0b10000000 : // res b r
        case 0b11000000 : // set b r
            return 8;
    }

    switch(opcode & bit_mask_2) {
        case 0b01000110 : // bit b HL
            return 12;
        case 0b10000110 : // res b HL
        case 0b11000110 : // set b HL
            return 16;
    }
}


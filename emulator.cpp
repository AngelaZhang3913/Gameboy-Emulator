#include "emulator.h"

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

void update_timers(int cycles) {

}

void execute_interrupts() {

}

void render_screen() {
    
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
        case 0b00111000 : // srl r
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

    return 0;
}

BYTE get_reg_value(BYTE bits) {
    switch(bits) {
        case 7 : // A
            return reg_AF.hi;
        case 0 : // B
            return reg_BC.hi;
        case 1 : // C
            return reg_BC.lo;
        case 2 : // D
            return reg_DE.hi;
        case 3 : // E
            return reg_DE.lo;
        case 4 : // H
            return reg_HL.hi;
        case 5 : // L
            return reg_HL.lo;
        case 6 : // HL
            return reg_HL.wrd;
    }
    return 0;
}

BYTE get_bit(BYTE num, BYTE bit) {
    return (num & (1 << bit)) >> bit;
}

BYTE get_flag(BYTE bit_num) {
    return get_bit(reg_AF.lo, bit_num);
}

void set_flag(BYTE bit_num, bool b) {
    if(b) reg_AF.lo = reg_AF.lo | (1 << bit_num);
    else reg_AF.lo = reg_AF.lo & ~(1 << bit_num);
}

void print_result() {
    printf("result = %d\n", reg_AF.hi);
    printf("flag z: %d\n", get_flag(FLAG_Z) );
    printf("flag s: %d\n", get_flag(FLAG_S) );
    printf("flag h: %d\n", get_flag(FLAG_H) );
    printf("flag c: %d\n", get_flag(FLAG_C) );
}

// for adding with registers and immediates (8 bit only)
void execute_add(bool carry, BYTE n) {
    printf("A = %d\n", reg_AF.hi);
    printf("n = %d\n", n);
    BYTE sum = reg_AF.hi + n;
    int int_sum = reg_AF.hi + n;
    if(carry) sum += get_flag(FLAG_C);
    BYTE half_sum = (reg_AF.hi & 0b1111) + (n & 0b1111);
    printf("half sum = %d\n", half_sum);

    // set flags
    set_flag(FLAG_Z, sum == 0);
    set_flag(FLAG_S, 0);
    set_flag(FLAG_H, half_sum > 0xf);
    set_flag(FLAG_C, int_sum > 0xff);
    reg_AF.hi = sum;
    print_result();
}

void execute_sub(bool carry, BYTE n) {
    printf("A = %d\n", reg_AF.hi);
    printf("n = %d\n", n);
    BYTE diff = reg_AF.hi - n;
    if(carry) diff -= get_flag(FLAG_C);
    BYTE bottomHalfA = reg_AF.hi & 0b1111;
    BYTE bottomHalfN = n & 0b1111;
    

    // set flags
    set_flag(FLAG_Z, diff == 0);
    set_flag(FLAG_S, 1);
    // printf("reg_af half carry is 0: %d\n", (get_bit(reg_AF.hi, 3) == 0));
    // printf("n half carry is 1: %d\n", (get_bit(n, 3) == 1));
    set_flag(FLAG_H, bottomHalfA < bottomHalfN);
    set_flag(FLAG_C, reg_AF.hi < n);
    reg_AF.hi = diff;
    printf("diff = %d\n", reg_AF.hi);
    printf("flag z: %d\n", get_flag(FLAG_Z) );
    printf("flag s: %d\n", get_flag(FLAG_S) );
    printf("flag h: %d\n", get_flag(FLAG_H) );
    printf("flag c: %d\n", get_flag(FLAG_C) );
}

void execute_and(BYTE n) {
    BYTE res = reg_AF.hi & n;
    set_flag(FLAG_Z, res == 0);
    set_flag(FLAG_S, 0);
    set_flag(FLAG_H, 0);
    set_flag(FLAG_C, 0);
    reg_AF.hi = res;
}

void execute_xor(BYTE n) {
    BYTE res = reg_AF.hi ^ n;
    set_flag(FLAG_Z, res == 0);
    set_flag(FLAG_S, 0);
    set_flag(FLAG_H, 0);
    set_flag(FLAG_C, 0);
    reg_AF.hi = res;
}

void execute_or(BYTE n) {
    BYTE res = reg_AF.hi | n;
    set_flag(FLAG_Z, res == 0);
    set_flag(FLAG_S, 0);
    set_flag(FLAG_H, 0);
    set_flag(FLAG_C, 0);
    reg_AF.hi = res;
}

void execute_cp(BYTE n) {
    BYTE diff = reg_AF.hi - n;
    BYTE bottomHalfA = reg_AF.hi & 0b1111;
    BYTE bottomHalfN = n & 0b1111;
    

    // set flags
    set_flag(FLAG_Z, diff == 0);
    set_flag(FLAG_S, 1);
    set_flag(FLAG_H, bottomHalfA < bottomHalfN);
    set_flag(FLAG_C, reg_AF.hi < n);
}

void set_reg_8(BYTE reg, BYTE val) {
    switch(reg) {
        case 7 : // A
            reg_AF.hi = val;
        case 0 : // B
            reg_BC.hi = val;
        case 1 : // C
            reg_BC.lo = val;
        case 2 : // D
            reg_DE.hi = val;
        case 3 : // E
            reg_DE.lo = val;
        case 4 : // H
            reg_HL.hi = val;
        case 5 : // L
            reg_HL.lo = val;
        case 6 : // HL
            reg_HL.wrd = val;
    }
}

void execute_inc(BYTE reg, BYTE n) {
    BYTE res = n + 1;
    set_flag(FLAG_Z, res == 0);
    set_flag(FLAG_S, 0);
    set_flag(FLAG_H, (n & 0b1111) == 0xf);
    set_reg_8(reg, res);
    print_result();
}

void execute_dec(BYTE reg, BYTE n) {
    BYTE res = n - 1;
    set_flag(FLAG_Z, res == 0);
    set_flag(FLAG_S, 0);
    set_flag(FLAG_H, (n & 0b1111) == 0);
    set_reg_8(reg, res);
    print_result();
}

void execute_daa() {
    int int_res = reg_AF.hi;
    if (!get_flag(FLAG_S)) {
        if ((reg_AF.hi & 0b1111) > 0x9) {
            int_res += 0x6;
            reg_AF.hi += 0x6;
        } else if (reg_AF.hi > 0x99) {
            int_res += 0x60;
            reg_AF.hi += 0x60;
        }
    }
    if (get_flag(FLAG_H)) {
        int_res += 0x6;
        reg_AF.hi += 0x6;
    } 
    if (get_flag(FLAG_C)) {
        int_res += 0x60;
        reg_AF.hi += 0x60;
    }

    set_flag(FLAG_Z, reg_AF.hi == 0);
    set_flag(FLAG_H, 0);
    set_flag(FLAG_C, int_res > 0xff);
}

void execute_cpl() {
    reg_AF.hi = ~reg_AF.hi;
    set_flag(FLAG_S, 1);
    set_flag(FLAG_H, 1);
}

int execute_opcode(BYTE op) {
    // returns the number of cycles for the instruction

    BYTE val;
    BYTE reg_num;
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
        
        /* 8 BIT ARITHMETIC/LOGICAL */
        case 0x27 : // daa
            execute_daa();
            return 4;
        case 0x2F : // cpl
            execute_cpl();
            return 4;

        case 0xC6 : // add A, n
            val = read_memory(program_counter);
            program_counter++;
            execute_add(false, val);
            return 8;
        case 0xCE : // adc A, n
            val = read_memory(program_counter);
            program_counter++;
            execute_add(true, val);
            return 8;
        case 0x86 : // add A, HL
            val = read_memory(reg_HL.wrd);
            execute_add(false, val);
            return 8;
        case 0x8E : // adc A, HL
            val = read_memory(reg_HL.wrd);
            execute_add(true, val);
            return 8;

        case 0xD6 : // sub n
            val = read_memory(program_counter);
            program_counter++;
            execute_sub(false, val);
            return 8;
        case 0xDE : // sbc A
            val = read_memory(program_counter);
            program_counter++;
            execute_sub(true, val);
            return 8;
        case 0x96 : // sub HL
            val = read_memory(reg_HL.wrd);
            execute_sub(false, val);
            return 8;
        case 0x9E : // sbc A, HL
            val = read_memory(reg_HL.wrd);
            execute_sub(true, val);
            return 8;

        case 0xE6 : // and n
            val = read_memory(program_counter);
            program_counter++;
            execute_and(val);
            return 8;
        case 0xA6 : // and HL
            val = read_memory(reg_HL.wrd);
            execute_and(val);
            return 8;

        case 0xEE : // xor n
            val = read_memory(program_counter);
            program_counter++;
            execute_xor(val);
            return 8;
        case 0xAE : // xor HL
            val = read_memory(reg_HL.wrd);
            execute_xor(val);
            return 8;

        case 0xF6 : // or n
            val = read_memory(program_counter);
            program_counter++;
            execute_or(val);
            return 8;
        case 0xB6 : // or HL
            val = read_memory(reg_HL.wrd);
            execute_or(val);
            return 8;
        
        case 0xFE : // cp n
            val = read_memory(program_counter);
            program_counter++;
            execute_cp(val);
            return 8;
        case 0xBE : // cp HL
            val = read_memory(reg_HL.wrd);
            execute_cp(val);
            return 8;

        case 0x34 : // inc HL
            // val = read_memory(reg_HL.wrd);
            // execute_inc(val);
            return 12;
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
    if ((op & ld_r_r_mask) == 0b01000000) {
        return 4;
    } else if ((op & ld_r_n_mask) == 0b00000110) {
        return 8;
    } else if ((op & ld_r_hl_mask) == 0b01000110) {
        return 8;
    } else if ((op & ld_hl_r_mask) == 0b01110000) {
        return 8;
    } else if ((op & ld_rr_nn_mask) == 0b00000001) {
        return 12;
    } else if ((op & push_rr_mask) == 0b11000101) {
        return 16;
    } else if ((op & pop_rr_mask) == 0b11000001) {
        return 12;
    } else if ((op & add_A_r_mask) == 0b10000000) {
        val = get_reg_value(op & 0b111);
        execute_add(false, val);
        return 4;
    } else if ((op & adc_A_r_mask) == 0b10001000) {
        val = get_reg_value(op & 0b111);
        execute_add(true, val);
        return 4;
    } else if ((op & sub_r_mask) == 0b10010000) {
        val = get_reg_value(op & 0b111);
        execute_sub(false, val);
        return 4;
    } else if ((op & sbc_A_r_mask) == 0b10011000) {
        val = get_reg_value(op & 0b111);
        execute_sub(true, val);
        return 4;
    } else if ((op & cp_r_mask) == 0b10111000) {
        val = get_reg_value(op & 0b111);
        execute_cp(val);
        return 4;
    } else if ((op & inc_r_mask) == 0b00000100) {
        reg_num = (op >> 3) & 0b111;
        val = get_reg_value(reg_num);
        execute_inc(reg_num, val);
        return 4;
    } else if ((op & dec_r_mask )== 0b00000101) {
        reg_num = (op >> 3) & 0b111;
        val = get_reg_value(reg_num);
        execute_dec(reg_num, val);
        return 4;
    } else if ((op & and_r_mask) == 0b10100000) {
        val = get_reg_value(op & 0b111);
        execute_and(val);
        return 4;
    } else if ((op & or_r_mask) == 0b10110000) {
        val = get_reg_value(op & 0b111);
        execute_or(val);
        return 4;
    } else if ((op & xor_r_mask) == 0b10101000) {
        val = get_reg_value(op & 0b111);
        execute_xor(val);
        return 4;
    } else if ((op & inc_rr_mask) == 0b000000011) {
        return 8;
    } else if ((op & dec_rr_mask) == 0b00001011) {
        return 8;
    } else if ((op & add_HL_rr_mask) == 0b00001001) {
        return 8;
    } else if ((op & jp_cc_nn_mask) == 0b11000010) {
        // return execute_jp_cc_nn(); 
    } else if ((op & jr_cc_e_mask) == 0b00100000) {
        // return execute_jr_cc_e();
    } else if ((op & call_cc_nn_mask) == 0b11000100) {
        // return execute_call_cc_nn();
    } else if ((op & ret_cc_mask) == 0b11000000) {
        // return execute_ret_cc();
    } else if ((op & rst_n_mask) == 0b11000111) {
        return 16;
    }
    return 0;
}

int execute_next_opcode() {
    // returns the number of cycles for the instruction
    BYTE opcode = 0; //= read_memory(program_counter);
    program_counter++;
    return execute_opcode(opcode);
}

void update() {
    int current_cycle;
    while (current_cycle < 69905) {
        int new_cycles = execute_next_opcode();
        current_cycle += new_cycles;
        update_timers(new_cycles);
        //update_graphics(new_cycles);
        execute_interrupts();
    }
    render_screen();
}
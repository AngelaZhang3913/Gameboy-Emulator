#include "emulator.h"
#include "interrupts.h"
#include "graphics.h"

int intrpt_next_inst = -1;
bool en_interrupt = 0;

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

bool halt = false;

BYTE get_reg_value_8(BYTE bits) {
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

WORD get_reg_value_16(BYTE bits) {
    switch(bits) {
        case 0 : // BC
            return reg_BC.wrd;
        case 1 : // DE
            return reg_DE.wrd;
        case 2 : // HL
            return reg_HL.wrd;
        case 3 : // SP
            return stack_pointer.wrd;
    }
    return 0;
}

Register* get_register(BYTE bits) {
    switch (bits) {
        case 0:
            return &reg_BC;
        case 1:
            return &reg_DE;
        case 2:
            return &reg_HL;
        case 3:
            return &stack_pointer;
    }
    return 0;
}

void set_reg_8(BYTE reg, BYTE val) {
    switch(reg) {
        case 7 : // A
            reg_AF.hi = val;
            break;
        case 0 : // B
            reg_BC.hi = val;
            break;
        case 1 : // C
            reg_BC.lo = val;
            break;
        case 2 : // D
            reg_DE.hi = val;
            break;
        case 3 : // E
            reg_DE.lo = val;
            break;
        case 4 : // H
            reg_HL.hi = val;
            break;
        case 5 : // L
            reg_HL.lo = val;
            break;
        case 6 : // HL
            reg_HL.wrd = val;
            break;
    }
}

void set_reg_16(BYTE reg, WORD val) {
    switch(reg) {
        case 0 : // BC
            reg_BC.wrd = val;
            break;
        case 1 : // DE
            reg_DE.wrd = val;
            break;
        case 2 : // HL
            reg_HL.wrd = val;
            break;
        case 3 : // SP
            stack_pointer.wrd = val;
            break;
    }
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

void set_all_flags(BYTE z, BYTE s, BYTE h, BYTE c) {
    set_flag(FLAG_Z, z);
    set_flag(FLAG_S, s);
    set_flag(FLAG_H, h);
    set_flag(FLAG_C, c);
}

void print_result() {
    printf("result = %d\n", reg_AF.hi);
    printf("flag z: %d\n", get_flag(FLAG_Z) );
    printf("flag s: %d\n", get_flag(FLAG_S) );
    printf("flag h: %d\n", get_flag(FLAG_H) );
    printf("flag c: %d\n\n", get_flag(FLAG_C) );
}

void print_result_regs(BYTE bits) {
    switch(bits) {
        case 7 : // A
            printf("result = %d\n", reg_AF.hi);
            printf("flag z: %d\n", get_flag(FLAG_Z) );
            printf("flag s: %d\n", get_flag(FLAG_S) );
            printf("flag h: %d\n", get_flag(FLAG_H) );
            printf("flag c: %d\n\n", get_flag(FLAG_C) );
            break;
        case 0 : // B
            printf("result = %d\n", reg_BC.hi);
            printf("flag z: %d\n", get_flag(FLAG_Z) );
            printf("flag s: %d\n", get_flag(FLAG_S) );
            printf("flag h: %d\n", get_flag(FLAG_H) );
            printf("flag c: %d\n\n", get_flag(FLAG_C) );
            break;
        case 1 : // C
            printf("result = %d\n", reg_BC.lo);
            printf("flag z: %d\n", get_flag(FLAG_Z) );
            printf("flag s: %d\n", get_flag(FLAG_S) );
            printf("flag h: %d\n", get_flag(FLAG_H) );
            printf("flag c: %d\n\n", get_flag(FLAG_C) );
            break;
        case 2 : // D
            printf("result = %d\n", reg_DE.hi);
            printf("flag z: %d\n", get_flag(FLAG_Z) );
            printf("flag s: %d\n", get_flag(FLAG_S) );
            printf("flag h: %d\n", get_flag(FLAG_H) );
            printf("flag c: %d\n\n", get_flag(FLAG_C) );
            break;
        case 3 : // E
            printf("result = %d\n", reg_DE.lo);
            printf("flag z: %d\n", get_flag(FLAG_Z) );
            printf("flag s: %d\n", get_flag(FLAG_S) );
            printf("flag h: %d\n", get_flag(FLAG_H) );
            printf("flag c: %d\n\n", get_flag(FLAG_C) );
            break;
        case 4 : // H
            printf("result = %d\n", reg_HL.hi);
            printf("flag z: %d\n", get_flag(FLAG_Z) );
            printf("flag s: %d\n", get_flag(FLAG_S) );
            printf("flag h: %d\n", get_flag(FLAG_H) );
            printf("flag c: %d\n\n", get_flag(FLAG_C) );
            break;
        case 5 : // L
            printf("result = %d\n", reg_HL.lo);
            printf("flag z: %d\n", get_flag(FLAG_Z) );
            printf("flag s: %d\n", get_flag(FLAG_S) );
            printf("flag h: %d\n", get_flag(FLAG_H) );
            printf("flag c: %d\n\n", get_flag(FLAG_C) );
            break;
        case 6 : // HL
            printf("result = %d\n", reg_HL.wrd);
            printf("flag z: %d\n", get_flag(FLAG_Z) );
            printf("flag s: %d\n", get_flag(FLAG_S) );
            printf("flag h: %d\n", get_flag(FLAG_H) );
            printf("flag c: %d\n\n", get_flag(FLAG_C) );
            break;
    }    
}

// for adding with registers and immediates (8 bit only)
void execute_add(bool carry, BYTE n) {
    BYTE sum = reg_AF.hi + n;
    int int_sum = reg_AF.hi + n;
    if(carry) sum += get_flag(FLAG_C);
    BYTE half_sum = (reg_AF.hi & 0b1111) + (n & 0b1111);

    set_all_flags(sum == 0, 0, half_sum > 0xf, int_sum > 0xff);
    reg_AF.hi = sum;
}

void execute_add_HL_rr(Register* reg) {
    WORD sum = reg_HL.wrd + (*reg).wrd;
    int int_sum = reg_HL.wrd + (*reg).wrd;
    WORD half_sum = (reg_HL.wrd & 0xfff) + ((*reg).wrd & 0xfff);

    set_all_flags(sum == 0, 0, half_sum > 0xfff, int_sum > 0xffff);
    reg_HL.wrd = sum;
}

void execute_sub(bool carry, BYTE n) {
    BYTE diff = reg_AF.hi - n;
    if(carry) diff -= get_flag(FLAG_C);
    BYTE bottomHalfA = reg_AF.hi & 0b1111;
    BYTE bottomHalfN = n & 0b1111;

    set_all_flags(diff == 0, 1, bottomHalfA < bottomHalfN, reg_AF.hi < n);
    reg_AF.hi = diff;
}

void execute_and(BYTE n) {
    BYTE res = reg_AF.hi & n;
    set_all_flags(res == 0, 0, 0, 0);
    reg_AF.hi = res;
}

void execute_xor(BYTE n) {
    BYTE res = reg_AF.hi ^ n;
    set_all_flags(res == 0, 0, 0, 0);
    reg_AF.hi = res;
}

void execute_or(BYTE n) {
    BYTE res = reg_AF.hi | n;
    set_all_flags(res == 0, 0, 0, 0);
    reg_AF.hi = res;
}

void execute_cp(BYTE n) {
    BYTE diff = reg_AF.hi - n;
    BYTE bottomHalfA = reg_AF.hi & 0b1111;
    BYTE bottomHalfN = n & 0b1111;
    set_all_flags(diff == 0, 1, bottomHalfA < bottomHalfN, reg_AF.hi < n);
}

void execute_inc(BYTE reg, BYTE n, WORD addr, bool isHL) {
    BYTE res = n + 1;
    set_flag(FLAG_Z, res == 0);
    set_flag(FLAG_S, 0);
    set_flag(FLAG_H, (n & 0b1111) == 0xf);
    if (isHL) write_memory(addr, res);
    else set_reg_8(reg, res);
}

void execute_inc_rr(Register* reg) { // no flags need to be set off
    (*reg).wrd += 1;
}

void execute_dec(BYTE reg, BYTE n, WORD addr, bool isHL) {
    BYTE res = n - 1;
    set_flag(FLAG_Z, res == 0);
    set_flag(FLAG_S, 0);
    set_flag(FLAG_H, (n & 0b1111) == 0);
    if (isHL) write_memory(addr, res);
    else set_reg_8(reg, res);
    print_result();
}

void execute_dec_rr(Register* reg) { // no flags need to be set off
    (*reg).wrd -= 1;
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

// left shifts and rotates
void execute_left_shift_rotate(BYTE reg_num, WORD addr, BYTE n, bool is_reg, BYTE type) {
    BYTE bit_7 = n >> 7;
    BYTE res = n << 1;
    if(type == 0) res |= bit_7; // rotate circular
    else if(type == 1) res |= get_flag(FLAG_C); // rotate
    
    set_all_flags(res == 0, 0, 0, bit_7);
    if(is_reg) set_reg_8(reg_num, res);
    else write_memory(addr, res);
}

// right shifts and rotates
void execute_right_shift_rotate(BYTE reg_num, WORD addr, BYTE n, bool is_reg, BYTE type) {
    BYTE bit_0 = n & 1;
    BYTE res = n >> 1;
    if(type == 0) res |= (bit_0 << 7); // rotate circular
    else if(type == 1) res |= (get_flag(FLAG_C) << 7); // rotate
    else if(type == 2) res |= n >> 7; // shift arithmetic
    
    set_all_flags(res == 0, 0, 0, bit_0);
    if(is_reg) set_reg_8(reg_num, res);
    else write_memory(addr, res);
    print_result();
}

void execute_swap(BYTE reg_num, WORD addr, BYTE n, bool is_reg) {
    BYTE res = (n >> 4) | (n & 0b1111);
    set_all_flags(res == 0, 0, 0, 0);
    if(is_reg) set_reg_8(reg_num, res);
    else write_memory(addr, res);
}

void execute_bit(BYTE val, BYTE bit_num) {
    BYTE res = (val >> bit_num) & 0b1;
    set_flag(FLAG_Z, res == 0);
    set_flag(FLAG_S, 0);
    set_flag(FLAG_H, 1);
}

void execute_reset(BYTE reg_num, WORD addr, BYTE val, BYTE bit_num, bool is_reg) {
    BYTE res = val & ~(1 << bit_num);
    if (is_reg) set_reg_8(reg_num, res);
    else write_memory(addr, res);
}

void execute_set(BYTE reg_num, WORD addr, BYTE val, BYTE bit_num, bool is_reg) {
    BYTE res = val | (1 << bit_num);
    if(is_reg) set_reg_8(reg_num, res);
    else write_memory(addr, res);
}

bool check_flag(BYTE val) {
    switch(val) {
        case 0 :
            return !get_flag(FLAG_Z);
        case 1 :
            return get_flag(FLAG_Z);
        case 2 :
            return !get_flag(FLAG_C);
        case 3 : 
            return get_flag(FLAG_C);
    }
    return 0;
}

int execute_extended_opcode() {
    BYTE op = read_memory(program_counter);
    program_counter++;

    BYTE val;
    BYTE reg_num;
    switch(op) {
        case 0x06 : // rlc HL
            val = read_memory(reg_HL.wrd);
            execute_left_shift_rotate(0, reg_HL.wrd, val, false, 0);
            return 16;
        case 0x16 : // rl HL
            val = read_memory(reg_HL.wrd);
            execute_left_shift_rotate(0, reg_HL.wrd, val, false, 1);
            return 16;
        case 0x0E : // rrc HL
            val = read_memory(reg_HL.wrd);
            execute_right_shift_rotate(0, reg_HL.wrd, val, false, 0);
            return 16;
        case 0x1E : // rr HL
            val = read_memory(reg_HL.wrd);
            execute_right_shift_rotate(0, reg_HL.wrd, val, false, 1);
            return 16;
        case 0x26 : // sla (HL)
            val = read_memory(reg_HL.wrd);
            execute_left_shift_rotate(0, reg_HL.wrd, val, false, 2);
            return 16;
        case 0x36 : // swap HL
            val = read_memory(reg_HL.wrd);
            execute_swap(0, reg_HL.wrd, val, false);
            return 16;
        case 0x2E : // sra HL
            val = read_memory(reg_HL.wrd);
            execute_right_shift_rotate(0, reg_HL.wrd, val, false, 2);
            return 16;
        case 0x3E : // srl HL
            val = read_memory(reg_HL.wrd);
            execute_right_shift_rotate(0, reg_HL.wrd, val, false, 3);
            return 16;
    }

    switch(op & rotate_shift_mask) {
        case 0 : // rlc r
            reg_num = op & 0b111;
            val = get_reg_value_8(reg_num);
            execute_left_shift_rotate(reg_num, 0, val, true, 0);
            return 8;
        case 0b00010000 : // rl r
            reg_num = op & 0b111;
            val = get_reg_value_8(reg_num);
            execute_left_shift_rotate(reg_num, 0, val, true, 1);
            return 8;
        case 0b00001000 : // rrc r
            reg_num = op & 0b111;
            val = get_reg_value_8(reg_num);
            execute_right_shift_rotate(reg_num, 0, val, true, 0);
            return 8;
        case 0b00011000 : // rr r
            reg_num = op & 0b111;
            val = get_reg_value_8(reg_num);
            execute_right_shift_rotate(reg_num, 0, val, true, 1);
            return 8;
        case 0b00100000 : // sla r
            reg_num = op & 0b111;
            val = get_reg_value_8(reg_num);
            execute_left_shift_rotate(reg_num, 0, val, true, 2);
            return 8;
        case 0b00110000 : // swap r
            reg_num = op & 0b111;
            val = get_reg_value_8(reg_num);
            execute_swap(reg_num, 0, val, true);
            return 8;
        case 0b00101000 : // sra r
            reg_num = op & 0b111;
            val = get_reg_value_8(reg_num);
            execute_right_shift_rotate(reg_num, 0, val, true, 2);
            return 8;
        case 0b00111000 : // srl r
            reg_num = op & 0b111;
            val = get_reg_value_8(reg_num);
            execute_right_shift_rotate(reg_num, 0, val, true, 3);
            return 8;
    }

    switch(op & bit_mask_2) {
        case 0b01000110 : // bit b HL
            val = (op >> 3) & 0b111;
            execute_bit(read_memory(reg_HL.wrd), val);
            return 12;
        case 0b10000110 : // res b HL
            val = (op >> 3) & 0b111;
            execute_reset(0, reg_HL.wrd, read_memory(reg_HL.wrd), val, false);
            return 16;
        case 0b11000110 : // set b HL
            val = (op >> 3) & 0b111;
            execute_set(0, reg_HL.wrd, read_memory(reg_HL.wrd), val, false);
            return 16;
    }

    switch(op & bit_mask_1) {
        case 0b01000000 : // bit b r
            reg_num = op  & 0b111;
            val = (op >> 3) & 0b111;
            execute_bit(get_reg_value_8(reg_num), val);
            return 8;
        case 0b10000000 : // res b r
            reg_num = op & 0b111;
            val = (op >> 3) & 0b111;
            execute_reset(reg_num, 0, get_reg_value_8(reg_num), val, true);
            return 8;
        case 0b11000000 : // set b r
            reg_num = op & 0b111;
            val = (op >> 3) & 0b111;
            execute_set(reg_num, 0, get_reg_value_8(reg_num), val, true);
            return 8;
    }

    return 0;
}

int execute_opcode(BYTE op) {
    // returns the number of cycles for the instruction

    BYTE val;
    BYTE reg_num;
    BYTE first; // for two immediates 
    BYTE second; // for two immediates
    WORD addr; // combines the two immediates
    WORD val_16;
    switch (op) {
        // 8 BIT LOAD (total 19)
        case 0xFA : // ld A, nn
            first = read_memory(program_counter);
            program_counter++;
            second = read_memory(program_counter);
            program_counter++;
            addr = (second << 8) | first;
            val = read_memory(addr);
            set_reg_8(7, val); // loads to register A
            return 16;
        case 0xEA : // ld nn, A
            first = read_memory(program_counter);
            program_counter++;
            second = read_memory(program_counter);
            program_counter++;
            addr = (second << 8) | first;
            val = get_reg_value_8(7);
            write_memory(addr, val);
            return 16;
        
        case 0x36 : // ld HL, n
            val = read_memory(program_counter);
            program_counter++;
            write_memory(reg_HL.wrd, val);
            return 12;
        case 0xF0 : // ld A, FF00+n
            second = read_memory(program_counter);
            program_counter++;
            addr = 0xFF00 + second;
            val = read_memory(addr);
            set_reg_8(7, val);
            return 12;
        case 0xE0 : // ld FF00+n, A
            second = read_memory(program_counter);
            program_counter++;
            addr = 0xFF00 + second;
            val = get_reg_value_8(7);
            write_memory(addr, val);
            return 12;

        case 0x0A : // ld A, BC
            val = read_memory(reg_BC.wrd);
            set_reg_8(7, val);
            return 8;
        case 0x1A : // ld A, DE        
            val = read_memory(reg_DE.wrd);
            set_reg_8(7, val);
            return 8;
        case 0x02 : // ld BC, A
            val = get_reg_value_8(7);
            write_memory(reg_BC.wrd, val);
            return 8;
        case 0x12 : // ld DE, A
            val = get_reg_value_8(7);
            write_memory(reg_DE.wrd, val);
            return 8;
        case 0xF2 : // ld A, FF00+C
            addr = 0xFF00 + get_reg_value_8(1); // 1 is C
            val = read_memory(addr);
            set_reg_8(7, val);
            return 8;
        case 0xE2 : // ld FF00+C, A
            addr = 0xFF00 + get_reg_value_8(1); // 1 is C
            val = get_reg_value_8(7);
            write_memory(addr, val);
            return 8;
        case 0x22 : // ldi HL, A
            write_memory(reg_HL.wrd, reg_AF.hi);
            reg_HL.wrd += 1;
            return 8;
        case 0x2A : // ldi A, HL
            val = read_memory(reg_HL.wrd);
            set_reg_8(7, val);
            reg_HL.wrd += 1;
            return 8;
        case 0x32 : // ldd HL, A
            write_memory(reg_HL.wrd, reg_AF.hi);
            reg_HL.wrd -= 1;
            return 8;
        case 0x3A : // ldd A, HL
            val = read_memory(reg_HL.wrd);
            set_reg_8(7, val);
            reg_HL.wrd -= 1;
            return 8;

        
        // 16 BIT LOAD (total 4)
        case 0xF9 : // ld SP, HL
            set_reg_16(3, reg_HL.wrd);
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
            val = read_memory(reg_HL.wrd);
            execute_inc(0, val, reg_HL.wrd, true);
            return 12;
        case 0x35 : // dec HL
            val = read_memory(reg_HL.wrd);
            execute_dec(0, val, reg_HL.wrd, true);
            return 12;

        // 16 BIT ARITHMETIC/LOGICAL
        case 0xE8 : // add SP, dd
            val = read_memory(program_counter);
            program_counter++;
            stack_pointer.wrd += val;
            return 16;
        
        case 0xF8 : // ld HL, SP+dd
            val = read_memory(program_counter);
            program_counter++;
            reg_HL.wrd = stack_pointer.wrd + val;
            return 12;
        
        // ROTATE AND SHIFT
        // 7 is the A register
        case 0x07 : // rlca
            val = get_reg_value_8(7);
            execute_left_shift_rotate(7, 0, val, true, 0);
            return 4; 
        case 0x17 : // rla
            val = get_reg_value_8(7);
            execute_left_shift_rotate(7, 0, val, true, 1);
            return 4;
        case 0x0F : // rrca
            val = get_reg_value_8(7);
            execute_right_shift_rotate(7, 0, val, true, 0);
            return 4;
        case 0x1F : // rra
            val = get_reg_value_8(7);
            execute_right_shift_rotate(7, 0, val, true, 1);
            return 4;
        
        // CPU CONTROL
        case 0x3F : // ccf
            set_flag(FLAG_C, ~get_flag(FLAG_C));
            return 4;
        case 0x37 : // scf
            set_flag(FLAG_C, 1);
            return 4;
        case 0x00 : // nop
            return 4;
        case 0xF3 : // di
            // disables interrupts after 1 instruction delay
            intrpt_next_inst = 0;
            en_interrupt = 0;
            return 4;
        case 0xFB : // ei
            // enable interrupts after 1 instruction delay
            intrpt_next_inst = 0;
            en_interrupt = 1;
            return 4;
            
        case 0x76 : // halt
            halt = false;
            return 4;
        //case 0x10 00 : // stop

        // JUMP COMMANDS
        case 0xC3 : // jp nn
            first = read_memory(program_counter);
            program_counter++;
            second = read_memory(program_counter);
            program_counter++;
            addr = (second << 8) | first;
            program_counter = addr;
            return 12;
        case 0xC9 : // ret
            addr = pop_word_from_stack();
            program_counter = addr;
            return 16;
        case 0xD9 : // reti
            addr = pop_word_from_stack();
            program_counter = addr;
            // enable interrupts
            en_interrupt = 1;
            intrpt_next_inst = 0;
            return 16;
        
        case 0xE9 : // jp HL
            program_counter = reg_HL.wrd;
            return 4;

        case 0x18 : // jr PC+dd
            val = read_memory(program_counter);
            program_counter++;
            program_counter += val;
            return 12;

        case 0xCD : // call nn
            first = read_memory(program_counter);
            program_counter++;
            second = read_memory(program_counter);
            program_counter++;
            addr = (second << 8) | first;
            // pushes next instruction address onto stack
            push_word_onto_stack(program_counter);
            // jumps to address at nn
            program_counter = addr;
            return 24;
        
        case 0xCB : 
            return execute_extended_opcode();
    }

    if ((op & ld_hl_r_mask) == 0b01110000) {
        val = get_reg_value_8(op & 0b111);
        write_memory(reg_HL.wrd, val);
        return 8;
    } else if ((op & ld_r_hl_mask) == 0b01000110) {
        val = read_memory(reg_HL.wrd);
        reg_num = (op >> 3) & 0b111;
        set_reg_8(reg_num, val);
        return 8;
    } else if ((op & ld_r_r_mask) == 0b01000000) {
        val = get_reg_value_8(op & 0b111);
        reg_num = (op >> 3) & 0b111;
        set_reg_8(reg_num, val);
        return 4;
    } else if ((op & ld_r_n_mask) == 0b00000110) {
        val = read_memory(program_counter);
        program_counter++;
        reg_num = (op >> 3) & 0b111;
        set_reg_8(reg_num, val);
        return 8;
    } else if ((op & ld_rr_nn_mask) == 0b00000001) {
        first = read_memory(program_counter);
        program_counter++;
        second = read_memory(program_counter);
        program_counter++;
        val_16 = (second << 8) | first;
        reg_num = (op >> 4) & 0b11;
        set_reg_16(reg_num, val_16);
        return 12;
    } else if ((op & push_rr_mask) == 0b11000101) {
        val_16 = get_reg_value_16((op >> 4) & 0b11);
        push_word_onto_stack(val_16);
        return 16;
    } else if ((op & pop_rr_mask) == 0b11000001) {
        val_16 = pop_word_from_stack();
        reg_num = (op >> 4) & 0b11;
        set_reg_16(reg_num, val_16);
        return 12;
    } else if ((op & add_A_r_mask) == 0b10000000) {
        val = get_reg_value_8(op & 0b111);
        execute_add(false, val);
        return 4;
    } else if ((op & adc_A_r_mask) == 0b10001000) {
        val = get_reg_value_8(op & 0b111);
        execute_add(true, val);
        return 4;
    } else if ((op & sub_r_mask) == 0b10010000) {
        val = get_reg_value_8(op & 0b111);
        execute_sub(false, val);
        return 4;
    } else if ((op & sbc_A_r_mask) == 0b10011000) {
        val = get_reg_value_8(op & 0b111);
        execute_sub(true, val);
        return 4;
    } else if ((op & cp_r_mask) == 0b10111000) {
        val = get_reg_value_8(op & 0b111);
        execute_cp(val);
        return 4;
    } else if ((op & inc_r_mask) == 0b00000100) {
        reg_num = (op >> 3) & 0b111;
        val = get_reg_value_8(reg_num);
        execute_inc(reg_num, val, 0, false);
        return 4;
    } else if ((op & dec_r_mask) == 0b00000101) {
        reg_num = (op >> 3) & 0b111;
        val = get_reg_value_8(reg_num);
        execute_dec(reg_num, val, 0, false);
        return 4;
    } else if ((op & and_r_mask) == 0b10100000) {
        val = get_reg_value_8(op & 0b111);
        execute_and(val);
        return 4;
    } else if ((op & or_r_mask) == 0b10110000) {
        val = get_reg_value_8(op & 0b111);
        execute_or(val);
        return 4;
    } else if ((op & xor_r_mask) == 0b10101000) {
        val = get_reg_value_8(op & 0b111);
        execute_xor(val);
        return 4;
    } else if ((op & inc_rr_mask) == 0b000000011) {
        reg_num = (op >> 4) & 0b11;
        execute_inc_rr(get_register(reg_num));
        return 8;
    } else if ((op & dec_rr_mask) == 0b00001011) {
        reg_num = (op >> 4) & 0b11;
        execute_dec_rr(get_register(reg_num));
        return 8;
    } else if ((op & add_HL_rr_mask) == 0b00001001) {
        reg_num = (op >> 4) & 0b11;
        execute_add_HL_rr(get_register(reg_num));
        return 8;
    } else if ((op & jp_cc_nn_mask) == 0b11000010) {
        val = (op >> 3) & 0b11;
        first = read_memory(program_counter);
        program_counter++;
        second = read_memory(program_counter);
        program_counter++;
        addr = (second << 8) | first;
        if (check_flag(val)) {
            program_counter = addr;
            return 16;
        } else {
            return 12;
        }
    } else if ((op & jr_cc_e_mask) == 0b00100000) {
        val = (op >> 3) & 0b11;
        second = read_memory(program_counter);
        program_counter++;
        addr = program_counter + second;
        if (check_flag(val)) {
            program_counter = addr;
            return 12; 
        } else {
            return 8;
        }
    } else if ((op & call_cc_nn_mask) == 0b11000100) {
        val = (op >> 3) & 0b11;
        first = read_memory(program_counter);
        program_counter++;
        second = read_memory(program_counter);
        program_counter++;
        addr = (second << 8) | first;
        if (check_flag(val)) {
            push_word_onto_stack(program_counter);
            program_counter = addr;
            return 24;
        } else {
            return 12;
        }
    } else if ((op & ret_cc_mask) == 0b11000000) {
        val = (op >> 3) & 0b11;
        if (check_flag(val)) {
            addr = pop_word_from_stack();
            program_counter = addr;
            return 20;
        } else {
            return 8;
        }
    } else if ((op & rst_n_mask) == 0b11000111) {
        push_word_onto_stack(program_counter);
        val = (op >> 3) & 0b111;
        program_counter = val;
        return 16;
    }
    return 0;
}

void check_interrupt_enable() {
    // enables the interrupt swtich if the previous inst was EI/DI
    if (intrpt_next_inst == 1) {
        if (en_interrupt) {
            interrupt_switch = 1;
        } else {
            interrupt_switch = 0;
        }
        intrpt_next_inst = -1;
    } else if (intrpt_next_inst == 0) {
        intrpt_next_inst = 1;
    } else {
        intrpt_next_inst = -1; // no enable/disable
    }
}

int execute_next_opcode() {
    // returns the number of cycles for the instruction
    if(!halt) {
        BYTE opcode = read_memory(program_counter);
        program_counter++;
        int cycles = execute_opcode(opcode);
        check_interrupt_enable();
        return cycles;
    }
    return 0;
}


void update() {
    int current_cycle;
    while (current_cycle < 69905) {
        int new_cycles = execute_next_opcode();
        current_cycle += new_cycles;
        update_timers(new_cycles);
        update_graphics(new_cycles);
        do_interrupts();
    }
    render_screen();
}
#include "emulator.h"

void test_8(BYTE expected, BYTE actual) {
    if(expected != actual) 
        printf("FAILED: expected = %x, actual = %x\n", expected, actual);
    else
        printf("PASSED :)\n");
}

void test_16(WORD expected, WORD actual) {
    if(expected != actual) 
        printf("FAILED: expected = %x, actual = %x\n", expected, actual);
    else
        printf("PASSED :)\n");
}

void test_ld(BYTE expected, BYTE actual, WORD address) {
    if(expected != actual || reg_HL.wrd != address) {
        printf("FAILED: expected = %x, actual = %x, ", expected, actual);
        printf("reg_HL expected = %x, actual = %x\n", address, reg_HL.wrd);
    }
    else
        printf("PASSED :)\n");
}

void test_stack(WORD expected, WORD address) {
    BYTE lsb = read_memory(stack_pointer.wrd);
    BYTE msb = read_memory(stack_pointer.wrd+1);
    WORD actual = (msb << 8) | lsb;
    if(expected != actual || stack_pointer.wrd != address) {
        printf("FAILED: expected = %x, actual = %x, ", expected, actual);
        printf("SP expected = %x, actual = %x\n", address, stack_pointer.wrd);
    }
    else
        printf("PASSED :)\n");
}

void load_test() {
    printf("TEST LOADS\n");
    printf("%-23s", "ld B, 0x12");
    write_memory(program_counter, 0x06); // ld B, n
    write_memory(program_counter+1, 0x12); // n = 0x12
    execute_next_opcode();
    test_8(0x12, reg_BC.hi);

    printf("%-23s", "ld A, B");
    write_memory(program_counter, 0x78); // ld A, B
    execute_next_opcode();
    test_8(0x12, reg_AF.hi);

    WORD address = 0x9120;
    reg_HL.wrd = address;
    printf("%-23s", "ld C, (HL)");
    write_memory(address, 0xAB);
    write_memory(program_counter, 0x4E);
    execute_next_opcode();
    test_8(0xAB, reg_BC.lo);

    printf("%-23s", "ld (HL), B");
    write_memory(program_counter, 0x70);
    execute_next_opcode();
    test_8(0x12, read_memory(reg_HL.wrd));

    printf("%-23s", "ld (HL), 0xCD");
    write_memory(program_counter, 0x36);
    write_memory(program_counter+1, 0xCD);
    execute_next_opcode();
    test_8(0xCD, read_memory(reg_HL.wrd));

    printf("%-23s", "ld A, (BC)");
    address++;
    write_memory(program_counter, 0x0A);
    write_memory(address, 0x25);
    reg_BC.wrd = address;
    execute_next_opcode();
    test_8(0x25, reg_AF.hi);

    printf("%-23s", "ld A, (0x1234)");
    address++;
    write_memory(address, 0x35);
    write_memory(program_counter, 0xFA);
    write_memory(program_counter+1, 0x22);
    write_memory(program_counter+2, 0x91);
    execute_next_opcode();
    test_8(0x35, reg_AF.hi);

    printf("%-23s", "ld (BC), A");
    address++;
    reg_BC.wrd = address;
    write_memory(program_counter, 0x02);
    execute_next_opcode();
    test_8(0x35, read_memory(reg_BC.wrd));

    reg_AF.hi = 0xAC;
    printf("%-23s", "ld (address), A");
    write_memory(program_counter, 0xEA);
    write_memory(program_counter+1, 0x23);
    write_memory(program_counter+2, 0x91);
    execute_next_opcode();
    test_8(0xAC, read_memory(address));

    printf("%-23s", "ld A, (0xFF00 + 0x2)");
    write_memory(program_counter, 0xF0);
    write_memory(program_counter+1, 2);
    write_memory(0xFF02, 0x12);
    execute_next_opcode();
    test_8(0x12, reg_AF.hi);

    printf("%-23s", "(0xFF00 + 0x3), ld A");
    write_memory(program_counter, 0xE0);
    write_memory(program_counter+1, 3);
    execute_next_opcode();
    test_8(0x12, read_memory(0xFF03));

    printf("%-23s", "ld A, (0xFF00 + C)");
    reg_BC.lo = 8;
    write_memory(0xFF08, 0x23);
    write_memory(program_counter, 0xF2);
    execute_next_opcode();
    test_8(0x23, reg_AF.hi);

    printf("%-23s", "ld (0xFF00 + C), A");
    reg_BC.lo = 9;
    write_memory(program_counter, 0xE2);
    execute_next_opcode();
    test_8(0x23, read_memory(0xFF09));

    printf("%-23s", "ldi (HL), A");
    address++;
    reg_HL.wrd = address;
    write_memory(program_counter, 0x22);
    execute_next_opcode();
    test_ld(0x23, read_memory(address), address+1);

    printf("%-23s", "ldi A, (HL)");
    address++;
    write_memory(address, 0x34);
    write_memory(program_counter, 0x2A);
    execute_next_opcode();
    test_ld(0x34, reg_AF.hi, address+1);

    printf("%-23s", "ldd (HL), A");
    address++;
    write_memory(program_counter, 0x32);
    execute_next_opcode();
    test_ld(0x34, read_memory(address), address-1);

    printf("%-23s", "ldd A, (HL)");
    address--;
    write_memory(address, 0x45);
    write_memory(program_counter, 0x3A);
    execute_next_opcode();
    test_ld(0x45, reg_AF.hi, address-1);

    printf("%-23s", "ld BC, 0x4567");
    write_memory(program_counter, 0x01);
    write_memory(program_counter+1, 0x67);
    write_memory(program_counter+2, 0x45);
    execute_next_opcode();
    test_16(0x4567, reg_BC.wrd);

    printf("%-23s", "ld SP, HL");
    stack_pointer.wrd = 0;
    reg_HL.wrd = 0xFFFE;
    write_memory(program_counter, 0xF9);
    execute_next_opcode();
    test_16(0xFFFE, stack_pointer.wrd);

    printf("%-23s", "push BC");
    write_memory(program_counter, 0xC5);
    execute_next_opcode();
    test_stack(0x4567, 0xFFFC);

    printf("%-23s", "pop DE");
    write_memory(program_counter, 0xD1);
    execute_next_opcode();
    test_16(0x4567, reg_DE.wrd);
    printf("\n");
}

void daa_check_add(BYTE first, BYTE second, BYTE sum) {
    reg_AF.hi = 0;
    printf("A = %x + %-14x", first, second);
    write_memory(program_counter, 0xC6);
    write_memory(program_counter+1, first);
    execute_next_opcode();
    write_memory(program_counter, 0xC6);
    write_memory(program_counter+1, second);
    execute_next_opcode();
    write_memory(program_counter, 0x27);
    execute_next_opcode();
    test_8(sum, reg_AF.hi);
}

void daa_check_sub(BYTE first, BYTE second, BYTE diff) {
    reg_AF.hi = 0;
    printf("A = %02x - %-14x", first, second);
    write_memory(program_counter, 0xC6);
    write_memory(program_counter+1, first);
    execute_next_opcode();
    write_memory(program_counter, 0xD6);
    write_memory(program_counter+1, second);
    execute_next_opcode();
    write_memory(program_counter, 0x27);
    execute_next_opcode();
    test_8(diff, reg_AF.hi);
}

void daa_test() {
    printf("TEST DAA\n");
    
    daa_check_add(0x42, 0x35, 0x77);
    daa_check_add(0x42, 0x29, 0x71);
    daa_check_add(0x54, 0x28, 0x82);
    daa_check_add(0x98, 0x04, 0x02);
    daa_check_add(0x90, 0x80, 0x70);

    daa_check_sub(0x25, 0x12, 0x13);
    daa_check_sub(0x20, 0x13, 0x07);
    daa_check_sub(0x05, 0x21, 0x84);
    printf("\n");
}

void function_test() {
    /*
        ld A, 0x02
        ld B, 0x03
        add A, 0x01     // A = 3
        call f
        ld B, 0x05
        add A, 0x01     // A = 7
    f:
        add A, B        // A = 6
        ret
        add A, 0x01
    */

    printf("FUNCTION TEST\n");
    write_memory(program_counter, 0x3E);    // ld A, 0x02
    write_memory(program_counter+1, 0x02);
    write_memory(program_counter+2, 0x06);  // ld B, 0x03
    write_memory(program_counter+3, 0x03);
    write_memory(program_counter+4, 0xC6);  // add A, 0x01
    write_memory(program_counter+5, 0x01);
    write_memory(program_counter+6, 0xCD);  // call f
    WORD jump_to = program_counter+13;
    write_memory(program_counter+7, jump_to & 0xff);
    write_memory(program_counter+8, (jump_to >> 8) & 0xff);
    write_memory(program_counter+9, 0x06);  // ld B, 0x05
    write_memory(program_counter+10, 0x05);
    write_memory(program_counter+11, 0xC6); // add, 0x01
    write_memory(program_counter+12, 0x01);
    write_memory(program_counter+13, 0x80); // add A, B
    write_memory(program_counter+14, 0xC9); // ret
    write_memory(program_counter+15, 0xC6); // add, 0x01
    write_memory(program_counter+16, 0x01);
    printf("%-23s", "test 1");
    for(int i = 0; i < 8; i++) execute_next_opcode();
    test_8(7, reg_AF.hi);

    /*
        ld A, 0x02
        ld B, 0x03
        add A, 0x01     // A = 3, Z = 0
        call Z, f1      // shouldn't call
        add A, 0xff     // A = 2, C = 1
        ld B, 0xff
        call C, f2      // should call
        ld B, 0x07
        add A, 0x01     // A = 6
    f1:
        add A, B
        ret
        add A, 0x01
    f2:
        add A, B        // A = 1, C = 1
        ret NC
        add A, 0x04     // A = 5, C = 0
        ret NC
        add A, 0x01
    */
    program_counter = 0x9100;
    write_memory(program_counter, 0x3E);    // ld A, 0x02
    write_memory(program_counter+1, 0x02);
    write_memory(program_counter+2, 0x06);  // ld B, 0x03
    write_memory(program_counter+3, 0x03);
    write_memory(program_counter+4, 0xC6);  // add A, 0x01
    write_memory(program_counter+5, 0x01);
    write_memory(program_counter+6, 0xCC);  // call Z, f1
    jump_to = program_counter + 20;
    write_memory(program_counter+7, jump_to & 0xff);
    write_memory(program_counter+8, (jump_to >> 8) & 0xff);
    write_memory(program_counter+9, 0xC6);  // add A, 0xff
    write_memory(program_counter+10, 0xFF);
    write_memory(program_counter+11, 0x06); // ld B, 0xff
    write_memory(program_counter+12, 0xFF);
    write_memory(program_counter+13, 0xDC);  // call C, f2
    jump_to = program_counter + 24;
    write_memory(program_counter+14, jump_to & 0xff);
    write_memory(program_counter+15, (jump_to >> 8) & 0xff);
    write_memory(program_counter+16, 0x06); // ld B, 0x07
    write_memory(program_counter+17, 0x07);
    write_memory(program_counter+18, 0xC6);  // add A, 0x01
    write_memory(program_counter+19, 0x01);
    write_memory(program_counter+20, 0x80); // f1: add A, B
    write_memory(program_counter+21, 0xC9); // ret
    write_memory(program_counter+22, 0xC6); // add, 0x01
    write_memory(program_counter+23, 0x01);
    write_memory(program_counter+24, 0x80); // f2: add A, B
    write_memory(program_counter+25, 0xD0); // ret NC
    write_memory(program_counter+26, 0xC6); // add, 0x04
    write_memory(program_counter+27, 0x04);
    write_memory(program_counter+28, 0xD0); // ret NC
    write_memory(program_counter+29, 0xC6); // add, 0x01
    write_memory(program_counter+30, 0x01);
    printf("%-23s", "test 2");
    for(int i = 0; i < 13; i++) execute_next_opcode();
    test_8(6, reg_AF.hi);
    printf("\n");
}

int main(int argc, char const *argv[])
{
    program_counter = 0x9100;
    load_test();
    daa_test();
    function_test();

    return 0;
}
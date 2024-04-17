#include "emulator.h"

int main(int argc, char const *argv[])
{
    // TESTING: adding using an immediate from memory (pg 80)
    
    reg_AF.lo = 0x01;
    reg_AF.hi = 0xFF;
    
    WORD address = 0x9100; // should be unrestricted
    BYTE opcode = 0xC6;
    BYTE opcode_imm = 0x02;
    write_memory(address, opcode);
    write_memory(address + 1, opcode_imm);
    BYTE result = read_memory(address);
    printf("memory[address]: %X\n", result);
    result = read_memory(address + 1);
    printf("memory[address + 1]: %X\n", result);
    result = read_memory(address + 2);
    printf("memory[address + 2]: %X\n", result);
    program_counter = address;
    printf("%X\n", execute_opcode(read_memory(program_counter)));
    printf("result: %X\n", reg_AF.hi);
    printf("reg AF: %X\n", reg_AF.wrd);


    // TESTING: adding using a value from HL register (pg 80)
    address = 0x9120;
    reg_HL.wrd = address;
    BYTE mem_val = 0x02;
    write_memory(address, mem_val);
    reg_AF.hi = 0x01;
    opcode = 0x86;
    execute_opcode(opcode);
    printf("result: %X\n", reg_AF.hi);
    printf("reg AF: %X\n", reg_AF.wrd);
    
    
    
    // reg_AF.wrd = 0x01B0;
    // reg_BC.wrd = 0x0013;
    // reg_DE.wrd = 0x00D8;
    // reg_HL.wrd = 0x014D;

    // execute_opcode(0x87); // add a, a
    // printf("\n");
    // execute_opcode(0x97); // sub a, a
    // printf("\n");
    // execute_opcode(0x91); // sub a, c
    // printf("\n");

    // reg_AF.wrd = 0x01B0;
    // reg_BC.wrd = 0x0013;
    // reg_DE.wrd = 0x00D8;
    // reg_HL.wrd = 0x014D;
    // execute_opcode(0xA4); // and a, h -> a = 1
    // printf("A = %d\n", reg_AF.hi);
    // execute_opcode(0xA0); // and a, b -> a = 0
    // printf("A = %d\n", reg_AF.hi);
    // execute_opcode(0xA2); // and a, d -> a = 0
    // printf("A = %d\n", reg_AF.hi);

    // reg_AF.wrd = 0x0EB0;
    // reg_BC.wrd = 0x0013;
    // reg_DE.wrd = 0x00D8;
    // reg_HL.wrd = 0x014D;
    // //execute_opcode(0x07);
    // // execute_opcode(0x17);
    // // execute_opcode(0xCF);
    // //execute_opcode(0x1F);
    // execute_opcode(0x0F);
    

    return 0;
}

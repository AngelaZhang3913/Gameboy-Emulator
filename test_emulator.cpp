#include "emulator.h"

int main(int argc, char const *argv[])
{
    // TESTING: adding using an immediate from memory (pg 80)
    
    // set a value in memory to be the opcode using write_memory
    // set PC to that location in memory
    // execute_opcode(read_memory(program_counter));


    // TESTING: adding using a value from HL register (pg 80)
    
    // manually set the HL register to be equal to a memory address
    // set that value in memory to be the correct value using write_memory
    // execute_opcode(opcode);
    
    
    
    reg_AF.wrd = 0x01B0;
    reg_BC.wrd = 0x0013;
    reg_DE.wrd = 0x00D8;
    reg_HL.wrd = 0x014D;

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

    reg_AF.wrd = 0x0EB0;
    reg_BC.wrd = 0x0013;
    reg_DE.wrd = 0x00D8;
    reg_HL.wrd = 0x014D;
    //execute_opcode(0x07);
    // execute_opcode(0x17);
    // execute_opcode(0xCF);
    //execute_opcode(0x1F);
    execute_opcode(0x0F);
    

    return 0;
}

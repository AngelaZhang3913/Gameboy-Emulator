#include "emulator.h"

int main(int argc, char const *argv[])
{
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

    reg_AF.wrd = 0x0FB0;
    reg_BC.wrd = 0x0013;
    reg_DE.wrd = 0x00D8;
    reg_HL.wrd = 0x014D;
    execute_opcode(0x07);
    // execute_opcode(0x17);
    // execute_opcode(0xCF);
    

    return 0;
}

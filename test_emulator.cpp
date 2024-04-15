#include "emulator.cpp"

int main(int argc, char const *argv[])
{
    reg_AF.wrd = 0x01B0;
    reg_BC.wrd = 0x0013;
    reg_DE.wrd = 0x00D8;
    reg_HL.wrd = 0x014D;

    execute_opcode(0x87); // add a, a
    printf("\n");
    execute_opcode(0x97); // sub a, a
    printf("\n");
    execute_opcode(0x91); // add a, c
    return 0;
}

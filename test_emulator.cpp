#include "emulator.h"

int main(int argc, char const *argv[])
{
    // TESTING: adding using an immediate from memory (pg 80)
    
    // reg_AF.lo = 0x01;
    // reg_AF.hi = 0xFF;
    
    // WORD address = 0x9100; // should be unrestricted
    // BYTE opcode = 0xC6;
    // BYTE opcode_imm = 0x02;
    // write_memory(address, opcode);
    // write_memory(address + 1, opcode_imm);
    // BYTE result = read_memory(address);
    // printf("memory[address]: %X\n", result);
    // result = read_memory(address + 1);
    // printf("memory[address + 1]: %X\n", result);
    // result = read_memory(address + 2);
    // printf("memory[address + 2]: %X\n", result);
    // program_counter = address;
    
    // //execute_next_opcode();
    // print_result();
    
    // printf("%X\n", execute_opcode(read_memory(program_counter)));
    // printf("result: %X\n", reg_AF.hi);
    // printf("reg AF: %X\n", reg_AF.wrd);


    // // TESTING: adding using a value from HL register (pg 80)
    // address = 0x9120;
    // reg_HL.wrd = address;
    // BYTE mem_val = 0x02;
    // write_memory(address, mem_val);
    // reg_AF.hi = 0x01;
    // opcode = 0x86;
    // execute_opcode(opcode);
    // printf("result: %X\n", reg_AF.hi);
    // printf("reg AF: %X\n", reg_AF.wrd);
    
    
    
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

    // // reg_AF.wrd = 0xCCB0;
    // reg_AF.wrd = 0x40B0;
    // // reg_AF.wrd = 0x80B0;
    // // reg_AF.wrd = 0x01B0;
    // reg_BC.wrd = 0x0013;
    // // reg_BC.wrd = 0xCC13;
    // reg_DE.wrd = 0x00D8;
    // reg_HL.wrd = 0x014D;
    // // execute_opcode(0x1F);
    // execute_opcode(0x17);
    // // execute_opcode(0xCF);
    // //execute_opcode(0x1F);
    // // execute_opcode(0x0F);


    // testing the rotates with different registers :)
    // reg_AF.wrd = 0xCCB0;
    // reg_BC.wrd = 0xCC13;
    // reg_DE.wrd = 0x00D8;
    // reg_HL.wrd = 0x014D;
    // WORD address = 0x9100;
    // BYTE opcode = 0xCB;
    // BYTE opcode_imm = 0x00;
    // write_memory(address, opcode);
    // write_memory(address + 1, opcode_imm);
    // program_counter = address;
    // execute_next_opcode();
    
    // reg_AF.wrd = 0xCCB0;
    // reg_BC.wrd = 0x0040;
    // reg_DE.wrd = 0x00D8;
    // reg_HL.wrd = 0x014D;
    // WORD address = 0x9100;
    // BYTE opcode = 0xCB;
    // BYTE opcode_imm = 0x01;
    // write_memory(address, opcode);
    // write_memory(address + 1, opcode_imm);
    // program_counter = address;
    // execute_next_opcode();

    // reg_AF.wrd = 0xCCB0;
    // reg_BC.wrd = 0xCC13;
    // reg_DE.wrd = 0xCC40;
    // reg_HL.wrd = 0x014D;
    // WORD address = 0x9100;
    // BYTE opcode = 0xCB;
    // BYTE opcode_imm = 0x11;
    // // BYTE opcode_2 = 0xCB;
    // // BYTE opcode_imm_2 = 0x12;
    // write_memory(address, opcode);
    // write_memory(address + 1, opcode_imm);
    // // write_memory(address + 2, opcode_2);
    // // write_memory(address + 3, opcode_imm_2);
    // program_counter = address;
    // execute_next_opcode();
    // // printf("-------------\n");
    // // execute_next_opcode();

    // reg_AF.wrd = 0xCCB0;
    // reg_BC.wrd = 0x0040;
    // reg_DE.wrd = 0xCCD8;
    // reg_HL.wrd = 0x014D;
    // WORD address = 0x9100;
    // BYTE opcode = 0xCB;
    // BYTE opcode_imm = 0x12;
    // // BYTE opcode_2 = 0xCB;
    // // BYTE opcode_imm_2 = 0x12;
    // write_memory(address, opcode);
    // write_memory(address + 1, opcode_imm);
    // // write_memory(address + 2, opcode_2);
    // // write_memory(address + 3, opcode_imm_2);
    // program_counter = address;
    // execute_next_opcode();
    // execute_next_opcode();

    // // testing rotations with HL
    // WORD address = 0x9120;
    // reg_HL.wrd = address;
    // BYTE mem_val = 0xCC;
    // write_memory(address, mem_val);
    // BYTE pre_opcode = 0xCB;
    // BYTE opcode = 0x06;
    // WORD prog_addr = 0x9100;
    // write_memory(prog_addr, pre_opcode);
    // write_memory(prog_addr + 1, opcode);
    // program_counter = prog_addr;
    // execute_next_opcode();
    // // execute_opcode(opcode);
    // printf("result: %d\n", read_memory(address));
    // // reg_AF.hi = 0x01;
    // // opcode = 0x86;
    // // execute_opcode(opcode);
    // // printf("result: %X\n", reg_AF.hi);
    // // printf("reg AF: %X\n", reg_AF.wrd);

    // // shifts testing
    // reg_AF.wrd = 0xCCB0;
    // reg_BC.wrd = 0x0040;
    // reg_DE.wrd = 0xCCD8;
    // reg_HL.wrd = 0x014D;
    // BYTE pre_opcode = 0xCB;
    // BYTE opcode = 0x27;
    // WORD prog_addr = 0x9100;
    // write_memory(prog_addr, pre_opcode);
    // write_memory(prog_addr + 1, opcode);
    // program_counter = prog_addr;
    // execute_next_opcode();

    // WORD address = 0x9120;
    // reg_HL.wrd = address;
    // BYTE mem_val = 0xCC;
    // write_memory(address, mem_val);
    // BYTE pre_opcode = 0xCB;
    // BYTE opcode = 0x26;
    // WORD prog_addr = 0x9100;
    // write_memory(prog_addr, pre_opcode);
    // write_memory(prog_addr + 1, opcode);
    // program_counter = prog_addr;
    // execute_next_opcode();
    // printf("result: %d\n", read_memory(address));

    // reg_AF.wrd = 0xC0B0;
    // reg_BC.wrd = 0x0040;
    // reg_DE.wrd = 0xCCD8;
    // reg_HL.wrd = 0x014D;
    // BYTE pre_opcode = 0xCB;
    // BYTE opcode = 0x37;
    // WORD prog_addr = 0x9100;
    // write_memory(prog_addr, pre_opcode);
    // write_memory(prog_addr + 1, opcode);
    // program_counter = prog_addr;
    // execute_next_opcode();

    // // testing regular AND
    // reg_AF.wrd = 0xC0B0;
    // reg_BC.wrd = 0x0040;
    // reg_DE.wrd = 0xCCD8;
    // reg_HL.wrd = 0x014D;
    // execute_opcode(0xA7);
    // // execute_opcode(0xA0);
    // // execute_opcode(0xA1);
    // execute_opcode(0xA2);
    // return 0;

    // // testing AND with immediate
    // reg_AF.wrd = 0xC0B0;
    // reg_BC.wrd = 0x0040;
    // reg_DE.wrd = 0xCCD8;
    // reg_HL.wrd = 0x014D;
    // WORD prog_addr = 0x9100;
    // BYTE opcode = 0xE6;
    // write_memory(prog_addr, opcode);
    // write_memory(prog_addr + 1, 0xCC);
    // program_counter = prog_addr;
    // execute_next_opcode();

    // testing and with HL
    // WORD address = 0x9120;
    // reg_AF.wrd = 0xC0B0;
    // reg_BC.wrd = 0x0040;
    // reg_DE.wrd = 0xCCD8;
    // reg_HL.wrd = 0x014D;
    // reg_HL.wrd = address;
    // BYTE mem_val = 0xCC;
    // write_memory(address, mem_val);
    // BYTE opcode = 0xA6;
    // WORD prog_addr = 0x9100;
    // write_memory(prog_addr, opcode);
    // program_counter = prog_addr;
    // execute_next_opcode();

    // // testing regular XOR
    // reg_AF.wrd = 0xC0B0;
    // reg_BC.wrd = 0x0040;
    // reg_DE.wrd = 0xCCD8;
    // reg_HL.wrd = 0x014D;
    // // execute_opcode(0xAF);
    // // execute_opcode(0xA8);
    // execute_opcode(0xAA);
    // return 0;

    // // testing regular OR
    // reg_AF.wrd = 0xC0B0;
    // reg_BC.wrd = 0x0040;
    // reg_DE.wrd = 0xCCD8;
    // reg_HL.wrd = 0x014D;
    // // execute_opcode(0xB7);
    // // execute_opcode(0xB0);
    // execute_opcode(0xB2);

    // // testing register CP
    // reg_AF.wrd = 0xC0B0;
    // reg_BC.wrd = 0x0040;
    // reg_DE.wrd = 0xCCD8;
    // reg_HL.wrd = 0x014D;
    // // execute_opcode(0xBF);
    // // execute_opcode(0xB8);
    // // execute_opcode(0xBA);

    // // testing inc with registers
    // reg_AF.wrd = 0xC0B0;
    // // reg_BC.wrd = 0x0040;
    // reg_BC.wrd = 0x0F40;
    // reg_DE.wrd = 0xCCD8;
    // reg_HL.wrd = 0x014D;
    // // execute_opcode(0x3C);
    // execute_opcode(0x04);

    // // testing inc HL
    // WORD address = 0x9120;
    // reg_AF.wrd = 0xC0B0;
    // reg_BC.wrd = 0x0040;
    // reg_DE.wrd = 0xCCD8;
    // reg_HL.wrd = 0x014D;
    // reg_HL.wrd = address;
    // BYTE mem_val = 0xCB;
    // write_memory(address, mem_val);
    // BYTE opcode = 0x34;
    // WORD prog_addr = 0x9100;
    // write_memory(prog_addr, opcode);
    // program_counter = prog_addr;
    // execute_next_opcode();
    // printf("result: %d\n", read_memory(address));

    // // testing dec with registers
    // reg_AF.wrd = 0xC0B0;
    // reg_BC.wrd = 0x0F40;
    // reg_DE.wrd = 0xCCD8;
    // reg_HL.wrd = 0x014D;
    // execute_opcode(0x3D);

    // // testing daa
    // reg_AF.wrd = 0x7CB0;
    // execute_opcode(0x27);

    // // testing bit
    // BYTE pre_opcode = 0xCB;
    // BYTE opcode = 0b01001000; // gets bit 1 of register B
    // reg_BC.wrd = 0x0240;
    // WORD prog_addr = 0x9100;
    // write_memory(prog_addr, pre_opcode);
    // write_memory(prog_addr + 1, opcode);
    // program_counter = prog_addr;
    // execute_next_opcode();

    // // testing set
    // BYTE pre_opcode = 0xCB;
    // BYTE opcode = 0b11001000; // sets bit 1 of register B
    // WORD prog_addr = 0x9100;
    // reg_BC.wrd = 0xFD40;
    // write_memory(prog_addr, pre_opcode);
    // write_memory(prog_addr + 1, opcode);
    // program_counter = prog_addr;
    // execute_next_opcode();

    // // testing reset
    // BYTE pre_opcode = 0xCB;
    // BYTE opcode = 0b10001000; // resets bit 1 of register B
    // WORD prog_addr = 0x9100;
    // reg_BC.wrd = 0x1240;
    // write_memory(prog_addr, pre_opcode);
    // write_memory(prog_addr + 1, opcode);
    // program_counter = prog_addr;
    // execute_next_opcode();

    // // testing inc rr
    // BYTE opcode = 0b00000011; // incrementing register BC
    // reg_BC.wrd = 0x0001;
    // printf("result = %d\n", reg_BC.wrd);
    // execute_opcode(opcode);
    // printf("result = %d\n", reg_BC.wrd);

    // testing dec rr
    BYTE opcode = 0b00001011; // decrementing register BC
    reg_BC.wrd = 0x0001;
    printf("result = %d\n", reg_BC.wrd);
    execute_opcode(opcode);
    printf("result = %d\n", reg_BC.wrd);

    return 0;
}

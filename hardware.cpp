#include "hardware.h"
#include "timer.h"
#include "joypad.h"
#include <cstring>
#include <iostream>
using namespace std;

Register reg_AF = {0x01B0};
Register reg_BC = {0x0013};
Register reg_DE = {0x00D8};
Register reg_HL = {0x014D};

WORD program_counter = 0;
Register stack_pointer = {0xFFFE};

BYTE rom[0x10000];

void initialize_rom() {
    rom[0xFF05] = 0x00;
    rom[0xFF06] = 0x00;
    rom[0xFF07] = 0x00;
    rom[0xFF10] = 0x80;
    rom[0xFF11] = 0xBF;
    rom[0xFF12] = 0xF3;
    rom[0xFF14] = 0xBF;
    rom[0xFF16] = 0x3F;
    rom[0xFF17] = 0x00;
    rom[0xFF19] = 0xBF;
    rom[0xFF1A] = 0x7F;
    rom[0xFF1B] = 0xFF;
    rom[0xFF1C] = 0x9F;
    rom[0xFF1E] = 0xBF;
    rom[0xFF20] = 0xFF;
    rom[0xFF21] = 0x00;
    rom[0xFF22] = 0x00;
    rom[0xFF23] = 0xBF;
    rom[0xFF24] = 0x77;
    rom[0xFF25] = 0xF3;
    rom[0xFF26] = 0xF1;
    rom[0xFF40] = 0x91;
    rom[0xFF42] = 0x00;
    rom[0xFF43] = 0x00;
    rom[0xFF45] = 0x00;
    rom[0xFF47] = 0xFC;
    rom[0xFF48] = 0xFF;
    rom[0xFF49] = 0xFF;
    rom[0xFF4A] = 0x00;
    rom[0xFF4B] = 0x00;
    rom[0xFFFF] = 0x00;
}

BYTE current_rom_bank = 1;
BYTE ram_banks[0x8000];
BYTE current_ram_bank = 0;
BYTE cartridge_memory[0x200000];
BYTE game_memory[0x80000];

bool enable_ram;
bool rom_banking;

/* -------------
     MEMORY
 ------------- */

// write memory

void handle_banking(WORD address, BYTE data) {
    if (address < 0x2000) {
        // do RAM bank enable
        // allows for the game to write to RAM
        BYTE test_data = data & 0xF;
        if (test_data == 0xA) enable_ram = true;
        else if (test_data == 0x0) enable_ram = false;

    } else if (address >= 0x2000 && address < 0x4000) {
        // change lower bits of rom bank
        current_rom_bank &= 224;
        current_rom_bank |= data & 31;
        if (current_rom_bank == 0) current_rom_bank++;

    } else if (address >= 0x4000 && address < 0x6000) {
        // change higher bits of rom bank
        if (rom_banking) {
            current_rom_bank &= 31;
            current_rom_bank |= data & 224;
            if (current_rom_bank == 0) current_rom_bank++;
        }
        // change ram bank
        else {
            current_ram_bank = data & 0x3;
        }
    } else if (address >= 0x6000 && address < 0x8000) {
        // change mode between rom and ram
        rom_banking = ((data & 0x1) == 0) ? true : false;
        if (rom_banking) current_ram_bank = 0;
    }
}

void initialize_ram_bank() {
    memset(&ram_banks, 0, sizeof(ram_banks));
}

BYTE read_memory(WORD address) {
    if (address >= 0 && address <= 0x3FFF) {
        return cartridge_memory[address];
    } else if ((address >= 0x4000) && (address <= 0x7FFF)) {
        // rom banking
        return cartridge_memory[address - 0x4000 + current_rom_bank * 0x4000];
    } else if ((address >= 0xA000) && (address <= 0xBFFF)){
        // ram banking
        return ram_banks[address - 0xA000 + current_ram_bank * 0x4000];
    } else if (address == 0xFF00) {
        // return joypad state
        // return get_joypad_state();
        return 0;
    } else {
        return rom[address];
    }
}

// direct memory access
void dma_transfer(BYTE data) {
    WORD source_address = data << 8;
    // copies the memory from the source address into 0xFE00-0xFE9F
    for (int i = 0x0; i < 0xA0; i++) {
        write_memory(0xFE00 + i, read_memory(source_address + i));
    }
}

void write_memory(WORD address, BYTE data) {
    if (address == 0xFF40) {
        printf("ADDRESS IS FF40");
    }

    // printf("WRITING TO MEMORY %0X\n", address);

    if (address < 0x8000) {
        // ROM banking
        handle_banking(address, data);
    }
    else if (address >= 0xA000 && address < 0xC000) {
        if (enable_ram) {
            ram_banks[address - 0xA000 + (current_ram_bank*0x2000)] = data;
        }
    } else if (address >= 0xE000 && address < 0xFE00) {
        // echo RAM: also write in RAM section
        // don't need to implement
        rom[address] = data ;
        write_memory(address-0x2000, data) ;
    } else if (address >= 0xFEA0 && address < 0xFEFF) {
        // restricted - don't edit
    } else if (address == 0xFF04) {
        // divider register is restricted (don't edit)
        rom[0xFF04] = 0 ;
    } else if (address == TMC) {
        // trying to change timer controller
        BYTE current_frequency = get_clock_frequency();
        game_memory[TMC] = current_frequency;
        BYTE new_frequency = get_clock_frequency();

        // check if the new frequency is different
        if (current_frequency != new_frequency) {
            set_clock_frequency();
        }
    } else if (address == 0xFF44) {
        // can't write to scan line memory address
        rom[address] = 0 ;
    } else if (address == 0xFF46) {
        dma_transfer(data);
    } else {
        // no restriction
        if(address >= 0x8000 && address <= 0x97ff)
            printf("address = %x, data = %x\n", address, data);
        rom[address] = data;
    }
}
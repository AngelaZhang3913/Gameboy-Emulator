#include <hardware.h>
#include <cstring>
#include <iostream>
using namespace std;

/* -------------
     MEMORY
 ------------- */

// write memory

void write_memory(WORD address, BYTE data) {
    if (address < 0x8000) {
        // ROM banking - make another function
        handle_banking(address, data);
    } else if (address <= 0xE000 && address < 0xFE00) {
        // echo RAM: also write in RAM section
        // don't need to implement
    } else if (address >= 0xFEA0 && address < 0xFEFF) {
        // restricted ??? still don't do anything
    } else {
        // no restriction
        rom[address] = data;
    }
}

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
        rom_banking = (data & 0x1 == 0) ? true : false;
        if (rom_banking) current_ram_bank = 0;
    }
}

void initialize_ram_bank() {
    memset(&ram_banks, 0, sizeof(ram_banks));
}

BYTE read_memory(WORD address) {
    if ((address >= 0x4000) && (address <= 0x7FFF)) {
        // rom banking
        return cartridge_memory[address - 0x4000 + current_rom_bank * 0x4000];
    } else if ((address >= 0xA000) && (address <= 0xBFFF)){
        // ram banking
        return ram_banks[address - 0xA000 + current_ram_bank * 0x4000];
    } else {
        return rom[address];
    }
}

bool test_bit(WORD word, int index) {
    return ((word >> index) & 1) == 1;
}

bool test_bit(BYTE byte, int index) {
    return ((byte >> index) & 1) == 1;
}

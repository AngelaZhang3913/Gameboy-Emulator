#include <hardware.h>

/* -------------
     MEMORY
 ------------- */

// write memory

write_memory(WORD address, BYTE data) {
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

handle_banking(WORD address, BYTE data) {
    if (address < 0x2000) {

    } else if (address >= 0x200 && address < 0x4000) {

    } else if (address >= 0x4000 && address < 0x6000) {

    } else if (address >= 0x6000 && address < 0x8000) {
        
    }
}

initialize_ram_bank() {
    memset(&ram_banks, 0, sizeof(ram_banks));
}

BYTE read_memory(WORD address) const {
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

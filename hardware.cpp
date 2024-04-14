#include <hardware.h>

/* -------------
     MEMORY
 ------------- */

// write memory

write_memory(WORD address, BYTE data) {
    if (address < 0x8000) {
        // ROM banking - make another function
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

initialize_ram_bank() {
    memset(&ram_banks, 0, sizeof(ram_banks));
}

BYTE read_memory(WORD address) const {
    if ((address >= 0x4000) && (address <= 0x7FFF)) {
        
    }
}


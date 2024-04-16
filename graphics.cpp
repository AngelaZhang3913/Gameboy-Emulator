#include "typedef.h"
#include "hardware.h"
#include "interrupts.h"

int scanline_counter = 0;

void update_graphics(int cycles) {
    set_lcd_status();

    // check if lcd is enabled
    if (is_lcd_enabled()) {
        scanline_counter -= cycles;
    } else {
        return;
    }

    if (scanline_counter <= 0) {
        scanline_counter = 456;
        rom[0xFF44]++; // current scan line is stored in 0xFF44
        BYTE current_scanline = read_memory(0xFF44);

        if (current_scanline == 144) {
            // vertical blank
            request_interrupts(0);
        } else if (current_scanline >= 153) {
            // reset scan line
            rom[0xFF44] = 0;
        } else if (current_scanline < 144) {
            //draw_scanline(); // need to write
        }
    }
}

/*
    00: H-Blank
    01: V-Blank
    10: Searching Sprites Atts
    11: Transfering Data to LCD Driver
*/

/*
    Notes: 
    0xFF41 - LCD status register
     - Bit 2 is set to 1 if register (0xFF44) is the same value as (0xFF45) otherwise it is set to 0
     - Bit 6 is the same as the interrupt enabled bits 3-5 but it isnt to do with the current lcd mode it is to do with the bit 2 coincidence flag
     - 
*/

void set_lcd_status() {
    BYTE current_status = read_memory(0xFF41);

    if (!is_lcd_enabled()) {
        // set mode to one and reset scan line
        scanline_counter = 456;
        rom[0xFF44] = 0;
        
    }
}

bool is_lcd_enabled() {
    return test_bit(read_memory(0xFF40), 7);
}
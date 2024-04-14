#include "typdef.h"

int scanline_counter = 0;

void update_graphics(int cycles) {
    set_lcd_status();

    // check if lcd is enabled
    if (is_lcd_enabled()) {
        scanline_counter -= cycles;
    } else {
        return;
    }

    if (scaline_counter <= 0) {
        scanline_counter = 456;
        rom[0xFF44]++; // current scan line is stored in 0xFF44
        BYTE current_scanline = read_memory(0xFF44);

        if (current_scanline == 144) {
            // vertical blank
            request_interupt(0);
        } else if (current_scanline >= 153) {
            // reset scan line
            rom[0xFF44] = 0;
        } else if (current_scanline < 144) {
            draw_scanline();
        }
    }
}

/*
    00: H-Blank
    01: V-Blank
    10: Searching Sprites Atts
    11: Transfering Data to LCD Driver
*/

void set_lcd_status() {
    BYTE current_status = read_memory(0xFF41);

    if (!is_lcd_enabled()) {
        
    }
}

bool is_lcd_enabled() {
    return is_bit_true(read_memory(0xFF40), 7);
}
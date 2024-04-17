#include "typedef.h"
#include "hardware.h"
#include "interrupts.h"

int scanline_counter = 0; // when to move on to the next line

void update_graphics(int cycles) {
    set_lcd_status();

    // check if lcd is enabled
    if (is_lcd_enabled()) {
        scanline_counter -= cycles;
    } else {
        return;
    }

    if (scanline_counter <= 0) {
        // move on to the next scan line

        scanline_counter = 456;
        rom[0xFF44]++; // current scan line is stored in 0xFF44
        BYTE current_scanline = read_memory(0xFF44);

        if (current_scanline == 144) {
            // vertical blank
            request_interrupt(0);
        } else if (current_scanline >= 153) {
            // reset scan line
            rom[0xFF44] = 0;
        } else if (current_scanline < 144) {
            draw_scanline(); // need to write
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
     - Bit 3: Mode 0 Interupt Enabled
     - Bit 4: Mode 1 Interupt Enabled
     - Bit 5: Mode 2 Interupt Enabled


    0xFF44 - current scan line

     LCD Control Register - 0xFF40
     Bit 7 - LCD Display Enable (0=Off, 1=On)
        Bit 6 - Window Tile Map Display Select (0=9800-9BFF, 1=9C00-9FFF)
        Bit 5 - Window Display Enable (0=Off, 1=On)
        Bit 4 - BG & Window Tile Data Select (0=8800-97FF, 1=8000-8FFF)
        Bit 3 - BG Tile Map Display Select (0=9800-9BFF, 1=9C00-9FFF)
        Bit 2 - OBJ (Sprite) Size (0=8x8, 1=8x16)
        Bit 1 - OBJ (Sprite) Display Enable (0=Off, 1=On)
        Bit 0 - BG Display (for CGB see below) (0=Off, 1=On)

    ScrollY (0xFF42): The Y Position of the BACKGROUND where to start drawing the viewing area from
    ScrollX (0xFF43): The X Position of the BACKGROUND to start drawing the viewing area from
    WindowY (0xFF4A): The Y Position of the VIEWING AREA to start drawing the window from
    WindowX (0xFF4B): The X Positions -7 of the VIEWING AREA to start drawing the window from
*/

void set_lcd_status() {
    BYTE current_status = read_memory(0xFF41);

    if (!is_lcd_enabled()) {
        // set mode to one and reset scan line
        scanline_counter = 456;
        rom[0xFF44] = 0;
        current_status &= 252 ; // zero out the lower two bits
        current_status = bitset(current_status, 0) ;
        write_memory(0xFF41, current_status) ; // reset current scan line
        return ;
    }

    BYTE current_scanline = read_memory(0xFF44);
    BYTE current_mode = current_status & 3; // lower two bits
    BYTE new_mode = 0;
    bool should_request_interrupt = false;

    if (current_scanline >= 144) {
        // vblank mode (01)
        new_mode = 1;
        current_status = bitset(current_status, 0) ;
        current_status = bitreset(current_status, 1) ;
        should_request_interrupt = test_bit(current_status, 4); // interrupt enabled for mode 1 controlled by bit 4
    } else {
        if (scanline_counter >= 376) {
            // Searching Sprites Atts
            new_mode = 2 ;
            current_status = bitset(current_status, 1) ;
            current_status = bitreset(current_status, 0) ;
            should_request_interrupt = test_bit(current_status, 5) ;
        } else if (scanline_counter >= 204) {
            new_mode = 3;
            current_status = bitset(current_status, 1) ;
            current_status = bitset(current_status, 0) ;
        } else {
            new_mode = 0;
            current_status = bitreset(current_status,1) ;
            current_status = bitreset(current_status,0) ;
            should_request_interrupt = test_bit(current_status, 3) ;
        }
    }

    if (should_request_interrupt && (current_mode != new_mode)) {
        // request interrupt since a new mode was entered
        request_interrupt(1);
    }

    if (current_scanline == read_memory(0xFF45)) {
        // current scanline is the same as the scanline the game is interested in (for special effects)
        current_status = bitset(current_status, 2) ;
        if (test_bit(current_status, 6)) {
            request_interrupt(1);
        } 
    } else {
        current_status = bitreset(current_status, 2) ;
    }
    
    write_memory(0xFF41, current_status);
}

bool is_lcd_enabled() {
    return test_bit(read_memory(0xFF40), 7);
}

void draw_scanline() {
    BYTE lcd_control_reg = read_memory(0xFF40);
    if (test_bit(lcd_control_reg, 0)) {
        render_tiles();
    }

    if (test_bit(lcd_control_reg, 1)) {
        render_sprites();
    }
}

void render_tiles() {

}

void render_sprites() {
    
}
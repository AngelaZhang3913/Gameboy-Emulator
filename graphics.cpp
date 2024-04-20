#include "typedef.h"
#include "hardware.h"
#include "interrupts.h"
#include <glut/glut.h>
#include "graphics.h"

enum COLOR { WHITE, LIGHT_GREY, DARK_GREY, BLACK};

int scanline_counter = 0; // when to move on to the next line
const WORD memory_region = 0x8800 ;
const int tile_size = 16 ;
const int offset = 128 ;

int tile_indentifier = 0;

WORD tile_address = memory_region + ((tile_indentifier+offset)*tile_size) ;

BYTE screen_data[160][144][3] ;

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

    pixel# = 1 2 3 4 5 6 7 8
    data 2 = 1 0 1 0 1 1 1 0
    data 1 = 0 0 1 1 0 1 0 1

    Pixel 1 colour id: 10
    Pixel 2 colour id: 00
    Pixel 3 colour id: 11
    Pixel 4 colour id: 01
    Pixel 5 colour id: 10
    Pixel 6 colour id: 11
    Pixel 7 colour id: 10
    Pixel 8 colour id: 01

    00: White
    01: Light Grey
    10: Dark Grey
    11: Black
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

COLOR get_color(BYTE num_color, WORD address) {
    BYTE palette = read_memory(address);
    int bit1 = num_color == 0 ? 1:
               num_color == 1 ? 3:
               num_color == 2 ? 5:
               7;

    int bit2 = num_color == 0 ? 0:
               num_color == 1 ? 2:
               num_color == 2 ? 4:
               6;

    int color_val = (get_bit(palette, bit1) << 1) | get_bit(palette, bit2);

    return  color_val == 1 ? LIGHT_GREY :
            color_val == 2 ? DARK_GREY :
            color_val == 3 ? BLACK :
            WHITE;
}

void render_tiles(BYTE lcd_control_reg) {
    WORD tile_data = 0 ;
    WORD background_memory = 0 ;
    bool unsign = true ;

    BYTE scrollY = read_memory(0xFF42) ;
    BYTE scrollX = read_memory(0xFF43) ;
    BYTE windowY = read_memory(0xFF4A) ;
    BYTE windowX = read_memory(0xFF4B) - 7;

    bool in_window = false;

    if (test_bit(lcd_control_reg, 5)) {
        // check that the window is enabled
        if (windowY <= read_memory(0xFF44)) {
            // check that the current scan line is within the window
            in_window = true ;
        }
    }

    // choose where the tile data is from
    if (test_bit(lcd_control_reg, 4)) {
        tile_data = 0x8000;
    } else {
        tile_data = 0x8800 ;
        unsign = false ;
    }

    // set the background memory
    if (!in_window) {
        background_memory = test_bit(lcd_control_reg, 3) ? 0x9C00 : 0x9800;
    } else {
        background_memory = test_bit(lcd_control_reg, 6) ? 0x9C00 : 0x9800;
    }

    BYTE y_position = in_window ? read_memory(0xFF44) - windowY : scrollY + read_memory(0xFF44);

    WORD tile_row = (((BYTE)(y_position/8))*32) ;

    // draw pixels for this scanline
    for (int x = 0 ; x < 160; x++) {
        BYTE x_position = x + scrollX ;

        // translate the current x pos to window space if necessary
        if (in_window) {
            if (x >= windowX) {
                x_position = x - windowX ;
            }
        }

        // determine which tile this pixel is in
        WORD tile_col = (x_position/8) ;
        WORD tile_address = background_memory + tile_row + tile_col;      
        SIGNED_WORD tile_num = unsign ? (BYTE)read_memory(tile_address) : (SIGNED_BYTE) read_memory(tile_address);

        WORD tile_location = unsign ? tile_data + tile_num * 16 : tile_data + (tile_num + 128) * 16;

        // determine the vertical location (each vertical line )
        BYTE line = (y_position % 8) * 2;
        BYTE data1 = read_memory(tile_location + line);
        BYTE data2 = read_memory(tile_location + line + 1);

        // pixel 0 = bit 7, pixel 1 = bit 6, etc
        int color_bit = (x_position % 8 - 7) * -1 ;
        int color_value = (get_bit(data2, color_bit) << 1) | get_bit(data1, color_bit);

        COLOR color = get_color(color_value, 0xFF47) ;
        int red = color == WHITE ? 255 :
                  color == LIGHT_GREY ? 0xCC :
                  color == DARK_GREY ? 0x77 :
                  0;
        int green = color == WHITE ? 255 :
                  color == LIGHT_GREY ? 0xCC :
                  color == DARK_GREY ? 0x77 :
                  0;
        int blue = color == WHITE ? 255 :
                  color == LIGHT_GREY ? 0xCC :
                  color == DARK_GREY ? 0x77 :
                  0;

        int y = read_memory(0xFF44);

        if ((y >= 0) && (y < 144) && (x >= 0) && (x < 160)) {
            screen_data[x][y][0] = red ;
            screen_data[x][y][1] = green ;
            screen_data[x][y][2] = blue ;
        }
    }
}

void render_sprites(BYTE lcd_control_reg) {

    for (int sprite = 0 ; sprite < 40; sprite++) {

        /*
        4 bytes of sprite attributes
        0: Sprite Y Position: Position of the sprite on the Y axis of the viewing display minus 16
        1: Sprite X Position: Position of the sprite on the X axis of the viewing display minus 8
        2: Pattern number: This is the sprite identifier used for looking up the sprite data in memory region 0x8000-0x8FFF
        3: Attributes: These are the attributes of the sprite, discussed later.
        */

        BYTE index = sprite * 4 ;
        BYTE yPos = read_memory(0xFE00 + index) - 16;
        BYTE xPos = read_memory(0xFE00 + index + 1) - 8;
        BYTE tileLocation = read_memory(0xFE00 + index + 2) ;
        BYTE attributes = read_memory(0xFE00 + index + 3) ;

        /*
        sprite attribute:
            Bit7: Sprite to Background Priority
            Bit6: Y flip - used to turn characters upside down
            Bit5: X flip - used to turn characters horizontally
            Bit4: Palette number
            Bit3: Not used in standard gameboy
            Bit2-0: Not used in standard gameboy
        */

        bool yFlip = test_bit(attributes, 6) ;
        bool xFlip = test_bit(attributes, 5) ;

        int scanline = read_memory(0xFF44);
        int y_size = test_bit(lcd_control_reg, 2) ? 16 : 8; // Bit 2 - OBJ (Sprite) Size (0=8x8, 1=8x16)

        // check if the sprite intercepts with scanline (should we draw)
        if ((scanline >= yPos) && (scanline < (yPos + y_size))) {
            int line = scanline - yPos ;
            if (yFlip) {
                line -= y_size ;
                line *= -1 ;
            }

            line *= 2; // same as for tiles
            WORD dataAddress = (0x8000 + (tileLocation * 16)) + line ;
            BYTE data1 = read_memory(dataAddress) ;
            BYTE data2 = read_memory(dataAddress + 1) ;

            for (int tilePixel = 7; tilePixel >= 0; tilePixel--) {
                int colorbit = tilePixel ;

                if (xFlip) {
                    colorbit -= 7 ;
                    colorbit *= -1 ;
                }

                int color_value = (get_bit(data2, colorbit) << 1) | get_bit(data1, colorbit);
                WORD color_address = test_bit(attributes, 4) ? 0xFF49 : 0xFF48 ;
                COLOR color = get_color(color_value, color_address) ;

                // white is transparent for sprites.
                if (color == WHITE) {
                    continue ;
                }

                int red =   color == WHITE ? 255 :
                            color == LIGHT_GREY ? 0xCC :
                            color == DARK_GREY ? 0x77 :
                            0;
                int green = color == WHITE ? 255 :
                            color == LIGHT_GREY ? 0xCC :
                            color == DARK_GREY ? 0x77 :
                            0;
                int blue =  color == WHITE ? 255 :
                            color == LIGHT_GREY ? 0xCC :
                            color == DARK_GREY ? 0x77 :
                            0;

                int x = xPos + 7 - tilePixel;

                // check in bounds
                int y = scanline;
                if ((y >= 0) && (y < 144) && (x >= 0) && (x < 160)) {
                    continue ;
                }

                screen_data[x][y][0] = red ;
                screen_data[x][y][1] = green ;
                screen_data[x][y][2] = blue ;
            }
        }
    }
}

void draw_scanline() {
    BYTE lcd_control_reg = read_memory(0xFF40);
    if (test_bit(lcd_control_reg, 0)) {
        render_tiles(lcd_control_reg);
    }

    if (test_bit(lcd_control_reg, 1)) {
        render_sprites(lcd_control_reg);
    }
}

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

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawPixels(WIDTH, HEIGHT, GL_RGB, GL_UNSIGNED_BYTE, screen_data);
    glutSwapBuffers(); 
}

void render_screen() {
    glutDisplayFunc(display);
}

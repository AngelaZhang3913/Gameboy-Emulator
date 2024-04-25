#include <cstring>
#include <iostream>
#include "joypad.h"
#include "bitmath.h"
#include "hardware.h"
#include "interrupts.h"

BYTE joypad_state = 0xFF;

void key_pressed(int key) {
    // 0 represents pressed key; 1 represented unpressed key
    bool previously_unset = false;

    if (!test_bit(joypad_state, key)) {
        previously_unset = true; // is the key already pressed (0)?
    }

    joypad_state = bitreset(joypad_state, key); // set state to 0

    bool standard_button = true;

    if (key >= 4 && key <= 7) {
        // standard button (4-7)
        standard_button = true;
    } else {
        // directional button (0-3)
        standard_button = false;
    }

    BYTE key_request = rom[0xFF00];
    bool req_interrupt = false;

    if (standard_button && !test_bit(key_request, 5)) {
        // game is looking for a standard button press
        req_interrupt = true;
    } else if (!standard_button && !test_bit(key_request, 4)) {
        // game is looking for a directional button press
        req_interrupt = true;
    }

    if (req_interrupt && !previously_unset) {
         // request an interrupt of type joypad
        request_interrupt(4);
    }

}
void key_released(int key) {
    joypad_state = bitset(joypad_state, key);
}

BYTE get_joypad_state() {
    BYTE result = rom[0xFF00];
    result ^= 0xFF; // flip bits

    if (!test_bit(result, 4)) {
        // directional buttons
        BYTE top_jp = joypad_state >> 4; // standard buttons in top nibble
        top_jp |= 0xF0; // turn on top 4 bits
        result &= top_jp; // returns pressed buttons
    } else if (!test_bit(result, 5)) {
        // standard buttons
        BYTE bottom_jp = joypad_state & 0xF; // directional buttons in bottom nibble
        bottom_jp |= 0xF0 ; // turn on top 4 bits
        result &= bottom_jp; // returns pressed buttons
    }

    //printf("result: %0X", result);
    return result;
}
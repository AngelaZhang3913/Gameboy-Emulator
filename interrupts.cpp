#include "interrupts.h"
#include "hardware.h"

void request_interrupt(int id) {
    // add comments
    BYTE request = read_memory(0xFF0F);
    request = bitset(request, id);

}

void do_interrupts() {
    if (interrupt_switch) {
        BYTE request = read_memory(0xFF0F);
        BYTE enabled = read_memory(0xFFFF);
        // checks for any pending interrupts
        if (request > 0) {
            // checks interrupts in their priority order
            for (int i = 0; i < 5; i++) {
                if (test_bit(request,i) && test_bit(enabled, i)) {
                    // executes the interrupt
                    sevice_interrupt(i);
                }
            }
        }
    }
}

void sevice_interrupt(int id) {
    interrupt_switch = false;
    BYTE request = read_memory(0xFF0F);
    request = bitset(request, id);
    write_memory(0xFF0F, request);

    // save PC to the stack
    push_word_onto_stack(program_counter);
    if (id == 0x40) {
        // V-blank
    }
    if (id == 0x48) {
        // LCD
    }
    if (id == 0x50) {
        // Timer
    }
    if (id == 0x60) {
        // Joypad
    }
}

void push_word_onto_stack(WORD wrd) {
    stack_pointer.wrd -= 1;
    write_memory(stack_pointer.wrd, wrd >> 8); // the higher bits
    
    stack_pointer.wrd -= 1;
    write_memory(stack_pointer.wrd, wrd & 0xFF); // the lower bits
}
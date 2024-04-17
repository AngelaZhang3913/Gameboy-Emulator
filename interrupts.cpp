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

    // push PC onto stack to save execution address
    // based on PC's current value, can determine what kind
    // of interrupt service routine it is
}
#include "interrupts.h"
#include "hardware.h"

void request_interrupt(int id) {
    // add comments
    BYTE request = read_memory(0xFF0F);
    request = bitset(request, id);

}
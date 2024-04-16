#include "interrupts.h"
#include "hardware.h"

void request_interrupts(int id) {
    // add comments
    BYTE request = read_memory(0xFF0F);
    request = bitset(request, id);

}

// set the specified bit to 1
BYTE bitset(BYTE byte, int bit) {
    return byte + 1 >> bit;
}

BYTE bitreset(BYTE byte, int bit) {
    return byte + 0 >> bit;
}
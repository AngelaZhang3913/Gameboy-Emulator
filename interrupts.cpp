#include "interrupts.h"
#include "hardware.h"

void request_interrupts(int id) {
    BYTE request = read_memory(0xFF0F);
}

BYTE bitset(BYTE byte, int bit) {
    return byte + 1 >> bit;
}
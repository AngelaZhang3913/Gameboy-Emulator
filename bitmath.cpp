#include "bitmath.h"

bool test_bit(WORD word, int index) {
    return ((word >> index) & 1) == 1;
}

bool test_bit(BYTE byte, int index) {
    return ((byte >> index) & 1) == 1;
}

// set the specified bit to 1
BYTE bitset(BYTE byte, int bit) {
    return ((1 << bit) | byte);
}

BYTE bitreset(BYTE byte, int bit) {
    return ((~(1 << bit)) & byte);
}

int get_bit(BYTE byte, int bit)  {
    return (byte >> bit) & 1;
}
#include "typedef.h"

bool test_bit(WORD word, int index) ;

bool test_bit(BYTE byte, int index) ;

// set the specified bit to 1
BYTE bitset(BYTE byte, int bit) ;

BYTE bitreset(BYTE byte, int bit) ;

int get_bit(BYTE byte, int bit) ;
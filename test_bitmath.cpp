#include "bitmath.h"
#include <iostream>

int main(int argc, char const *argv[]) {
    printf("%d", bitset(0b100000, 2)); // 36
    printf("%d",  bitset(0b100100, 2)); // 36
    printf("%d", bitreset(0b100100, 2)); // 32
    printf("%d", bitreset(0b100100, 1)); // 36
}
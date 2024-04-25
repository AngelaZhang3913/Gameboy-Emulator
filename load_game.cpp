#include <stdio.h>
#include "hardware.h"
#include <cstring>
#include <iostream>
using namespace std;
#include "typedef.h"
#include "load_game.h"

BYTE first_100 [0x100];
void load_game_method() {
    memset(cartridge_memory, 0, sizeof(BYTE));

    FILE *game_in;
    game_in = fopen("Tetris.gb", "rb" );
    fread(cartridge_memory, 1, 0x200000, game_in);
    memcpy(first_100, cartridge_memory, 0x100 * sizeof(BYTE));
    fclose(game_in);

    initialize_rom();

    //printf("hi%0X", read_memory(0x100));
}
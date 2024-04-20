#include <stdio.h>
#include "hardware.h"
#include <cstring>
#include <iostream>
using namespace std;
#include "typedef.h"
#include "load_game.h"

void load_game_method() {
    memset(cartridge_memory, 0, sizeof(BYTE)) ;

    FILE *game_in;
    game_in = fopen( "Mario.gb", "rb" );
    fread(cartridge_memory, 1, 0x200000, game_in);
    fclose(game_in);
}
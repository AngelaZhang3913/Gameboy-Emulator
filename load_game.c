#include <stdio.h>

BYTE cartridge_memory[0x200000] ;

memset(cartridge_memory, 0, sizeof(BYTE)) ;

FILE *game_in;
game_in = fopen( "Mario.gb", "rb" );
fread(cartridge_memory, 1, 0x200000, game_in);
fclose(game_in);
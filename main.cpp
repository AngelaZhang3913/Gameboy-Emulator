#include "hardware.h"
#include "load_game.h"
#include "emulator.h"

int main(int argc, char* argv[]) {
    load_game_method(); // possibly use args to determine which game to load
    while (true) {
        update();
        // sleep
    }
}
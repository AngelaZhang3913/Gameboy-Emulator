#include "joypad.h"
#include "hardware.h"
#include <cassert>


int main(){
    //rom[0xFF00] = 0xFF;
    assert(get_joypad_state() == 0xFF);
}
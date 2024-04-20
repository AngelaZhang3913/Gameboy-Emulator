#include "typedef.h"

extern BYTE joypad_state;

/* -------------
   JOYPAD STATE:

    Bit 7 - Not used
    Bit 6 - Not used
    Bit 5 - P15 Select Button Keys (0=Select)
    Bit 4 - P14 Select Direction Keys (0=Select)
    Bit 3 - P13 Input Down or Start (0=Pressed) (Read Only)
    Bit 2 - P12 Input Up or Select (0=Pressed) (Read Only)
    Bit 1 - P11 Input Left or Button B (0=Pressed) (Read Only)
    Bit 0 - P10 Input Right or Button A (0=Pressed) (Read Only)

 ------------- */




extern void key_pressed(int key);
extern void key_released(int key);

extern BYTE get_joypad_state();
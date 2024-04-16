#pragma once
#include "typedef.h"

#define TIMA 0xFF05 // timer address
#define TMA 0xFF06 // timer reset address
#define TMC 0xFF07 // timer controller
#define CLOCKSPEED 4194304 ;

extern int timer_counter ;

extern int divider_counter ;

extern int divider_register ;

extern BYTE get_clock_frequency();

extern void update_timers(int cycle_count) ;

extern void set_clock_frequency();




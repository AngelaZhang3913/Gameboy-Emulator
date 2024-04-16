#include "timer.h"
#include "hardware.h"
#include "typedef.h"

int timer_counter = 1024 ;
int divider_counter = 0 ;
int divider_register = 0 ;

bool is_clock_enabled() {
   return test_bit(read_memory(TMC),2);
}

BYTE get_clock_frequency() {
    // first two bits of timer controller
    return read_memory(TMC) & 3;
}

void set_clock_frequency() {
    BYTE clock_frequency = get_clock_frequency();

    // timmer_counter = clockspeed / frequency
    if (clock_frequency == 0) {
        timer_counter = 1024; // frequency 4096
    } else if (clock_frequency == 1) {
        timer_counter = 16; // frequency 262144
    } else if (clock_frequency == 2) {
        timer_counter = 64; // frequency 65536
    } else if (clock_frequency == 3) {
        timer_counter = 256; // frequency 16382
    }
}

void execute_divider_register(int cycle_count) {
    divider_register += cycle_count;
    if (divider_counter >= 255) {
        divider_counter = 0;
        rom[0xFF04]++;
    } else {
        divider_counter++;
    }
}

void update_timers(int cycle_count) {
    execute_divider_register(cycle_count);

    if (is_clock_enabled()) {
        timer_counter -= cycle_count;
        
        // update timer when enough cycles have passed
        if (timer_counter <= 0) {
            set_clock_frequency();

            BYTE timer = read_memory(TIMA);
            if (timer == 255) {
                // timer overflow
                write_memory(TIMA, read_memory(TMA));
                //request_interupt(2);
            } else {
                write_memory(TIMA, timer + 1);
            }
        }
    }
}
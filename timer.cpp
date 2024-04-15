#include "timer.h"
#include "hardware.h"

update_timers(int cycle_count) {
    divider_register(cycles_count);

    if (is_clock_enabled()) {
        timer_counter -= cycles_count;
        
        // update timer when enough cycles have passed
        if (timer_counter <= 0) {
            set_clock_frequency();

            BYTE timer = read_memory(TIMA);
            if (timer == 255) {
                // timer overflow
                write_memory(TIMA, read_memory(TMA));
                request_interupt(2);
            } else {
                write_memory(TIMA, timer + 1);
            }
        }
    }
}

bool s_clock_enabled() const {
   return test_bit(read_memory(TMC),2);
}
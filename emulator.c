#include "typdef.h"

void update() {
    int current_cycle;
    while (current_cycle < 69905) {
        int new_cycles = execute_next_opcode()
        current_cycle += new_cycles;
        update_timers(new_cycles);
        update_graphics(new_cycles);
        execute_interrupts();
    }
    render_screen();
}

int execute_next_opcode() {

}

update_timers(int cycles) {

}

update_graphics(int cycles) {

}

execute_interrupts() {

}

render_screen() {
    
}

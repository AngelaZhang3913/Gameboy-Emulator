#define TIMA 0xFF05 // timer address
#define TMA 0xFF06 // timer reset address
#define TMC 0xFF07 // timer controller
#define CLOCKSPEED 4194304 ;

int timer_counter = 1024 ;

extern update_timers(int cycle) ;
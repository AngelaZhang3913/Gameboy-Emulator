#define TIMA 0xFF05 // timer address
#define TMA 0xFF06 // timer reset address
#define TMC 0xFF07 // timer controller
#define CLOCKSPEED 4194304 ;

int timer_counter = 1024 ;

int divider_counter = 0 ;

int divider_register = 0 ;

extern void update_timers(int cycle) ;
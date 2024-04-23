#include "typedef.h"
#include "hardware.h"
#include "timer.h"

extern int execute_opcode(BYTE op);
extern void print_result();
extern int execute_next_opcode();

// for control of DE/IE instructions
extern int intrpt_next_inst;
extern bool en_interrupt;

extern void update();

extern void makefile();

extern void closefile();
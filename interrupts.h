#include "typedef.h"

extern void request_interrupt(int id) ;

extern void do_interrupts() ;

extern void sevice_interrupt(int id) ;

extern void push_word_onto_stack(WORD wrd);

extern WORD pop_word_from_stack();

bool interrupt_switch;
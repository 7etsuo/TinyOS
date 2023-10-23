#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "mem_map.h"
#include "types.h"

extern UINT16 * const vbl_counter;
extern Vector * const vector_table;

extern void vbl_isr();
extern void timer_A_isr();

void do_vbl_isr();
void do_timer_A_isr(UINT16 sr);
void init_vector_table();

#endif /* INTERRUPT_H */


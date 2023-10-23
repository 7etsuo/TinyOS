#ifndef IKBD_H
#define IKBD_H

#include "types.h"
#include "mem_map.h"

extern IO_PORT8    IKBD_ACIA_CR;
extern IO_PORT8_RO IKBD_ACIA_SR;
extern IO_PORT8_RO IKBD_ACIA_RDR;

extern UINT16 * const kybd_isr_state; /* 0=not in mouse packet, 1=expecting delta x, 2=expecting delta y */
extern UINT16 * const kybd_buff_head;
extern UINT16 * const kybd_buff_tail;
extern UINT16 * const kybd_buff_fill;
extern UINT16 * const kybd_num_lines;
extern UINT16 * const kybd_len_line; /* number of characters in buffer for current line */
extern UINT16 * const kybd_shifted;
extern UINT8  * const kybd_auto_ch;
extern UINT16 * const kybd_auto_count;      
extern UINT16 * const kybd_blocked_proc;
extern UINT16 * const kybd_fg_proc;
extern UINT8  * const kybd_buff; /* 128 byte circular queue - must be a power of 2 */

extern const UINT8 scan2ascii[2][128];

extern void ikbd_isr();

void do_ikbd_isr();
void input_enqueue(char ch);

#endif /* IKBD_H */


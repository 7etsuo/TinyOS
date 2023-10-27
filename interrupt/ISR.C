#include "isr.h"
#include "irq.h"
#include "../types.h"
#include "../hal/mfp.h"
#include "../hal/mem_map.h"
#include "../keyboard/ikbd.h"
#include "../scheduler/proc.h"
#include "../scheduler/sched.h"
#include "../keyboard/conio.h"
#include "../scheduler/cpu.h"
#include "../kern/kernel.h"
#include "../vid/video.h"
#include "../hal/acia.h"

UINT16 * const vbl_counter = (UINT16 * const) VBL_COUNTER_ADDR;
extern UINT16 * const kybd_isr_state = (UINT16 * const) KYBD_ISR_STATE_ADDR; 

void do_vbl_isr()
{
	UINT16 orig_ipl = set_ipl(7); /* [TO DO] mask less aggressively? */

	if (*kybd_auto_ch && ++(*kybd_auto_count) > 10)
		input_enqueue(*kybd_auto_ch);

	set_ipl(orig_ipl);

	if (!(++(*vbl_counter) & 0x001F))
	{
		set_ipl(7);
		invert_cursor();
		set_ipl(orig_ipl);
	}
}

void do_timer_A_isr(UINT16 sr)
{
	/* NOTE: timer A is very high priority.  Could use lower (e.g. C) */
	/* NOTE: could lower 68000 IPL to 4 to allow higher priority IRQs */

	if (CURR_PROC->state == PROC_RUNNING)
	{
		CURR_PROC->state = PROC_READY;
		*resched_needed = YES;
	}

	*MFP_ISRA &= ~MFP_TIMER_A;
}

void do_exception_isr(UINT16 sr)
{
	if (sr & 0x2000)
		panic();

	print_str_safe("fault in process ");
	print_char_safe((char)(CURR_PROC->pid + '0'));
	print_str_safe("\r\n");

	terminate();
}

void do_addr_exception_isr(UINT16 flags, UINT32 addr, UINT16 ir, UINT16 sr)
{
	do_exception_isr(sr);
}

void do_ikbd_isr()
{
	/* [TO DO] allow unbuffered (raw) mode? */
	/* [TO DO] implement caps lock */
	/* [TO DO] beep if buffer full? */
	/* [TO DO] tweak to minimize chance of overrruns (algorithm, assembly, IRQ prioritization) */

	if (*IKBD_ACIA_SR & ACIA_IRQ)
	{
		UINT8 data = *IKBD_ACIA_RDR;

		switch (*kybd_isr_state)
		{
			case 0:
				if ((data & 0xFC) == 0xF8)
					*kybd_isr_state = 1;
				else if (data == 0x2A || data == 0x36)     /* [TO DO] incorporate shifting into autorepeat logic */
					(*kybd_shifted)++;
				else if (data == 0xAA || data == 0xB6)
					(*kybd_shifted)--;
				else if (!(data & 0x80))
				{
					*kybd_auto_ch = scan2ascii[*kybd_shifted ? 1 : 0][data];

					if (*kybd_auto_ch)
					{
						*kybd_auto_count = 0;
						input_enqueue(*kybd_auto_ch);
					}
				}
				else
					*kybd_auto_ch = 0;

				break;

			case 1:
				*kybd_isr_state = 2;
				break;

			case 2:
				*kybd_isr_state = 0;
				break;
		}

		/* [TO DO] write a "reset IKBD ACIA" function */

		if (*IKBD_ACIA_SR & ACIA_OVRN)     /* overrun during this slow ISR?  IRQ will still be asserted! */
		{
			*IKBD_ACIA_CR = 0x17;     /* reset the ACIA to clear IRQ, or the MFP won't re-ack the IRQ ... */
			*IKBD_ACIA_CR = 0x96;     /* ... and the IKBD will hang!                                      */
		}
	}

	*MFP_ISRB &= ~MFP_GPIP4;
}


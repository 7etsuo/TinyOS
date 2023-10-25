#include "irq.h"
#include "cpu.h"
#include "mfp.h"
#include "video.h"
#include "sched.h"
#include "proc.h"
#include "syscalls.h"
#include "exc.h"
#include "ikbd.h"
#include "mem_map.h"

UINT16 * const vbl_counter = (UINT16 * const) VBL_COUNTER_ADDR;
Vector * const vector_table = (Vector * const) VECTOR_TABLE_ADDR;

void do_vbl_isr()
{
	UINT16 orig_ipl = set_ipl(7);     /* [TO DO] mask less aggressively? */

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

void init_vector_table()
{
	int num;

	for (num = 2; num < 80; num++)
		vector_table[num] = panic;

	for (num = 2; num <= 3; num++)
		vector_table[num] = addr_exception_isr;

	for (num = 4; num <= 8; num++)
		vector_table[num] = exception_isr;

	for (num = 10; num <= 11; num++)
		vector_table[num] = exception_isr;

	for (num = 32; num <= 47; num++)
		vector_table[num] = exception_isr;

	vector_table[VBL_VECTOR]     = vbl_isr;
	vector_table[TRAP_0_VECTOR]	 = sys_reboot;
	vector_table[TRAP_1_VECTOR]  = sys_exit;
	vector_table[TRAP_2_VECTOR]  = sys_create_process;
	vector_table[TRAP_3_VECTOR]  = sys_write;
	vector_table[TRAP_4_VECTOR]  = sys_read;
	vector_table[TRAP_5_VECTOR]  = sys_get_pid;
	vector_table[TRAP_7_VECTOR]  = sys_yield;
	vector_table[IKBD_VECTOR]    = ikbd_isr;
	vector_table[TIMER_A_VECTOR] = timer_A_isr;
}


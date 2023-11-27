#include "SCHED.H"
#include "PROC.H"
#include "CPU.H"
#include "../HAL/MEM_MAP.H"
#include "../GLOBAL.H"

/* 0=no, 1=yes, 2=yes with eventual trap restart (blocking) */
UINT16 * const resched_needed = (UINT16 *const) RESCHED_NEEDED_ADDR; 

void schedule()
{
	/* NOTE caller must have already saved CPU context */

	int i;

	while (1)
	{
		(void) set_ipl(7);

		for (i = 0; i < MAX_NUM_PROC; i++)
		{
			*curr_proc = (*curr_proc + 1) & (MAX_NUM_PROC - 1); /* this is a fast modulo needs to be a power of 2 */

			if (CURR_PROC->state == PROC_READY)
			{
				CURR_PROC->state = PROC_RUNNING;
				*resched_needed = NO;
				load_cpu_context(&CURR_PROC->cpu_context);
			}
		}

		await_interrupt();
	}
}

void do_exit()
{
	terminate();
}


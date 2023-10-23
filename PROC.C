#include "proc.h"
#include "types.h"
#include "video.h"
#include "ikbd.h"
#include "sched.h"
#include "conio.h"
#include "user.h"

UINT16 * const curr_proc    = (UINT16 *const) CURR_PROC_ADDR;
struct process * const proc = (struct process * const) PROC_ADDR; /* array of max_num_proc (4) process structures */

int find_empty_proc_slot() 
{
	for (int i = 0; i < MAX_NUM_PROC; i++) {
		if (proc[i].state == PROC_INVALID) /* empty proc index found */
			return i;
	}
	return -1; /* no empty proc found */
}

int set_kybd_fg_proc(int index) 
{
	if (*curr_proc == *kybd_fg_proc) {
		*kybd_fg_proc = index;
		return 1;
	}

	return -1;
}

int do_create_process(UINT16 prog_num, UINT16 is_fg)
{
	int index = find_empty_proc_slot();

	if (index == -1) {
		print_str_safe("process table full\r\n");
		return -1;
	}

	load(index, prog[prog_num]);

	if (is_fg)
		return set_kybd_fg_proc(index);

	return 1;
}

void init_proc_table()
{
	int i;

	*curr_proc = MAX_NUM_PROC - 1;

	for (i = 0; i < MAX_NUM_PROC; i++)
		proc[i].state = PROC_INVALID;
}

void load(UINT16 i, void (*p)())
{
	struct process *c = proc + i;

	c->cpu_context.usp = (UINT32)get_video_base() - ((UINT32)i << 8);
	c->cpu_context.pc = (UINT32)p;
	c->cpu_context.sr = 0x0200;

	c->state = PROC_READY;
	c->pid = i;                /* [TO DO] partially decouple pid from process table index */
	c->parent = CURR_PROC;     /* [TO DO] deal with root process? */
}

void terminate()
{
	if (*curr_proc == *kybd_fg_proc)
		*kybd_fg_proc = CURR_PROC->parent->pid;     /* [TO DO] handle case where parent has terminated! */

	CURR_PROC->state = PROC_INVALID;
	schedule();
}

int do_get_pid()
{
	return CURR_PROC->pid;
}

void do_yield()
{
	CURR_PROC->state = PROC_READY;
	*resched_needed = 1;
}


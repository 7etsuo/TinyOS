/* IMPORTANT: VERY LIMITED STACK SPACE! AVOID >~5 STACK FRAMES PER SYSTEM CALL
   HANDLER, WITH <=~16 BYTES OF PARAMETERS, <=~16 BYTES OF LOCALS.
   NOTE ALSO THAT ISRs CAN NEST!
   */
/* interface for memory configuration and management */
#include "../kb/io.h"			/* init_IO() */
#include "../kern/memory.h" 		/* init_memory() */
#include "../vid/video.h"		/* init_video() */
#include "../int/irq.h"			/* init_vector_table() */
#include "../kb/conio.h"		/* init_console() */
#include "kernel.h"

/* process table and scheduler */
#include "../schd/proc.h"		/* init_proc_table() and do_create_process() */
#include "../schd/sched.h"		/* schedule() */

/* 600-7FF kernel stack (512 bytes) */
UINT16 * const kernel_stack_top = (UINT16 *const) OS_RAM_TOP_ADDR; 

void init()
{
	init_memory();
	init_video();
	init_IO();
	init_vector_table();

	/* [TO DO]
	 * fg/bg (deal with parent termination, block instead of terminate when bg process reads)
	 - implement wait system call
	 - finish console driver
	 - clean up code
	 - see misc. TO DOs below
	 - clean up code incl. assembly; better modularize project
	 */

	init_console();	
	init_proc_table();

	do_create_process(0, 1);     /* load shell */
	schedule();
}

void panic()
{
	volatile UINT32 i;

	set_ipl(7);
	print_str("kernel panic!");

	for (i = 0; i < 100000L; i++)
		;

	restart();
}


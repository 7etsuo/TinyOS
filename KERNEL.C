/* IMPORTANT: VERY LIMITED STACK SPACE! AVOID >~5 STACK FRAMES PER SYSTEM CALL
   HANDLER, WITH <=~16 BYTES OF PARAMETERS, <=~16 BYTES OF LOCALS.
   NOTE ALSO THAT ISRs CAN NEST!
   */

/* interface for memory configuration and management */
#include "io.h"				/* init_IO() */
#include "memory.h" 		/* init_memory() */
#include "video.h"			/* init_video() */
#include "irq.h"			/* init_vector_table() */
#include "conio.h"			/* init_console() */

/* process table and scheduler */
#include "proc.h"			/* init_proc_table() and do_create_process() */
#include "sched.h"			/* schedule() */

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


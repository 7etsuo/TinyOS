#ifndef PROC_H
#define PROC_H

#include "../TYPES.H"
#include "CPU.H"

#define PROC_INVALID 0
#define PROC_READY   1
#define PROC_RUNNING 2
#define PROC_BLOCKED 3

struct process /* 102 bytes */
{
	struct CPU_context cpu_context;     /* must be first element in structure */
	UINT16 state;

	UINT16 pid;
	struct process *parent;

	UINT32 dummy1;
	UINT32 dummy2;
	UINT32 dummy3;
	UINT32 dummy4;
	UINT32 dummy5;
	UINT32 dummy6;

	/* [TO DO] add:
	   - priority?
	   - next/prev pointers for queue insertion?
	   - memory bounds?
	   */
};

#define MAX_NUM_PROC 4 /* must be a power of 2 because of the modulo operation in the scheduler */
#define CURR_PROC (proc + *curr_proc)

extern UINT16 * const curr_proc;
extern struct process * const proc; /* array of max_num_proc (4) process structures */

void init_proc_table(void);
void load(UINT16, void (*)(void));
void terminate(void);

void do_exit(void);
int do_get_pid(void);
int do_create_process(UINT16, UINT16); /* [TODO]: we need create_process to return the int from this */

int set_kybd_fg_proc(int);
int find_empty_proc_slot(void);

#endif /* PROC_H */


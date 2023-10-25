#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "types.h"

/*  Vector #:   system call #:	name:			details:
    32           0		reboot
    33           1		exit
    34           2		create_process
    35           3		write
    36           4		read
    37           5		get_pid
    38           6		(reserved)		(reserved for SCI expansion adjust priority?)
    39           7		yield
    40           8		wait			(not implementedd)
    41           9		(reserved)		(reserved for SCI expansion)
    42          10		semaphore operation	(not implemented)
    43          11		(reserved)		(reserved for SCI expansion)
    44          12		open			(not implemented)
    45          13		close			(not implemented)
    46          14		create_file		(not implemented)
    47          15		delete_file		(not implemented) 
    */
#define HBL_VECTOR	26
#define VBL_VECTOR	28
#define TRAP_0_VECTOR	32  /* rebooot	      */
#define TRAP_1_VECTOR	33  /* exit           */
#define TRAP_2_VECTOR	34  /* create_process */
#define TRAP_3_VECTOR	35  /* write          */
#define TRAP_4_VECTOR	36  /* read           */
#define TRAP_5_VECTOR	37  /* get_pid        */
#define TRAP_7_VECTOR	39  /* yield          */
#define IKBD_VECTOR	70
#define TIMER_A_VECTOR	77

extern void reboot();
extern void exit();
extern void create_process(UINT16 prog_num, UINT16 is_fg);
extern void write(const char *buf, unsigned int len);
extern int read(char *buf, unsigned int len);
extern int get_pid();
extern void yield();

extern void sys_reboot();
extern void sys_exit();
extern void sys_create_process();
extern void sys_write();
extern void sys_read();
extern void sys_get_pid();
extern void sys_yield();

#endif /* SYSCALLS_H */


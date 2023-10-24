#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "types.h"

/*  system call #:	name:

    0				(reserved)
    1				exit
    2				create_process
    3				write
    4				read
    5				get_pid
    6				(reserved)		(adjust priority?)
    7				yield
    8				wait			(not implementedd)
    9				(reserved)
    10				semaphore operation	(not implemented)
    11				(reserved)
    12				open			(not implemented)
    13				close			(not implemented)
    14				create_file		(not implemented)
    15				delete_file		(not implemented)
    */

#define HBL_VECTOR     26
#define VBL_VECTOR     28
#define TRAP_1_VECTOR  33     /* exit           */
#define TRAP_2_VECTOR  34     /* create_process */
#define TRAP_3_VECTOR  35     /* write          */
#define TRAP_4_VECTOR  36     /* read           */
#define TRAP_5_VECTOR  37     /* get_pid        */
#define TRAP_7_VECTOR  39     /* yield          */
#define IKBD_VECTOR    70
#define TIMER_A_VECTOR 77

extern void exit();
extern void create_process(UINT16 prog_num, UINT16 is_fg);
extern void write(const char *buf, unsigned int len);
extern int read(char *buf, unsigned int len);
extern int get_pid();
extern void yield();

extern void sys_exit();
extern void sys_create_process();
extern void sys_write();
extern void sys_read();
extern void sys_get_pid();
extern void sys_yield();

#endif /* SYSCALLS_H */


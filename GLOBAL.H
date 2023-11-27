#ifndef GLOBAL_H
#define GLOBAL_H

#include "TYPES.H"

struct CPU_context;

/* isr_asm.s */
void vbl_isr(void);
void exception_isr(void);
void timer_A_isr(void);
void ikbd_isr(void);

/* kern_asm.s */
void restart(void);

/* sys_asm.s */
void reboot(void); 
void sys_reboot(void);
void exit(void);
void sys_exit(void);
void create_process(UINT16, UINT16); 
void sys_create_process(void);
void write(const char *, unsigned int);
void sys_write(void);
int read(char *, unsigned int);
void sys_read(void);
int get_pid(void);
void sys_get_pid(void);
void yield(void);
void sys_yield(void);

/* proc_asm.s */
UINT16 read_SR(void);
void write_SR(UINT16 sr);
void await_interrupt(void);

/* schd_asm.s */ 
void load_cpu_context(struct CPU_context *);

/* vid_asm.s */
void clear_screen(UINT8 *);
void scroll(void);

#endif /* GLOBAL_H */

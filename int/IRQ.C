#include "../kern/kernel.h"
#include "../kern/syscall.h"
#include "../hal/mem_map.h"
#include "../kb/ikbd.h"
#include "../vid/video.h"
#include "isr.h"
#include "irq.h"

Vector * const vector_table = (Vector * const) VECTOR_TABLE_ADDR;

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
	vector_table[TRAP_0_VECTOR]  = sys_reboot;
	vector_table[TRAP_1_VECTOR]  = sys_exit;
	vector_table[TRAP_2_VECTOR]  = sys_create_process;
	vector_table[TRAP_3_VECTOR]  = sys_write;
	vector_table[TRAP_4_VECTOR]  = sys_read;
	vector_table[TRAP_5_VECTOR]  = sys_get_pid;
	vector_table[TRAP_7_VECTOR]  = sys_yield;
	vector_table[IKBD_VECTOR]    = ikbd_isr;
	vector_table[TIMER_A_VECTOR] = timer_A_isr;
}


#include "exc.h"
#include "conio.h"
#include "proc.h"

void do_exception_isr(UINT16 sr)
{
	if (sr & 0x2000)
		panic();

	print_str_safe("fault in process ");
	print_char_safe((char)(CURR_PROC->pid + '0'));
	print_str_safe("\r\n");

	terminate();
}

void do_addr_exception_isr(UINT16 flags, UINT32 addr, UINT16 ir, UINT16 sr)
{
	do_exception_isr(sr);
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


#include "REBOOT.H"
#include "SYSCALL.H"

extern void restart();

void do_reboot()
{
	restart();
}


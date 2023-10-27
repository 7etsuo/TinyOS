#include "reboot.h"
#include "syscall.h"

extern void restart();

void do_reboot()
{
	restart();
}


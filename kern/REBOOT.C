#include "reboot.h"
#include "syscalls.h"

extern void restart();

void do_reboot()
{
	restart();
}


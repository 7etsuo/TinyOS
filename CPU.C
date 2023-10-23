#include "cpu.h"
#include "types.h"

UINT16 set_ipl(UINT16 ipl)
{
	UINT16 sr = read_SR();
	UINT16 old_ipl = ((sr >> 8) & 0x0007);

	write_SR((sr & 0xF8FF) | ((ipl & 0x0007) << 8));

	return old_ipl;
}


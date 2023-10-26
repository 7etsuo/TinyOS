#include "memory.h"
#include "mem_map.h"
#include "types.h"

IO_PORT8 MEM_CONFIG_REG = (IO_PORT8) MEM_CTL_REG_ADDR;

void init_memory()
{
	/* move.b #RAM_4M,MEM_CONFIG_REG */
	*MEM_CONFIG_REG = RAM_4M;
}

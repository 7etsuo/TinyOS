#ifndef MEMORY_H
#define MEMORY_H

#include "../types.h"

#define RAM_4M 0x0A

extern IO_PORT8 MEM_CONFIG_REG;

/* added by Mike Walker */
void init_memory();

#endif /* MEMORY_H */

#ifndef MEMORY_H
#define MEMORY_H

#include "../TYPES.H"

#define RAM_4M 0x0A

extern IO_PORT8 MEM_CONFIG_REG;

void init_memory(void);

#endif /* MEMORY_H */

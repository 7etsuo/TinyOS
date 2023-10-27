#ifndef KERNEL_H
#define KERNEL_H

#include "../types.h"

extern UINT16 * const kernel_stack_top; 

extern void restart();
void panic();

#endif /* KERNEL_H */


#ifndef KERNEL_H
#define KERNEL_H

#include "../TYPES.H"

extern UINT16 * const kernel_stack_top; 
extern void restart();

void init();
void panic();

#endif /* KERNEL_H */


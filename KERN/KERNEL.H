#ifndef KERNEL_H
#define KERNEL_H

#include "../TYPES.H"

extern UINT16 * const kernel_stack_top; 

void init(void);
void panic(void);

#endif /* KERNEL_H */


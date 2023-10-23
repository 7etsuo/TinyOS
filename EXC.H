#ifndef EXCEPTION_H
#define EXCEPTION_H

#include "types.h"

extern void restart();
extern void exception_isr();
extern void addr_exception_isr();

void do_exception_isr(UINT16 sr);
void do_addr_exception_isr(UINT16 flags, UINT32 addr, UINT16 ir, UINT16 sr);
void panic();

#endif /* EXCEPTION_H */

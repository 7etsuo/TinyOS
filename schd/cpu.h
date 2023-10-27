#ifndef CPU_H
#define CPU_H

#include "../types.h"

struct CPU_context     /* 70 bytes */
{
    UINT32 pc;
    UINT16 sr;
    UINT32 usp;
    UINT32 d0_to_7_a1_to_6[14];
    UINT32 a0;
};

extern UINT16 read_SR();
extern void write_SR(UINT16 sr);
extern void load_cpu_context(struct CPU_context *);

UINT16 set_ipl(UINT16 ipl);

#endif /* CPU_STATE_H */


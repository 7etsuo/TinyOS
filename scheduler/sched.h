#ifndef SCHED_H
#define SCHED_H

#include "../types.h"

enum SCHED_STATE {
    NO          = 0,
    YES         = 1,
    YES_BLOCK   = 2,
};

extern UINT16 * const resched_needed; /* 0=no, 1=yes, 2=yes with eventual trap restart (blocking) */

extern void await_interrupt();
void schedule();
void do_exit();

#endif /* SCHED_H */


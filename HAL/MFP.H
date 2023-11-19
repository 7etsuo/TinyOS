#ifndef MFP_H
#define MFP_H

#include "../TYPES.H"

#define MFP_TIMER_A 0x20
#define MFP_GPIP4   0x40

extern IO_PORT8 MFP_IERA;
extern IO_PORT8 MFP_IERB;
extern IO_PORT8 MFP_ISRA;
extern IO_PORT8 MFP_ISRB;
extern IO_PORT8 MFP_IMRA;
extern IO_PORT8 MFP_IMRB;
extern IO_PORT8 MFP_VR;
extern IO_PORT8 MFP_TACR;
extern IO_PORT8 MFP_TADR;

#endif /* MFP_H */


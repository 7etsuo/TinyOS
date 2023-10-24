/* Type definitions and utility macros for basic data types and I/O */
#ifndef TYPES_H
#define TYPES_H

#define NULL ((void *)0)

typedef unsigned char UINT8;
typedef unsigned int  UINT16;
typedef unsigned long UINT32;

typedef void (*Vector)();
typedef volatile UINT8 * const IO_PORT8;
typedef volatile const UINT8 * const IO_PORT8_RO;


#endif

#ifndef CONIO_DRV_H
#define CONIO_DRV_H

#include "types.h"
#include "mem_map.h"

extern UINT8 * const console_x_p;
extern UINT8 * const console_y_p;
extern UINT16 * const cursor_visible;

void print_char(char);
void print_str(char *);

void print_char_safe(char);
void print_str_safe(char *);

void do_write(const char *buf, unsigned int len);
int do_read(char *buf, unsigned int len);

void init_console();

#endif /* CON_DRV_H */

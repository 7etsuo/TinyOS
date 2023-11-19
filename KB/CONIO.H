#ifndef CONIO_DRV_H
#define CONIO_DRV_H

#include "../TYPES.H"

extern UINT8 * const console_x_p;
extern UINT8 * const console_y_p;
extern UINT16 * const cursor_visible;

void print_char(char);
void print_str(char *);

void print_char_safe(char);
void print_str_safe(char *);

void do_write(const char *buf, unsigned int len);

/**
 * @brief Reads keyboard input into a buffer.
 *
 * This function reads characters from the keyboard into a given buffer. 
 * If the current process has keyboard focus and lines are available in 
 * the keyboard buffer, the function reads up to `len` characters into `buf`. 
 * If no lines are available, the current process is blocked, and a flag is set 
 * to indicate that this system call will need to be restarted.
 * 
 * @param buf Pointer to the buffer where the keyboard input will be stored.
 * @param len Maximum number of characters to read into `buf`.
 * @return The number of characters actually read into `buf`.
 * 
 * @note The function sets the current process state to `PROC_BLOCKED` and sets 
 * `resched_needed` to `YES_BLOCK` if there are no lines to read, signaling that 
 * the system call will need to be restarted.
 */
int do_read(char *buf, unsigned int len);

void init_console();

#endif /* CON_DRV_H */
